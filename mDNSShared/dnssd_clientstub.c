/* -*- Mode: C; tab-width: 4 -*-
 *
 * Copyright (c) 2003-2004, Apple Computer, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Change History (most recent first):

$Log: dnssd_clientstub.c,v $
Revision 1.72  2007/05/22 01:07:42  cheshire
<rdar://problem/3563675> API: Need a way to get version/feature information

Revision 1.71  2007/05/18 23:55:22  cheshire
<rdar://problem/4454655> Allow multiple register/browse/resolve operations to share single Unix Domain Socket

Revision 1.70  2007/05/17 20:58:22  cheshire
<rdar://problem/4647145> DNSServiceQueryRecord should return useful information with NXDOMAIN

Revision 1.69  2007/05/16 16:58:27  cheshire
<rdar://problem/4471320> Improve reliability of kDNSServiceFlagsMoreComing flag on multiprocessor machines
As long as select indicates that data is waiting, loop within DNSServiceProcessResult delivering additional results

Revision 1.68  2007/05/16 01:06:52  cheshire
<rdar://problem/4471320> Improve reliability of kDNSServiceFlagsMoreComing flag on multiprocessor machines

Revision 1.67  2007/05/15 21:57:16  cheshire
<rdar://problem/4608220> Use dnssd_SocketValid(x) macro instead of just
assuming that all negative values (or zero!) are invalid socket numbers

Revision 1.66  2007/03/27 22:23:04  cheshire
Add "dnssd_clientstub" prefix onto syslog messages

Revision 1.65  2007/03/21 22:25:23  cheshire
<rdar://problem/4172796> Remove client retry logic now that mDNSResponder uses launchd for its Unix Domain Socket

Revision 1.64  2007/03/21 19:01:56  cheshire
<rdar://problem/5078494> IPC code not 64-bit-savvy: assumes long=32bits, and short=16bits

Revision 1.63  2007/03/12 21:48:21  cheshire
<rdar://problem/5000162> Scary unlink errors in system.log
Code was using memory after it had been freed

Revision 1.62  2007/02/28 01:44:30  cheshire
<rdar://problem/5027863> Byte order bugs in uDNS.c, uds_daemon.c, dnssd_clientstub.c

Revision 1.61  2007/02/09 03:09:42  cheshire
<rdar://problem/3869251> Cleanup: Stop returning kDNSServiceErr_Unknown so often
<rdar://problem/4177924> API: Should return kDNSServiceErr_ServiceNotRunning

Revision 1.60  2007/02/08 20:33:44  cheshire
<rdar://problem/4985095> Leak on error path in DNSServiceProcessResult

Revision 1.59  2007/01/05 08:30:55  cheshire
Trim excessive "$Log" checkin history from before 2006
(checkin history still available via "cvs log ..." of course)

Revision 1.58  2006/10/27 00:38:22  cheshire
Strip accidental trailing whitespace from lines

Revision 1.57  2006/09/30 01:06:54  cheshire
Protocol field should be uint32_t

Revision 1.56  2006/09/27 00:44:16  herscher
<rdar://problem/4249761> API: Need DNSServiceGetAddrInfo()

Revision 1.55  2006/09/26 01:52:01  herscher
<rdar://problem/4245016> NAT Port Mapping API (for both NAT-PMP and UPnP Gateway Protocol)

Revision 1.54  2006/09/21 21:34:09  cheshire
<rdar://problem/4100000> Allow empty string name when using kDNSServiceFlagsNoAutoRename

Revision 1.53  2006/09/07 04:43:12  herscher
Fix compile error on Win32 platform by moving inclusion of syslog.h

Revision 1.52  2006/08/15 23:04:21  mkrochma
<rdar://problem/4090354> Client should be able to specify service name w/o callback

Revision 1.51  2006/07/24 23:45:55  cheshire
<rdar://problem/4605276> DNSServiceReconfirmRecord() should return error code

Revision 1.50  2006/06/28 08:22:27  cheshire
<rdar://problem/4605264> dnssd_clientstub.c needs to report unlink failures in syslog

Revision 1.49  2006/06/28 07:58:59  cheshire
Minor textual tidying

*/

#include <errno.h>
#include <stdlib.h>

#include "dnssd_ipc.h"

#if defined(_WIN32)

	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <windows.h>
	
	#define sockaddr_mdns sockaddr_in
	#define AF_MDNS AF_INET
	
	// Disable warning: "'type cast' : from data pointer 'void *' to function pointer"
	#pragma warning(disable:4055)
	
	// Disable warning: "nonstandard extension, function/data pointer conversion in expression"
	#pragma warning(disable:4152)
	
	extern BOOL IsSystemServiceDisabled();
	
	#define sleep(X) Sleep((X) * 1000)
	
	static int g_initWinsock = 0;

#else

	#include <sys/time.h>
	#include <sys/socket.h>
	#include <syslog.h>
	
	#define sockaddr_mdns sockaddr_un
	#define AF_MDNS AF_LOCAL

#endif

// <rdar://problem/4096913> Specifies how many times we'll try and connect to the server.

#define DNSSD_CLIENT_MAXTRIES 4

#define CTL_PATH_PREFIX "/tmp/dnssd_clippath."
// Error socket (if needed) is named "dnssd_clipath.[pid].xxx:n" where xxx are the
// last 3 digits of the time (in seconds) and n is the 6-digit microsecond time

typedef struct
	{
	ipc_msg_hdr         ipc_hdr;
	DNSServiceFlags     cb_flags;
	uint32_t            cb_interface;
	DNSServiceErrorType cb_err;
	} CallbackHeader;

typedef struct _DNSServiceRef_t DNSServiceOp;
typedef struct _DNSRecordRef_t DNSRecord;

// client stub callback to process message from server and deliver results to client application
typedef void (*ProcessReplyFn)(DNSServiceOp *sdr, CallbackHeader *cbh, char *msg);

