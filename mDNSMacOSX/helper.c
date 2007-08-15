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
#include <sys/cdefs.h>
#include <arpa/inet.h>
#include <bsm/libbsm.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#include <netinet6/ipsec.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <asl.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <Security/Security.h>
#include <SystemConfiguration/SCDynamicStore.h>
#include <SystemConfiguration/SCPreferencesSetSpecific.h>
#include "mDNSEmbeddedAPI.h"
#include "dns_sd.h"
#include "dnssd_ipc.h"
#include "libpfkey.h"
#include "helper.h"
#include "helpermsgServer.h"
#include "helper-server.h"

typedef struct sadb_x_policy *ipsec_policy_t;

uid_t mDNSResponderUID;
gid_t mDNSResponderGID;
static const char kTunnelAddressInterface[] = "lo0";

#define debug(...) debug_(__func__, __VA_ARGS__)

static void
debug_(const char *func, const char *fmt, ...)
{
	char buf[2048];
	va_list ap;
	ssize_t n = snprintf(buf, sizeof(buf), "%s: ", func);

	if (n >= (int)sizeof(buf))
		return;
	va_start(ap, fmt);
	vsnprintf(&buf[n], sizeof(buf)-n, fmt, ap);
	va_end(ap);
	helplog(ASL_LEVEL_DEBUG, buf);
}

static int
authorized(audit_token_t *token)
{
	int ok = 0;
	pid_t pid = (pid_t)-1;
	uid_t euid = (uid_t)-1;

	audit_token_to_au32(*token, NULL, &euid, NULL, NULL, NULL, &pid, NULL,
	    NULL);
	ok = (euid == mDNSResponderUID || euid == 0);
	if (!ok)
		helplog(ASL_LEVEL_NOTICE,
		    "Unauthorized access by euid=%lu pid=%lu",
		    (unsigned long)euid, (unsigned long)pid);
	return ok;
}

static void
closefds(int from)
{
	int fd = 0;
	struct dirent entry, *entryp = NULL;
	DIR *dirp = opendir("/dev/fd");

	if (dirp == NULL) {
		/* fall back to the erroneous getdtablesize method */
		for (fd = from; fd < getdtablesize(); ++fd)
			close(fd);
		return;
	}
	while (0 == readdir_r(dirp, &entry, &entryp) && NULL != entryp) {
		fd = atoi(entryp->d_name);
		if (fd >= from && fd != dirp->__dd_fd)
			close(fd);
	}
	closedir(dirp);
}

kern_return_t
do_mDNSIdleExit(__unused mach_port_t port, audit_token_t token)
{
	if (!authorized(&token))
		goto fin;
	helplog(ASL_LEVEL_NOTICE, "Idle exit");
	exit(0);

fin:
	return KERN_SUCCESS;
}

kern_return_t
do_mDNSDynamicStoreSetConfig(__unused mach_port_t port, int key,
    vm_offset_t value, mach_msg_type_number_t valueCnt, int *err,
    audit_token_t token)
{
	CFStringRef sckey = NULL;
	CFDataRef bytes = NULL;
	CFPropertyListRef plist = NULL;
	SCDynamicStoreRef store = NULL;

	debug("entry");
	*err = 0;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	switch ((enum mDNSDynamicStoreSetConfigKey)key) {
	case kmDNSMulticastConfig:
		sckey = CFSTR("State:/Network/" kDNSServiceCompMulticastDNS);
		break;
	case kmDNSDynamicConfig:
		sckey = CFSTR("State:/Network/DynamicDNS");
		break;
	case kmDNSPrivateConfig:
		sckey = CFSTR("State:/Network/" kDNSServiceCompPrivateDNS);
		break;
	case kmDNSBackToMyMacConfig:
		sckey = CFSTR("State:/Network/BackToMyMac");
		break;
	default:
		debug("unrecognized key %d", key);
		*err = kmDNSHelperInvalidConfigKey;
		goto fin;
	}
	if (NULL == (bytes = CFDataCreateWithBytesNoCopy(NULL, (void *)value,
	    valueCnt, kCFAllocatorNull))) {
		debug("CFDataCreateWithBytesNoCopy of value failed");
		*err = kmDNSHelperCreationFailed;
		goto fin;
	}
	if (NULL == (plist = CFPropertyListCreateFromXMLData(NULL, bytes,
	    kCFPropertyListImmutable, NULL))) {
		debug("CFPropertyListCreateFromXMLData of bytes failed");
		*err = kmDNSHelperInvalidPList;
		goto fin;
	}
	CFRelease(bytes);
	bytes = NULL;
	if (NULL == (store = SCDynamicStoreCreate(NULL,
	    CFSTR(kmDNSHelperServiceName), NULL, NULL))) {
		debug("SCDynamicStoreCreate failed");
		*err = kmDNSHelperDynamicStoreFailed;
		goto fin;
	}
	SCDynamicStoreSetValue(store, sckey, plist);
	*err = 0;
	debug("succeeded");

fin:
	if (0 != *err)
		debug("failed err=%d", *err);
	if (NULL != bytes)
		CFRelease(bytes);
	if (NULL != plist)
		CFRelease(plist);
	if (NULL != store)
		CFRelease(store);
	vm_deallocate(mach_task_self(), value, valueCnt);
	update_idle_timer();
	return KERN_SUCCESS;
}

