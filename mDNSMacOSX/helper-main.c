/*
 * Copyright (c) 2007 Apple Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define _FORTIFY_SOURCE 2
#include <sys/cdefs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <bootstrap_priv.h>
#include <asl.h>
#include <launch.h>
#include <pwd.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <Security/Security.h>
#include "helper.h"
#include "helper-server.h"
#include "helpermsg.h"
#include "helpermsgServer.h"

union max_msg_size {
	union __RequestUnion__proxy_helper_subsystem req;
	union __ReplyUnion__proxy_helper_subsystem rep;
};

static const mach_msg_size_t MAX_MSG_SIZE =
    sizeof(union max_msg_size) + MAX_TRAILER_SIZE;
static aslclient logclient = NULL;
static int opt_debug;
static struct timeval last_message;
static pthread_t idletimer_thread;

unsigned long maxidle = 10;

static void
helplogv(int level, const char *fmt, va_list ap)
{
	if (NULL == logclient) {
		vfprintf(stderr, fmt, ap);
		fflush(stderr);
	} else
		asl_vlog(logclient, NULL, level, fmt, ap);
}

void
helplog(int level, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	helplogv(level, fmt, ap);
	va_end(ap);
}

static void
initialize_logging(void)
{
	logclient = asl_open(NULL, kmDNSHelperServiceName,
	    (opt_debug ? ASL_OPT_STDERR : 0));
	if (NULL == logclient) {
		fprintf(stderr, "Could not initialize ASL logging.\n");
		fflush(stderr);
		return;
	}
	if (opt_debug)
		asl_set_filter(logclient,
		    ASL_FILTER_MASK_UPTO(ASL_LEVEL_DEBUG));
}

static void
initialize_id(void)
{
	static char login[] = "_mdnsresponder";
	struct passwd *pwd = getpwnam(login);

	if (NULL == pwd) {
		helplog(ASL_LEVEL_ERR,
		    "Could not find account name `%s'.  I will only help root.",
		    login);
		return;
	}
	mDNSResponderUID = pwd->pw_uid;
	mDNSResponderGID = pwd->pw_gid;
}

void
update_idle_timer(void)
{
	assert(0 == gettimeofday(&last_message, NULL));
}

static void *
idletimer(void *context)
{
	static struct timeval now;

	for (;;) {
		assert(0 == gettimeofday(&now, NULL));
		if (now.tv_sec - last_message.tv_sec > (long)maxidle) {
			(void)proxy_mDNSIdleExit((mach_port_t)context);
			sleep(1);
		} else {
			int t = maxidle - (now.tv_sec - last_message.tv_sec);
			if (t < 1)
				t = 1;
			sleep(t);
		}
	}
}

static void
initialize_timer(mach_port_t port)
{
	int err = 0;

	update_idle_timer();
	if (0 != (err = pthread_create(&idletimer_thread, NULL, idletimer,
	    (void *)port)))
		helplog(ASL_LEVEL_ERR, "Could not start idletimer thread: %s",
		    strerror(err));
}

static mach_port_t
checkin(char *service_name)
{
	kern_return_t kr = KERN_SUCCESS;
	mach_port_t port = MACH_PORT_NULL;
	launch_data_t msg = NULL, reply = NULL, datum = NULL;

	if (NULL == (msg = launch_data_new_string(LAUNCH_KEY_CHECKIN))) {
		helplog(ASL_LEVEL_ERR,
		    "Could not create checkin message for launchd.");
		goto fin;
	}
	reply = launch_msg(msg);
	if (NULL == reply) {
		helplog(ASL_LEVEL_ERR, "Could not message launchd.");
		goto fin;
	}
	if (LAUNCH_DATA_ERRNO == launch_data_get_type(reply)) {
		helplog(ASL_LEVEL_ERR, "Launchd checkin failed: %s.",
		    strerror(launch_data_get_errno(reply)));
		goto fin;
	}
	if (NULL == (datum = launch_data_dict_lookup(reply,
	    LAUNCH_JOBKEY_MACHSERVICES)) ||
	    LAUNCH_DATA_DICTIONARY != launch_data_get_type(datum)) {
		helplog(ASL_LEVEL_ERR,
		    "Launchd reply does not contain %s dictionary.",
		    LAUNCH_JOBKEY_MACHSERVICES);
		goto fin;
	}
	if (NULL == (datum = launch_data_dict_lookup(datum, service_name)) ||
	    LAUNCH_DATA_MACHPORT != launch_data_get_type(datum)) {
		helplog(ASL_LEVEL_ERR,
		    "Launchd reply does not contain %s Mach port.",
		    service_name);
		goto fin;
	}
	if (MACH_PORT_NULL == (port = launch_data_get_machport(datum))) {
		helplog(ASL_LEVEL_ERR, "Launchd gave me a null Mach port.");
		goto fin;
	}
	if (KERN_SUCCESS != (kr = mach_port_insert_right(mach_task_self(),
	    port, port, MACH_MSG_TYPE_MAKE_SEND))) {
		helplog(ASL_LEVEL_ERR, "mach_port_insert_right: %s",
		    mach_error_string(kr));
		goto fin;
	}

fin:
	if (NULL != msg)
		launch_data_free(msg);
	if (NULL != reply)
		launch_data_free(reply);
	if (MACH_PORT_NULL == port)
		exit(EXIT_FAILURE);
	return port;
}

static mach_port_t
register_service(const char *service_name)
{
	mach_port_t port = MACH_PORT_NULL;
	kern_return_t kr;

	if (KERN_SUCCESS != (kr = mach_port_allocate(mach_task_self(),
	    MACH_PORT_RIGHT_RECEIVE, &port))) {
		helplog(ASL_LEVEL_ERR, "mach_port_allocate: %s",
		    mach_error_string(kr));
		goto error;
	}
	if (KERN_SUCCESS != (kr = mach_port_insert_right(mach_task_self(),
	    port, port, MACH_MSG_TYPE_MAKE_SEND))) {
		helplog(ASL_LEVEL_ERR, "mach_port_insert_right: %s",
		    mach_error_string(kr));
		goto error;
	}
	/* XXX bootstrap_register2 does not modify its second argument,
	 * but the prototype does not include const.
	 */
	if (KERN_SUCCESS != (kr = bootstrap_register2(bootstrap_port,
	    (char *)service_name, port, 0))) {
		helplog(ASL_LEVEL_ERR, "bootstrap_register2 failed: %s",
		    mach_error_string(kr));
		goto error;
	}
	return port;