// When using kDNSServiceFlagsShareConnection, there is one primary _DNSServiceOp_t, and zero or more subbordinates
// For the primary, the 'next' field points to the first subbordinate, and its 'next' field points to the next, and so on.
// For the primary, the 'primary' field is NULL; for subbordinates the 'primary' field points back to the associated primary
struct _DNSServiceRef_t
	{
	DNSServiceOp   *next;				// For shared connection
	DNSServiceOp   *primary;			// For shared connection
	dnssd_sock_t    sockfd;				// Connected socket between client and daemon
	uint32_t        op;					// request_op_t or reply_op_t
	uint32_t        max_index;			// Largest assigned record index - 0 if no additional records registered
	ProcessReplyFn  ProcessReply;		// Function pointer to the code to handle received messages
	void           *AppCallback;		// Client callback function and context
	void           *AppContext;
	} _DNSServiceRef_t;

struct _DNSRecordRef_t
	{
	void *AppContext;
	DNSServiceRegisterRecordReply AppCallback;
	DNSRecordRef recref;
	uint32_t record_index;  // index is unique to the ServiceDiscoveryRef
	DNSServiceOp *sdr;
	};

// Exported functions

// Write len bytes. Return 0 on success, -1 on error
static int write_all(dnssd_sock_t sd, char *buf, int len)
	{
	// Don't use "MSG_WAITALL"; it returns "Invalid argument" on some Linux versions; use an explicit while() loop instead.
	//if (send(sd, buf, len, MSG_WAITALL) != len)   return -1;
	while (len)
		{
		ssize_t num_written = send(sd, buf, len, 0);
		if (num_written < 0 || num_written > len)
			{
			// Should never happen. If it does, it indicates some OS bug,
			// or that the mDNSResponder daemon crashed (which should never happen).
			syslog(LOG_WARNING, "dnssd_clientstub write_all(%d) failed %d/%d %d %s", sd, num_written, len, errno, strerror(errno));
			return -1;
			}
		buf += num_written;
		len -= num_written;
		}
	return 0;
	}

// Read len bytes. Return 0 on success, -1 on error
static int read_all(dnssd_sock_t sd, char *buf, int len)
	{
	// Don't use "MSG_WAITALL"; it returns "Invalid argument" on some Linux versions; use an explicit while() loop instead.
	//if (recv(sd, buf, len, MSG_WAITALL) != len) return -1;

	while (len)
		{
		ssize_t num_read = recv(sd, buf, len, 0);
		if ((num_read == 0) || (num_read < 0) || (num_read > len))
			{
			// Should never happen. If it does, it indicates some OS bug,
			// or that the mDNSResponder daemon crashed (which should never happen).
			syslog(LOG_WARNING, "dnssd_clientstub read_all(%d) failed %d/%d %d %s", sd, num_read, len, errno, strerror(errno));
			return -1;
			}
		buf += num_read;
		len -= num_read;
		}
	return 0;
	}

// Returns 1 if more bytes remain to be read on socket descriptor sd
static int more_bytes(dnssd_sock_t sd)
	{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(sd, &readfds);
	struct timeval tv = { 0, 0 };
	return(select(sd+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv) > 0);
	}

/* create_hdr
 *
 * allocate and initialize an ipc message header. Value of len should initially be the
 * length of the data, and is set to the value of the data plus the header. data_start
 * is set to point to the beginning of the data section. reuse_socket should be non-zero
 * for calls that can receive an immediate error return value on their primary socket.
 * if zero, the path to a control socket is appended at the beginning of the message buffer.
 * data_start is set past this string.
 */
static ipc_msg_hdr *create_hdr(uint32_t op, size_t *len, char **data_start, int SeparateReturnSocket, void *cookie)
	{
	char *msg = NULL;
	ipc_msg_hdr *hdr;
	int datalen;
#if !defined(USE_TCP_LOOPBACK)
	char ctrl_path[256];
#endif

	if (SeparateReturnSocket)
		{
#if defined(USE_TCP_LOOPBACK)
		*len += 2;  // Allocate space for two-byte port number
#else
		struct timeval time;
		if (gettimeofday(&time, NULL) < 0)
			{ syslog(LOG_WARNING, "dnssd_clientstub create_hdr: gettimeofday failed %d %s", errno, strerror(errno)); return NULL; }
		sprintf(ctrl_path, "%s%d-%.3lx-%.6lu", CTL_PATH_PREFIX, (int)getpid(),
			(unsigned long)(time.tv_sec & 0xFFF), (unsigned long)(time.tv_usec));
		*len += strlen(ctrl_path) + 1;
#endif
		}

	datalen = (int) *len;
	*len += sizeof(ipc_msg_hdr);

	// Write message to buffer
	msg = malloc(*len);
	if (!msg) { syslog(LOG_WARNING, "dnssd_clientstub create_hdr: malloc failed"); return NULL; }

	bzero(msg, *len);
	hdr = (ipc_msg_hdr *)msg;
	hdr->version                = VERSION;
	hdr->datalen                = datalen;
	hdr->ipc_flags              = 0;
	hdr->op                     = op;
	hdr->client_context.context = cookie;
	hdr->reg_index              = 0;
	if (!SeparateReturnSocket) hdr->ipc_flags |= IPC_FLAGS_REUSE_SOCKET;
	*data_start = msg + sizeof(ipc_msg_hdr);
#if defined(USE_TCP_LOOPBACK)
	// Put dummy data in for the port, since we don't know what it is yet.
	// The data will get filled in before we send the message. This happens in deliver_request().
	if (SeparateReturnSocket) put_uint16(0, data_start);
#else
	if (SeparateReturnSocket) put_string(ctrl_path, data_start);
#endif
	return hdr;
	}