kern_return_t
do_mDNSPreferencesSetName(__unused mach_port_t port, int key, vm_offset_t value,
    mach_msg_type_number_t valueCnt, unsigned int encoding, int *err,
    audit_token_t token)
{
	SCPreferencesRef session = NULL;
	Boolean ok = FALSE;
	Boolean locked = FALSE;
	CFDataRef bytes = NULL;
	CFPropertyListRef plist = NULL;

	debug("entry");
	*err = 0;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	switch ((enum mDNSPreferencesSetNameKey)key) {
	case kmDNSComputerName:
	case kmDNSLocalHostName:
		break;
	default:
		debug("unrecognized key: %d", key);
		*err = kmDNSHelperInvalidNameKey;
		goto fin;
	}
	if (NULL == (bytes = CFDataCreateWithBytesNoCopy(NULL, (void *)value,
	    valueCnt, kCFAllocatorNull))) {
		debug("CFDataCreateWithBytesNoCopy for value failed");
		*err = kmDNSHelperCreationFailed;
		goto fin;
	}
	if (NULL == (plist = CFPropertyListCreateFromXMLData(NULL, bytes,
	    kCFPropertyListImmutable, NULL))) {
		debug("CFPropertyListCreateFromXMLData for bytes failed");
		*err = kmDNSHelperInvalidPList;
		goto fin;
	}
	CFRelease(bytes);
	bytes = NULL;
	if (CFStringGetTypeID() != CFGetTypeID(plist)) {
		debug("expected CFString");
		*err = kmDNSHelperTypeError;
		goto fin;
	}
	if (NULL == (session = SCPreferencesCreate(NULL,
	    CFSTR(kmDNSHelperServiceName), NULL))) {
		debug("SCPreferencesCreate failed");
		*err = kmDNSHelperPreferencesFailed;
		goto fin;
	}
	if (!SCPreferencesLock(session, 0)) {
		debug("lock failed");
		*err = kmDNSHelperPreferencesLockFailed;
		goto fin;
	}
	locked = TRUE;
	switch ((enum mDNSPreferencesSetNameKey)key) {
	case kmDNSComputerName:
		ok = SCPreferencesSetComputerName(session, plist, encoding);
		break;
	case kmDNSLocalHostName:
		ok = SCPreferencesSetLocalHostName(session, plist);
		break;
	default:
		break;
	}
	if (!ok || !SCPreferencesCommitChanges(session) ||
	    !SCPreferencesApplyChanges(session)) {
		debug("SCPreferences update failed");
		*err = kmDNSHelperPreferencesSetFailed;
		goto fin;
	}
	*err = 0;
	debug("succeeded");

fin:
	if (0 != *err)
		debug("failed err=%d", *err);
	if (NULL != plist)
		CFRelease(plist);
	if (NULL != bytes)
		CFRelease(bytes);
	if (NULL != session) {
		if (locked)
			SCPreferencesUnlock(session);
		CFRelease(session);
	}
	vm_deallocate(mach_task_self(), value, valueCnt);
	update_idle_timer();
	return KERN_SUCCESS;
}

enum DNSKeyFormat {
	formatNotDNSKey, formatDdnsTypeItem, formatDnsPrefixedServiceItem
};
static const char dnsprefix[] = "dns:";
static const char ddns[] = "ddns";
static const char ddnsrev[] = "sndd";

static enum DNSKeyFormat
getDNSKeyFormat(SecKeychainItemRef item, SecKeychainAttributeList **attributesp)
{
	static UInt32 tags[3] = {
		kSecTypeItemAttr, kSecServiceItemAttr, kSecAccountItemAttr
	};
	static SecKeychainAttributeInfo attributeInfo = {
		sizeof(tags)/sizeof(tags[0]), tags, NULL
	};
	SecKeychainAttributeList *attributes = NULL;
	enum DNSKeyFormat format;
	Boolean malformed = FALSE;
	OSStatus status = noErr;
	int i = 0;

	*attributesp = NULL;
	if (noErr != (status = SecKeychainItemCopyAttributesAndData(item,
	    &attributeInfo, NULL, &attributes, NULL, NULL))) {
		debug("SecKeychainItemCopyAttributesAndData %d - skipping",
		    status);
		goto skip;
	}
	if (attributeInfo.count != attributes->count)
		malformed = TRUE;
	for (i = 0; !malformed && i < (int)attributeInfo.count; ++i)
		if (attributeInfo.tag[i] != attributes->attr[i].tag)
			malformed = TRUE;
	if (malformed) {
		debug(
       "malformed result from SecKeychainItemCopyAttributesAndData - skipping");
		goto skip;
	}
	debug("entry (\"%.*s\", \"%.*s\", \"%.*s\")",
	    (int)attributes->attr[0].length, attributes->attr[0].data,
	    (int)attributes->attr[1].length, attributes->attr[1].data,
	    (int)attributes->attr[2].length, attributes->attr[2].data);
	if (attributes->attr[1].length >= MAX_ESCAPED_DOMAIN_NAME +
	    sizeof(dnsprefix)-1) {
		debug("kSecServiceItemAttr too long (%u) - skipping",
		    (unsigned int)attributes->attr[1].length);
		goto skip;
	}
	if (attributes->attr[2].length >= MAX_ESCAPED_DOMAIN_NAME) {
		debug("kSecAccountItemAttr too long (%u) - skipping",
		    (unsigned int)attributes->attr[2].length);
		goto skip;
	}
	if (attributes->attr[1].length >= sizeof(dnsprefix)-1 &&
	    0 == strncasecmp(attributes->attr[1].data, dnsprefix,
	    sizeof(dnsprefix)-1))
		format = formatDnsPrefixedServiceItem;
	else if (attributes->attr[0].length == sizeof(ddns)-1 &&
	    0 == strncasecmp(attributes->attr[0].data, ddns, sizeof(ddns)-1))
		format = formatDdnsTypeItem;
	else if (attributes->attr[0].length == sizeof(ddnsrev)-1 &&
	    0 == strncasecmp(attributes->attr[0].data, ddnsrev, sizeof(ddnsrev)-1))
		format = formatDdnsTypeItem;
	else {
		debug("uninterested in this entry");
		goto skip;
	}
	*attributesp = attributes;
	debug("accepting this entry");
	return format;

skip:
	SecKeychainItemFreeAttributesAndData(attributes, NULL);
	return formatNotDNSKey;
}