error:
	if (MACH_PORT_NULL != port)
		mach_port_deallocate(mach_task_self(), port);
	return MACH_PORT_NULL;
}

int
main(int ac, char *av[])
{
	char *p = NULL;
	kern_return_t kr = KERN_FAILURE;
	mach_port_t port = MACH_PORT_NULL;
	long n;
	int ch;

	while ((ch = getopt(ac, av, "dt:")) != -1)
		switch (ch) {
		case 'd':
			opt_debug = 1;
			break;
		case 't':
			n = strtol(optarg, &p, 0);
			if ('\0' == optarg[0] || '\0' != *p ||
			    n > LONG_MAX || n < 1) {
				fprintf(stderr, "Invalid idle timeout: %s\n",
				    optarg);
				exit(EXIT_FAILURE);
			}
			maxidle = n;
			break;
		case '?':
		default:
			fprintf(stderr, "Usage: mDNSResponderHelper [-d] [-t maxidle]\n");
			exit(EXIT_FAILURE);
		}
	ac -= optind;
	av += optind;

	initialize_logging();
	helplog(ASL_LEVEL_NOTICE, "Starting");
	initialize_id();
	/* We should normally be running as a system daemon.  However,
	 * that might not be the case in some scenarios
	 * (e.g. debugging).  Explicitly ensure that our Keychain
	 * operations utilize the system domain.
	 */
	SecKeychainSetPreferenceDomain(kSecPreferencesDomainSystem);
	if (opt_debug)
		port = register_service(kmDNSHelperServiceName);
	else
		port = checkin(kmDNSHelperServiceName);
	initialize_timer(port);
	if (KERN_SUCCESS != (kr = mach_msg_server(helper_server, MAX_MSG_SIZE,
	    port, MACH_RCV_TRAILER_ELEMENTS(MACH_RCV_TRAILER_AUDIT) |
	    MACH_RCV_TRAILER_TYPE(MACH_MSG_TRAILER_FORMAT_0)))) {
		helplog(ASL_LEVEL_ERR, "mach_msg_server: %s\n",
		    mach_error_string(kr));
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}