// Return a connected service ref (deallocate with DNSServiceRefDeallocate)
static DNSServiceErrorType ConnectToServer(DNSServiceRef *ref, DNSServiceFlags flags, uint32_t op, ProcessReplyFn ProcessReply, void *AppCallback, void *AppContext)
	{
	#if APPLE_OSX_mDNSResponder
	int NumTries = DNSSD_CLIENT_MAXTRIES;
	#else
	int NumTries = 0;
	#endif

	dnssd_sockaddr_t saddr;
	DNSServiceOp *sdr;

	#if defined(_WIN32)
	if (!g_initWinsock)
		{
		WSADATA wsaData;
		g_initWinsock = 1;
		if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) { *ref = NULL; return kDNSServiceErr_ServiceNotRunning; }
		}
	// <rdar://problem/4096913> If the system service is disabled, we only want to try to connect once
	if (IsSystemServiceDisabled()) NumTries = DNSSD_CLIENT_MAXTRIES;
	#endif

	sdr = malloc(sizeof(DNSServiceOp));
	if (!sdr) { syslog(LOG_WARNING, "dnssd_clientstub ConnectToServer: malloc failed"); *ref = NULL; return kDNSServiceErr_NoMemory; }
	sdr->next          = NULL;
	sdr->primary       = NULL;
	sdr->sockfd        = dnssd_InvalidSocket;
	sdr->op            = op;
	sdr->max_index     = 0;
	sdr->ProcessReply  = ProcessReply;
	sdr->AppCallback   = AppCallback;
	sdr->AppContext    = AppContext;

	if (flags & kDNSServiceFlagsShareConnection)
		{
		DNSServiceOp **p = &(*ref)->next;		// Append ourselves to end of primary's list
		while (*p) p = &(*p)->next;
		*p = sdr;
		sdr->primary = *ref;					// Set our primary pointer
		sdr->sockfd  = (*ref)->sockfd;			// Inherit primary's socket
		//printf("ConnectToServer sharing socket %d\n", sdr->sockfd);
		}
	else
		{
		*ref = NULL;
		sdr->sockfd = socket(AF_DNSSD, SOCK_STREAM, 0);
		if (!dnssd_SocketValid(sdr->sockfd))
			{
			syslog(LOG_WARNING, "dnssd_clientstub ConnectToServer: socket failed %d %s", errno, strerror(errno));
			free(sdr);
			return kDNSServiceErr_NoMemory;
			}
		#if defined(USE_TCP_LOOPBACK)
		saddr.sin_family      = AF_INET;
		saddr.sin_addr.s_addr = inet_addr(MDNS_TCP_SERVERADDR);
		saddr.sin_port        = htons(MDNS_TCP_SERVERPORT);
		#else
		saddr.sun_family      = AF_LOCAL;
		strcpy(saddr.sun_path, MDNS_UDS_SERVERPATH);
		#endif
	
		while (1)
			{
			int err = connect(sdr->sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
			if (!err) break; // If we succeeded, return sdr
			// If we failed, then it may be because the daemon is still launching.
			// This can happen for processes that launch early in the boot process, while the
			// daemon is still coming up. Rather than fail here, we'll wait a bit and try again.
			// If, after four seconds, we still can't connect to the daemon,
			// then we give up and return a failure code.
			if (++NumTries < DNSSD_CLIENT_MAXTRIES) sleep(1); // Sleep a bit, then try again
			else { dnssd_close(sdr->sockfd); free(sdr); return kDNSServiceErr_ServiceNotRunning; }
			}
		//printf("ConnectToServer opened socket %d\n", sdr->sockfd);
		}

	*ref = sdr;
	return kDNSServiceErr_NoError;
	}

static DNSServiceErrorType deliver_request(ipc_msg_hdr *hdr, DNSServiceOp *sdr)
	{
	uint32_t datalen = hdr->datalen;
	dnssd_sockaddr_t caddr, daddr;  // (client and daemon address structs)
	char *const data = (char *)hdr + sizeof(ipc_msg_hdr);
	dnssd_sock_t listenfd = dnssd_InvalidSocket, errsd = dnssd_InvalidSocket;
	int ret;
	dnssd_socklen_t len = (dnssd_socklen_t) sizeof(caddr);
	DNSServiceErrorType err;
	int MakeSeparateReturnSocket = 0;

	if (sdr->primary ||
		sdr->op == reg_record_request || sdr->op == add_record_request || sdr->op == update_record_request || sdr->op == remove_record_request)
		MakeSeparateReturnSocket = 1;

	if (!hdr)
		{ syslog(LOG_WARNING, "dnssd_clientstub deliver_request: !hdr"                  ); return kDNSServiceErr_Unknown; }
	if (!dnssd_SocketValid(sdr->sockfd))
		{ syslog(LOG_WARNING, "dnssd_clientstub deliver_request: sockfd %d", sdr->sockfd); return kDNSServiceErr_Unknown; }

	if (MakeSeparateReturnSocket)
		{
		// Setup temporary error socket
		listenfd = socket(AF_DNSSD, SOCK_STREAM, 0);
		if (!dnssd_SocketValid(listenfd)) goto cleanup;
		bzero(&caddr, sizeof(caddr));

		#if defined(USE_TCP_LOOPBACK)
			{
			union { uint16_t s; u_char b[2]; } port;
			caddr.sin_family      = AF_INET;
			caddr.sin_port        = 0;
			caddr.sin_addr.s_addr = inet_addr(MDNS_TCP_SERVERADDR);
			ret = bind(listenfd, (struct sockaddr*) &caddr, sizeof(caddr));
			if (ret < 0) goto cleanup;
			if (getsockname(listenfd, (struct sockaddr*) &caddr, &len) < 0) goto cleanup;
			listen(listenfd, 1);
			port.s = caddr.sin_port;
			data[0] = port.b[0];  // don't switch the byte order, as the
			data[1] = port.b[1];  // daemon expects it in network byte order
			}
		#else
			{
			mode_t mask = umask(0);
			caddr.sun_family = AF_LOCAL;
			// According to Stevens (section 3.2), there is no portable way to
			// determine whether sa_len is defined on a particular platform.
			#ifndef NOT_HAVE_SA_LEN
			caddr.sun_len = sizeof(struct sockaddr_un);
			#endif
			//syslog(LOG_WARNING, "dnssd_clientstub deliver_request: creating UDS: %s", data);
			strcpy(caddr.sun_path, data);
			ret = bind(listenfd, (struct sockaddr *)&caddr, sizeof(caddr));
			umask(mask);
			if (ret < 0) goto cleanup;
			listen(listenfd, 1);
			}
		#endif
		}

	// At this point, our listening socket is set up and waiting, if necessary, for the daemon to connect back to
	ConvertHeaderBytes(hdr);
	//syslog(LOG_WARNING, "dnssd_clientstub deliver_request writing %ld bytes", datalen + sizeof(ipc_msg_hdr));
	//if (MakeSeparateReturnSocket) syslog(LOG_WARNING, "dnssd_clientstub deliver_request name is %s", data);
	if (write_all(sdr->sockfd, (char *)hdr, datalen + sizeof(ipc_msg_hdr)) < 0) goto cleanup;

	if (!MakeSeparateReturnSocket) errsd = sdr->sockfd;
	else
		{
		//syslog(LOG_WARNING, "dnssd_clientstub deliver_request: accept");
		// At this point we may block in accept for a few milliseconds waiting for the daemon to connect back to us,
		// but that's okay -- the daemon is a trusted service and we know if won't take more than a few milliseconds to repond.
		len = sizeof(daddr);
		errsd = accept(listenfd, (struct sockaddr *)&daddr, &len);
		//syslog(LOG_WARNING, "dnssd_clientstub deliver_request: accept returned %d", errsd);
		if (!dnssd_SocketValid(errsd)) goto cleanup;
		}

	// At this point we may block in read_all for a few milliseconds waiting for the daemon to send us the error code,
	// but that's okay -- the daemon is a trusted service and we know if won't take more than a few milliseconds to repond.
	if (read_all(errsd, (char*)&err, (int)sizeof(err)) < 0)
		err = kDNSServiceErr_Unknown;	// On failure read_all will have written a message to syslog for us
	else
		err = ntohl(err);

	//syslog(LOG_WARNING, "dnssd_clientstub deliver_request: retrieved error code %d", err);

cleanup:
	if (MakeSeparateReturnSocket)
		{
		if (dnssd_SocketValid(listenfd)) dnssd_close(listenfd);
		if (dnssd_SocketValid(errsd))    dnssd_close(errsd);
#if !defined(USE_TCP_LOOPBACK)
		// syslog(LOG_WARNING, "dnssd_clientstub deliver_request: removing UDS: %s", data);
		if (unlink(data) != 0)
			syslog(LOG_WARNING, "dnssd_clientstub WARNING: unlink(\"%s\") failed errno %d (%s)", data, errno, strerror(errno));
		// else syslog(LOG_WARNING, "dnssd_clientstub deliver_request: removed UDS: %s", data);
#endif
		}
	free(hdr);
	return err;
	}