static CFPropertyListRef
getKeychainItemInfo(SecKeychainItemRef item,
    SecKeychainAttributeList *attributes, enum DNSKeyFormat format)
{
	CFMutableArrayRef entry = NULL;
	CFDataRef data = NULL;
	OSStatus status = noErr;
	UInt32 keylen = 0;
	void *keyp = 0;

	if (NULL == (entry = CFArrayCreateMutable(NULL, 0,
	    &kCFTypeArrayCallBacks))) {
		debug("CFArrayCreateMutable failed");
		goto error;
	}
	switch ((enum DNSKeyFormat)format) {
	case formatDdnsTypeItem:
		data = CFDataCreate(kCFAllocatorDefault,
		    attributes->attr[1].data, attributes->attr[1].length);
		break;
	case formatDnsPrefixedServiceItem:
		data = CFDataCreate(kCFAllocatorDefault,
		    attributes->attr[1].data + sizeof(dnsprefix)-1,
		    attributes->attr[1].length - (sizeof(dnsprefix)-1));
	default:
		assert("unknown DNSKeyFormat value");
		break;
	}
	if (NULL == data) {
		debug("CFDataCreate for attr[1] failed");
		goto error;
	}
	CFArrayAppendValue(entry, data);
	CFRelease(data);
	if (NULL == (data = CFDataCreate(kCFAllocatorDefault,
	    attributes->attr[2].data, attributes->attr[2].length))) {
		debug("CFDataCreate for attr[2] failed");
		goto error;
	}
	CFArrayAppendValue(entry, data);
	CFRelease(data);
	if (noErr != (status = SecKeychainItemCopyAttributesAndData(item, NULL,
	    NULL, NULL, &keylen, &keyp))) {
		debug("could not retrieve key for \"%.*s\": %d",
		    (int)attributes->attr[1].length, attributes->attr[1].data,
		    status);
		goto error;
	}
	data = CFDataCreate(kCFAllocatorDefault, keyp, keylen);
	SecKeychainItemFreeAttributesAndData(NULL, keyp);
	if (NULL == data) {
		debug("CFDataCreate for keyp failed");
		goto error;
	}
	CFArrayAppendValue(entry, data);
	CFRelease(data);
	return entry;

error:
	if (NULL != entry)
		CFRelease(entry);
	return NULL;
}

kern_return_t
do_mDNSKeychainGetSecrets(__unused mach_port_t port, unsigned int *numsecrets,
    vm_offset_t *secrets, mach_msg_type_number_t *secretsCnt, int *err,
    audit_token_t token)
{
	CFWriteStreamRef stream = NULL;
	CFDataRef result = NULL;
	CFPropertyListRef entry = NULL;
	CFMutableArrayRef keys = NULL;
	SecKeychainItemRef item = NULL;
	SecKeychainSearchRef search = NULL;
	SecKeychainAttributeList *attributes = NULL;
	enum DNSKeyFormat format;
	OSStatus status = 0;

	debug("entry");
	*err = 0;
	*numsecrets = 0;
	*secrets = (vm_offset_t)NULL;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	if (NULL == (keys = CFArrayCreateMutable(NULL, 0,
	    &kCFTypeArrayCallBacks))) {
		debug("CFArrayCreateMutable failed");
		*err = kmDNSHelperCreationFailed;
		goto fin;
	}
	if (noErr != (status = SecKeychainSearchCreateFromAttributes(NULL,
	    kSecGenericPasswordItemClass, NULL, &search))) {
		*err = kmDNSHelperKeychainSearchCreationFailed;
		goto fin;
	}
	for (status = SecKeychainSearchCopyNext(search, &item);
	     noErr == status;
	     status = SecKeychainSearchCopyNext(search, &item)) {
		if (formatNotDNSKey != (format = getDNSKeyFormat(item,
		    &attributes)) &&
		    NULL != (entry = getKeychainItemInfo(item, attributes,
		    format))) {
			CFArrayAppendValue(keys, entry);
			CFRelease(entry);
		}
		SecKeychainItemFreeAttributesAndData(attributes, NULL);
		CFRelease(item);
	}
	if (errSecItemNotFound != status)
		helplog(ASL_LEVEL_ERR, "%s: SecKeychainSearchCopyNext failed: %d",
		    __func__, status);
	if (NULL == (stream =
	    CFWriteStreamCreateWithAllocatedBuffers(kCFAllocatorDefault,
	    kCFAllocatorDefault))) {
		*err = kmDNSHelperCreationFailed;
		debug("CFWriteStreamCreateWithAllocatedBuffers failed");
		goto fin;
	}
	CFWriteStreamOpen(stream);
	if (0 == CFPropertyListWriteToStream(keys, stream,
	    kCFPropertyListBinaryFormat_v1_0, NULL)) {
		*err = kmDNSHelperPListWriteFailed;
		debug("CFPropertyListWriteToStream failed");
		goto fin;
	}
	result = CFWriteStreamCopyProperty(stream,
	    kCFStreamPropertyDataWritten);
	if (KERN_SUCCESS != vm_allocate(mach_task_self(), secrets,
	    CFDataGetLength(result), VM_FLAGS_ANYWHERE)) {
		*err = kmDNSHelperCreationFailed;
		debug("vm_allocate failed");
		goto fin;
	}
	CFDataGetBytes(result, CFRangeMake(0, CFDataGetLength(result)),
	    (void *)*secrets);
	*secretsCnt = CFDataGetLength(result);
	*numsecrets = CFArrayGetCount(keys);
	debug("succeeded");

fin:
	debug("returning %u secrets", *numsecrets);
	if (NULL != stream) {
		CFWriteStreamClose(stream);
		CFRelease(stream);
	}
	if (NULL != result)
		CFRelease(result);
	if (NULL != keys)
		CFRelease(keys);
	if (NULL != search)
		CFRelease(search);
	update_idle_timer();
	return KERN_SUCCESS;
}