int DNSSD_API DNSServiceRefSockFD(DNSServiceRef sdRef)
	{
	if (!sdRef) return -1;
	return (int) sdRef->sockfd;
	}

// Handle reply from server, calling application client callback. If there is no reply
// from the daemon on the socket contained in sdRef, the call will block.
DNSServiceErrorType DNSSD_API DNSServiceProcessResult(DNSServiceRef sdRef)
	{
	int morebytes = 0;

	if (!sdRef || !dnssd_SocketValid(sdRef->sockfd) || !sdRef->ProcessReply)
		return kDNSServiceErr_BadReference;

	do
		{
		CallbackHeader cbh;
		char *data;
	
		// return NoError on EWOULDBLOCK. This will handle the case
		// where a non-blocking socket is told there is data, but it was a false positive.
		// On error, read_all will have written a message to syslog for us
		if (read_all(sdRef->sockfd, (void *)&cbh.ipc_hdr, sizeof(cbh.ipc_hdr)) < 0)
			{
			if (dnssd_errno() == dnssd_EWOULDBLOCK)
				{
				if (morebytes)
					syslog(LOG_WARNING, "dnssd_clientstub DNSServiceProcessResult error: select indicated data was waiting but read_all returned EWOULDBLOCK");
				return kDNSServiceErr_NoError;
				}
			else
				{
				syslog(LOG_WARNING, "dnssd_clientstub DNSServiceProcessResult error %d reading result from daemon", dnssd_errno());
				return kDNSServiceErr_Unknown;
				}
			}
	
		ConvertHeaderBytes(&cbh.ipc_hdr);
		if (cbh.ipc_hdr.version != VERSION) return kDNSServiceErr_Incompatible;
	
		data = malloc(cbh.ipc_hdr.datalen);
		if (!data) return kDNSServiceErr_NoMemory;
		if (read_all(sdRef->sockfd, data, cbh.ipc_hdr.datalen) < 0)
			{ free(data); return kDNSServiceErr_Unknown; } // read_all will have written a message to syslog for us
		else
			{
			char *ptr = data;
			cbh.cb_flags     = get_flags(&ptr);
			cbh.cb_interface = get_uint32(&ptr);
			cbh.cb_err       = get_error_code(&ptr);
			morebytes = more_bytes(sdRef->sockfd);
			if (morebytes) cbh.cb_flags |= kDNSServiceFlagsMoreComing;
			sdRef->ProcessReply(sdRef, &cbh, ptr);
			}
		free(data);
		} while (morebytes);

	return kDNSServiceErr_NoError;
	}

void DNSSD_API DNSServiceRefDeallocate(DNSServiceRef sdRef)
	{
	if (!sdRef) return;
	if (sdRef->primary)		// If this is a subordinate DNSServiceOp, just send a 'stop' command
		{
		DNSServiceOp **p = &sdRef->primary->next;
		while (*p && *p != sdRef) p = &(*p)->next;
		if (*p)
			{
			char *ptr;
			size_t len = 0;
			ipc_msg_hdr *hdr = create_hdr(cancel_request, &len, &ptr, 0, sdRef);
			ConvertHeaderBytes(hdr);
			write_all(sdRef->sockfd, (char *)hdr, len);
			*p = sdRef->next;
			free(sdRef);
			}
		}
	else					// else, make sure to terminate all subordinates as well
		{
		if (dnssd_SocketValid(sdRef->sockfd)) dnssd_close(sdRef->sockfd);
		while (sdRef)
			{
			DNSServiceOp *p = sdRef->next;
			sdRef = sdRef->next;
			free(p);
			}
		}
	}

DNSServiceErrorType DNSSD_API DNSServiceGetProperty(const char *property, void *result, uint32_t *size)
	{
	char *ptr;
	size_t len = strlen(property) + 1;
	ipc_msg_hdr *hdr;
	DNSServiceOp *tmp;
	uint32_t actualsize;

	DNSServiceErrorType err = ConnectToServer(&tmp, 0, getproperty_request, NULL, NULL, NULL);
	if (err) return err;

	hdr = create_hdr(getproperty_request, &len, &ptr, 0, NULL);
	if (!hdr) { DNSServiceRefDeallocate(tmp); return kDNSServiceErr_NoMemory; }

	put_string(property, &ptr);
	err = deliver_request(hdr, tmp);		// Will free hdr for us
	if (read_all(tmp->sockfd, (char*)&actualsize, (int)sizeof(actualsize)) < 0)
		{ DNSServiceRefDeallocate(tmp); return kDNSServiceErr_ServiceNotRunning; }

	actualsize = ntohl(actualsize);
	if (read_all(tmp->sockfd, (char*)result, actualsize < *size ? actualsize : *size) < 0)
		{ DNSServiceRefDeallocate(tmp); return kDNSServiceErr_ServiceNotRunning; }
	DNSServiceRefDeallocate(tmp);

	// Swap version result back to local process byte order
	if (!strcmp(property, kDNSServiceProperty_DaemonVersion) && *size >= 4)
		*(uint32_t*)result = ntohl(*(uint32_t*)result);

	*size = actualsize;
	return kDNSServiceErr_NoError;
	}