kern_return_t
do_mDNSCreateStateDir(__unused mach_port_t port, int *err, audit_token_t token)
{
	static char oldsocketpath[] = "/var/run/mDNSResponder";
	char path[] = MDNS_UDS_SERVERPATH;
	char *p = NULL;

	debug("entry");
	*err = 0;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	p = strrchr(path, '/');
	*p = '\0';
	if (0 > mkdir(path, 0755) && EEXIST != errno) {
		*err = kmDNSHelperDirectoryCreationFailed;
		debug("mkdir(\"%s\", 0755) failed: %s", path, strerror(errno));
		goto fin;
	}
	if (0 > chown(path, mDNSResponderUID, mDNSResponderGID)) {
		*err = kmDNSHelperSetOwnershipFailed;
		debug("chown(\"%s\", %lu, %lu) failed: %s", path,
		    (unsigned long)mDNSResponderUID,
		    (unsigned long)mDNSResponderGID, strerror(errno));
		goto fin;
	}
	(void)unlink(oldsocketpath);
	if (0 > symlink(MDNS_UDS_SERVERPATH, oldsocketpath)) {
		*err = kmDNSHelperSymlinkFailed;
		debug("symlink(\"%s\", \"%s\") failed: %s", MDNS_UDS_SERVERPATH,
		    oldsocketpath, strerror(errno));
		goto fin;
	}
	debug("succeeded");

fin:
	update_idle_timer();
	return KERN_SUCCESS;
}

static int
aliasTunnelAddress(v6addr_t address)
{
	struct in6_aliasreq ifra_in6;
	int err = 0;
	int s = -1;

	if (0 > (s = socket(AF_INET6, SOCK_DGRAM, 0))) {
		helplog(ASL_LEVEL_ERR, "socket(AF_INET6, ...) failed: %s",
		    strerror(errno));
		err = kmDNSHelperDatagramSocketCreationFailed;
		goto fin;
	}
	bzero(&ifra_in6, sizeof(ifra_in6));
	strlcpy(ifra_in6.ifra_name, kTunnelAddressInterface,
	    sizeof(ifra_in6.ifra_name));
	ifra_in6.ifra_lifetime.ia6t_vltime = ND6_INFINITE_LIFETIME;
	ifra_in6.ifra_lifetime.ia6t_pltime = ND6_INFINITE_LIFETIME;

	ifra_in6.ifra_addr.sin6_family = AF_INET6;
	ifra_in6.ifra_addr.sin6_len = sizeof(struct sockaddr_in6);
	memcpy(&(ifra_in6.ifra_addr.sin6_addr), address,
	    sizeof(ifra_in6.ifra_addr.sin6_addr));

	ifra_in6.ifra_prefixmask.sin6_family = AF_INET6;
	ifra_in6.ifra_prefixmask.sin6_len = sizeof(struct sockaddr_in6);
	memset(&(ifra_in6.ifra_prefixmask.sin6_addr), 0xFF,
	    sizeof(ifra_in6.ifra_prefixmask.sin6_addr));

	if (0 > ioctl(s, SIOCAIFADDR_IN6, &ifra_in6)) {
		helplog(ASL_LEVEL_ERR,
		    "ioctl(..., SIOCAIFADDR_IN6, ...) failed: %s",
		    strerror(errno));
		err = kmDNSHelperInterfaceCreationFailed;
		goto fin;
	}

fin:
	if (0 <= s)
		close(s);
	return err;
}

static int
unaliasTunnelAddress(v6addr_t address)
{
	struct in6_ifreq ifr;
	int err = 0;
	int s = -1;

	if (0 > (s = socket(AF_INET6, SOCK_DGRAM, 0))) {
		helplog(ASL_LEVEL_ERR, "socket(AF_INET6, ...) failed: %s",
		    strerror(errno));
		err = kmDNSHelperDatagramSocketCreationFailed;
		goto fin;
	}
	bzero(&ifr, sizeof(ifr));
	strlcpy(ifr.ifr_name, kTunnelAddressInterface, sizeof(ifr.ifr_name));
	ifr.ifr_ifru.ifru_addr.sin6_family = AF_INET6;
	ifr.ifr_ifru.ifru_addr.sin6_len = sizeof(struct sockaddr_in6);
	memcpy(&(ifr.ifr_ifru.ifru_addr.sin6_addr), address,
	    sizeof(ifr.ifr_ifru.ifru_addr.sin6_addr));

	if (0 > ioctl(s, SIOCDIFADDR_IN6, &ifr)) {
		helplog(ASL_LEVEL_ERR,
		    "ioctl(..., SIOCDIFADDR_IN6, ...) failed: %s",
		    strerror(errno));
		err = kmDNSHelperInterfaceDeletionFailed;
		goto fin;
	}

fin:
	if (0 <= s)
		close(s);
	return err;
}

int
do_mDNSAutoTunnelInterfaceUpDown(__unused mach_port_t port, int updown,
    v6addr_t address, int *err, audit_token_t token)
{
	debug("entry");
	*err = 0;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	switch ((enum mDNSAutoTunnelInterfaceUpDown)updown) {
	case kmDNSAutoTunnelInterfaceUp:
		*err = aliasTunnelAddress(address);
		break;
	case kmDNSAutoTunnelInterfaceDown:
		*err = unaliasTunnelAddress(address);
		break;
	default:
		*err = kmDNSHelperInvalidInterfaceState;
		goto fin;
	}
	debug("succeeded");

fin:
	update_idle_timer();
	return KERN_SUCCESS;
}

static int
createAnonymousRacoonConfiguration(const char *keydata)
{
	static const char config1[] =
	  "remote anonymous {\n"
	  "  exchange_mode aggressive;\n"
	  "  doi ipsec_doi;\n"
	  "  situation identity_only;\n"
	  "  verify_identifier off;\n"
	  "  generate_policy on;\n"
	  "  shared_secret use \"";
	static const char config2[] =
	  "\";\n"
	  "  nonce_size 16;\n"
	  "  lifetime time 5 min;\n"
	  "  initial_contact on;\n"
	  "  support_proxy on;\n"
	  "  nat_traversal force;\n"
	  "  proposal_check claim;\n"
	  "  proposal {\n"
	  "    encryption_algorithm aes;\n"
	  "    hash_algorithm sha1;\n"
	  "    authentication_method pre_shared_key;\n"
	  "    dh_group 2;\n"
	  "    lifetime time 5 min;\n"
	  "  }\n"
	  "}\n\n"
	  "sainfo anonymous { \n"
	  "  pfs_group 2;\n"
	  "  lifetime time 60 min;\n"
	  "  encryption_algorithm aes;\n"
	  "  authentication_algorithm hmac_sha1;\n"
	  "  compression_algorithm deflate;\n"
	  "}\n";
	char tmp_config_path[] =
	    "/etc/racoon/remote/tmp.XXXXXX";
	static const char racoon_config_path[] =
	    "/etc/racoon/remote/anonymous.conf";
	int fd = mkstemp(tmp_config_path);
	if (0 > fd) {
		helplog(ASL_LEVEL_ERR, "mkstemp \"%s\" failed: %s",
		    tmp_config_path, strerror(errno));
		return -1;
	}
	write(fd, config1, sizeof(config1)-1);
	write(fd, keydata, strlen(keydata));
	write(fd, config2, sizeof(config2)-1);
	close(fd);
	if (0 > rename(tmp_config_path, racoon_config_path)) {
		unlink(tmp_config_path);
		helplog(ASL_LEVEL_ERR, "rename \"%s\" \"%s\" failed: %s",
		    tmp_config_path, racoon_config_path, strerror(errno));
		return -1;
	}
	return 0;
}

static int
notifyRacoon(void)
{
	debug("entry");
	static const char racoon_pid_path[] = "/var/run/racoon.pid";
	char buf[] = "18446744073709551615"; /* largest 64-bit integer */
	char *p = NULL;
	ssize_t n = 0;
	unsigned long m = 0;
	int fd = open(racoon_pid_path, O_RDONLY);

	if (0 > fd) {
		debug("open \"%s\" failed, and that's OK: %s", racoon_pid_path,
		    strerror(errno));
		return kmDNSHelperRacoonNotificationFailed;
	}
	n = read(fd, buf, sizeof(buf)-1);
	close(fd);
	if (1 > n) {
		debug("read of \"%s\" failed: %s", racoon_pid_path,
		    n == 0 ? "empty file" : strerror(errno));
		return kmDNSHelperRacoonNotificationFailed;
	}
	buf[n] = '\0';
	m = strtoul(buf, &p, 10);
	if (*p != '\0' && !isspace(*p)) {
		debug("invalid PID \"%s\" (around '%c')", buf, *p);
		return kmDNSHelperRacoonNotificationFailed;
	}
	if (2 > m) {
		debug("refusing to kill PID %lu", m);
		return kmDNSHelperRacoonNotificationFailed;
	}
	if (0 != kill(m, SIGHUP)) {
		debug("Could not signal racoon (%lu): %s", m, strerror(errno));
		return kmDNSHelperRacoonNotificationFailed;
	}
	debug("Sent SIGHUP to racoon (%lu)", m);
	return 0;
}