static void handle_resolve_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	char fullname[kDNSServiceMaxDomainName];
	char target[kDNSServiceMaxDomainName];
	uint16_t txtlen;
	union { uint16_t s; u_char b[2]; } port;
	unsigned char *txtrecord;
	int str_error = 0;

	if (get_string(&data, fullname, kDNSServiceMaxDomainName) < 0) str_error = 1;
	if (get_string(&data, target,   kDNSServiceMaxDomainName) < 0) str_error = 1;
	port.b[0] = *data++;
	port.b[1] = *data++;
	txtlen = get_uint16(&data);
	txtrecord = (unsigned char *)get_rdata(&data, txtlen);

	if (!cbh->cb_err && str_error)
		{ cbh->cb_err = kDNSServiceErr_Unknown; syslog(LOG_WARNING, "dnssd_clientstub handle_resolve_response: error reading result from daemon"); }

	((DNSServiceResolveReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_interface, cbh->cb_err, fullname, target, port.s, txtlen, txtrecord, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceResolve
	(
	DNSServiceRef                 *sdRef,
	DNSServiceFlags               flags,
	uint32_t                      interfaceIndex,
	const char                    *name,
	const char                    *regtype,
	const char                    *domain,
	DNSServiceResolveReply        callBack,
	void                          *context
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;

	if (!sdRef) return kDNSServiceErr_BadParam;
	if (!name || !regtype || !domain || !callBack) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, resolve_request, handle_resolve_response, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	// Calculate total message length
	len = sizeof(flags);
	len += sizeof(interfaceIndex);
	len += strlen(name) + 1;
	len += strlen(regtype) + 1;
	len += strlen(domain) + 1;

	hdr = create_hdr(resolve_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_string(name, &ptr);
	put_string(regtype, &ptr);
	put_string(domain, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}

static void handle_query_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	uint32_t ttl;
	char name[kDNSServiceMaxDomainName];
	uint16_t rrtype, rrclass, rdlen;
	char *rdata;
	int str_error = 0;

	if (get_string(&data, name, kDNSServiceMaxDomainName) < 0) str_error = 1;
	rrtype = get_uint16(&data);
	rrclass = get_uint16(&data);
	rdlen = get_uint16(&data);
	rdata = get_rdata(&data, rdlen);
	ttl = get_uint32(&data);

	if (!cbh->cb_err && str_error)
		{ cbh->cb_err = kDNSServiceErr_Unknown; syslog(LOG_WARNING, "dnssd_clientstub handle_query_response: error reading result from daemon"); }
	((DNSServiceQueryRecordReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_interface, cbh->cb_err, name, rrtype, rrclass,
													rdlen, rdata, ttl, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceQueryRecord
	(
	DNSServiceRef              *sdRef,
	DNSServiceFlags             flags,
	uint32_t                    interfaceIndex,
	const char                 *name,
	uint16_t                    rrtype,
	uint16_t                    rrclass,
	DNSServiceQueryRecordReply  callBack,
	void                       *context
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;

	if (!sdRef) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, query_request, handle_query_response, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	if (!name) name = "\0";

	// Calculate total message length
	len = sizeof(flags);
	len += sizeof(uint32_t);  //interfaceIndex
	len += strlen(name) + 1;
	len += 2 * sizeof(uint16_t);  // rrtype, rrclass

	hdr = create_hdr(query_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_string(name, &ptr);
	put_uint16(rrtype, &ptr);
	put_uint16(rrclass, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}

static void handle_addrinfo_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	uint32_t ttl;
	char hostname[kDNSServiceMaxDomainName];
	int str_error = 0;
	uint16_t rrtype, rrclass, rdlen;
	char *rdata;
	struct sockaddr_in  sa4;
	struct sockaddr_in6 sa6;
	struct sockaddr   * sa = NULL;

	if (get_string(&data, hostname, kDNSServiceMaxDomainName) < 0) str_error = 1;
	rrtype  = get_uint16(&data);
	rrclass = get_uint16(&data);
	rdlen   = get_uint16(&data);
	rdata   = get_rdata(&data, rdlen);
	ttl     = get_uint32(&data);
	
	if (!cbh->cb_err && str_error)
		{ cbh->cb_err = kDNSServiceErr_Unknown; syslog(LOG_WARNING, "dnssd_clientstub handle_addrinfo_response: error reading result from daemon"); }

	if (rrtype == kDNSServiceType_A)
		{
		sa = (struct sockaddr *)&sa4;
		bzero(&sa4, sizeof(sa4));
		#ifndef NOT_HAVE_SA_LEN
		sa->sa_len = sizeof(struct sockaddr_in);
		#endif
		sa->sa_family = AF_INET;
		if (!cbh->cb_err) memcpy(&sa4.sin_addr, rdata, rdlen);
		}
	else if (rrtype == kDNSServiceType_AAAA)
		{
		sa = (struct sockaddr *)&sa6;
		bzero(&sa6, sizeof(sa6));
		#ifndef NOT_HAVE_SA_LEN
		sa->sa_len = sizeof(struct sockaddr_in6);
		#endif
		sa->sa_family = AF_INET6;
		if (!cbh->cb_err) memcpy(&sa6.sin6_addr, rdata, rdlen);
		}

	((DNSServiceGetAddrInfoReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_interface, cbh->cb_err, hostname, sa, ttl, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceGetAddrInfo
	(
	DNSServiceRef                    *sdRef,
	DNSServiceFlags                  flags,
	uint32_t                         interfaceIndex,
	uint32_t                         protocol,
	const char                       *hostname,
	DNSServiceGetAddrInfoReply       callBack,
	void                             *context          /* may be NULL */
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;

	if (!sdRef)    return kDNSServiceErr_BadParam;
	if (!hostname) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, addrinfo_request, handle_addrinfo_response, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	// Calculate total message length
	len = sizeof(flags);
	len += sizeof(uint32_t);      //interfaceIndex
	len += sizeof(uint32_t);      // protocol
	len += strlen(hostname) + 1;

	hdr = create_hdr(addrinfo_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_uint32(protocol, &ptr);
	put_string(hostname, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}
	
static void handle_browse_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	char replyName[256], replyType[kDNSServiceMaxDomainName],
		replyDomain[kDNSServiceMaxDomainName];
	int str_error = 0;

	if (get_string(&data, replyName, 256) < 0) str_error = 1;
	if (get_string(&data, replyType, kDNSServiceMaxDomainName) < 0) str_error = 1;
	if (get_string(&data, replyDomain, kDNSServiceMaxDomainName) < 0) str_error = 1;
	if (!cbh->cb_err && str_error)
		{ cbh->cb_err = kDNSServiceErr_Unknown; syslog(LOG_WARNING, "dnssd_clientstub handle_browse_response: error reading result from daemon"); }
	((DNSServiceBrowseReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_interface, cbh->cb_err, replyName, replyType, replyDomain, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceBrowse
	(
	DNSServiceRef         *sdRef,
	DNSServiceFlags        flags,
	uint32_t               interfaceIndex,
	const char            *regtype,
	const char            *domain,
	DNSServiceBrowseReply  callBack,
	void                  *context
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;

	if (!sdRef) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, browse_request, handle_browse_response, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	if (!domain) domain = "";
	len = sizeof(flags);
	len += sizeof(interfaceIndex);
	len += strlen(regtype) + 1;
	len += strlen(domain) + 1;

	hdr = create_hdr(browse_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_string(regtype, &ptr);
	put_string(domain, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}

DNSServiceErrorType DNSSD_API DNSServiceSetDefaultDomainForUser
	(
	DNSServiceFlags  flags,
	const char      *domain
	)
	{
	DNSServiceOp *sdr;
	DNSServiceErrorType err;
	char *ptr = NULL;
	size_t len = sizeof(flags) + strlen(domain) + 1;
	ipc_msg_hdr *hdr = create_hdr(setdomain_request, &len, &ptr, 0, NULL);
	if (!hdr) return kDNSServiceErr_NoMemory;

	put_flags(flags, &ptr);
	put_string(domain, &ptr);

	err = deliver_request(hdr, sdr);		// Will free hdr for us
	DNSServiceRefDeallocate(sdr);
	return err;
	}

static void handle_regservice_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	char name[256], regtype[kDNSServiceMaxDomainName], domain[kDNSServiceMaxDomainName];
	int str_error = 0;

	if (get_string(&data, name, 256) < 0) str_error = 1;
	if (get_string(&data, regtype, kDNSServiceMaxDomainName) < 0) str_error = 1;
	if (get_string(&data, domain,  kDNSServiceMaxDomainName) < 0) str_error = 1;
	if (!cbh->cb_err && str_error)
		{ cbh->cb_err = kDNSServiceErr_Unknown; syslog(LOG_WARNING, "dnssd_clientstub handle_regservice_response: error reading result from daemon"); }
	((DNSServiceRegisterReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_err, name, regtype, domain, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceRegister
	(
	DNSServiceRef                       *sdRef,
	DNSServiceFlags                     flags,
	uint32_t                            interfaceIndex,
	const char                          *name,
	const char                          *regtype,
	const char                          *domain,
	const char                          *host,
	uint16_t                            PortInNetworkByteOrder,
	uint16_t                            txtLen,
	const void                          *txtRecord,
	DNSServiceRegisterReply             callBack,
	void                                *context
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;
	union { uint16_t s; u_char b[2]; } port = { PortInNetworkByteOrder };

	if (!sdRef) return kDNSServiceErr_BadParam;
	if (!name) name = "";
	if (!regtype) return kDNSServiceErr_BadParam;
	if (!domain) domain = "";
	if (!host) host = "";
	if (!txtRecord) txtRecord = (void*)"";

	// No callback must have auto-rename
	if (!callBack && (flags & kDNSServiceFlagsNoAutoRename)) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, reg_service_request, callBack ? handle_regservice_response : NULL, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	len = sizeof(DNSServiceFlags);
	len += sizeof(uint32_t);  // interfaceIndex
	len += strlen(name) + strlen(regtype) + strlen(domain) + strlen(host) + 4;
	len += 2 * sizeof(uint16_t);  // port, txtLen
	len += txtLen;

	hdr = create_hdr(reg_service_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }
	if (!callBack) hdr->ipc_flags |= IPC_FLAGS_NOREPLY;

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_string(name, &ptr);
	put_string(regtype, &ptr);
	put_string(domain, &ptr);
	put_string(host, &ptr);
	*ptr++ = port.b[0];
	*ptr++ = port.b[1];
	put_uint16(txtLen, &ptr);
	put_rdata(txtLen, txtRecord, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}

static void handle_enumeration_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	char domain[kDNSServiceMaxDomainName];
	int str_error = 0;

	if (get_string(&data, domain, kDNSServiceMaxDomainName) < 0) str_error = 1;
	if (!cbh->cb_err && str_error)
		{ cbh->cb_err = kDNSServiceErr_Unknown; syslog(LOG_WARNING, "dnssd_clientstub handle_enumeration_response: error reading result from daemon"); }
	((DNSServiceDomainEnumReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_interface, cbh->cb_err, domain, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceEnumerateDomains
	(
	DNSServiceRef             *sdRef,
	DNSServiceFlags            flags,
	uint32_t                   interfaceIndex,
	DNSServiceDomainEnumReply  callBack,
	void                      *context
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;
	int f1 = (flags & kDNSServiceFlagsBrowseDomains) != 0;
	int f2 = (flags & kDNSServiceFlagsRegistrationDomains) != 0;
	if (f1 + f2 != 1) return kDNSServiceErr_BadParam;

	if (!sdRef) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, enumeration_request, handle_enumeration_response, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	len = sizeof(DNSServiceFlags);
	len += sizeof(uint32_t);

	hdr = create_hdr(enumeration_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}

static void ConnectionResponse(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	DNSRecordRef rref = cbh->ipc_hdr.client_context.context;
	(void)data; // Unused

	//printf("ConnectionResponse got %d\n", cbh->ipc_hdr.op);
	if (cbh->ipc_hdr.op != reg_record_reply_op)
		{
		//DNSServiceOp *x = cbh->ipc_hdr.client_context.context;
		while (sdr && sdr != cbh->ipc_hdr.client_context.context) sdr = sdr->next;
		if (sdr) sdr->ProcessReply(sdr, cbh, data);
		// WARNING: Don't touch sdr after this -- clinet may have called DNSServiceRefDeallocate
		return;
		}

	if (sdr->op == connection_request)
		rref->AppCallback(rref->sdr, rref, cbh->cb_flags, cbh->cb_err, rref->AppContext);
	else
		{
		syslog(LOG_WARNING, "dnssd_clientstub handle_regrecord_response: sdr->op != connection_request");
		rref->AppCallback(rref->sdr, rref, 0, kDNSServiceErr_Unknown, rref->AppContext);
		}

	}

DNSServiceErrorType DNSSD_API DNSServiceCreateConnection(DNSServiceRef *sdRef)
	{
	char *ptr;
	size_t len = 0;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;
	if (!sdRef) return kDNSServiceErr_BadParam;
	err = ConnectToServer(sdRef, 0, connection_request, ConnectionResponse, NULL, NULL);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL
	
	hdr = create_hdr(connection_request, &len, &ptr, 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}

DNSServiceErrorType DNSSD_API DNSServiceRegisterRecord
	(
	DNSServiceRef                  sdRef,
	DNSRecordRef                  *RecordRef,
	DNSServiceFlags                flags,
	uint32_t                       interfaceIndex,
	const char                    *fullname,
	uint16_t                       rrtype,
	uint16_t                       rrclass,
	uint16_t                       rdlen,
	const void                    *rdata,
	uint32_t                       ttl,
	DNSServiceRegisterRecordReply  callBack,
	void                          *context
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr = NULL;
	DNSRecordRef rref = NULL;
	int f1 = (flags & kDNSServiceFlagsShared) != 0;
	int f2 = (flags & kDNSServiceFlagsUnique) != 0;
	if (f1 + f2 != 1) return kDNSServiceErr_BadParam;

	if (!sdRef || sdRef->op != connection_request || !dnssd_SocketValid(sdRef->sockfd))
		return kDNSServiceErr_BadReference;
	*RecordRef = NULL;

	len = sizeof(DNSServiceFlags);
	len += 2 * sizeof(uint32_t);  // interfaceIndex, ttl
	len += 3 * sizeof(uint16_t);  // rrtype, rrclass, rdlen
	len += strlen(fullname) + 1;
	len += rdlen;

	hdr = create_hdr(reg_record_request, &len, &ptr, 1, NULL);
	if (!hdr) return kDNSServiceErr_NoMemory;

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_string(fullname, &ptr);
	put_uint16(rrtype, &ptr);
	put_uint16(rrclass, &ptr);
	put_uint16(rdlen, &ptr);
	put_rdata(rdlen, rdata, &ptr);
	put_uint32(ttl, &ptr);

	rref = malloc(sizeof(DNSRecord));
	if (!rref) { free(hdr); return kDNSServiceErr_NoMemory; }
	rref->AppContext = context;
	rref->AppCallback = callBack;
	rref->record_index = sdRef->max_index++;
	rref->sdr = sdRef;
	*RecordRef = rref;
	hdr->client_context.context = rref;
	hdr->reg_index = rref->record_index;

	return deliver_request(hdr, sdRef);		// Will free hdr for us
	}

//sdRef returned by DNSServiceRegister()
DNSServiceErrorType DNSSD_API DNSServiceAddRecord
	(
	DNSServiceRef    sdRef,
	DNSRecordRef    *RecordRef,
	DNSServiceFlags  flags,
	uint16_t         rrtype,
	uint16_t         rdlen,
	const void      *rdata,
	uint32_t         ttl
	)
	{
	ipc_msg_hdr *hdr;
	size_t len = 0;
	char *ptr;
	DNSRecordRef rref;

	if (!sdRef || (sdRef->op != reg_service_request) || !RecordRef)
		return kDNSServiceErr_BadReference;
	*RecordRef = NULL;

	len += 2 * sizeof(uint16_t);  //rrtype, rdlen
	len += rdlen;
	len += sizeof(uint32_t);
	len += sizeof(DNSServiceFlags);

	hdr = create_hdr(add_record_request, &len, &ptr, 1, NULL);
	if (!hdr) return kDNSServiceErr_NoMemory;
	put_flags(flags, &ptr);
	put_uint16(rrtype, &ptr);
	put_uint16(rdlen, &ptr);
	put_rdata(rdlen, rdata, &ptr);
	put_uint32(ttl, &ptr);

	rref = malloc(sizeof(DNSRecord));
	if (!rref) { free(hdr); return kDNSServiceErr_NoMemory; }
	rref->AppContext = NULL;
	rref->AppCallback = NULL;
	rref->record_index = sdRef->max_index++;
	rref->sdr = sdRef;
	*RecordRef = rref;
	hdr->client_context.context = rref;
	hdr->reg_index = rref->record_index;
	return deliver_request(hdr, sdRef);		// Will free hdr for us
	}

//DNSRecordRef returned by DNSServiceRegisterRecord or DNSServiceAddRecord
DNSServiceErrorType DNSSD_API DNSServiceUpdateRecord
	(
	DNSServiceRef    sdRef,
	DNSRecordRef     RecordRef,
	DNSServiceFlags  flags,
	uint16_t         rdlen,
	const void      *rdata,
	uint32_t         ttl
	)
	{
	ipc_msg_hdr *hdr;
	size_t len = 0;
	char *ptr;

	if (!sdRef) return kDNSServiceErr_BadReference;

	len += sizeof(uint16_t);
	len += rdlen;
	len += sizeof(uint32_t);
	len += sizeof(DNSServiceFlags);

	hdr = create_hdr(update_record_request, &len, &ptr, 1, NULL);
	if (!hdr) return kDNSServiceErr_NoMemory;
	hdr->reg_index = RecordRef ? RecordRef->record_index : TXT_RECORD_INDEX;
	put_flags(flags, &ptr);
	put_uint16(rdlen, &ptr);
	put_rdata(rdlen, rdata, &ptr);
	put_uint32(ttl, &ptr);
	return deliver_request(hdr, sdRef);		// Will free hdr for us
	}

DNSServiceErrorType DNSSD_API DNSServiceRemoveRecord
	(
	DNSServiceRef    sdRef,
	DNSRecordRef     RecordRef,
	DNSServiceFlags  flags
	)
	{
	ipc_msg_hdr *hdr;
	size_t len = 0;
	char *ptr;
	DNSServiceErrorType err;

	if (!sdRef || !RecordRef || !sdRef->max_index)
		return kDNSServiceErr_BadReference;

	len += sizeof(flags);
	hdr = create_hdr(remove_record_request, &len, &ptr, 1, NULL);
	if (!hdr) return kDNSServiceErr_NoMemory;
	hdr->reg_index = RecordRef->record_index;
	put_flags(flags, &ptr);
	err = deliver_request(hdr, sdRef);		// Will free hdr for us
	if (!err) free(RecordRef);
	return err;
	}

DNSServiceErrorType DNSSD_API DNSServiceReconfirmRecord
	(
	DNSServiceFlags  flags,
	uint32_t         interfaceIndex,
	const char      *fullname,
	uint16_t         rrtype,
	uint16_t         rrclass,
	uint16_t         rdlen,
	const void      *rdata
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceOp *tmp;

	DNSServiceErrorType err = ConnectToServer(&tmp, flags, reconfirm_record_request, NULL, NULL, NULL);
	if (err) return err;

	len = sizeof(DNSServiceFlags);
	len += sizeof(uint32_t);
	len += strlen(fullname) + 1;
	len += 3 * sizeof(uint16_t);
	len += rdlen;
	hdr = create_hdr(reconfirm_record_request, &len, &ptr, 0, NULL);
	if (!hdr) { DNSServiceRefDeallocate(tmp); return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_string(fullname, &ptr);
	put_uint16(rrtype, &ptr);
	put_uint16(rrclass, &ptr);
	put_uint16(rdlen, &ptr);
	put_rdata(rdlen, rdata, &ptr);

	err = deliver_request(hdr, tmp);		// Will free hdr for us
	DNSServiceRefDeallocate(tmp);
	return err;
	}

static void handle_port_mapping_response(DNSServiceOp *sdr, CallbackHeader *cbh, char *data)
	{
	union { uint32_t l; u_char b[4]; } addr;
	uint8_t protocol;
	union { uint16_t s; u_char b[2]; } privatePort;
	union { uint16_t s; u_char b[2]; } publicPort;
	uint32_t ttl;

	addr       .b[0] = *data++;
	addr       .b[1] = *data++;
	addr       .b[2] = *data++;
	addr       .b[3] = *data++;
	protocol         = *data++;
	privatePort.b[0] = *data++;
	privatePort.b[1] = *data++;
	publicPort .b[0] = *data++;
	publicPort .b[1] = *data++;
	ttl              = get_uint32(&data);

	((DNSServiceNATPortMappingReply)sdr->AppCallback)(sdr, cbh->cb_flags, cbh->cb_interface, cbh->cb_err, addr.l, protocol, privatePort.s, publicPort.s, ttl, sdr->AppContext);
	}

DNSServiceErrorType DNSSD_API DNSServiceNATPortMappingCreate
	(
	DNSServiceRef                       *sdRef,
	DNSServiceFlags                     flags,
	uint32_t                            interfaceIndex,
	uint32_t                            protocol,     /* TCP and/or UDP */
	uint16_t                            privatePortInNetworkByteOrder,
	uint16_t                            publicPortInNetworkByteOrder,
	uint32_t                            ttl,          /* time to live in seconds */
	DNSServiceNATPortMappingReply       callBack,
	void                                *context      /* may be NULL */
	)
	{
	char *ptr;
	size_t len;
	ipc_msg_hdr *hdr;
	DNSServiceErrorType err;
	union { uint16_t s; u_char b[2]; } privatePort = { privatePortInNetworkByteOrder };
	union { uint16_t s; u_char b[2]; } publicPort  = { publicPortInNetworkByteOrder };

	if (!sdRef) return kDNSServiceErr_BadParam;

	err = ConnectToServer(sdRef, flags, port_mapping_request, handle_port_mapping_response, callBack, context);
	if (err) return err;	// On error ConnectToServer leaves *sdRef set to NULL

	len = sizeof(flags);
	len += sizeof(interfaceIndex);
	len += sizeof(protocol);
	len += sizeof(privatePort);
	len += sizeof(publicPort);
	len += sizeof(ttl);

	hdr = create_hdr(port_mapping_request, &len, &ptr, (*sdRef)->primary ? 1 : 0, *sdRef);
	if (!hdr) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; return kDNSServiceErr_NoMemory; }

	put_flags(flags, &ptr);
	put_uint32(interfaceIndex, &ptr);
	put_uint32(protocol, &ptr);
	*ptr++ = privatePort.b[0];
	*ptr++ = privatePort.b[1];
	*ptr++ = publicPort .b[0];
	*ptr++ = publicPort .b[1];
	put_uint32(ttl, &ptr);

	err = deliver_request(hdr, *sdRef);		// Will free hdr for us
	if (err) { DNSServiceRefDeallocate(*sdRef); *sdRef = NULL; }
	return err;
	}