static int
startRacoon(void)
{
	debug("entry");
	char * const racoon_args[] = { "/usr/sbin/racoon", NULL };
	ssize_t n = 0;
	pid_t pid = 0;
	int status = 0;

	if (0 == (pid = fork())) {
		closefds(0);
		execve(racoon_args[0], racoon_args, NULL);
		helplog(ASL_LEVEL_ERR, "execve of \"%s\" failed: %s",
		    racoon_args[0], strerror(errno));
		exit(2);
	}
	helplog(ASL_LEVEL_NOTICE, "racoon (pid=%lu) started",
	    (unsigned long)pid);
	n = waitpid(pid, &status, 0);
	if (-1 == n) {
		helplog(ASL_LEVEL_ERR, "Unexpected waitpid failure: %s",
		    strerror(errno));
		return kmDNSHelperRacoonStartFailed;
	} else if (pid != n) {
		helplog(ASL_LEVEL_ERR, "Unexpected waitpid return value %d",
		    (int)n);
		return kmDNSHelperRacoonStartFailed;
	} else if (WIFSIGNALED(status)) {
		helplog(ASL_LEVEL_ERR,
		    "racoon (pid=%lu) terminated due to signal %d",
		    (unsigned long)pid, WTERMSIG(status));
		return kmDNSHelperRacoonStartFailed;
	} else if (WIFSTOPPED(status)) {
		helplog(ASL_LEVEL_ERR,
		    "racoon (pid=%lu) has stopped due to signal %d",
		    (unsigned long)pid, WSTOPSIG(status));
		return kmDNSHelperRacoonStartFailed;
	} else if (0 != WEXITSTATUS(status)) {
		helplog(ASL_LEVEL_ERR,
		    "racoon (pid=%lu) exited with status %d",
		    (unsigned long)pid, WEXITSTATUS(status));
		return kmDNSHelperRacoonStartFailed;
	}
	debug("racoon (pid=%lu) daemonized normally", (unsigned long)pid);
	return 0;
}

static int
kickRacoon(void)
{
	if ( 0 == notifyRacoon() )
		return 0;
	return startRacoon();
}

int
do_mDNSRacoonNotify(__unused mach_port_t port, const char *keydata, int *err,
    audit_token_t token)
{
	debug("entry");
	*err = 0;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	if (0 != createAnonymousRacoonConfiguration(keydata)) {
		*err = kmDNSHelperRacoonConfigCreationFailed;
		goto fin;
	}
	if (0 != (*err = kickRacoon()))
		goto fin;
	debug("succeeded");

fin:
	update_idle_timer();
	return KERN_SUCCESS;
}

static unsigned int routeSeq = 1;

static int
setupTunnelRoute(v6addr_t local, v6addr_t remote)
{
	struct {
		struct rt_msghdr    hdr;
		struct sockaddr_in6 dst;
		struct sockaddr_in6 gtwy;
	} msg;
	int err = 0;
	int s = -1;

	if (0 > (s = socket(PF_ROUTE, SOCK_RAW, AF_INET))) {
		helplog(ASL_LEVEL_ERR, "socket(PF_ROUTE, ...) failed: %s",
		    strerror(errno));
		err = kmDNSHelperRoutingSocketCreationFailed;
		goto fin;
	}
	memset(&msg, 0, sizeof(msg));
	msg.hdr.rtm_msglen = sizeof(msg);
	msg.hdr.rtm_type = RTM_ADD;
	/* The following flags are set by `route add -inet6 -host ...` */
	msg.hdr.rtm_flags = RTF_UP | RTF_GATEWAY | RTF_HOST | RTF_STATIC;
	msg.hdr.rtm_version = RTM_VERSION;
	msg.hdr.rtm_seq = routeSeq++;
	msg.hdr.rtm_addrs = RTA_DST | RTA_GATEWAY;

	msg.dst.sin6_len = sizeof(msg.dst);
	msg.dst.sin6_family = AF_INET6;
	memcpy(&msg.dst.sin6_addr, remote, sizeof(msg.dst.sin6_addr));

	msg.gtwy.sin6_len = sizeof(msg.gtwy);
	msg.gtwy.sin6_family = AF_INET6;
	memcpy(&msg.gtwy.sin6_addr, local, sizeof(msg.gtwy.sin6_addr));

	/* send message, ignore error when route already exists */
	if (0 > write(s, &msg, msg.hdr.rtm_msglen)) {
		int errno_ = errno;

		debug("write to routing socket failed: %s", strerror(errno_));
		if (EEXIST != errno_) {
			err = kmDNSHelperRouteAdditionFailed;
			goto fin;
		}
	}

fin:
	if (0 <= s)
		close(s);
	return err;
}

static int
teardownTunnelRoute(v6addr_t remote)
{
	struct {
		struct rt_msghdr    hdr;
		struct sockaddr_in6 dst;
	} msg;
	int err = 0;
	int s = -1;

	if (0 > (s = socket(PF_ROUTE, SOCK_RAW, AF_INET))) {
		helplog(ASL_LEVEL_ERR, "socket(PF_ROUTE, ...) failed: %s",
		    strerror(errno));
		err = kmDNSHelperRoutingSocketCreationFailed;
		goto fin;
	}
	memset(&msg, 0, sizeof(msg));

	msg.hdr.rtm_msglen = sizeof(msg);
	msg.hdr.rtm_type = RTM_DELETE;
	msg.hdr.rtm_version = RTM_VERSION;
	msg.hdr.rtm_seq = routeSeq++;
	msg.hdr.rtm_addrs = RTA_DST;

	msg.dst.sin6_len = sizeof(msg.dst);
	msg.dst.sin6_family = AF_INET6;
	memcpy(&msg.dst.sin6_addr, remote, sizeof(msg.dst.sin6_addr));
	if (0 > write(s, &msg, msg.hdr.rtm_msglen)) {
		int errno_ = errno;

		debug("write to routing socket failed: %s", strerror(errno_));
		if (ESRCH != errno_) {
			err = kmDNSHelperRouteDeletionFailed;
			goto fin;
		}
	}

fin:
	if (0 <= s)
		close(s);
	return err;
}

static int
v4addr_to_string(v4addr_t addr, char *buf, size_t buflen)
{
	if (NULL == inet_ntop(AF_INET, addr, buf, buflen)) {
		helplog(ASL_LEVEL_ERR, "inet_ntop failed: %s",
		    strerror(errno));
		return kmDNSHelperInvalidNetworkAddress;
	} else
		return 0;
}

static int
v6addr_to_string(v6addr_t addr, char *buf, size_t buflen)
{
	if (NULL == inet_ntop(AF_INET6, addr, buf, buflen)) {
		helplog(ASL_LEVEL_ERR, "inet_ntop failed: %s",
		    strerror(errno));
		return kmDNSHelperInvalidNetworkAddress;
	} else
		return 0;
}

/* Caller owns object returned in `policy' */
static int
generateTunnelPolicy(int setup, int in,
		     v4addr_t src, short src_port,
		     v4addr_t dst, short dst_port,
		     ipsec_policy_t *policy, size_t *len)
{
	char srcs[INET_ADDRSTRLEN], dsts[INET_ADDRSTRLEN];
	char buf[128];
	char *inOut = in ? "in" : "out";
	ssize_t n = 0;
	int err = 0;

	*policy = NULL;
	*len = 0;

	if (setup) {
		if (0 != (err = v4addr_to_string(src, srcs, sizeof(srcs))))
			goto fin;
		if (0 != (err = v4addr_to_string(dst, dsts, sizeof(dsts))))
			goto fin;
		n = snprintf(buf, sizeof(buf),
		    "%s ipsec esp/tunnel/%s[%d]-%s[%d]/require",
		    inOut, srcs, src_port, dsts, dst_port);
	} else
		n = strlcpy(buf, inOut, sizeof(buf));
	if (n >= (int)sizeof(buf)) {
		err = kmDNSHelperResultTooLarge;
		goto fin;
	}
	debug("policy=\"%s\"", buf);
	if (NULL == (*policy = (ipsec_policy_t)ipsec_set_policy(buf, n))) {
		helplog(ASL_LEVEL_ERR,
		    "Could not create IPsec policy from \"%s\"", buf);
		err = kmDNSHelperIPsecPolicyCreationFailed;
		goto fin;
	}
	*len = ((ipsec_policy_t)(*policy))->sadb_x_policy_len * 8;

fin:
	return err;
}

static int
sendPolicy(int s, int setup, struct sockaddr *src, struct sockaddr *dst,
    ipsec_policy_t policy, size_t len)
{
	static unsigned int policySeq = 0;
	int err = 0;

	debug("entry, setup=%d", setup);
	/* 128 below is bitmask (whole v6 addr) */
	if (setup)
		err = pfkey_send_spdadd(s, src, 128, dst, 128, -1,
		    (char *)policy, len, policySeq++);
	else
		err = pfkey_send_spddelete(s, src, 128, dst, 128, -1,
		    (char *)policy, len, policySeq++);
	if (0 > err) {
		helplog(ASL_LEVEL_ERR, "Could not set IPsec policy: %s",
		    ipsec_strerror());
		err = kmDNSHelperIPsecPolicySetFailed;
		goto fin;
	} else
		err = 0;
	debug("succeeded");

fin:
	return err;
}

static int
doTunnelPolicy(int setup,
	       v6addr_t loc_inner, v4addr_t loc_outer, short loc_port, 
	       v6addr_t rmt_inner, v4addr_t rmt_outer, short rmt_port)
{
	struct sockaddr_in6 sin_loc;
	struct sockaddr_in6 sin_rmt;
	ipsec_policy_t policy = NULL;
	size_t len = 0;
	int s = -1;
	int err = 0;

	debug("entry");
	if (0 > (s = pfkey_open())) {
		helplog(ASL_LEVEL_ERR,
		    "Could not create IPsec policy socket: %s",
		    ipsec_strerror());
		err = kmDNSHelperIPsecPolicySocketCreationFailed;
		goto fin;
	}

	memset(&sin_loc, 0, sizeof(sin_loc));
	sin_loc.sin6_len = sizeof(sin_loc);
	sin_loc.sin6_family = AF_INET6;
	sin_loc.sin6_port = htons(0);
	memcpy(&sin_loc.sin6_addr, loc_inner, sizeof(sin_loc.sin6_addr));

	memset(&sin_rmt, 0, sizeof(sin_rmt));
	sin_rmt.sin6_len = sizeof(sin_rmt);
	sin_rmt.sin6_family = AF_INET6;
	sin_rmt.sin6_port = htons(0);
	memcpy(&sin_rmt.sin6_addr, rmt_inner, sizeof(sin_rmt.sin6_addr));

	if (0 != (err = generateTunnelPolicy(setup, 1,
	    rmt_outer, rmt_port,
	    loc_outer, loc_port,
	    &policy, &len)))
		goto fin;
	if (0 != (err = sendPolicy(s, setup, (struct sockaddr *)&sin_rmt,
	    (struct sockaddr *)&sin_loc, policy, len)))
		goto fin;
	if (NULL != policy) {
		free(policy);
		policy = NULL;
	}
	if (0 != (err = generateTunnelPolicy(setup, 0,
	    loc_outer, loc_port,
	    rmt_outer, rmt_port,
	    &policy, &len)))
		goto fin;
	if (0 != (err = sendPolicy(s, setup, (struct sockaddr *)&sin_loc,
	    (struct sockaddr *)&sin_rmt, policy, len)))
		goto fin;
	debug("succeeded");
fin:
	if (0 >= s)
		close(s);
	if (NULL != policy)
		free(policy);
	return err;
}

int
do_mDNSAutoTunnelSetKeys(__unused mach_port_t port, int replacedelete,
    v6addr_t loc_inner, v4addr_t loc_outer, short loc_port, v6addr_t rmt_inner,
    v4addr_t rmt_outer, short rmt_port, const char *keydata, int *err,
    audit_token_t token)
{
	static const char config[] =
	  "remote %s [%d] {\n"
	  "  exchange_mode aggressive;\n"
	  "  doi ipsec_doi;\n"
	  "  situation identity_only;\n"
	  "  verify_identifier off;\n"
	  "  shared_secret use \"%s\";\n"
	  "  nonce_size 16;\n"
	  "  lifetime time 5 min;\n"
	  "  initial_contact on;\n"
	  "  support_proxy on;\n"
	  "  nat_traversal force;\n"
	  "  proposal_check claim;\n"
	  "  proposal {\n"
	  "    encryption_algorithm aes;\n"
	  "    hash_algorithm sha1;\n"
	  "    authentication_method pre_shared_key;\n"
	  "    dh_group 2;\n"
	  "    lifetime time 5 min;\n"
	  "  }\n"
	  "}\n\n"
	  "sainfo address %s any address %s any {\n"
	  "  pfs_group 2;\n"
	  "  lifetime time 60 min;\n"
	  "  encryption_algorithm aes;\n"
	  "  authentication_algorithm hmac_sha1;\n"
	  "  compression_algorithm deflate;\n"
	  "}\n\n"
	  "sainfo address %s any address %s any {\n"
	  "  pfs_group 2;\n"
	  "  lifetime time 60 min;\n"
	  "  encryption_algorithm aes;\n"
	  "  authentication_algorithm hmac_sha1;\n"
	  "  compression_algorithm deflate;\n"
	  "}\n";
	char path[PATH_MAX] = "";
	char li[INET6_ADDRSTRLEN], lo[INET_ADDRSTRLEN],
	    ri[INET6_ADDRSTRLEN], ro[INET_ADDRSTRLEN];
	FILE *fp = NULL;
	int fd = -1;
	char tmp_path[PATH_MAX] = "";

	debug("entry");
	*err = 0;
	if (!authorized(&token)) {
		*err = kmDNSHelperNotAuthorized;
		goto fin;
	}
	switch ((enum mDNSAutoTunnelSetKeysReplaceDelete)replacedelete) {
	case kmDNSAutoTunnelSetKeysReplace:
	case kmDNSAutoTunnelSetKeysDelete:
		break;
	default:
		*err = kmDNSHelperInvalidTunnelSetKeysOperation;
		goto fin;
	}
	if (0 != (*err = v6addr_to_string(loc_inner, li, sizeof(li))))
		goto fin;
	if (0 != (*err = v6addr_to_string(rmt_inner, ri, sizeof(ri))))
		goto fin;
	if (0 != (*err = v4addr_to_string(loc_outer, lo, sizeof(lo))))
		goto fin;
	if (0 != (*err = v4addr_to_string(rmt_outer, ro, sizeof(ro))))
		goto fin;
	debug("loc_inner=%s rmt_inner=%s", li, ri);
	debug("loc_outer=%s loc_port=%d rmt_outer=%s rmt_port=%d",
	    lo, loc_port, ro, rmt_port);

	if ((int)sizeof(path) <= snprintf(path, sizeof(path),
	    "/etc/racoon/remote/%s.%d.conf", ro,
	    rmt_port)) {
		*err = kmDNSHelperResultTooLarge;
		goto fin;
	}
	if (kmDNSAutoTunnelSetKeysReplace == replacedelete) {
		if ((int)sizeof(tmp_path) <=
		    snprintf(tmp_path, sizeof(tmp_path), "%s.XXXXXX", path)) {
			*err = kmDNSHelperResultTooLarge;
			goto fin;
		}       
		if (0 > (fd = mkstemp(tmp_path))) {
			helplog(ASL_LEVEL_ERR, "mktemp \"%s\" failed: %s",
			    tmp_path, strerror(errno));
			*err = kmDNSHelperRacoonConfigCreationFailed;
			goto fin;
		}
		if (NULL == (fp = fdopen(fd, "w"))) {
			helplog(ASL_LEVEL_ERR, "fdopen: %s",
			    strerror(errno));
			*err = kmDNSHelperRacoonConfigCreationFailed;
			goto fin;
		}
		fd = -1;
		fprintf(fp, config, ro, rmt_port, keydata, ri, li, li, ri);
		fclose(fp);
		fp = NULL;
		if (0 > rename(tmp_path, path)) {
			helplog(ASL_LEVEL_ERR,
			    "rename \"%s\" \"%s\" failed: %s",
			    tmp_path, path, strerror(errno));
			*err = kmDNSHelperRacoonConfigCreationFailed;
			goto fin;
		}
		if (0 != (*err = kickRacoon()))
			goto fin;
	} else {
		if (0 != unlink(path))
			debug("unlink \"%s\" failed: %s", path,
			    strerror(errno));
	}

	if (0 != (*err = doTunnelPolicy(0, loc_inner, NULL, loc_port,
									rmt_inner, NULL, rmt_port)))
		goto fin;
	if (kmDNSAutoTunnelSetKeysReplace == replacedelete &&
	    0 != (*err = doTunnelPolicy(1, loc_inner, loc_outer, loc_port,
									rmt_inner, rmt_outer, rmt_port)))
		goto fin;

	if (0 != (*err = teardownTunnelRoute(rmt_inner)))
		goto fin;
	if (kmDNSAutoTunnelSetKeysReplace == replacedelete &&
		0 != (*err = setupTunnelRoute(loc_inner, rmt_inner)))
		goto fin;

	debug("succeeded");

fin:
	if (NULL != fp)
		fclose(fp);
	if (0 <= fd)
		close(fd);
	unlink(tmp_path);
	update_idle_timer();
	return KERN_SUCCESS;
}