/* -*- Mode: C; tab-width: 4 -*-
 *
 * Copyright (c) 2003-2006 Apple Computer, Inc. All rights reserved.
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

	Change History (most recent first):

$Log: uds_daemon.c,v $
Revision 1.309  2007/06/29 23:12:49  vazquez
<rdar://problem/5294103> Stop using generate_final_fatal_reply_with_garbage

Revision 1.308  2007/06/29 00:10:07  vazquez
<rdar://problem/5301908> Clean up NAT state machine (necessary for 6 other fixes)

Revision 1.307  2007/05/25 00:25:44  cheshire
<rdar://problem/5227737> Need to enhance putRData to output all current known types

Revision 1.306  2007/05/24 22:31:35  vazquez
Bug #: 4272956
Reviewed by: Stuart Cheshire
<rdar://problem/4272956> WWDC API: Return ADD/REMOVE events in registration callback

Revision 1.305  2007/05/23 18:59:22  cheshire
Remove unnecessary IPC_FLAGS_REUSE_SOCKET

Revision 1.304  2007/05/22 01:07:42  cheshire
<rdar://problem/3563675> API: Need a way to get version/feature information

Revision 1.303  2007/05/22 00:32:58  cheshire
Make a send_all() subroutine -- will be helpful for implementing DNSServiceGetProperty(DaemonVersion)

Revision 1.302  2007/05/21 18:54:54  cheshire
Add "Cancel" LogOperation message when we get a cancel_request command over the UDS

Revision 1.301  2007/05/18 23:55:22  cheshire
<rdar://problem/4454655> Allow multiple register/browse/resolve operations to share single Unix Domain Socket

Revision 1.300  2007/05/18 21:27:11  cheshire
Rename connected_registration_termination to connection_termination

Revision 1.299  2007/05/18 21:24:34  cheshire
Rename rstate to request

Revision 1.298  2007/05/18 21:22:35  cheshire
Convert uint16_t etc. to their locally-defined equivalents, like the rest of the core code

Revision 1.297  2007/05/18 20:33:11  cheshire
Avoid declaring lots of uninitialized variables in read_rr_from_ipc_msg

Revision 1.296  2007/05/18 19:04:19  cheshire
Rename msgdata to msgptr (may be modified); rename (currently unused) bufsize to msgend

Revision 1.295  2007/05/18 17:57:13  cheshire
Reorder functions in file to arrange them in logical groups; added "#pragma mark" headers for each group

Revision 1.294  2007/05/17 20:58:22  cheshire
<rdar://problem/4647145> DNSServiceQueryRecord should return useful information with NXDOMAIN

Revision 1.293  2007/05/17 19:46:20  cheshire
Routine name deliver_async_error() is misleading. What it actually does is write a message header
(containing an error code) followed by 256 bytes of garbage zeroes onto a client connection,
thereby trashing it and making it useless for any subsequent communication. It's destructive,
and not very useful. Changing name to generate_final_fatal_reply_with_garbage().

Revision 1.292  2007/05/16 01:06:52  cheshire
<rdar://problem/4471320> Improve reliability of kDNSServiceFlagsMoreComing flag on multiprocessor machines

Revision 1.291  2007/05/15 21:57:16  cheshire
<rdar://problem/4608220> Use dnssd_SocketValid(x) macro instead of just
assuming that all negative values (or zero!) are invalid socket numbers

Revision 1.290  2007/05/10 23:30:57  cheshire
<rdar://problem/4084490> Only one browse gets remove events when disabling browse domain

Revision 1.289  2007/05/02 22:18:08  cheshire
Renamed NATTraversalInfo_struct context to NATTraversalContext

Revision 1.288  2007/04/30 21:33:39  cheshire
Fix crash when a callback unregisters a service while the UpdateSRVRecords() loop
is iterating through the m->ServiceRegistrations list

Revision 1.287  2007/04/27 19:03:22  cheshire
Check q->LongLived not q->llq to tell if a query is LongLived

Revision 1.286  2007/04/26 16:00:01  cheshire
Show interface number in DNSServiceBrowse RESULT output

Revision 1.285  2007/04/22 19:03:39  cheshire
Minor code tidying

Revision 1.284  2007/04/22 06:02:03  cheshire
<rdar://problem/4615977> Query should immediately return failure when no server

Revision 1.283  2007/04/21 21:47:47  cheshire
<rdar://problem/4376383> Daemon: Add watchdog timer

Revision 1.282  2007/04/20 21:17:24  cheshire
For naming consistency, kDNSRecordTypeNegative should be kDNSRecordTypePacketNegative

Revision 1.281  2007/04/19 23:25:20  cheshire
Added debugging message

Revision 1.280  2007/04/17 19:21:29  cheshire
<rdar://problem/5140339> Domain discovery not working over VPN

Revision 1.279  2007/04/16 21:53:49  cheshire
Improve display of negative cache entries

Revision 1.278  2007/04/16 20:49:40  cheshire
Fix compile errors for mDNSPosix build

Revision 1.277  2007/04/05 22:55:36  cheshire
<rdar://problem/5077076> Records are ending up in Lighthouse without expiry information

Revision 1.276  2007/04/05 19:20:13  cheshire
Non-blocking mode not being set correctly -- was clobbering other flags

Revision 1.275  2007/04/04 21:21:25  cheshire
<rdar://problem/4546810> Fix crash: In regservice_callback service_instance was being referenced after being freed

Revision 1.274  2007/04/04 01:30:42  cheshire
<rdar://problem/5075200> DNSServiceAddRecord is failing to advertise NULL record
Add SIGINFO output lising our advertised Authoritative Records

Revision 1.273  2007/04/04 00:03:27  cheshire
<rdar://problem/5089862> DNSServiceQueryRecord is returning kDNSServiceErr_NoSuchRecord for empty rdata

Revision 1.272  2007/04/03 20:10:32  cheshire
Show ADD/RMV in DNSServiceQueryRecord log message instead of just "RESULT"

Revision 1.271  2007/04/03 19:22:32  cheshire
Use mDNSSameIPv4Address (and similar) instead of accessing internal fields directly

Revision 1.270  2007/03/30 21:55:30  cheshire
Added comments

Revision 1.269  2007/03/29 01:31:44  cheshire
Faulty logic was incorrectly suppressing some NAT port mapping callbacks

Revision 1.268  2007/03/29 00:13:58  cheshire
Remove unnecessary fields from service_instance structure: autoname, autorename, allowremotequery, name

Revision 1.267  2007/03/28 20:59:27  cheshire
<rdar://problem/4743285> Remove inappropriate use of IsPrivateV4Addr()

Revision 1.266  2007/03/28 15:56:37  cheshire
<rdar://problem/5085774> Add listing of NAT port mapping and GetAddrInfo requests in SIGINFO output

Revision 1.265  2007/03/27 22:52:07  cheshire
Fix crash in udsserver_automatic_browse_domain_changed

Revision 1.264  2007/03/27 00:49:40  cheshire
Should use mallocL, not plain malloc

Revision 1.263  2007/03/27 00:45:01  cheshire
Removed unnecessary "void *termination_context" pointer

Revision 1.262  2007/03/27 00:40:43  cheshire
Eliminate resolve_termination_t as a separately-allocated structure, and make it part of the request_state union

Revision 1.261  2007/03/27 00:29:00  cheshire
Eliminate queryrecord_request data as a separately-allocated structure, and make it part of the request_state union

Revision 1.260  2007/03/27 00:18:42  cheshire
Eliminate enum_termination_t and domain_enum_t as separately-allocated structures,
and make them part of the request_state union

Revision 1.259  2007/03/26 23:48:16  cheshire
<rdar://problem/4848295> Advertise model information via Bonjour
Refinements to reduce unnecessary transmissions of the DeviceInfo TXT record

Revision 1.258  2007/03/24 00:40:04  cheshire
Minor code cleanup

Revision 1.257  2007/03/24 00:23:12  cheshire
Eliminate port_mapping_info_t as a separately-allocated structure, and make it part of the request_state union

Revision 1.256  2007/03/24 00:07:18  cheshire
Eliminate addrinfo_info_t as a separately-allocated structure, and make it part of the request_state union

Revision 1.255  2007/03/23 23:56:14  cheshire
Move list of record registrations into the request_state union

Revision 1.254  2007/03/23 23:48:56  cheshire
Eliminate service_info as a separately-allocated structure, and make it part of the request_state union

Revision 1.253  2007/03/23 23:04:29  cheshire
Eliminate browser_info_t as a separately-allocated structure, and make it part of request_state

Revision 1.252  2007/03/23 22:59:58  cheshire
<rdar://problem/4848295> Advertise model information via Bonjour
Use kStandardTTL, not kHostNameTTL

Revision 1.251  2007/03/23 22:44:07  cheshire
Instead of calling AbortUnlinkAndFree() haphazardly all over the place, make the handle* routines
return an error code, and then request_callback() does all necessary cleanup in one place.

Revision 1.250  2007/03/22 20:30:07  cheshire
Remove pointless "if (request->ts != t_complete) ..." checks

Revision 1.249  2007/03/22 20:13:27  cheshire
Delete unused client_context field

Revision 1.248  2007/03/22 20:03:37  cheshire
Rename variables for clarity: instead of using variable rs for both request_state
and reply_state, use req for request_state and rep for reply_state

Revision 1.247  2007/03/22 19:31:42  cheshire
<rdar://problem/4848295> Advertise model information via Bonjour
Add missing "model=" at start of DeviceInfo data

Revision 1.246  2007/03/22 18:31:48  cheshire
Put dst parameter first in mDNSPlatformStrCopy/mDNSPlatformMemCopy, like conventional Posix strcpy/memcpy

Revision 1.245  2007/03/22 00:49:20  cheshire
<rdar://problem/4848295> Advertise model information via Bonjour

Revision 1.244  2007/03/21 21:01:48  cheshire
<rdar://problem/4789793> Leak on error path in regrecord_callback, uds_daemon.c

Revision 1.243  2007/03/21 19:01:57  cheshire
<rdar://problem/5078494> IPC code not 64-bit-savvy: assumes long=32bits, and short=16bits

Revision 1.242  2007/03/21 18:51:21  cheshire
<rdar://problem/4549320> Code in uds_daemon.c passes function name instead of type name to mallocL/freeL

Revision 1.241  2007/03/20 00:04:50  cheshire
<rdar://problem/4837929> Should allow "udp" or "tcp" for protocol command-line arg
Fix LogOperation("DNSServiceNATPortMappingCreate(...)") message to actually show client arguments

Revision 1.240  2007/03/16 23:25:35  cheshire
<rdar://problem/5067001> NAT-PMP: Parameter validation not working correctly

Revision 1.239  2007/03/10 02:29:36  cheshire
Added comment about port_mapping_create_reply()

Revision 1.238  2007/03/07 00:26:48  cheshire
<rdar://problem/4426754> DNSServiceRemoveRecord log message should include record type

Revision 1.237  2007/02/28 01:44:29  cheshire
<rdar://problem/5027863> Byte order bugs in uDNS.c, uds_daemon.c, dnssd_clientstub.c

Revision 1.236  2007/02/14 01:58:19  cheshire
<rdar://problem/4995831> Don't delete Unix Domain Socket on exit if we didn't create it on startup

Revision 1.235  2007/02/08 21:12:28  cheshire
<rdar://problem/4386497> Stop reading /etc/mDNSResponder.conf on every sleep/wake

Revision 1.234  2007/02/06 19:06:49  cheshire
<rdar://problem/3956518> Need to go native with launchd

Revision 1.233  2007/01/10 20:49:37  cheshire
Remove unnecessary setting of q->Private fields

Revision 1.232  2007/01/09 00:03:23  cheshire
Call udsserver_handle_configchange() once at the end of udsserver_init()
to set up the automatic registration and browsing domains.

Revision 1.231  2007/01/06 02:50:19  cheshire
<rdar://problem/4632919> Instead of copying SRV and TXT record data, just store pointers to cache entities

Revision 1.230  2007/01/06 01:00:35  cheshire
Improved SIGINFO output

Revision 1.229  2007/01/05 08:30:56  cheshire
Trim excessive "$Log" checkin history from before 2006
(checkin history still available via "cvs log ..." of course)

Revision 1.228  2007/01/05 08:09:05  cheshire
Reorder code into functional sections, with "#pragma mark" headers

Revision 1.227  2007/01/05 07:04:24  cheshire
Minor code tidying

Revision 1.226  2007/01/05 05:44:35  cheshire
Move automatic browse/registration management from uDNS.c to mDNSShared/uds_daemon.c,
so that mDNSPosix embedded clients will compile again

Revision 1.225  2007/01/04 23:11:15  cheshire
<rdar://problem/4720673> uDNS: Need to start caching unicast records
When an automatic browsing domain is removed, generate appropriate "remove" events for legacy queries

Revision 1.224  2007/01/04 20:57:49  cheshire
Rename ReturnCNAME to ReturnIntermed (for ReturnIntermediates)

Revision 1.223  2006/12/21 01:25:49  cheshire
Tidy up SIGINFO state log

Revision 1.222  2006/12/21 00:15:22  cheshire
Get rid of gmDNS macro; fixed a crash in udsserver_info()

Revision 1.221  2006/12/20 04:07:38  cheshire
Remove uDNS_info substructure from AuthRecord_struct

Revision 1.220  2006/12/19 22:49:25  cheshire
Remove uDNS_info substructure from ServiceRecordSet_struct

Revision 1.219  2006/12/14 03:02:38  cheshire
<rdar://problem/4838433> Tools: dns-sd -G 0 only returns IPv6 when you have a routable IPv6 address

Revision 1.218  2006/11/18 05:01:33  cheshire
Preliminary support for unifying the uDNS and mDNS code,
including caching of uDNS answers

Revision 1.217  2006/11/15 19:27:53  mkrochma
<rdar://problem/4838433> Tools: dns-sd -G 0 only returns IPv6 when you have a routable IPv6 address

Revision 1.216  2006/11/10 00:54:16  cheshire
<rdar://problem/4816598> Changing case of Computer Name doesn't work

Revision 1.215  2006/10/27 01:30:23  cheshire
Need explicitly to set ReturnIntermed = mDNSfalse

Revision 1.214  2006/10/20 05:37:23  herscher
Display question list information in udsserver_info()

Revision 1.213  2006/10/05 03:54:31  herscher
Remove embedded uDNS_info struct from DNSQuestion_struct

Revision 1.212  2006/09/30 01:22:35  cheshire
Put back UTF-8 curly quotes in log messages

Revision 1.211  2006/09/27 00:44:55  herscher
<rdar://problem/4249761> API: Need DNSServiceGetAddrInfo()

Revision 1.210  2006/09/26 01:52:41  herscher
<rdar://problem/4245016> NAT Port Mapping API (for both NAT-PMP and UPnP Gateway Protocol)

Revision 1.209  2006/09/21 21:34:09  cheshire
<rdar://problem/4100000> Allow empty string name when using kDNSServiceFlagsNoAutoRename

Revision 1.208  2006/09/21 21:28:24  cheshire
Code cleanup to make it consistent with daemon.c: change rename_on_memfree to renameonmemfree

Revision 1.207  2006/09/15 21:20:16  cheshire
Remove uDNS_info substructure from mDNS_struct

Revision 1.206  2006/08/14 23:24:56  cheshire
Re-licensed mDNSResponder daemon source code under Apache License, Version 2.0

Revision 1.205  2006/07/20 22:07:30  mkrochma
<rdar://problem/4633196> Wide-area browsing is currently broken in TOT
More fixes for uninitialized variables

Revision 1.204  2006/07/15 02:01:33  cheshire
<rdar://problem/4472014> Add Private DNS client functionality to mDNSResponder
Fix broken "empty string" browsing

Revision 1.203  2006/07/07 01:09:13  cheshire
<rdar://problem/4472013> Add Private DNS server functionality to dnsextd
Only use mallocL/freeL debugging routines when building mDNSResponder, not dnsextd

Revision 1.202  2006/07/05 22:00:10  cheshire
Wide-area cleanup: Rename mDNSPlatformGetRegDomainList() to uDNS_GetDefaultRegDomainList()

Revision 1.201  2006/06/29 03:02:47  cheshire
<rdar://problem/4607042> mDNSResponder NXDOMAIN and CNAME support

Revision 1.200  2006/06/28 08:56:26  cheshire
Added "_op" to the end of the operation code enum values,
to differentiate them from the routines with the same names

Revision 1.199  2006/06/28 08:53:39  cheshire
Added (commented out) debugging messages

Revision 1.198  2006/06/27 20:16:07  cheshire
Fix code layout

Revision 1.197  2006/05/18 01:32:35  cheshire
<rdar://problem/4472706> iChat: Lost connection with Bonjour
(mDNSResponder insufficiently defensive against malformed browsing PTR responses)

Revision 1.196  2006/05/05 07:07:13  cheshire
<rdar://problem/4538206> mDNSResponder fails when UDS reads deliver partial data

Revision 1.195  2006/04/25 20:56:28  mkrochma
Added comment about previous checkin

Revision 1.194  2006/04/25 18:29:36  mkrochma
Workaround for warning: unused variable 'status' when building mDNSPosix

Revision 1.193  2006/03/19 17:14:38  cheshire
<rdar://problem/4483117> Need faster purging of stale records
read_rr_from_ipc_msg was not setting namehash and rdatahash

Revision 1.192  2006/03/18 20:58:32  cheshire
Misplaced curly brace

Revision 1.191  2006/03/10 22:19:43  cheshire
Update debugging message in resolve_result_callback() to indicate whether event is ADD or RMV

Revision 1.190  2006/03/10 21:56:12  cheshire
<rdar://problem/4111464> After record update, old record sometimes remains in cache
When service TXT and SRV record both change, clients with active resolve calls get *two* callbacks, one
when the TXT data changes, and then immediately afterwards a second callback with the new port number
This change suppresses the first unneccessary (and confusing) callback

Revision 1.189  2006/01/06 00:56:31  cheshire
<rdar://problem/4400573> Should remove PID file on exit

*/

#if defined(_WIN32)
#include <process.h>
#define dnssd_strerror(X) win32_strerror(X)
#define usleep(X) Sleep(((X)+999)/1000)
mDNSlocal char *win32_strerror(int inErrorCode)
	{
	static char buffer[1024];
	DWORD       n;
	memset(buffer, 0, sizeof(buffer));
	n = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			(DWORD) inErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer,
			sizeof(buffer),
			NULL);
	if (n > 0)
		{
		// Remove any trailing CR's or LF's since some messages have them.
		while ((n > 0) && isspace(((unsigned char *) buffer)[n - 1]))
			buffer[--n] = '\0';
		}
	return buffer;
	}
#else
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#define dnssd_strerror(X) strerror(X)
#endif

#include <stdlib.h>
#include <stdio.h>
#include "mDNSEmbeddedAPI.h"
#include "DNSCommon.h"
#include "uDNS.h"
#include "uds_daemon.h"
#include "dns_sd.h"
#include "dnssd_ipc.h"

// Apple specific configuration functionality, not required for other platforms
#if APPLE_OSX_mDNSResponder
#include <sys/ucred.h>
#ifndef LOCAL_PEERCRED
#define LOCAL_PEERCRED 0x001 /* retrieve peer credentials */
#endif // LOCAL_PEERCRED
#endif // APPLE_OSX_mDNSResponder

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - Types and Data Structures
#endif

typedef enum
	{
	t_uninitialized,
	t_morecoming,
	t_complete,
	t_error,
	t_terminated
	} transfer_state;

typedef struct request_state request_state;

typedef void (*req_termination_fn)(request_state *request);

typedef struct registered_record_entry
	{
	struct registered_record_entry *next;
	mDNSu32 key;
	AuthRecord *rr;		// Variable-sized AuthRecord
	client_context_t client_context;
	request_state *request;
	} registered_record_entry;

// A single registered service: ServiceRecordSet + bookkeeping
// Note that we duplicate some fields from parent service_info object
// to facilitate cleanup, when instances and parent may be deallocated at different times.
typedef struct service_instance
	{
	struct service_instance *next;
	request_state *request;
	dnssd_sock_t sd;
	AuthRecord *subtypes;
	mDNSBool renameonmemfree;  		// Set on config change when we deregister original name
    mDNSBool clientnotified;		// Has client been notified of successful registration yet?
	mDNSBool default_local;			// is this the "local." from an empty-string registration?
	domainname domain;
	ServiceRecordSet srs;			// note - must be last field in struct
	} service_instance;

// for multi-domain default browsing
typedef struct browser_t
	{
	struct browser_t *next;
	domainname domain;
	DNSQuestion q;
	} browser_t;

struct request_state
	{
	request_state *next;
	request_state *primary;			// If this operation is on a shared socket, pointer to
									// primary request_state for the original DNSServiceConnect() operation
	dnssd_sock_t sd;

	// NOTE: On a shared connection these fields in the primary structure, including hdr, are re-used
	// for each new request. This is because, until we've read the ipc_msg_hdr to find out what the
	// operation is, we don't know if we're going to need to allocate a new request_state or not.
	transfer_state ts;
	mDNSu32 hdr_bytes;				// bytes of header already read
	ipc_msg_hdr hdr;
	mDNSu32 data_bytes;			// bytes of message data already read
	char *msgbuf;					// pointer to data storage to pass to free()
	char *msgptr;					// pointer to data to be read from (may be modified)
	char *msgend;					// pointer to byte after last byte of message

	// reply, termination, error, and client context info
	int no_reply;					// don't send asynchronous replies to client
	int time_blocked;				// record time of a blocked client
	struct reply_state *replies;	// corresponding (active) reply list
	req_termination_fn terminate;

	union
		{
		registered_record_entry *reg_recs;  // list of registrations for a connection-oriented request
		struct
			{
			mDNSInterfaceID interface_id;
			mDNSBool default_domain;
			mDNSBool ForceMCast;
			domainname regtype;
			browser_t *browsers;
			} browser;
		struct
			{
			mDNSInterfaceID InterfaceID;
			mDNSu16 txtlen;
			void *txtdata;
			mDNSIPPort port;
			domainlabel name;
			char type_as_string[MAX_ESCAPED_DOMAIN_NAME];
			domainname type;
			mDNSBool default_domain;
			domainname host;
			mDNSBool autoname;				// Set if this name is tied to the Computer Name
			mDNSBool autorename;			// Set if this client wants us to automatically rename on conflict
			mDNSBool allowremotequery;		// Respond to unicast queries from outside the local link?
			int num_subtypes;
			service_instance *instances;
			} servicereg;
		struct
			{
			mDNSInterfaceID      interface_id;
			mDNSu32              flags;
			mDNSu32              protocol;
			DNSQuestion          q4;
			DNSQuestion          q6;
			} addrinfo;
		struct
			{
			mDNSInterfaceID      interface_id;
			mDNSu8               protocol;
			mDNSIPPort           privatePort;
			mDNSIPPort           requestedPub;	// Requested public port
			mDNSIPPort           actualPub;		// Actual public port assigned by NAT gateway
			mDNSu32              requestedTTL;
			mDNSu32              receivedTTL;
			mDNSv4Addr           addr;
			NATTraversalInfo     NATinfo;
			} pm;
		struct
			{
			DNSQuestion q_all;
			DNSQuestion q_default;
			} enumeration;
		struct
			{
			DNSQuestion q;
			} queryrecord;
		struct
			{
			DNSQuestion qtxt;
			DNSQuestion qsrv;
			const ResourceRecord *txt;
			const ResourceRecord *srv;
			} resolve;
		 ;
		} u;
	};

// struct physically sits between ipc message header and call-specific fields in the message buffer
typedef struct
	{
	DNSServiceFlags flags;			// Note: This field is in NETWORK byte order
	mDNSu32 ifi;					// Note: This field is in NETWORK byte order
	DNSServiceErrorType error;		// Note: This field is in NETWORK byte order
	} reply_hdr;

typedef struct reply_state
	{
	dnssd_sock_t sd;
	transfer_state ts;
	mDNSu32 nwriten;
	mDNSu32 len;
	request_state *request;		// the request that this answers
	struct reply_state *next;	// if there are multiple unsent replies
	char *msgbuf;				// pointer to malloc'd buffer
	ipc_msg_hdr *mhdr;			// pointer into message buffer - allows fields to be changed after message is formatted
	reply_hdr *rhdr;
	char *sdata;				// pointer to start of call-specific data
	} reply_state;

#ifdef _HAVE_SETDOMAIN_SUPPORT_
typedef struct default_browse_list_t
	{
	struct default_browse_list_t *next;
	uid_t uid;
	AuthRecord ptr_rec;
	} default_browse_list_t;

static default_browse_list_t *default_browse_list = NULL;
#endif // _HAVE_SETDOMAIN_SUPPORT_

// globals
mDNSexport mDNS mDNSStorage;
mDNSexport const char ProgramName[] = "mDNSResponder";

static dnssd_sock_t listenfd = dnssd_InvalidSocket;
static request_state *all_requests = NULL;

#define MSG_PAD_BYTES 5		// pad message buffer (read from client) with n zero'd bytes to guarantee
							// n get_string() calls w/o buffer overrun
// initialization, setup/teardown functions

// If a platform specifies its own PID file name, we use that
#ifndef PID_FILE
#define PID_FILE "/var/run/mDNSResponder.pid"
#endif

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - General Utility Functions
#endif

mDNSlocal void FatalError(char *errmsg)
	{
	LogMsg("%s: %s", errmsg, dnssd_strerror(dnssd_errno()));
	*(long*)0 = 0;	// On OS X abort() doesn't generate a crash log, but writing to zero does
	abort();		// On platforms where writing to zero doesn't generate an exception, abort instead
	}

mDNSlocal mDNSu32 dnssd_htonl(mDNSu32 l)
	{
	mDNSu32 ret;
	char *data = (char*) &ret;
	put_uint32(l, &data);
	return ret;
	}

// hack to search-replace perror's to LogMsg's
mDNSlocal void my_perror(char *errmsg)
	{
	LogMsg("%s: %s", errmsg, dnssd_strerror(dnssd_errno()));
	}

mDNSlocal void abort_request(request_state *req)
	{
	// First stop whatever mDNSCore operation we were doing
	if (req->terminate) req->terminate(req);

	// Now, if this request_state is not subbordinate to some other primary, close file descriptor and discard replies
	if (!req->primary)
		{
		LogOperation("%3d: Removing FD", req->sd);
		udsSupportRemoveFDFromEventLoop(req->sd);		// Note: This also closes file descriptor req->sd for us

		while (req->replies)	// free pending replies
			{
			reply_state *ptr = req->replies;
			req->replies = req->replies->next;
			if (ptr->msgbuf) freeL("reply_state msgbuf (abort)", ptr->msgbuf);
			freeL("reply_state (abort)", ptr);
			}
		}

	// Set req->sd to something invalid, so that udsserver_idle knows to unlink and free this structure
	#if APPLE_OSX_mDNSResponder && MACOSX_MDNS_MALLOC_DEBUGGING
		// Don't use dnssd_InvalidSocket (-1) because that's the sentinel value MACOSX_MDNS_MALLOC_DEBUGGING uses
		// for detecting when the memory for an object is inadvertently freed while the object is still on some list
		req->sd = -2;
	#else
		req->sd = dnssd_InvalidSocket;
	#endif
	}

#if APPLE_OSX_mDNSResponder && MACOSX_MDNS_MALLOC_DEBUGGING
mDNSexport void uds_validatelists(void)
	{
	request_state *req;
	for (req = all_requests; req; req=req->next)
		if (req->sd < 0 && req->sd != -2)
			LogMemCorruption("UDS request list: %p is garbage (%X)", req, req->sd);
	}
#endif

mDNSlocal void AbortUnlinkAndFree(request_state *req)
	{
	request_state **p = &all_requests;
	abort_request(req);
	while (*p && *p != req) p=&(*p)->next;
	if (*p) { *p = req->next; freeL("request_state/AbortUnlinkAndFree", req); }
	}

mDNSlocal reply_state *create_reply(const reply_op_t op, const size_t datalen, request_state *const request)
	{
	reply_state *reply;
	int totallen = (int) (datalen + sizeof(ipc_msg_hdr));

	if ((unsigned)datalen < sizeof(reply_hdr))
		{
		LogMsg("ERROR: create_reply - data length less than lenght of required fields");
		return NULL;
		}

	reply = mallocL("reply_state", sizeof(reply_state));
	if (!reply) FatalError("ERROR: malloc");
	mDNSPlatformMemZero(reply, sizeof(reply_state));
	reply->ts      = t_morecoming;
	reply->sd      = request->sd;
	reply->request = request;
	reply->len     = totallen;
	reply->msgbuf  = mallocL("reply_state msgbuf", totallen);
	if (!reply->msgbuf) FatalError("ERROR: malloc");
	mDNSPlatformMemZero(reply->msgbuf, totallen);
	reply->mhdr    = (ipc_msg_hdr *)reply->msgbuf;
	reply->rhdr    = (reply_hdr *)(reply->msgbuf + sizeof(ipc_msg_hdr));
	reply->sdata   = reply->msgbuf + sizeof(ipc_msg_hdr) + sizeof(reply_hdr);
	reply->mhdr->version   = VERSION;
	reply->mhdr->datalen   = datalen;
	reply->mhdr->ipc_flags = 0;
	reply->mhdr->op        = op;
	reply->mhdr->client_context = request->hdr.client_context;
	reply->mhdr->reg_index = 0;
	return reply;
	}

// Append a reply to the list in a request object
// If our request is sharing a connection, then we append our reply_state onto the primary's list
mDNSlocal void append_reply(request_state *req, reply_state *rep)
	{
	request_state *r = req->primary ? req->primary : req;
	reply_state **ptr = &r->replies;
	while (*ptr) ptr = &(*ptr)->next;
	*ptr = rep;
	rep->next = NULL;
	}

// Generates a response message giving name, type, domain, plus interface index,
// suitable for a browse result or service registration result.
// On successful completion rep is set to point to a malloc'd reply_state struct
mDNSlocal mStatus GenerateNTDResponse(const domainname *const servicename, const mDNSInterfaceID id,
	request_state *const request, reply_state **const rep, reply_op_t op, DNSServiceFlags flags, mStatus err)
	{
	domainlabel name;
	domainname type, dom;
	*rep = NULL;
	if (!DeconstructServiceName(servicename, &name, &type, &dom))
		return kDNSServiceErr_Invalid;
	else
		{
		char namestr[MAX_DOMAIN_LABEL+1];
		char typestr[MAX_ESCAPED_DOMAIN_NAME];
		char domstr [MAX_ESCAPED_DOMAIN_NAME];
		int len;
		char *data;

		ConvertDomainLabelToCString_unescaped(&name, namestr);
		ConvertDomainNameToCString(&type, typestr);
		ConvertDomainNameToCString(&dom, domstr);

		// Calculate reply data length
		len = sizeof(DNSServiceFlags);
		len += sizeof(mDNSu32);  // if index
		len += sizeof(DNSServiceErrorType);
		len += (int) (strlen(namestr) + 1);
		len += (int) (strlen(typestr) + 1);
		len += (int) (strlen(domstr) + 1);

		// Build reply header
		*rep = create_reply(op, len, request);
		(*rep)->rhdr->flags = dnssd_htonl(flags);
		(*rep)->rhdr->ifi   = dnssd_htonl(mDNSPlatformInterfaceIndexfromInterfaceID(&mDNSStorage, id));
		(*rep)->rhdr->error = dnssd_htonl(err);

		// Build reply body
		data = (*rep)->sdata;
		put_string(namestr, &data);
		put_string(typestr, &data);
		put_string(domstr, &data);

		return mStatus_NoError;
		}
	}

// returns a resource record (allocated w/ malloc) containing the data found in an IPC message
// data must be in format flags, interfaceIndex, name, rrtype, rrclass, rdlen, rdata, (optional)ttl
// (ttl only extracted/set if ttl argument is non-zero). Returns NULL for a bad-parameter error
mDNSlocal AuthRecord *read_rr_from_ipc_msg(char **ptr, int GetTTL, int validate_flags)
	{
	DNSServiceFlags flags = get_flags(ptr);
	mDNSu32 interfaceIndex = get_uint32(ptr);
	char name[256];
	int str_err = get_string(ptr, name, sizeof(name));
	mDNSu16 type  = get_uint16(ptr);
	mDNSu16 class = get_uint16(ptr);
	mDNSu16 rdlen = get_uint16(ptr);
	char *rdata = get_rdata(ptr, rdlen);
	int storage_size = rdlen > sizeof(RDataBody) ? rdlen : sizeof(RDataBody);
	AuthRecord *rr;

	if (validate_flags &&
		!((flags & kDNSServiceFlagsShared) == kDNSServiceFlagsShared) &&
		!((flags & kDNSServiceFlagsUnique) == kDNSServiceFlagsUnique))
		{
		LogMsg("ERROR: Bad resource record flags (must be kDNSServiceFlagsShared or kDNSServiceFlagsUnique)");
		return NULL;
		}

	if (str_err) { LogMsg("ERROR: read_rr_from_ipc_msg - get_string"); return NULL; }

	rr = mallocL("AuthRecord/read_rr_from_ipc_msg", sizeof(AuthRecord) - sizeof(RDataBody) + storage_size);
	if (!rr) FatalError("ERROR: malloc");
	mDNS_SetupResourceRecord(rr, mDNSNULL, mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex),
		type, 0, (mDNSu8) ((flags & kDNSServiceFlagsShared) ? kDNSRecordTypeShared : kDNSRecordTypeUnique), mDNSNULL, mDNSNULL);

	if (!MakeDomainNameFromDNSNameString(&rr->namestorage, name))
		{
		LogMsg("ERROR: bad name: %s", name);
		freeL("AuthRecord/read_rr_from_ipc_msg", rr);
		return NULL;
		}

	if (flags & kDNSServiceFlagsAllowRemoteQuery) rr->AllowRemoteQuery = mDNStrue;
	rr->resrec.rrclass = class;
	rr->resrec.rdlength = rdlen;
	rr->resrec.rdata->MaxRDLength = rdlen;
	mDNSPlatformMemCopy(rr->resrec.rdata->u.data, rdata, rdlen);
	if (GetTTL) rr->resrec.rroriginalttl = get_uint32(ptr);
	rr->resrec.namehash = DomainNameHashValue(rr->resrec.name);
	SetNewRData(&rr->resrec, mDNSNULL, 0);	// Sets rr->rdatahash for us
	return rr;
	}

mDNSlocal int build_domainname_from_strings(domainname *srv, char *name, char *regtype, char *domain)
	{
	domainlabel n;
	domainname d, t;

	if (!MakeDomainLabelFromLiteralString(&n, name)) return -1;
	if (!MakeDomainNameFromDNSNameString(&t, regtype)) return -1;
	if (!MakeDomainNameFromDNSNameString(&d, domain)) return -1;
	if (!ConstructServiceName(srv, &n, &t, &d)) return -1;
	return 0;
	}

mDNSlocal void send_all(dnssd_sock_t s, const char *ptr, int len)
	{
	int n = send(s, ptr, len, 0);
	// On a freshly-created Unix Domain Socket, the kernel should *never* fail to buffer a small write for us
	// (four bytes for a typical error code return, 12 bytes for DNSServiceGetProperty(DaemonVersion)).
	// If it does fail, we don't attempt to handle this failure, but we do log it so we know something is wrong.
	if (n < len)
		LogMsg("ERROR: failed to write error response back to caller: %d/%d %d %s",
			n, len, dnssd_errno(), dnssd_strerror(dnssd_errno()));
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceRegister
#endif

mDNSexport void FreeExtraRR(mDNS *const m, AuthRecord *const rr, mStatus result)
	{
	ExtraResourceRecord *extra = (ExtraResourceRecord *)rr->RecordContext;
	(void)m;  //unused

	if (result != mStatus_MemFree) { LogMsg("Error: FreeExtraRR invoked with unexpected error %d", result); return; }

	debugf("%##s: MemFree", rr->resrec.name->c);
	if (rr->resrec.rdata != &rr->rdatastorage)
		freeL("Extra RData", rr->resrec.rdata);
	freeL("ExtraResourceRecord/FreeExtraRR", extra);
	}

mDNSlocal void free_service_instance(service_instance *srv)
	{
	request_state *request = srv->request;
	ExtraResourceRecord *e = srv->srs.Extras, *tmp;

	// clear pointers from parent struct
	if (request)
		{
		service_instance **p = &request->u.servicereg.instances;
		while (*p)
			{
			if (*p == srv)
				{
				*p = (*p)->next;
				break;
				}
			p = &(*p)->next;
			}
		}

	while (e)
		{
		e->r.RecordContext = e;
		tmp = e;
		e = e->next;
		FreeExtraRR(&mDNSStorage, &tmp->r, mStatus_MemFree);
		}

	if (srv->subtypes) { freeL("ServiceSubTypes", srv->subtypes); srv->subtypes = NULL; }
	freeL("service_instance", srv);
	}

// Count how many other service records we have locally with the same name, but different rdata.
// For auto-named services, we can have at most one per machine -- if we allowed two auto-named services of
// the same type on the same machine, we'd get into an infinite autoimmune-response loop of continuous renaming.
mDNSexport int CountPeerRegistrations(mDNS *const m, ServiceRecordSet *const srs)
	{
	int count = 0;
	ResourceRecord *r = &srs->RR_SRV.resrec;
	AuthRecord *rr;
	ServiceRecordSet *s;

	for (rr = m->ResourceRecords; rr; rr=rr->next)
		if (rr->resrec.rrtype == kDNSType_SRV && SameDomainName(rr->resrec.name, r->name) && !SameRData(&rr->resrec, r))
			count++;

	for (s = m->ServiceRegistrations; s; s = s->uDNS_next)
		if (s->state != regState_Unregistered && SameDomainName(s->RR_SRV.resrec.name, r->name) && !SameRData(&s->RR_SRV.resrec, r))
			count++;

	verbosedebugf("%d peer registrations for %##s", count, r->name->c);
	return(count);
	}

mDNSexport int CountExistingRegistrations(domainname *srv, mDNSIPPort port)
	{
	int count = 0;
	AuthRecord *rr;
	for (rr = mDNSStorage.ResourceRecords; rr; rr=rr->next)
		if (rr->resrec.rrtype == kDNSType_SRV &&
			mDNSSameIPPort(rr->resrec.rdata->u.srv.port, port) &&
			SameDomainName(rr->resrec.name, srv))
			count++;
	return(count);
	}

mDNSlocal void SendServiceRemovalNotification(ServiceRecordSet *const srs)
	{
	reply_state *rep;
	service_instance *instance = srs->ServiceContext;
	if (GenerateNTDResponse(srs->RR_SRV.resrec.name, srs->RR_SRV.resrec.InterfaceID, instance->request, &rep, reg_service_reply_op, 0, mStatus_NoError) != mStatus_NoError)
		LogMsg("%3d: regservice_callback: %##s is not valid DNS-SD SRV name", instance->sd, srs->RR_SRV.resrec.name->c);
	else { append_reply(instance->request, rep); instance->clientnotified = mDNSfalse; }
	}

// service registration callback performs three duties - frees memory for deregistered services,
// handles name conflicts, and delivers completed registration information to the client (via
// process_service_registraion())
mDNSlocal void regservice_callback(mDNS *const m, ServiceRecordSet *const srs, mStatus result)
	{
	mStatus err;
	mDNSBool SuppressError = mDNSfalse;
	service_instance *instance = srs->ServiceContext;
	reply_state         *rep;
	(void)m; // Unused
	if (!srs)      { LogMsg("regservice_callback: srs is NULL %d",                 result); return; }
	if (!instance) { LogMsg("regservice_callback: srs->ServiceContext is NULL %d", result); return; }

	// don't send errors up to client for wide-area, empty-string registrations
	if (instance->request &&
		instance->request->u.servicereg.default_domain &&
		!instance->default_local)
		SuppressError = mDNStrue;

	if (result == mStatus_NoError)
		LogOperation("%3d: DNSServiceRegister(%##s, %u) REGISTERED  ",  instance->sd, srs->RR_SRV.resrec.name->c, mDNSVal16(srs->RR_SRV.resrec.rdata->u.srv.port));
	else if (result == mStatus_MemFree)
		LogOperation("%3d: DNSServiceRegister(%##s, %u) DEREGISTERED",  instance->sd, srs->RR_SRV.resrec.name->c, mDNSVal16(srs->RR_SRV.resrec.rdata->u.srv.port));
	else if (result == mStatus_NameConflict)
		LogOperation("%3d: DNSServiceRegister(%##s, %u) NAME CONFLICT", instance->sd, srs->RR_SRV.resrec.name->c, mDNSVal16(srs->RR_SRV.resrec.rdata->u.srv.port));
	else
		LogOperation("%3d: DNSServiceRegister(%##s, %u) CALLBACK %d",   instance->sd, srs->RR_SRV.resrec.name->c, mDNSVal16(srs->RR_SRV.resrec.rdata->u.srv.port), result);

	if (!instance->request && result != mStatus_MemFree) { LogMsg("regservice_callback: instance->request is NULL %d", result); return; }

	if (result == mStatus_NoError)
		{
		if (instance->request->u.servicereg.allowremotequery)
			{
			ExtraResourceRecord *e;
			srs->RR_ADV.AllowRemoteQuery = mDNStrue;
			srs->RR_PTR.AllowRemoteQuery = mDNStrue;
			srs->RR_SRV.AllowRemoteQuery = mDNStrue;
			srs->RR_TXT.AllowRemoteQuery = mDNStrue;
			for (e = instance->srs.Extras; e; e = e->next) e->r.AllowRemoteQuery = mDNStrue;
			}

		if (GenerateNTDResponse(srs->RR_SRV.resrec.name, srs->RR_SRV.resrec.InterfaceID, instance->request, &rep, reg_service_reply_op, kDNSServiceFlagsAdd, result) != mStatus_NoError)
			LogMsg("%3d: regservice_callback: %##s is not valid DNS-SD SRV name", instance->sd, srs->RR_SRV.resrec.name->c);
		else { append_reply(instance->request, rep); instance->clientnotified = mDNStrue; }

		if (instance->request->u.servicereg.autoname && CountPeerRegistrations(m, srs) == 0)
			RecordUpdatedNiceLabel(m, 0);	// Successfully got new name, tell user immediately
		}
	else if (result == mStatus_MemFree)
		{
		if (instance->request && instance->renameonmemfree)
			{
			instance->renameonmemfree = 0;
			err = mDNS_RenameAndReregisterService(m, srs, &instance->request->u.servicereg.name);
			if (err) LogMsg("ERROR: regservice_callback - RenameAndReregisterService returned %ld", err);
			// error should never happen - safest to log and continue
			}
		else
			free_service_instance(instance);
		}
	else if (result == mStatus_NameConflict)
		{
		if (instance->request->u.servicereg.autorename)
			{
			if (instance->request->u.servicereg.autoname && CountPeerRegistrations(m, srs) == 0)
				{
				// On conflict for an autoname service, rename and reregister *all* autoname services
				IncrementLabelSuffix(&m->nicelabel, mDNStrue);
				m->MainCallback(m, mStatus_ConfigChanged);	// will call back into udsserver_handle_configchange()
				}
			else	// On conflict for a non-autoname service, rename and reregister just that one service
				{
				if (instance->clientnotified) SendServiceRemovalNotification(srs);
				mDNS_RenameAndReregisterService(m, srs, mDNSNULL);
				}
			}
		else
			{
			if (!SuppressError) 
				{
				if (GenerateNTDResponse(srs->RR_SRV.resrec.name, srs->RR_SRV.resrec.InterfaceID, instance->request, &rep, reg_service_reply_op, kDNSServiceFlagsAdd, result) != mStatus_NoError)
					LogMsg("%3d: regservice_callback: %##s is not valid DNS-SD SRV name", instance->sd, srs->RR_SRV.resrec.name->c);
				else { append_reply(instance->request, rep); instance->clientnotified = mDNStrue; }
				}
			free_service_instance(instance);
			}
		}
	else
		{
		if (result != mStatus_NATTraversal) LogMsg("ERROR: unknown result in regservice_callback: %ld", result);
		if (!SuppressError) 
			{
			if (GenerateNTDResponse(srs->RR_SRV.resrec.name, srs->RR_SRV.resrec.InterfaceID, instance->request, &rep, reg_service_reply_op, kDNSServiceFlagsAdd, result) != mStatus_NoError)
				LogMsg("%3d: regservice_callback: %##s is not valid DNS-SD SRV name", instance->sd, srs->RR_SRV.resrec.name->c);
			else { append_reply(instance->request, rep); instance->clientnotified = mDNStrue; }
			}
		free_service_instance(instance);
		}
	}

mDNSlocal void AddDefaultRegDomain(mDNS *const m, domainname *d)
	{
	DNameListElem *newelem = mDNSNULL, *ptr;

	// make sure name not already in list
	for (ptr = m->DefRegList; ptr; ptr = ptr->next)
		{
		if (SameDomainName(&ptr->name, d))
			{ debugf("duplicate addition of default reg domain %##s", d->c); return; }
		}

	newelem = mDNSPlatformMemAllocate(sizeof(*newelem));
	if (!newelem) { LogMsg("Error - malloc"); return; }
	AssignDomainName(&newelem->name, d);
	newelem->next = m->DefRegList;
	m->DefRegList = newelem;

	mDNSPlatformDefaultRegDomainChanged(d, mDNStrue);
	}

mDNSlocal void regrecord_callback(mDNS *const m, AuthRecord *rr, mStatus result)
	{
	(void)m; // Unused
	if (!rr->RecordContext)		// parent struct already freed by termination callback
		{
		if (result == mStatus_NoError)
			LogMsg("Error: regrecord_callback: successful registration of orphaned record");
		else
			{
			if (result != mStatus_MemFree) LogMsg("regrecord_callback: error %d received after parent termination", result);
			freeL("AuthRecord/regrecord_callback", rr);
			}
		}
	else
		{
		registered_record_entry *re = rr->RecordContext;
		request_state *request = re->request;
		int len = sizeof(DNSServiceFlags) + sizeof(mDNSu32) + sizeof(DNSServiceErrorType);
		reply_state *reply = create_reply(reg_record_reply_op, len, request);
		reply->mhdr->client_context = re->client_context;
		reply->rhdr->flags = dnssd_htonl(0);
		reply->rhdr->ifi   = dnssd_htonl(mDNSPlatformInterfaceIndexfromInterfaceID(m, rr->resrec.InterfaceID));
		reply->rhdr->error = dnssd_htonl(result);
	
		if (result)
			{
			// unlink from list, free memory
			registered_record_entry **ptr = &request->u.reg_recs;
			while (*ptr && (*ptr) != re) ptr = &(*ptr)->next;
			if (!*ptr) { LogMsg("regrecord_callback - record not in list!"); return; }
			*ptr = (*ptr)->next;
			freeL("registered_record_entry AuthRecord regrecord_callback", re->rr);
			freeL("registered_record_entry regrecord_callback", re);
			}
		append_reply(request, reply);
		}
	}

mDNSlocal void connection_termination(request_state *request)
	{
	request_state **req = &all_requests;
	while (*req)
		{
		if ((*req)->primary == request)
			{
			// Since we're already doing a list traversal, we unlink the request directly instead of using AbortUnlinkAndFree()
			request_state *tmp = *req;
			abort_request(tmp);
			*req = tmp->next;
			freeL("request_state/connection_termination", tmp);
			}
		else
			req = &(*req)->next;
		}

	while (request->u.reg_recs)
		{
		registered_record_entry *ptr = request->u.reg_recs;
		request->u.reg_recs = request->u.reg_recs->next;
		ptr->rr->RecordContext = NULL;
		mDNS_Deregister(&mDNSStorage, ptr->rr);		// Will free ptr->rr for us
		freeL("registered_record_entry/connection_termination", ptr);
		}
	}

mDNSlocal void handle_cancel_request(request_state *request)
	{
	request_state **req = &all_requests;
	LogOperation("%3d: Cancel", request->sd);
	while (*req)
		{
		if ((*req)->primary == request &&
			(*req)->hdr.client_context.ptr64[0] == request->hdr.client_context.ptr64[0] &&
			(*req)->hdr.client_context.ptr64[1] == request->hdr.client_context.ptr64[2])
			{
			// Since we're already doing a list traversal, we unlink the request directly instead of using AbortUnlinkAndFree()
			request_state *tmp = *req;
			abort_request(tmp);
			*req = tmp->next;
			freeL("request_state/handle_cancel_request", tmp);
			}
		else
			req = &(*req)->next;
		}
	}

mDNSlocal mStatus handle_regrecord_request(request_state *request)
	{
	registered_record_entry *re;
	mStatus err;

	AuthRecord *rr = read_rr_from_ipc_msg(&request->msgptr, 1, 1);
	if (!rr) return(mStatus_BadParamErr);

	// allocate registration entry, link into list
	re = mallocL("registered_record_entry", sizeof(registered_record_entry));
	if (!re) FatalError("ERROR: malloc");
	re->key = request->hdr.reg_index;
	re->rr = rr;
	re->request = request;
	re->client_context = request->hdr.client_context;
	rr->RecordContext = re;
	rr->RecordCallback = regrecord_callback;
	re->next = request->u.reg_recs;
	request->u.reg_recs = re;

	if (rr->resrec.rroriginalttl == 0)
		rr->resrec.rroriginalttl = DefaultTTLforRRType(rr->resrec.rrtype);

	LogOperation("%3d: DNSServiceRegisterRecord %s", request->sd, RRDisplayString(&mDNSStorage, &rr->resrec));
	err = mDNS_Register(&mDNSStorage, rr);
	return(err);
	}

mDNSlocal mStatus add_record_to_service(request_state *request, service_instance *instance, mDNSu16 rrtype, mDNSu16 rdlen, char *rdata, mDNSu32 ttl)
	{
	ServiceRecordSet *srs = &instance->srs;
	mStatus result;
	int size = rdlen > sizeof(RDataBody) ? rdlen : sizeof(RDataBody);
	ExtraResourceRecord *extra = mallocL("ExtraResourceRecord", sizeof(*extra) - sizeof(RDataBody) + size);
	if (!extra) { my_perror("ERROR: malloc"); return mStatus_NoMemoryErr; }

	mDNSPlatformMemZero(extra, sizeof(ExtraResourceRecord));  // OK if oversized rdata not zero'd
	extra->r.resrec.rrtype = rrtype;
	extra->r.rdatastorage.MaxRDLength = (mDNSu16) size;
	extra->r.resrec.rdlength = rdlen;
	mDNSPlatformMemCopy(&extra->r.rdatastorage.u.data, rdata, rdlen);

	result = mDNS_AddRecordToService(&mDNSStorage, srs, extra, &extra->r.rdatastorage, ttl);
	if (result) { freeL("ExtraResourceRecord/add_record_to_service", extra); return result; }

	extra->ClientID = request->hdr.reg_index;
	return result;
	}

mDNSlocal mStatus handle_add_request(request_state *request)
	{
	service_instance *i;
	mStatus result = mStatus_UnknownErr;
	DNSServiceFlags flags  = get_flags(&request->msgptr);
	mDNSu16        rrtype = get_uint16(&request->msgptr);
	mDNSu16        rdlen  = get_uint16(&request->msgptr);
	char           *rdata  = get_rdata(&request->msgptr, rdlen);
	mDNSu32        ttl    = get_uint32(&request->msgptr);
	if (!ttl) ttl = DefaultTTLforRRType(rrtype);
	(void)flags; // Unused

	LogOperation("%3d: DNSServiceAddRecord(%##s, %s, %d)", request->sd,
		(request->u.servicereg.instances) ? request->u.servicereg.instances->srs.RR_SRV.resrec.name->c : NULL, DNSTypeName(rrtype), rdlen);

	for (i = request->u.servicereg.instances; i; i = i->next)
		{
		result = add_record_to_service(request, i, rrtype, rdlen, rdata, ttl);
		if (result && i->default_local) break;
		else result = mStatus_NoError;  // suppress non-local default errors
		}

	return(result);
	}

mDNSlocal void update_callback(mDNS *const m, AuthRecord *const rr, RData *oldrd)
	{
	(void)m; // Unused
	if (oldrd != &rr->rdatastorage) freeL("RData/update_callback", oldrd);
	}

mDNSlocal mStatus update_record(AuthRecord *rr, mDNSu16 rdlen, char *rdata, mDNSu32 ttl)
	{
	int rdsize;
	RData *newrd;
	mStatus result;

	if (rdlen > sizeof(RDataBody)) rdsize = rdlen;
	else rdsize = sizeof(RDataBody);
	newrd = mallocL("RData/update_record", sizeof(RData) - sizeof(RDataBody) + rdsize);
	if (!newrd) FatalError("ERROR: malloc");
	newrd->MaxRDLength = (mDNSu16) rdsize;
	mDNSPlatformMemCopy(&newrd->u, rdata, rdlen);

	// BIND named (name daemon) doesn't allow TXT records with zero-length rdata. This is strictly speaking correct,
	// since RFC 1035 specifies a TXT record as "One or more <character-string>s", not "Zero or more <character-string>s".
	// Since some legacy apps try to create zero-length TXT records, we'll silently correct it here.
	if (rr->resrec.rrtype == kDNSType_TXT && rdlen == 0) { rdlen = 1; newrd->u.txt.c[0] = 0; }

	result = mDNS_Update(&mDNSStorage, rr, ttl, rdlen, newrd, update_callback);
	if (result) { LogMsg("ERROR: mDNS_Update - %ld", result); freeL("RData/update_record", newrd); }
	return result;
	}

mDNSlocal mStatus handle_update_request(request_state *request)
	{
	mDNSu16 rdlen;
	char *ptr, *rdata;
	mDNSu32 ttl;
	mStatus result = mStatus_BadReferenceErr;
	service_instance *i;
	AuthRecord *rr = NULL;

	// get the message data
	ptr = request->msgptr;
	get_flags(&ptr);	// flags unused
	rdlen = get_uint16(&ptr);
	rdata = get_rdata(&ptr, rdlen);
	ttl = get_uint32(&ptr);

	if (request->terminate == connection_termination)
		{
		// update an individually registered record
		registered_record_entry *reptr;
		for (reptr = request->u.reg_recs; reptr; reptr = reptr->next)
			{
			if (reptr->key == request->hdr.reg_index)
				{
				result = update_record(reptr->rr, rdlen, rdata, ttl);
				goto end;
				}
			}
		result = mStatus_BadReferenceErr;
		goto end;
		}

	// update a record from a service record set
	for (i = request->u.servicereg.instances; i; i = i->next)
		{
		if (request->hdr.reg_index == TXT_RECORD_INDEX) rr = &i->srs.RR_TXT;
		else
			{
			ExtraResourceRecord *e;
			for (e = i->srs.Extras; e; e = e->next)
				if (e->ClientID == request->hdr.reg_index) { rr = &e->r; break; }
			}

		if (!rr) { result = mStatus_BadReferenceErr; goto end; }
		result = update_record(rr, rdlen, rdata, ttl);
		if (result && i->default_local) goto end;
		else result = mStatus_NoError;  // suppress non-local default errors
		}

end:
	LogOperation("%3d: DNSServiceUpdateRecord(%##s, %s)", request->sd,
		(request->u.servicereg.instances) ? request->u.servicereg.instances->srs.RR_SRV.resrec.name->c : NULL,
		rr ? DNSTypeName(rr->resrec.rrtype) : "<NONE>");

	return(result);
	}

// remove a resource record registered via DNSServiceRegisterRecord()
mDNSlocal mStatus remove_record(request_state *request)
	{
	mStatus err = mStatus_UnknownErr;
	registered_record_entry *e, **ptr = &request->u.reg_recs;

	while (*ptr && (*ptr)->key != request->hdr.reg_index) ptr = &(*ptr)->next;
	if (!*ptr) { LogMsg("DNSServiceRemoveRecord - bad reference"); return mStatus_BadReferenceErr; }
	e = *ptr;
	*ptr = e->next; // unlink

	LogOperation("%3d: DNSServiceRemoveRecord(%##s)", request->sd, e->rr->resrec.name->c);
	e->rr->RecordContext = NULL;
	err = mDNS_Deregister(&mDNSStorage, e->rr);
	if (err)
		{
		LogMsg("ERROR: remove_record, mDNS_Deregister: %ld", err);
		freeL("registered_record_entry AuthRecord remove_record", e->rr);
		}
	freeL("registered_record_entry remove_record", e);
	return err;
	}

mDNSlocal mStatus remove_extra(const request_state *const request, service_instance *const serv, mDNSu16 *const rrtype)
	{
	mStatus err = mStatus_BadReferenceErr;
	ExtraResourceRecord *ptr;

	for (ptr = serv->srs.Extras; ptr; ptr = ptr->next)
		{
		if (ptr->ClientID == request->hdr.reg_index) // found match
			{
			*rrtype = ptr->r.resrec.rrtype;
			return mDNS_RemoveRecordFromService(&mDNSStorage, &serv->srs, ptr, FreeExtraRR, ptr);
			}
		}
	return err;
	}

mDNSlocal mStatus handle_removerecord_request(request_state *request)
	{
	mStatus err = mStatus_BadReferenceErr;
	char *ptr;

	ptr = request->msgptr;
	get_flags(&ptr);	// flags unused

	if (request->terminate == connection_termination)
		err = remove_record(request);  // remove individually registered record
	else
		{
		service_instance *i;
		mDNSu16 rrtype = 0;
		for (i = request->u.servicereg.instances; i; i = i->next)
			{
			err = remove_extra(request, i, &rrtype);
			if (err && i->default_local) break;
			else err = mStatus_NoError;  // suppress non-local default errors
			}
		LogOperation("%3d: DNSServiceRemoveRecord(%##s, %s)", request->sd,
			(request->u.servicereg.instances) ? request->u.servicereg.instances->srs.RR_SRV.resrec.name->c : NULL,
			rrtype ? DNSTypeName(rrtype) : "<NONE>");
		}

	return(err);
	}

mDNSlocal void RemoveDefaultRegDomain(mDNS *const m, domainname *d)
	{
	DNameListElem **p = &m->DefRegList;

	while ((*p))
		{
		if (SameDomainName(&(*p)->name, d))
			{
			DNameListElem *ptr = *p;
			*p = ptr->next;
			mDNSPlatformMemFree(ptr);
			mDNSPlatformDefaultRegDomainChanged(d, mDNSfalse);
			return;
			}
		p = &(*p)->next;
		}
	debugf("Requested removal of default registration domain %##s not in contained in list", d->c);
	}

// If there's a comma followed by another character,
// FindFirstSubType overwrites the comma with a nul and returns the pointer to the next character.
// Otherwise, it returns a pointer to the final nul at the end of the string
mDNSlocal char *FindFirstSubType(char *p)
	{
	while (*p)
		{
		if (p[0] == '\\' && p[1]) p += 2;
		else if (p[0] == ',' && p[1]) { *p++ = 0; return(p); }
		else p++;
		}
	return(p);
	}

// If there's a comma followed by another character,
// FindNextSubType overwrites the comma with a nul and returns the pointer to the next character.
// If it finds an illegal unescaped dot in the subtype name, it returns mDNSNULL
// Otherwise, it returns a pointer to the final nul at the end of the string
mDNSlocal char *FindNextSubType(char *p)
	{
	while (*p)
		{
		if (p[0] == '\\' && p[1])		// If escape character
			p += 2;						// ignore following character
		else if (p[0] == ',')			// If we found a comma
			{
			if (p[1]) *p++ = 0;
			return(p);
			}
		else if (p[0] == '.')
			return(mDNSNULL);
		else p++;
		}
	return(p);
	}

// Returns -1 if illegal subtype found
mDNSexport mDNSs32 ChopSubTypes(char *regtype)
	{
	mDNSs32 NumSubTypes = 0;
	char *stp = FindFirstSubType(regtype);
	while (stp && *stp)					// If we found a comma...
		{
		if (*stp == ',') return(-1);
		NumSubTypes++;
		stp = FindNextSubType(stp);
		}
	if (!stp) return(-1);
	return(NumSubTypes);
	}

mDNSexport AuthRecord *AllocateSubTypes(mDNSs32 NumSubTypes, char *p)
	{
	AuthRecord *st = mDNSNULL;
	if (NumSubTypes)
		{
		mDNSs32 i;
		st = mallocL("ServiceSubTypes", NumSubTypes * sizeof(AuthRecord));
		if (!st) return(mDNSNULL);
		for (i = 0; i < NumSubTypes; i++)
			{
			mDNS_SetupResourceRecord(&st[i], mDNSNULL, mDNSInterface_Any, kDNSQType_ANY, kStandardTTL, 0, mDNSNULL, mDNSNULL);
			while (*p) p++;
			p++;
			if (!MakeDomainNameFromDNSNameString(&st[i].namestorage, p))
				{ freeL("ServiceSubTypes", st); return(mDNSNULL); }
			}
		}
	return(st);
	}

mDNSlocal mStatus register_service_instance(request_state *request, const domainname *domain)
	{
	service_instance *ptr, *instance;
	int instance_size;
	mStatus result;

	for (ptr = request->u.servicereg.instances; ptr; ptr = ptr->next)
		{
		if (SameDomainName(&ptr->domain, domain))
			{ LogMsg("register_service_instance: domain %##s already registered", domain->c); return mStatus_AlreadyRegistered; }
		}

	instance_size = sizeof(*instance);
	if (request->u.servicereg.txtlen > sizeof(RDataBody)) instance_size += (request->u.servicereg.txtlen - sizeof(RDataBody));
	instance = mallocL("service_instance", instance_size);
	if (!instance) { my_perror("ERROR: malloc"); return mStatus_NoMemoryErr; }

	instance->subtypes = AllocateSubTypes(request->u.servicereg.num_subtypes, request->u.servicereg.type_as_string);
	if (request->u.servicereg.num_subtypes && !instance->subtypes)
		{ free_service_instance(instance); instance = NULL; FatalError("ERROR: malloc"); }
	instance->request           = request;
	instance->sd                = request->sd;
	instance->renameonmemfree   = 0;
	instance->clientnotified    = mDNSfalse;
	AssignDomainName(&instance->domain, domain);
	instance->default_local = (request->u.servicereg.default_domain && SameDomainName(domain, &localdomain));
	result = mDNS_RegisterService(&mDNSStorage, &instance->srs,
		&request->u.servicereg.name, &request->u.servicereg.type, domain,
		request->u.servicereg.host.c[0] ? &request->u.servicereg.host : NULL,
		request->u.servicereg.port,
		request->u.servicereg.txtdata, request->u.servicereg.txtlen,
		instance->subtypes, request->u.servicereg.num_subtypes,
		request->u.servicereg.InterfaceID, regservice_callback, instance);

	if (result)
		{
		LogMsg("register_service_instance %#s.%##s%##s error %d",
			&request->u.servicereg.name, &request->u.servicereg.type, domain->c, result);
		free_service_instance(instance);
		}
	else
		{
		instance->next = request->u.servicereg.instances;
		request->u.servicereg.instances = instance;
		}
	return result;
	}

mDNSlocal void UpdateDeviceInfoRecord(mDNS *const m);

mDNSlocal void regservice_termination_callback(request_state *request)
	{
	service_instance *i, *p;
	if (!request) { LogMsg("regservice_termination_callback context is NULL"); return; }
	i = request->u.servicereg.instances;
	while (i)
		{
		p = i;
		i = i->next;
		p->request = NULL;  // clear back pointer
		// only safe to free memory if registration is not valid, i.e. deregister fails (which invalidates p)
		LogOperation("%3d: DNSServiceRegister(%##s, %u) STOP",
			request->sd, p->srs.RR_SRV.resrec.name->c, mDNSVal16(p->srs.RR_SRV.resrec.rdata->u.srv.port));
		if (mDNS_DeregisterService(&mDNSStorage, &p->srs)) free_service_instance(p);
		}
	if (request->u.servicereg.txtdata)
		{ freeL("service_info txtdata", request->u.servicereg.txtdata); request->u.servicereg.txtdata = NULL; }
	if (request->u.servicereg.autoname) UpdateDeviceInfoRecord(&mDNSStorage);
	}

mDNSexport void udsserver_default_reg_domain_changed(const domainname *d, mDNSBool add)
	{
	request_state *request;

	LogMsg("%s registration domain %##s", add ? "Adding" : "Removing", d->c);
	for (request = all_requests; request; request = request->next)
		{
		if (request->terminate != regservice_termination_callback) continue;
		if (!request->u.servicereg.default_domain) continue;

		// valid default registration
		if (add) register_service_instance(request, d);
		else
			{
			// find the instance to remove
			service_instance **p = &request->u.servicereg.instances;
			while (*p)
				{
				if (SameDomainName(&(*p)->domain, d))
					{
					mStatus err;
					service_instance *si = *p;
					*p = si->next;
					if (si->clientnotified) SendServiceRemovalNotification(&si->srs);
					err = mDNS_DeregisterService(&mDNSStorage, &si->srs);
					if (err)
						{
						LogMsg("udsserver_default_reg_domain_changed - mDNS_DeregisterService err %d", err);
						free_service_instance(si);
						}
					break;
					}
				p = &(*p)->next;
				}
			if (!*p) debugf("udsserver_default_reg_domain_changed - domain %##s not registered", d->c); // normal if registration failed
			}
		}
	}

mDNSlocal mStatus handle_regservice_request(request_state *request)
	{
	char *ptr = request->msgptr;
	char name[256];	// Lots of spare space for extra-long names that we'll auto-truncate down to 63 bytes
	char domain[MAX_ESCAPED_DOMAIN_NAME], host[MAX_ESCAPED_DOMAIN_NAME];
	char type_as_string[MAX_ESCAPED_DOMAIN_NAME];
	domainname d, srv;
	mStatus err;

	DNSServiceFlags flags = get_flags(&ptr);
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	mDNSInterfaceID InterfaceID = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !InterfaceID)
		{ LogMsg("ERROR: handle_regservice_request - Couldn't find interfaceIndex %d", interfaceIndex); return(mStatus_BadParamErr); }

	if (get_string(&ptr, name, sizeof(name)) < 0 ||
		get_string(&ptr, type_as_string, MAX_ESCAPED_DOMAIN_NAME) < 0 ||
		get_string(&ptr, domain, MAX_ESCAPED_DOMAIN_NAME) < 0 ||
		get_string(&ptr, host, MAX_ESCAPED_DOMAIN_NAME) < 0)
		{ LogMsg("ERROR: handle_regservice_request - Couldn't read name/regtype/domain"); return(mStatus_BadParamErr); }

	request->u.servicereg.InterfaceID = InterfaceID;
	request->u.servicereg.instances = NULL;
	request->u.servicereg.txtlen  = 0;
	request->u.servicereg.txtdata = NULL;
	mDNSPlatformStrCopy(request->u.servicereg.type_as_string, type_as_string);

	request->u.servicereg.port.b[0] = *ptr++;
	request->u.servicereg.port.b[1] = *ptr++;

	request->u.servicereg.txtlen = get_uint16(&ptr);
	if (request->u.servicereg.txtlen)
		{
		request->u.servicereg.txtdata = mallocL("service_info txtdata", request->u.servicereg.txtlen);
		if (!request->u.servicereg.txtdata) FatalError("ERROR: handle_regservice_request - malloc");
		mDNSPlatformMemCopy(request->u.servicereg.txtdata, get_rdata(&ptr, request->u.servicereg.txtlen), request->u.servicereg.txtlen);
		}
	else request->u.servicereg.txtdata = NULL;

	// Check for sub-types after the service type
	request->u.servicereg.num_subtypes = ChopSubTypes(request->u.servicereg.type_as_string);	// Note: Modifies regtype string to remove trailing subtypes
	if (request->u.servicereg.num_subtypes < 0)
		{ LogMsg("ERROR: handle_regservice_request - ChopSubTypes failed %s", request->u.servicereg.type_as_string); return(mStatus_BadParamErr); }

	// Don't try to construct "domainname t" until *after* ChopSubTypes has worked its magic
	if (!*request->u.servicereg.type_as_string || !MakeDomainNameFromDNSNameString(&request->u.servicereg.type, request->u.servicereg.type_as_string))
		{ LogMsg("ERROR: handle_regservice_request - type_as_string bad %s", request->u.servicereg.type_as_string); return(mStatus_BadParamErr); }

	if (!name[0])
		{
		request->u.servicereg.name = mDNSStorage.nicelabel;
		request->u.servicereg.autoname = mDNStrue;
		}
	else
		{
		// If the client is allowing AutoRename, then truncate name to legal length before converting it to a DomainLabel
		if ((flags & kDNSServiceFlagsNoAutoRename) == 0)
			{
			int newlen = TruncateUTF8ToLength((mDNSu8*)name, mDNSPlatformStrLen(name), MAX_DOMAIN_LABEL);
			name[newlen] = 0;
			}
		if (!MakeDomainLabelFromLiteralString(&request->u.servicereg.name, name))
			{ LogMsg("ERROR: handle_regservice_request - name bad %s", name); return(mStatus_BadParamErr); }
		request->u.servicereg.autoname = mDNSfalse;
		}

	if (*domain)
		{
		request->u.servicereg.default_domain = mDNSfalse;
		if (!MakeDomainNameFromDNSNameString(&d, domain))
			{ LogMsg("ERROR: handle_regservice_request - domain bad %s", domain); return(mStatus_BadParamErr); }
		}
	else
		{
		request->u.servicereg.default_domain = mDNStrue;
		MakeDomainNameFromDNSNameString(&d, "local.");
		}

	if (!ConstructServiceName(&srv, &request->u.servicereg.name, &request->u.servicereg.type, &d))
		{
		LogMsg("ERROR: handle_regservice_request - Couldn't ConstructServiceName from, “%#s” “%##s” “%##s”",
			request->u.servicereg.name.c, request->u.servicereg.type.c, d.c); return(mStatus_BadParamErr);
		}

	if (!MakeDomainNameFromDNSNameString(&request->u.servicereg.host, host))
		{ LogMsg("ERROR: handle_regservice_request - host bad %s", host); return(mStatus_BadParamErr); }
	request->u.servicereg.autorename       = (flags & kDNSServiceFlagsNoAutoRename    ) == 0;
	request->u.servicereg.allowremotequery = (flags & kDNSServiceFlagsAllowRemoteQuery) != 0;

	// Some clients use mDNS for lightweight copy protection, registering a pseudo-service with
	// a port number of zero. When two instances of the protected client are allowed to run on one
	// machine, we don't want to see misleading "Bogus client" messages in syslog and the console.
	if (!mDNSIPPortIsZero(request->u.servicereg.port))
		{
		int count = CountExistingRegistrations(&srv, request->u.servicereg.port);
		if (count)
			LogMsg("Client application registered %d identical instances of service %##s port %u.",
				count+1, srv.c, mDNSVal16(request->u.servicereg.port));
		}

	LogOperation("%3d: DNSServiceRegister(\"%s\", \"%s\", \"%s\", \"%s\", %u) START",
		request->sd, name, request->u.servicereg.type_as_string, domain, host, mDNSVal16(request->u.servicereg.port));
	err = register_service_instance(request, &d);

	if (!err && !*domain)
		{
		DNameListElem *ptr;
		for (ptr = mDNSStorage.DefRegList; ptr; ptr = ptr->next)
			register_service_instance(request, &ptr->name);
			// note that we don't report errors for non-local, non-explicit domains
		}

	if (!err)
		{
		request->terminate = regservice_termination_callback;
		if (request->u.servicereg.autoname) UpdateDeviceInfoRecord(&mDNSStorage);
		}

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceBrowse
#endif

// Manually-generated local-only PTR records for browse domains we get from SCPreferences
static ARListElem *SCPrefBrowseDomains = mDNSNULL;

mDNSlocal void FoundInstance(mDNS *const m, DNSQuestion *question, const ResourceRecord *const answer, mDNSBool AddRecord)
	{
	const DNSServiceFlags flags = AddRecord ? kDNSServiceFlagsAdd : 0;
	request_state *req = question->QuestionContext;
	reply_state *rep;
	(void)m; // Unused

	if (answer->rrtype != kDNSType_PTR)
		{ LogMsg("%3d: FoundInstance: Should not be called with rrtype %d (not a PTR record)", req->sd, answer->rrtype); return; }

	if (GenerateNTDResponse(&answer->rdata->u.name, answer->InterfaceID, req, &rep, browse_reply_op, flags, mStatus_NoError) != mStatus_NoError)
		{
		LogMsg("%3d: FoundInstance: %##s PTR %##s received from network is not valid DNS-SD service pointer",
			req->sd, answer->name->c, answer->rdata->u.name.c);
		return;
		}

	LogOperation("%3d: DNSServiceBrowse(%##s, %s) RESULT %s %d: %s",
		req->sd, question->qname.c, DNSTypeName(question->qtype), AddRecord ? "Add" : "Rmv",
		mDNSPlatformInterfaceIndexfromInterfaceID(m, answer->InterfaceID), RRDisplayString(m, answer));

	append_reply(req, rep);
	}

mDNSlocal mStatus add_domain_to_browser(request_state *info, const domainname *d)
	{
	browser_t *b, *p;
	mStatus err;

	for (p = info->u.browser.browsers; p; p = p->next)
		{
		if (SameDomainName(&p->domain, d))
			{ debugf("add_domain_to_browser %##s already in list", d->c); return mStatus_AlreadyRegistered; }
		}

	b = mallocL("browser_t", sizeof(*b));
	if (!b) return mStatus_NoMemoryErr;
	AssignDomainName(&b->domain, d);
	err = mDNS_StartBrowse(&mDNSStorage, &b->q,
		&info->u.browser.regtype, d, info->u.browser.interface_id, info->u.browser.ForceMCast, FoundInstance, info);
	if (err)
		{
		LogMsg("mDNS_StartBrowse returned %d for type %##s domain %##s", err, info->u.browser.regtype.c, d->c);
		freeL("browser_t/add_domain_to_browser", b);
		}
	else
		{
		b->next = info->u.browser.browsers;
		info->u.browser.browsers = b;
		}
		return err;
	}

mDNSlocal void browse_termination_callback(request_state *info)
	{
	while (info->u.browser.browsers)
		{
		browser_t *ptr = info->u.browser.browsers;
		info->u.browser.browsers = ptr->next;
		LogOperation("%3d: DNSServiceBrowse(%##s) STOP", info->sd, ptr->q.qname.c);
		mDNS_StopBrowse(&mDNSStorage, &ptr->q);  // no need to error-check result
		freeL("browser_t/browse_termination_callback", ptr);
		}
	}

mDNSlocal void udsserver_automatic_browse_domain_changed(const domainname *d, mDNSBool add)
	{
	request_state *request;
	debugf("DefaultBrowseDomainChanged: %s default browse domain %##s", add ? "Adding" : "Removing", d->c);

#if APPLE_OSX_mDNSResponder
	extern void machserver_automatic_browse_domain_changed(const domainname *d, mDNSBool add);
	machserver_automatic_browse_domain_changed(d, add);
#endif // APPLE_OSX_mDNSResponder

	for (request = all_requests; request; request = request->next)
		{
		if (request->terminate != browse_termination_callback) continue;
		if (!request->u.browser.default_domain) continue;
		if (add) add_domain_to_browser(request, d);
		else
			{
			browser_t **ptr = &request->u.browser.browsers;
			while (*ptr)
				{
				if (SameDomainName(&(*ptr)->domain, d))
					{
					browser_t *remove = *ptr;
					*ptr = (*ptr)->next;
					mDNS_StopQueryWithRemoves(&mDNSStorage, &remove->q);
					freeL("browser_t/udsserver_automatic_browse_domain_changed", remove);
					break;
					}
				ptr = &(*ptr)->next;
				}
			}
		}
	}

mDNSlocal void FreeARElemCallback(mDNS *const m, AuthRecord *const rr, mStatus result)
	{
	(void)m;  // unused
	if (result == mStatus_MemFree) mDNSPlatformMemFree(rr->RecordContext);
	}

mDNSlocal void RegisterBrowseDomainPTR(mDNS *m, const domainname *d, int type)
	{
	// allocate/register legacy and non-legacy _browse PTR record
	mStatus err;
	ARListElem *browse = mDNSPlatformMemAllocate(sizeof(*browse));
	mDNS_SetupResourceRecord(&browse->ar, mDNSNULL, mDNSInterface_LocalOnly, kDNSType_PTR, 7200, kDNSRecordTypeShared, FreeARElemCallback, browse);
	MakeDomainNameFromDNSNameString(&browse->ar.namestorage, mDNS_DomainTypeNames[type]);
	AppendDNSNameString            (&browse->ar.namestorage, "local");
	AssignDomainName(&browse->ar.resrec.rdata->u.name, d);
	err = mDNS_Register(m, &browse->ar);
	if (err)
		{
		LogMsg("SetSCPrefsBrowseDomain: mDNS_Register returned error %d", err);
		mDNSPlatformMemFree(browse);
		}
	else
		{
		browse->next = SCPrefBrowseDomains;
		SCPrefBrowseDomains = browse;
		}
	}

mDNSlocal void DeregisterBrowseDomainPTR(mDNS *m, const domainname *d, int type)
	{
	ARListElem **ptr = &SCPrefBrowseDomains;
	domainname lhs; // left-hand side of PTR, for comparison

	MakeDomainNameFromDNSNameString(&lhs, mDNS_DomainTypeNames[type]);
	AppendDNSNameString            (&lhs, "local");

	while (*ptr)
		{
		if (SameDomainName(&(*ptr)->ar.resrec.rdata->u.name, d) && SameDomainName((*ptr)->ar.resrec.name, &lhs))
			{
			ARListElem *remove = *ptr;
			*ptr = (*ptr)->next;
			mDNS_Deregister(m, &remove->ar);
			return;
			}
		else ptr = &(*ptr)->next;
		}
	}

mDNSlocal void SetPrefsBrowseDomain(mDNS *m, const domainname *d, mDNSBool add)
	{
	LogOperation("SetPrefsBrowseDomain: %s default browse domain refcount for %##s",
		add ? "Incrementing" : "Decrementing", d->c);

	if (add)
		{
		RegisterBrowseDomainPTR(m, d, mDNS_DomainTypeBrowse);
		RegisterBrowseDomainPTR(m, d, mDNS_DomainTypeBrowseLegacy);
		}
	else
		{
		DeregisterBrowseDomainPTR(m, d, mDNS_DomainTypeBrowse);
		DeregisterBrowseDomainPTR(m, d, mDNS_DomainTypeBrowseLegacy);
		}
	}

mDNSlocal void SetPrefsBrowseDomains(mDNS *m, DNameListElem *browseDomains, mDNSBool add)
	{
	DNameListElem *browseDomain;
	for (browseDomain = browseDomains; browseDomain; browseDomain = browseDomain->next)
		if (browseDomain->name.c[0]) SetPrefsBrowseDomain(m, &browseDomain->name, add);
		else LogMsg("SetPrefsBrowseDomains bad browse domain: %p", browseDomain);
	}

mDNSlocal void UpdateDeviceInfoRecord(mDNS *const m)
	{
	int num_autoname = 0;
	request_state *req;
	for (req = all_requests; req; req = req->next)
		if (req->terminate == regservice_termination_callback && req->u.servicereg.autoname)
			num_autoname++;

	// If DeviceInfo record is currently registered, see if we need to deregister it
	if (m->DeviceInfo.resrec.RecordType != kDNSRecordTypeUnregistered)
		if (num_autoname == 0 || !SameDomainLabelCS(m->DeviceInfo.resrec.name->c, m->nicelabel.c))
			{
			LogOperation("UpdateDeviceInfoRecord Deregister %##s", m->DeviceInfo.resrec.name);
			mDNS_Deregister(m, &m->DeviceInfo);
			}

	// If DeviceInfo record is not currently registered, see if we need to register it
	if (m->DeviceInfo.resrec.RecordType == kDNSRecordTypeUnregistered)
		if (num_autoname > 0)
			{
			mDNSu8 len = m->HIHardware.c[0] < 255 - 6 ? m->HIHardware.c[0] : 255 - 6;
			mDNS_SetupResourceRecord(&m->DeviceInfo, mDNSNULL, mDNSNULL, kDNSType_TXT, kStandardTTL, kDNSRecordTypeAdvisory, mDNSNULL, mDNSNULL);
			ConstructServiceName(&m->DeviceInfo.namestorage, &m->nicelabel, &DeviceInfoName, &localdomain);
			mDNSPlatformMemCopy(m->DeviceInfo.resrec.rdata->u.data + 1, "model=", 6);
			mDNSPlatformMemCopy(m->DeviceInfo.resrec.rdata->u.data + 7, m->HIHardware.c + 1, len);
			m->DeviceInfo.resrec.rdata->u.data[0] = 6 + len;	// "model=" plus the device string
			m->DeviceInfo.resrec.rdlength         = 7 + len;	// One extra for the length byte at the start of the string
			LogOperation("UpdateDeviceInfoRecord   Register %##s", m->DeviceInfo.resrec.name);
			mDNS_Register(m, &m->DeviceInfo);
			}
	}

mDNSexport void udsserver_handle_configchange(mDNS *const m)
	{
	request_state *req;
	service_instance *ptr;
	domainname      regDomain;
	DNameListElem *browseDomains;

	UpdateDeviceInfoRecord(m);

	for (req = all_requests; req; req = req->next)
		if (req->terminate == regservice_termination_callback)
			if (req->u.servicereg.autoname && !SameDomainLabelCS(req->u.servicereg.name.c, m->nicelabel.c))
				{
				req->u.servicereg.name = m->nicelabel;
				for (ptr = req->u.servicereg.instances; ptr; ptr = ptr->next)
					{
					ptr->renameonmemfree = 1;
					if (ptr->clientnotified) SendServiceRemovalNotification(&ptr->srs);
					if (mDNS_DeregisterService(m, &ptr->srs)) // If service was deregistered already
						regservice_callback(m, &ptr->srs, mStatus_MemFree); // we can re-register immediately
					}
				}

	// Let the platform layer get the current DNS information
	mDNS_Lock(m);
	mDNSPlatformSetDNSConfig(m, mDNSfalse, mDNSfalse, mDNSNULL, &regDomain, &browseDomains);
	mDNS_Unlock(m);

	// Did our registration domain change?
	if (!SameDomainName(&regDomain, &m->RegDomain))
		{
		if (m->RegDomain.c[0])
			{
			RemoveDefaultRegDomain(m, &m->RegDomain);
			// if we were automatically browsing in our registration domain, stop
			SetPrefsBrowseDomain(m, &m->RegDomain, mDNSfalse);
			}

		AssignDomainName(&m->RegDomain, &regDomain);

		if (m->RegDomain.c[0])
			{
			AddDefaultRegDomain(m, &m->RegDomain);
			SetPrefsBrowseDomain(m, &m->RegDomain, mDNStrue);
			}
		}

	// Add new browse domains to internal list
	if (browseDomains) SetPrefsBrowseDomains(m, browseDomains, mDNStrue);

	// Remove old browse domains from internal list
	if (m->BrowseDomains)
		{
		SetPrefsBrowseDomains(m, m->BrowseDomains, mDNSfalse);
		while (m->BrowseDomains)
			{
			DNameListElem *fptr = m->BrowseDomains;
			m->BrowseDomains = m->BrowseDomains->next;
			mDNSPlatformMemFree(fptr);
			}
		}

	// Replace the old browse domains array with the new array
	m->BrowseDomains = browseDomains;
	}

mDNSlocal void LegacyBrowseDomainChange(mDNS *const m, DNSQuestion *q, const ResourceRecord *const answer, mDNSBool AddRecord)
	{
	(void)m; // unused;
	(void)q; // unused

	LogOperation("LegacyBrowseDomainChange: %s default browse domain %##s",
		AddRecord ? "Adding" : "Removing", answer->rdata->u.name.c);

	if (AddRecord)
		{
		DNameListElem *new = mDNSPlatformMemAllocate(sizeof(DNameListElem));
		if (!new) { LogMsg("ERROR: malloc"); return; }
		AssignDomainName(&new->name, &answer->rdata->u.name);
		new->next = m->DefBrowseList;
		m->DefBrowseList = new;
		udsserver_automatic_browse_domain_changed(&new->name, mDNStrue);
		return;
		}
	else
		{
		DNameListElem **p = &m->DefBrowseList;
		while (*p)
			{
			if (SameDomainName(&(*p)->name, &answer->rdata->u.name))
				{
				DNameListElem *ptr = *p;
				udsserver_automatic_browse_domain_changed(&ptr->name, mDNSfalse);
				*p = ptr->next;
				mDNSPlatformMemFree(ptr);
				return;
				}
			p = &(*p)->next;
			}
		LogMsg("LegacyBrowseDomainChange: Got remove event for domain %##s not in list", answer->rdata->u.name.c);
		}
	}

mDNSlocal void TrackLegacyBrowseDomains(mDNS *const m)
	{
	static DNSQuestion LegacyBrowseDomainQ; // our local enumeration query for _legacy._browse domains

	// start query for domains to be used in default (empty string domain) browses
	mStatus err = mDNS_GetDomains(m, &LegacyBrowseDomainQ, mDNS_DomainTypeBrowseLegacy,
		mDNSNULL, mDNSInterface_LocalOnly, LegacyBrowseDomainChange, mDNSNULL);

	// provide .local automatically
	SetPrefsBrowseDomain(m, &localdomain, mDNStrue);

	// <rdar://problem/4055653> dns-sd -E does not return "local."
	// register registration domain "local"
	RegisterBrowseDomainPTR(m, &localdomain, mDNS_DomainTypeRegistration);
	if (err) LogMsg("ERROR: dDNS_InitDNSConfig - mDNS_Register returned error %d", err);
	}

mDNSlocal mStatus handle_browse_request(request_state *request)
	{
	char *ptr = request->msgptr;

	char regtype[MAX_ESCAPED_DOMAIN_NAME], domain[MAX_ESCAPED_DOMAIN_NAME];
	domainname typedn, d, temp;
	mDNSs32 NumSubTypes;
	mStatus err = mStatus_NoError;

	DNSServiceFlags flags = get_flags(&ptr);
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	mDNSInterfaceID InterfaceID = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !InterfaceID) return(mStatus_BadParamErr);

	if (get_string(&ptr, regtype, MAX_ESCAPED_DOMAIN_NAME) < 0 ||
		get_string(&ptr, domain, MAX_ESCAPED_DOMAIN_NAME) < 0) return(mStatus_BadParamErr);

	if (!domain || (domain[0] == '\0')) uDNS_RegisterSearchDomains(&mDNSStorage);

	typedn.c[0] = 0;
	NumSubTypes = ChopSubTypes(regtype);	// Note: Modifies regtype string to remove trailing subtypes
	if (NumSubTypes < 0 || NumSubTypes > 1) return(mStatus_BadParamErr);
	if (NumSubTypes == 1 && !AppendDNSNameString(&typedn, regtype + strlen(regtype) + 1)) return(mStatus_BadParamErr);

	if (!regtype[0] || !AppendDNSNameString(&typedn, regtype)) return(mStatus_BadParamErr);

	if (!MakeDomainNameFromDNSNameString(&temp, regtype)) return(mStatus_BadParamErr);
	// For over-long service types, we only allow domain "local"
	if (temp.c[0] > 15 && domain[0] == 0) mDNSPlatformStrCopy(domain, "local.");

	// Set up browser info
	request->u.browser.ForceMCast = (flags & kDNSServiceFlagsForceMulticast) != 0;
	request->u.browser.interface_id = InterfaceID;
	AssignDomainName(&request->u.browser.regtype, &typedn);
	request->u.browser.default_domain = !domain[0];
	request->u.browser.browsers = NULL;

	LogOperation("%3d: DNSServiceBrowse(\"%##s\", \"%s\") START", request->sd, request->u.browser.regtype.c, domain);
	if (domain[0])
		{
		if (!MakeDomainNameFromDNSNameString(&d, domain)) return(mStatus_BadParamErr);
		err = add_domain_to_browser(request, &d);
		}

	else
		{
		DNameListElem *sdom;
		for (sdom = mDNSStorage.DefBrowseList; sdom; sdom = sdom->next)
			{
			err = add_domain_to_browser(request, &sdom->name);
			if (err)
				{
				if (SameDomainName(&sdom->name, &localdomain)) break;
				else err = mStatus_NoError;  // suppress errors for non-local "default" domains
				}
			}
		}

	if (!err) request->terminate = browse_termination_callback;

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceResolve
#endif

mDNSlocal void resolve_result_callback(mDNS *const m, DNSQuestion *question, const ResourceRecord *const answer, mDNSBool AddRecord)
	{
	size_t len = 0;
	char fullname[MAX_ESCAPED_DOMAIN_NAME], target[MAX_ESCAPED_DOMAIN_NAME];
	char *data;
	reply_state *rep;
	request_state *req = question->QuestionContext;
	(void)m; // Unused

	LogOperation("%3d: DNSServiceResolve(%##s, %s) %s %s",
		req->sd, question->qname.c, DNSTypeName(question->qtype), AddRecord ? "ADD" : "RMV", RRDisplayString(m, answer));

	// This code used to do this trick of just keeping a copy of the pointer to
	// the answer record in the cache, but the unicast query code doesn't currently
	// put its answer records in the cache, so for now we can't do this.

	if (!AddRecord)
		{
		if (answer->rrtype == kDNSType_SRV && req->u.resolve.srv == answer) req->u.resolve.srv = mDNSNULL;
		if (answer->rrtype == kDNSType_TXT && req->u.resolve.txt == answer) req->u.resolve.txt = mDNSNULL;
		return;
		}

	if (answer->rrtype == kDNSType_SRV) req->u.resolve.srv = answer;
	if (answer->rrtype == kDNSType_TXT) req->u.resolve.txt = answer;

	if (!req->u.resolve.txt || !req->u.resolve.srv) return;		// only deliver result to client if we have both answers

	ConvertDomainNameToCString(answer->name, fullname);
	ConvertDomainNameToCString(&req->u.resolve.srv->rdata->u.srv.target, target);

	// calculate reply length
	len += sizeof(DNSServiceFlags);
	len += sizeof(mDNSu32);  // interface index
	len += sizeof(DNSServiceErrorType);
	len += strlen(fullname) + 1;
	len += strlen(target) + 1;
	len += 2 * sizeof(mDNSu16);  // port, txtLen
	len += req->u.resolve.txt->rdlength;

	// allocate/init reply header
	rep = create_reply(resolve_reply_op, len, req);
	rep->rhdr->flags = dnssd_htonl(0);
	rep->rhdr->ifi   = dnssd_htonl(mDNSPlatformInterfaceIndexfromInterfaceID(m, answer->InterfaceID));
	rep->rhdr->error = dnssd_htonl(kDNSServiceErr_NoError);

	data = rep->sdata;

	// write reply data to message
	put_string(fullname, &data);
	put_string(target, &data);
	*data++ = req->u.resolve.srv->rdata->u.srv.port.b[0];
	*data++ = req->u.resolve.srv->rdata->u.srv.port.b[1];
	put_uint16(req->u.resolve.txt->rdlength, &data);
	put_rdata(req->u.resolve.txt->rdlength, req->u.resolve.txt->rdata->u.data, &data);

	append_reply(req, rep);
	}

mDNSlocal void resolve_termination_callback(request_state *request)
	{
	LogOperation("%3d: DNSServiceResolve(%##s) STOP", request->sd, request->u.resolve.qtxt.qname.c);
	mDNS_StopQuery(&mDNSStorage, &request->u.resolve.qtxt);
	mDNS_StopQuery(&mDNSStorage, &request->u.resolve.qsrv);
	}

mDNSlocal mStatus handle_resolve_request(request_state *request)
	{
	char *ptr = request->msgptr;
	char name[256], regtype[MAX_ESCAPED_DOMAIN_NAME], domain[MAX_ESCAPED_DOMAIN_NAME];
	domainname fqdn;
	mStatus err;

	// extract the data from the message
	DNSServiceFlags flags = get_flags(&ptr);
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	mDNSInterfaceID InterfaceID = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !InterfaceID)
		{ LogMsg("ERROR: handle_resolve_request bad interfaceIndex %d", interfaceIndex); return(mStatus_BadParamErr); }

	if (get_string(&ptr, name, 256) < 0 ||
		get_string(&ptr, regtype, MAX_ESCAPED_DOMAIN_NAME) < 0 ||
		get_string(&ptr, domain, MAX_ESCAPED_DOMAIN_NAME) < 0)
		{ LogMsg("ERROR: handle_resolve_request - Couldn't read name/regtype/domain"); return(mStatus_BadParamErr); }

	if (build_domainname_from_strings(&fqdn, name, regtype, domain) < 0)
		{ LogMsg("ERROR: handle_resolve_request bad “%s” “%s” “%s”", name, regtype, domain); return(mStatus_BadParamErr); }

	mDNSPlatformMemZero(&request->u.resolve, sizeof(request->u.resolve));

	// format questions
	request->u.resolve.qsrv.InterfaceID      = InterfaceID;
	request->u.resolve.qsrv.Target           = zeroAddr;
	AssignDomainName(&request->u.resolve.qsrv.qname, &fqdn);
	request->u.resolve.qsrv.qtype            = kDNSType_SRV;
	request->u.resolve.qsrv.qclass           = kDNSClass_IN;
	request->u.resolve.qsrv.LongLived        = (flags & kDNSServiceFlagsLongLivedQuery     ) != 0;
	request->u.resolve.qsrv.ExpectUnique     = mDNStrue;
	request->u.resolve.qsrv.ForceMCast       = (flags & kDNSServiceFlagsForceMulticast     ) != 0;
	request->u.resolve.qsrv.ReturnIntermed   = (flags & kDNSServiceFlagsReturnIntermediates) != 0;
	request->u.resolve.qsrv.QuestionCallback = resolve_result_callback;
	request->u.resolve.qsrv.QuestionContext  = request;

	request->u.resolve.qtxt.InterfaceID      = InterfaceID;
	request->u.resolve.qtxt.Target           = zeroAddr;
	AssignDomainName(&request->u.resolve.qtxt.qname, &fqdn);
	request->u.resolve.qtxt.qtype            = kDNSType_TXT;
	request->u.resolve.qtxt.qclass           = kDNSClass_IN;
	request->u.resolve.qtxt.LongLived        = (flags & kDNSServiceFlagsLongLivedQuery     ) != 0;
	request->u.resolve.qtxt.ExpectUnique     = mDNStrue;
	request->u.resolve.qtxt.ForceMCast       = (flags & kDNSServiceFlagsForceMulticast     ) != 0;
	request->u.resolve.qtxt.ReturnIntermed   = (flags & kDNSServiceFlagsReturnIntermediates) != 0;
	request->u.resolve.qtxt.QuestionCallback = resolve_result_callback;
	request->u.resolve.qtxt.QuestionContext  = request;

	// ask the questions
	LogOperation("%3d: DNSServiceResolve(%##s) START", request->sd, request->u.resolve.qsrv.qname.c);
	err = mDNS_StartQuery(&mDNSStorage, &request->u.resolve.qsrv);
	if (!err)
		{
		err = mDNS_StartQuery(&mDNSStorage, &request->u.resolve.qtxt);
		if (err) mDNS_StopQuery(&mDNSStorage, &request->u.resolve.qsrv);
		}

	if (!err) request->terminate = resolve_termination_callback;

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceQueryRecord
#endif

// mDNS operation functions. Each operation has 3 associated functions - a request handler that parses
// the client's request and makes the appropriate mDNSCore call, a result handler (passed as a callback
// to the mDNSCore routine) that sends results back to the client, and a termination routine that aborts
// the mDNSCore operation if the client dies or closes its socket.

// query and resolve calls have separate request handlers that parse the arguments from the client and
// massage the name parameters appropriately, but the rest of the operations (making the query call,
// delivering the result to the client, and termination) are identical.

// what gets called when a resolve is completed and we need to send the data back to the client
mDNSlocal void queryrecord_result_callback(mDNS *const m, DNSQuestion *question, const ResourceRecord *const answer, mDNSBool AddRecord)
	{
	char name[MAX_ESCAPED_DOMAIN_NAME];
	request_state *req = question->QuestionContext;
	reply_state *rep;
	char *data;
	size_t len;
	DNSServiceErrorType error = kDNSServiceErr_NoError;
	(void)m; // Unused

	LogOperation("%3d: %s(%##s, %s) %s %s", req->sd,
		req->hdr.op == query_request ? "DNSServiceQueryRecord" : "DNSServiceGetAddrInfo",
		question->qname.c, DNSTypeName(question->qtype), AddRecord ? "ADD" : "RMV", RRDisplayString(m, answer));

	if (answer->RecordType == kDNSRecordTypePacketNegative)
		{
		error = kDNSServiceErr_NoSuchRecord;
		ConvertDomainNameToCString(&question->qname, name);
		AddRecord = mDNStrue;
		}
	else
		ConvertDomainNameToCString(answer->name, name);

	len = sizeof(DNSServiceFlags);	// calculate reply data length
	len += sizeof(mDNSu32);		// interface index
	len += sizeof(DNSServiceErrorType);
	len += strlen(name) + 1;
	len += 3 * sizeof(mDNSu16);	// type, class, rdlen
	len += answer->rdlength;
	len += sizeof(mDNSu32);		// TTL

	rep = create_reply(req->hdr.op == query_request ? query_reply_op : addrinfo_reply_op, len, req);

	rep->rhdr->flags = dnssd_htonl(AddRecord ? kDNSServiceFlagsAdd : 0);
	rep->rhdr->ifi   = dnssd_htonl(mDNSPlatformInterfaceIndexfromInterfaceID(m, answer->InterfaceID));
	rep->rhdr->error = dnssd_htonl(error);

	data = rep->sdata;

	put_string(name, &data);

	if (answer->RecordType == kDNSRecordTypePacketNegative)
		{
		put_uint16(question->qtype, &data);
		put_uint16(question->qclass, &data);
		put_uint16(0, &data);
		put_rdata(0, mDNSNULL, &data);
		put_uint32(0, &data);
		}
	else
		{
		put_uint16(answer->rrtype, &data);
		put_uint16(answer->rrclass, &data);
		put_uint16(answer->rdlength, &data);
		//put_rdata(answer->rdlength, answer->rdata->u.data, &data);
		if (!putRData(mDNSNULL, (mDNSu8 *)data, (mDNSu8 *)rep->rhdr + len, answer))
			LogMsg("queryrecord_result_callback putRData failed %d", (mDNSu8 *)rep->rhdr + len - (mDNSu8 *)data);
		data += answer->rdlength;
		put_uint32(AddRecord ? answer->rroriginalttl : 0, &data);
		}

	append_reply(req, rep);
	}

mDNSlocal void queryrecord_termination_callback(request_state *request)
	{
	LogOperation("%3d: DNSServiceQueryRecord(%##s, %s) STOP",
		request->sd, request->u.queryrecord.q.qname.c, DNSTypeName(request->u.queryrecord.q.qtype));
	mDNS_StopQuery(&mDNSStorage, &request->u.queryrecord.q);  // no need to error check
	}

mDNSlocal mStatus handle_queryrecord_request(request_state *request)
	{
	char *ptr = request->msgptr;
	char name[256];
	mDNSu16 rrtype, rrclass;
	mStatus err;

	DNSServiceFlags flags = get_flags(&ptr);
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	mDNSInterfaceID InterfaceID = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !InterfaceID) return(mStatus_BadParamErr);

	if (get_string(&ptr, name, 256) < 0) return(mStatus_BadParamErr);
	rrtype  = get_uint16(&ptr);
	rrclass = get_uint16(&ptr);

	mDNSPlatformMemZero(&request->u.queryrecord.q, sizeof(&request->u.queryrecord.q));

	request->u.queryrecord.q.InterfaceID      = InterfaceID;
	request->u.queryrecord.q.Target           = zeroAddr;
	if (!MakeDomainNameFromDNSNameString(&request->u.queryrecord.q.qname, name)) return(mStatus_BadParamErr);
	request->u.queryrecord.q.qtype            = rrtype;
	request->u.queryrecord.q.qclass           = rrclass;
	request->u.queryrecord.q.LongLived        = (flags & kDNSServiceFlagsLongLivedQuery     ) != 0;
	request->u.queryrecord.q.ExpectUnique     = mDNSfalse;
	request->u.queryrecord.q.ForceMCast       = (flags & kDNSServiceFlagsForceMulticast     ) != 0;
	request->u.queryrecord.q.ReturnIntermed   = (flags & kDNSServiceFlagsReturnIntermediates) != 0;
	request->u.queryrecord.q.QuestionCallback = queryrecord_result_callback;
	request->u.queryrecord.q.QuestionContext  = request;

	LogOperation("%3d: DNSServiceQueryRecord(%##s, %s, %X) START",
		request->sd, request->u.queryrecord.q.qname.c, DNSTypeName(request->u.queryrecord.q.qtype), flags);
	err = mDNS_StartQuery(&mDNSStorage, &request->u.queryrecord.q);
	if (err) LogMsg("ERROR: mDNS_StartQuery: %d", (int)err);

	if (!err) request->terminate = queryrecord_termination_callback;

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceEnumerateDomains
#endif

mDNSlocal reply_state *format_enumeration_reply(request_state *request,
	const char *domain, DNSServiceFlags flags, mDNSu32 ifi, DNSServiceErrorType err)
	{
	size_t len;
	reply_state *reply;
	char *data;

	len = sizeof(DNSServiceFlags);
	len += sizeof(mDNSu32);
	len += sizeof(DNSServiceErrorType);
	len += strlen(domain) + 1;

	reply = create_reply(enumeration_reply_op, len, request);
	reply->rhdr->flags = dnssd_htonl(flags);
	reply->rhdr->ifi = dnssd_htonl(ifi);
	reply->rhdr->error = dnssd_htonl(err);
	data = reply->sdata;
	put_string(domain, &data);
	return reply;
	}

mDNSlocal void enum_termination_callback(request_state *request)
	{
	mDNS_StopGetDomains(&mDNSStorage, &request->u.enumeration.q_all);
	mDNS_StopGetDomains(&mDNSStorage, &request->u.enumeration.q_default);
	}

mDNSlocal void enum_result_callback(mDNS *const m,
	DNSQuestion *const question, const ResourceRecord *const answer, mDNSBool AddRecord)
	{
	char domain[MAX_ESCAPED_DOMAIN_NAME];
	request_state *request = question->QuestionContext;
	DNSServiceFlags flags = 0;
	reply_state *reply;
	(void)m; // Unused

	if (answer->rrtype != kDNSType_PTR) return;
	
	// We only return add/remove events for the browse and registration lists
	// For the default browse and registration answers, we only give an "ADD" event
	if (question == &request->u.enumeration.q_default && !AddRecord) return;

	if (AddRecord)
		{
		flags |= kDNSServiceFlagsAdd;
		if (question == &request->u.enumeration.q_default) flags |= kDNSServiceFlagsDefault;
		}

	ConvertDomainNameToCString(&answer->rdata->u.name, domain);
	// Note that we do NOT propagate specific interface indexes to the client - for example, a domain we learn from
	// a machine's system preferences may be discovered on the LocalOnly interface, but should be browsed on the
	// network, so we just pass kDNSServiceInterfaceIndexAny
	reply = format_enumeration_reply(request, domain, flags, kDNSServiceInterfaceIndexAny, kDNSServiceErr_NoError);
	if (!reply) { LogMsg("ERROR: enum_result_callback, format_enumeration_reply"); return; }
	append_reply(request, reply);
	}

mDNSlocal mStatus handle_enum_request(request_state *request)
	{
	char *ptr = request->msgptr;
	mStatus err;

	DNSServiceFlags flags = get_flags(&ptr);
	DNSServiceFlags reg = flags & kDNSServiceFlagsRegistrationDomains;
	mDNS_DomainType t_all     = reg ? mDNS_DomainTypeRegistration        : mDNS_DomainTypeBrowse;
	mDNS_DomainType t_default = reg ? mDNS_DomainTypeRegistrationDefault : mDNS_DomainTypeBrowseDefault;
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	mDNSInterfaceID InterfaceID = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !InterfaceID) return(mStatus_BadParamErr);

	// allocate context structures
	uDNS_RegisterSearchDomains(&mDNSStorage);

	// enumeration requires multiple questions, so we must link all the context pointers so that
	// necessary context can be reached from the callbacks
	request->u.enumeration.q_all    .QuestionContext = request;
	request->u.enumeration.q_default.QuestionContext = request;
	
	// if the caller hasn't specified an explicit interface, we use local-only to get the system-wide list.
	if (!InterfaceID) InterfaceID = mDNSInterface_LocalOnly;

	// make the calls
	LogOperation("%3d: DNSServiceEnumerateDomains(%X=%s)", request->sd, flags,
		(flags & kDNSServiceFlagsBrowseDomains      ) ? "kDNSServiceFlagsBrowseDomains" :
		(flags & kDNSServiceFlagsRegistrationDomains) ? "kDNSServiceFlagsRegistrationDomains" : "<<Unknown>>");
	err = mDNS_GetDomains(&mDNSStorage, &request->u.enumeration.q_all, t_all, NULL, InterfaceID, enum_result_callback, request);
	if (!err)
		{
		err = mDNS_GetDomains(&mDNSStorage, &request->u.enumeration.q_default, t_default, NULL, InterfaceID, enum_result_callback, request);
		if (err) mDNS_StopGetDomains(&mDNSStorage, &request->u.enumeration.q_all);
		}
	if (!err) request->terminate = enum_termination_callback;

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceReconfirmRecord & Misc
#endif

mDNSlocal mStatus handle_reconfirm_request(request_state *request)
	{
	mStatus status = mStatus_BadParamErr;
	AuthRecord *rr = read_rr_from_ipc_msg(&request->msgptr, 0, 0);
	if (rr)
		{
		status = mDNS_ReconfirmByValue(&mDNSStorage, &rr->resrec);
		LogOperation(
			(status == mStatus_NoError) ?
			"%3d: DNSServiceReconfirmRecord(%s) interface %d initiated" :
			"%3d: DNSServiceReconfirmRecord(%s) interface %d failed: %d",
			request->sd, RRDisplayString(&mDNSStorage, &rr->resrec),
			mDNSPlatformInterfaceIndexfromInterfaceID(&mDNSStorage, rr->resrec.InterfaceID), status);
		}
	freeL("AuthRecord/handle_reconfirm_request", rr);
	// We always want to abort a DNSServiceReconfirmRecord() immediately -- there are no asynchronous results to return
	return(status);
	}

#ifdef _HAVE_SETDOMAIN_SUPPORT_
mDNSlocal void free_defdomain(mDNS *const m, AuthRecord *const rr, mStatus result)
	{
	(void)m;  // unused
	if (result == mStatus_MemFree) freeL("AuthRecord/free_defdomain", rr->RecordContext);  // context is the enclosing list structure
	}
#endif

mDNSlocal mStatus handle_setdomain_request(request_state *request)
	{
	char *ptr = request->msgptr;
	mStatus err = mStatus_NoError;
	char domainstr[MAX_ESCAPED_DOMAIN_NAME];
	domainname domain;
#ifdef _HAVE_SETDOMAIN_SUPPORT_
	struct xucred xuc;
	socklen_t xuclen;
#endif

	// extract flags/domain from message
	DNSServiceFlags flags = get_flags(&ptr);
	(void)flags; // Unused
	if (get_string(&ptr, domainstr, MAX_ESCAPED_DOMAIN_NAME) < 0 ||
		!MakeDomainNameFromDNSNameString(&domain, domainstr)) return(mStatus_BadParamErr);

	debugf("%3d: DNSServiceSetDefaultDomainForUser(%##s)", request->sd, domain.c);

#ifdef _HAVE_SETDOMAIN_SUPPORT_
	// this functionality currently only used for Apple-specific configuration, so we don't burned other platforms by mandating
	// the existence of this socket option
	xuclen = sizeof(xuc);
	if (getsockopt(request->sd, 0, LOCAL_PEERCRED, &xuc, &xuclen))
		{ my_perror("ERROR: getsockopt, LOCAL_PEERCRED"); err = mStatus_UnknownErr; goto end; }
	if (xuc.cr_version != XUCRED_VERSION)
		{ LogMsg("LOCAL_PEERCRED - bad version %d %d", xuc.cr_version, XUCRED_VERSION); err = mStatus_UnknownErr; goto end; }
	LogMsg("Default domain %s %s for UID %d", domainstr, flags & kDNSServiceFlagsAdd ? "set" : "removed", xuc.cr_uid);

	if (flags & kDNSServiceFlagsAdd)
		{
		// register a local-only PRT record
		default_browse_list_t *n = mallocL("default_browse_list_t", sizeof(default_browse_list_t));
		if (!n) { LogMsg("ERROR: malloc"); err = mStatus_NoMemoryErr; goto end; }
		mDNS_SetupResourceRecord(&n->ptr_rec, mDNSNULL,
			mDNSInterface_LocalOnly, kDNSType_PTR, 7200, kDNSRecordTypeShared, free_defdomain, n);
		MakeDomainNameFromDNSNameString(&n->ptr_rec.resrec.name, mDNS_DomainTypeNames[mDNS_DomainTypeBrowseDefault]);
		AppendDNSNameString            (&n->ptr_rec.resrec.name, "local");
		AssignDomainName(&n->ptr_rec.resrec.rdata->u.name, &domain);
		n->uid = xuc.cr_uid;
		err = mDNS_Register(&mDNSStorage, &n->ptr_rec);
		if (err) freeL("default_browse_list_t/handle_setdomain_request", n);
		else
			{
			// link into list
			n->next = default_browse_list;
			default_browse_list = n;
			}

		}
	else
		{
		// remove - find in list, deregister
		default_browse_list_t **p = &default_browse_list;
		while (*p)
			{
			if (SameDomainName(&(*p)->ptr_rec.resrec.rdata->u.name, &domain))
				{
				default_browse_list_t *ptr = *p;
				*p = ptr->next;
				err = mDNS_Deregister(&mDNSStorage, &ptr->ptr_rec);
				break;
				}
			p = &(*p)->next;
			}
		if (!*p) { LogMsg("Nonexistent domain %s for UID %d", domainstr, xuc.cr_uid); err = mStatus_Invalid; }
		}
#else
	err = mStatus_NoError;
#endif // _HAVE_SETDOMAIN_SUPPORT_

	return(err);
	}

typedef packedstruct
	{
	mStatus err;
	mDNSu32 len;
	mDNSu32 vers;
	} DaemonVersionReply;

mDNSlocal void handle_getproperty_request(request_state *request)
	{
	const mStatus BadParamErr = dnssd_htonl(mStatus_BadParamErr);
	char *ptr = request->msgptr;
	char prop[256];
	if (get_string(&ptr, prop, sizeof(prop)) >= 0)
		{
		LogOperation("%3d: DNSServiceGetProperty(%s)", request->sd, prop);
		if (!strcmp(prop, kDNSServiceProperty_DaemonVersion))
			{
			DaemonVersionReply x = { 0, dnssd_htonl(4), dnssd_htonl(_DNS_SD_H) };
			send_all(request->sd, (const char *)&x, sizeof(x));
			return;
			}
		}

	// If we didn't recogize the requested property name, return BadParamErr
	send_all(request->sd, (const char *)&BadParamErr, sizeof(BadParamErr));
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceNATPortMappingCreate
#endif

mDNSlocal void port_mapping_termination_callback(request_state *request)
	{
	mDNS_StopNATOperation(&mDNSStorage, &request->u.pm.NATinfo);
	}

// Called via function pointer when we get a NAT-PMP address request or port mapping response
mDNSlocal void port_mapping_create_request_callback(mDNS *m, mDNSv4Addr ExternalAddress, NATTraversalInfo *n, mStatus err)
	{
	request_state *request = (request_state *)n->clientContext;
	reply_state *rep;
	int replyLen;
	char *data;

	if (!request) { LogMsg("port_mapping_create_request_callback called with unknown request_state object"); return; }

	request->u.pm.actualPub = n->publicPort;
	request->u.pm.receivedTTL = n->portMappingLease;
	request->u.pm.addr = ExternalAddress;

	// calculate reply data length
	replyLen = sizeof(DNSServiceFlags);
	replyLen += 3 * sizeof(mDNSu32);  // if index + addr + ttl
	replyLen += sizeof(DNSServiceErrorType);
	replyLen += 2 * sizeof(mDNSu16);  // publicAddress + privateAddress
	replyLen += sizeof(mDNSu8);       // protocol

	rep = create_reply(port_mapping_reply_op, replyLen, request);

	rep->rhdr->flags = dnssd_htonl(0);
	rep->rhdr->ifi   = dnssd_htonl(mDNSPlatformInterfaceIndexfromInterfaceID(m, request->u.pm.interface_id));
	rep->rhdr->error = dnssd_htonl(err);

	data = rep->sdata;

	*data++ = request->u.pm.addr.b[0];
	*data++ = request->u.pm.addr.b[1];
	*data++ = request->u.pm.addr.b[2];
	*data++ = request->u.pm.addr.b[3];
	*data++ = request->u.pm.protocol;
	*data++ = request->u.pm.privatePort.b[0];
	*data++ = request->u.pm.privatePort.b[1];
	*data++ = request->u.pm.actualPub.b[0];
	*data++ = request->u.pm.actualPub.b[1];
	put_uint32(request->u.pm.receivedTTL, &data);

	append_reply(request, rep);
	}

mDNSlocal mStatus handle_port_mapping_request(request_state *request)
	{
	char *ptr = request->msgptr;
	mDNSu8 protocol;
	mDNSIPPort privatePort;
	mDNSIPPort publicPort;
	mDNSu32 ttl;
	mStatus err = mStatus_NoError;

	DNSServiceFlags flags = get_flags(&ptr);
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	mDNSInterfaceID InterfaceID = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !InterfaceID) return(mStatus_BadParamErr);

	(void)flags; // Unused
	protocol         = get_uint32(&ptr);
	privatePort.b[0] = *ptr++;
	privatePort.b[1] = *ptr++;
	publicPort .b[0] = *ptr++;
	publicPort .b[1] = *ptr++;
	ttl              = get_uint32(&ptr);

	if (protocol == 0)
		{
		// If protocol == 0 (i.e. just request public address) then privatePort, publicPort, ttl must be zero too
		if (!mDNSIPPortIsZero(privatePort) || !mDNSIPPortIsZero(publicPort) || ttl) return(mStatus_BadParamErr);
		}
	else
		{
		if (mDNSIPPortIsZero(privatePort)) return(mStatus_BadParamErr);
		if (!(protocol & (kDNSServiceProtocol_UDP | kDNSServiceProtocol_TCP))) return(mStatus_BadParamErr);
		}

	request->u.pm.interface_id = InterfaceID;
	request->u.pm.protocol     = protocol;
	request->u.pm.privatePort  = privatePort;
	request->u.pm.requestedPub       = publicPort;
	request->u.pm.actualPub       = zeroIPPort;
	request->u.pm.requestedTTL = ttl;
	request->u.pm.receivedTTL  = 0;
	
	mDNSPlatformMemZero(&request->u.pm.NATinfo, sizeof(NATTraversalInfo));
	request->u.pm.NATinfo.clientCallback   = port_mapping_create_request_callback;
	request->u.pm.NATinfo.clientContext    = request;
	request->u.pm.NATinfo.protocol         = protocol;
	request->u.pm.NATinfo.privatePort      = request->u.pm.privatePort;
	request->u.pm.NATinfo.publicPortreq    = request->u.pm.requestedPub;
	request->u.pm.NATinfo.portMappingLease = request->u.pm.requestedTTL;
	
	LogOperation("%3d: DNSServiceNATPortMappingCreate(%X, %u, %u, %d) START", request->sd, protocol, mDNSVal16(privatePort), mDNSVal16(publicPort), ttl);
	err = mDNS_StartNATOperation(&mDNSStorage, &request->u.pm.NATinfo);
	if (!err) request->terminate = port_mapping_termination_callback;

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - DNSServiceGetAddrInfo
#endif

mDNSlocal void addrinfo_termination_callback(request_state *request)
	{
	if (request->u.addrinfo.q4.QuestionContext)
		{
		mDNS_StopQuery(&mDNSStorage, &request->u.addrinfo.q4);
		request->u.addrinfo.q4.QuestionContext = mDNSNULL;
		}

	if (request->u.addrinfo.q6.QuestionContext)
		{
		mDNS_StopQuery(&mDNSStorage, &request->u.addrinfo.q6);
		request->u.addrinfo.q6.QuestionContext = mDNSNULL;
		}
	}

mDNSlocal mStatus handle_addrinfo_request(request_state *request)
	{
	char *ptr = request->msgptr;
	char hostname[256];
	domainname d;
	mStatus err = 0;

	DNSServiceFlags flags = get_flags(&ptr);
	mDNSu32 interfaceIndex = get_uint32(&ptr);
	request->u.addrinfo.interface_id = mDNSPlatformInterfaceIDfromInterfaceIndex(&mDNSStorage, interfaceIndex);
	if (interfaceIndex && !request->u.addrinfo.interface_id) return(mStatus_BadParamErr);
	request->u.addrinfo.flags = flags;
	request->u.addrinfo.protocol = get_uint32(&ptr);
	if (request->u.addrinfo.protocol > (kDNSServiceProtocol_IPv4|kDNSServiceProtocol_IPv6))
		return(mStatus_BadParamErr);

	if (get_string(&ptr, hostname, 256) < 0) return(mStatus_BadParamErr);
	if (!MakeDomainNameFromDNSNameString(&d, hostname))
		{ LogMsg("ERROR: handle_addrinfo_request: bad hostname: %s", hostname); return(mStatus_BadParamErr); }

	if (!request->u.addrinfo.protocol)
		{
		NetworkInterfaceInfo *i;
		if (IsLocalDomain(&d))
			{
			for (i = mDNSStorage.HostInterfaces; i; i = i->next)
				{
				if      ((i->ip.type == mDNSAddrType_IPv4) && !mDNSIPv4AddressIsZero(i->ip.ip.v4)) request->u.addrinfo.protocol |= kDNSServiceProtocol_IPv4;
				else if ((i->ip.type == mDNSAddrType_IPv6) && !mDNSIPv6AddressIsZero(i->ip.ip.v6)) request->u.addrinfo.protocol |= kDNSServiceProtocol_IPv6;
				}
			}
		else
			{
			for (i = mDNSStorage.HostInterfaces; i; i = i->next)
				{
				if      ((i->ip.type == mDNSAddrType_IPv4) && !mDNSv4AddressIsLinkLocal(&i->ip.ip.v4)) request->u.addrinfo.protocol |= kDNSServiceProtocol_IPv4;
				else if ((i->ip.type == mDNSAddrType_IPv6) && !mDNSv4AddressIsLinkLocal(&i->ip.ip.v6)) request->u.addrinfo.protocol |= kDNSServiceProtocol_IPv6;
				}
			}
		}

	if (request->u.addrinfo.protocol & kDNSServiceProtocol_IPv4)
		{
		request->u.addrinfo.q4.InterfaceID      = request->u.addrinfo.interface_id;
		request->u.addrinfo.q4.Target           = zeroAddr;
		request->u.addrinfo.q4.qname            = d;
		request->u.addrinfo.q4.qtype            = kDNSServiceType_A;
		request->u.addrinfo.q4.qclass           = kDNSServiceClass_IN;
		request->u.addrinfo.q4.LongLived        = (flags & kDNSServiceFlagsLongLivedQuery     ) != 0;
		request->u.addrinfo.q4.ExpectUnique     = mDNSfalse;
		request->u.addrinfo.q4.ForceMCast       = (flags & kDNSServiceFlagsForceMulticast     ) != 0;
		request->u.addrinfo.q4.ReturnIntermed   = (flags & kDNSServiceFlagsReturnIntermediates) != 0;
		request->u.addrinfo.q4.QuestionCallback = queryrecord_result_callback;
		request->u.addrinfo.q4.QuestionContext  = request;

		err = mDNS_StartQuery(&mDNSStorage, &request->u.addrinfo.q4);
		if (err != mStatus_NoError)
			{
			LogMsg("ERROR: mDNS_StartQuery: %d", (int)err);
			request->u.addrinfo.q4.QuestionContext = mDNSNULL;
			}
		}

	if (!err && (request->u.addrinfo.protocol & kDNSServiceProtocol_IPv6))
		{
		request->u.addrinfo.q6.InterfaceID      = request->u.addrinfo.interface_id;
		request->u.addrinfo.q6.Target           = zeroAddr;
		request->u.addrinfo.q6.qname            = d;
		request->u.addrinfo.q6.qtype            = kDNSServiceType_AAAA;
		request->u.addrinfo.q6.qclass           = kDNSServiceClass_IN;
		request->u.addrinfo.q6.LongLived        = (flags & kDNSServiceFlagsLongLivedQuery     ) != 0;
		request->u.addrinfo.q6.ExpectUnique     = mDNSfalse;
		request->u.addrinfo.q6.ForceMCast       = (flags & kDNSServiceFlagsForceMulticast     ) != 0;
		request->u.addrinfo.q6.ReturnIntermed   = (flags & kDNSServiceFlagsReturnIntermediates) != 0;
		request->u.addrinfo.q6.QuestionCallback = queryrecord_result_callback;
		request->u.addrinfo.q6.QuestionContext  = request;

		err = mDNS_StartQuery(&mDNSStorage, &request->u.addrinfo.q6);
		if (err != mStatus_NoError)
			{
			LogMsg("ERROR: mDNS_StartQuery: %d", (int)err);
			request->u.addrinfo.q6.QuestionContext = mDNSNULL;
			}
		}

	LogOperation("%3d: DNSServiceGetAddrInfo(%##s) START", request->sd, d.c);

	if (!err) request->terminate = addrinfo_termination_callback;

	return(err);
	}

// ***************************************************************************
#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - Main Request Handler etc.
#endif

mDNSlocal request_state *NewRequest(void)
	{
	request_state **p = &all_requests;
	while (*p) p=&(*p)->next;
	*p = mallocL("request_state", sizeof(request_state));
	if (!*p) FatalError("ERROR: malloc");
	mDNSPlatformMemZero(*p, sizeof(request_state));
	return(*p);
	}

// read_msg may be called any time when the transfer state (req->ts) is t_morecoming.
// returns the current state of the request (morecoming, error, complete, terminated.)
// if there is no data on the socket, the socket will be closed and t_terminated will be returned
mDNSlocal int read_msg(request_state *req)
	{
	mDNSu32 nleft;
	int nread;

	if (req->ts == t_terminated || req->ts == t_error)
		{
		LogMsg("ERROR: read_msg called with transfer state terminated or error");
		req->ts = t_error;
		return t_error;
		}

	if (req->ts == t_complete)	// this must be death or something is wrong
		{
		char buf[4];	// dummy for death notification
		nread = recv(req->sd, buf, 4, 0);
		if (!nread) { req->ts = t_terminated; return t_terminated; }
		if (nread < 0) goto rerror;
		LogMsg("ERROR: read data from a completed request.");
		req->ts = t_error;
		return t_error;
		}

	if (req->ts != t_morecoming)
		{
		LogMsg("ERROR: read_msg called with invalid transfer state (%d)", req->ts);
		req->ts = t_error;
		return t_error;
		}

	if (req->hdr_bytes < sizeof(ipc_msg_hdr))
		{
		nleft = sizeof(ipc_msg_hdr) - req->hdr_bytes;
		nread = recv(req->sd, (char *)&req->hdr + req->hdr_bytes, nleft, 0);
		if (nread == 0) { req->ts = t_terminated; return t_terminated; }
		if (nread < 0) goto rerror;
		req->hdr_bytes += nread;
		if (req->hdr_bytes == sizeof(ipc_msg_hdr))
			{
			ConvertHeaderBytes(&req->hdr);
			if (req->hdr.version != VERSION)
				{
				LogMsg("ERROR: client version 0x%08X daemon version 0x%08X", req->hdr.version, VERSION);
				req->ts = t_error;
				return t_error;
				}
			}
		if (req->hdr_bytes > sizeof(ipc_msg_hdr))
			{
			LogMsg("ERROR: read_msg - read too many header bytes");
			req->ts = t_error;
			return t_error;
			}
		}

	// only read data if header is complete
	if (req->hdr_bytes == sizeof(ipc_msg_hdr))
		{
		if (req->hdr.datalen == 0)  // ok in removerecord requests
			{
			req->ts = t_complete;
			req->msgbuf = NULL;
			return t_complete;
			}

		if (!req->msgbuf)  // allocate the buffer first time through
			{
			req->msgbuf = mallocL("request_state msgbuf", req->hdr.datalen + MSG_PAD_BYTES);
			if (!req->msgbuf) { my_perror("ERROR: malloc"); req->ts = t_error; return t_error; }
			req->msgptr = req->msgbuf;
			req->msgend = req->msgbuf + req->hdr.datalen;
			mDNSPlatformMemZero(req->msgbuf, req->hdr.datalen + MSG_PAD_BYTES);
			}
		nleft = req->hdr.datalen - req->data_bytes;
		nread = recv(req->sd, req->msgbuf + req->data_bytes, nleft, 0);
		if (nread == 0) { req->ts = t_terminated; return t_terminated; }
		if (nread < 0) goto rerror;
		req->data_bytes += nread;
		if (req->data_bytes > req->hdr.datalen)
			{
			LogMsg("ERROR: read_msg - read too many data bytes");
			req->ts = t_error;
			return t_error;
			}
		}

	if (req->hdr_bytes == sizeof(ipc_msg_hdr) && req->data_bytes == req->hdr.datalen)
		req->ts = t_complete;
	else req->ts = t_morecoming;

	return req->ts;

rerror:
	if (dnssd_errno() == dnssd_EWOULDBLOCK || dnssd_errno() == dnssd_EINTR) return t_morecoming;
	my_perror("ERROR: read_msg");
	req->ts = t_error;
	return t_error;
	}

#define RecordOrientedOp(X) \
	((X) == reg_record_request || (X) == add_record_request || (X) == update_record_request || (X) == remove_record_request)

// The lightweight operations are the ones that don't need a dedicated request_state structure allocated for them
#define LightweightOp(X) (RecordOrientedOp(X) || (X) == cancel_request)

mDNSlocal void request_callback(int fd, short filter, void *info)
	{
	mStatus err = 0;
	request_state *req = info;
	transfer_state result;
	dnssd_sockaddr_t cliaddr;
	int dedicated_error_socket;
	dnssd_sock_t errfd = req->sd;
#if defined(_WIN32)
	u_long opt = 1;
#endif
	mDNSs32 min_size = sizeof(DNSServiceFlags);
	(void)fd; // Unused
	(void)filter; // Unused

	result = read_msg(req);
	if (result == t_morecoming) return;
	if (result == t_terminated || result == t_error) { AbortUnlinkAndFree(req); return; }

	if (req->hdr.version != VERSION)
		{
		LogMsg("ERROR: client incompatible with daemon (client version = %d, "
			   "daemon version = %d)\n", req->hdr.version, VERSION);
		AbortUnlinkAndFree(req);
		return;
		}

	switch(req->hdr.op)            //          Interface       + other data
		{
		case connection_request:       min_size = 0;                                                                           break;
		case reg_service_request:      min_size += sizeof(mDNSu32) + 4 /* name, type, domain, host */ + 4 /* port, textlen */; break;
		case add_record_request:       min_size +=                   4 /* type, rdlen */              + 4 /* ttl */;           break;
		case update_record_request:    min_size +=                   2 /* rdlen */                    + 4 /* ttl */;           break;
		case remove_record_request:                                                                                            break;
		case browse_request:           min_size += sizeof(mDNSu32) + 2 /* type, domain */;                                     break;
		case resolve_request:          min_size += sizeof(mDNSu32) + 3 /* type, type, domain */;                               break;
		case query_request:            min_size += sizeof(mDNSu32) + 1 /* name */                     + 4 /* type, class*/;    break;
		case enumeration_request:      min_size += sizeof(mDNSu32);                                                            break;
		case reg_record_request:       min_size += sizeof(mDNSu32) + 1 /* name */ + 6 /* type, class, rdlen */ + 4 /* ttl */;  break;
		case reconfirm_record_request: min_size += sizeof(mDNSu32) + 1 /* name */ + 6 /* type, class, rdlen */;                break;
		case setdomain_request:        min_size +=                   1 /* domain */;                                           break;
		case getproperty_request:      min_size = 2;                                                                           break;
		case port_mapping_request:     min_size += sizeof(mDNSu32) + 4 /* udp/tcp */ + 4 /* int/ext port */    + 4 /* ttl */;  break;
		case addrinfo_request:         min_size += sizeof(mDNSu32) + 4 /* v4/v6 */   + 1 /* hostname */;                       break;
		case cancel_request:           min_size = 0;                                                                           break;
		default: LogMsg("ERROR: validate_message - unsupported req type: %d", req->hdr.op); min_size = -1;                     break;
		}

	if ((mDNSs32)req->data_bytes < min_size)
		{ LogMsg("Invalid message %d bytes; min for %d is %d", req->data_bytes, req->hdr.op, min_size); AbortUnlinkAndFree(req); return; }

	if (LightweightOp(req->hdr.op) && !req->terminate)
		{ LogMsg("Reg/Add/Update/Remove %d require existing connection", req->hdr.op);                  AbortUnlinkAndFree(req); return; }

	// check if client wants silent operation
	if (req->hdr.ipc_flags & IPC_FLAGS_NOREPLY) req->no_reply = 1;

	dedicated_error_socket = (req->terminate && req->hdr.op != cancel_request);

	// check if primary socket is to be used for synchronous errors, else open new socket
	if (dedicated_error_socket)
		{
		errfd = socket(AF_DNSSD, SOCK_STREAM, 0);
		if (!dnssd_SocketValid(errfd)) { my_perror("ERROR: socket"); AbortUnlinkAndFree(req); return; }

		//LogOperation("request_callback: Opened dedicated errfd %d", errfd);

		#if defined(USE_TCP_LOOPBACK)
			{
			mDNSOpaque16 port;
			port.b[0] = req->msgptr[0];
			port.b[1] = req->msgptr[1];
			req->msgptr += 2;
			cliaddr.sin_family      = AF_INET;
			cliaddr.sin_port        = port.NotAnInteger;
			cliaddr.sin_addr.s_addr = inet_addr(MDNS_TCP_SERVERADDR);
			}
		#else
			{
			char ctrl_path[MAX_CTLPATH];
			get_string(&req->msgptr, ctrl_path, 256);	// path is first element in message buffer
			mDNSPlatformMemZero(&cliaddr, sizeof(cliaddr));
			cliaddr.sun_family = AF_LOCAL;
			mDNSPlatformStrCopy(cliaddr.sun_path, ctrl_path);
			}
		#endif
		//LogOperation("request_callback: Connecting to “%s”", cliaddr.sun_path);
		if (connect(errfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
			{
			//LogOperation("request_callback: Couldn't connect to “%s”", cliaddr.sun_path);
			my_perror("ERROR: connect");
			AbortUnlinkAndFree(req);
			return;
			}
#if defined(_WIN32)
		if (ioctlsocket(errfd, FIONBIO, &opt) != 0)
#else
		if (fcntl(errfd, F_SETFL, fcntl(errfd, F_GETFL, 0) | O_NONBLOCK) != 0)
#endif
			{
			my_perror("ERROR: could not set control socket to non-blocking mode");
			AbortUnlinkAndFree(req);
			return;
			}
		}

	// If req->terminate is already set, this means this operation is sharing an existing connection
	if (req->terminate && !LightweightOp(req->hdr.op))
		{
		request_state *newreq = NewRequest();
		newreq->primary = req;
		newreq->sd      = req->sd;
		newreq->hdr     = req->hdr;
		newreq->msgbuf  = req->msgbuf;
		newreq->msgptr  = req->msgptr;
		newreq->msgend  = req->msgend;
		req = newreq;
		}

	switch(req->hdr.op)
		{
		// These are all operations that have their own first-class request_state object
		case connection_request:           req->terminate = connection_termination; break;
		case resolve_request:              err = handle_resolve_request     (req); break;
		case query_request:                err = handle_queryrecord_request (req); break;
		case browse_request:               err = handle_browse_request      (req); break;
		case reg_service_request:          err = handle_regservice_request  (req); break;
		case enumeration_request:          err = handle_enum_request        (req); break;
		case reconfirm_record_request:     err = handle_reconfirm_request   (req); break;
		case setdomain_request:            err = handle_setdomain_request   (req); break;
		case getproperty_request:                handle_getproperty_request (req); break;
		case port_mapping_request:         err = handle_port_mapping_request(req); break;
		case addrinfo_request:             err = handle_addrinfo_request    (req); break;

		// These are all operations that work with an existing request_state object
		case reg_record_request:           err = handle_regrecord_request   (req); break;
		case add_record_request:           err = handle_add_request         (req); break;
		case update_record_request:        err = handle_update_request      (req); break;
		case remove_record_request:        err = handle_removerecord_request(req); break;
		case cancel_request:                     handle_cancel_request      (req); break;
		default: LogMsg("%3d: ERROR: Unsupported UDS req: %d", req->sd, req->hdr.op);
		}

	// req->msgbuf may be NULL, e.g. for connection_request or remove_record_request
	if (req->msgbuf) freeL("request_state msgbuf", req->msgbuf);

	// There's no return data for a cancel request (DNSServiceRefDeallocate returns no result)
	// For a DNSServiceGetProperty call, the handler already generated the response,
	// so no need to do it again here
	if (req->hdr.op != cancel_request && req->hdr.op != getproperty_request)
		{
		err = dnssd_htonl(err);
		send_all(errfd, (const char *)&err, sizeof(err));
		if (errfd != req->sd) dnssd_close(errfd);
		}

	// Reset ready to accept the next req on this pipe
	if (req->primary) req = req->primary;
	req->ts         = t_morecoming;
	req->hdr_bytes  = 0;
	req->data_bytes = 0;
	req->msgbuf     = mDNSNULL;
	req->msgptr     = mDNSNULL;
	req->msgend     = 0;
	}

mDNSlocal void connect_callback(int fd, short filter, void *info)
	{
	dnssd_sockaddr_t cliaddr;
	dnssd_socklen_t len = (dnssd_socklen_t) sizeof(cliaddr);
	dnssd_sock_t sd = accept(listenfd, (struct sockaddr*) &cliaddr, &len);
	const unsigned long optval = 1;

	(void)fd; // Unused
	(void)filter; // Unused
	(void)info; // Unused

	if (!dnssd_SocketValid(sd))
		{
		if (dnssd_errno() != dnssd_EWOULDBLOCK) my_perror("ERROR: accept");
		return;
		}

#ifdef SO_NOSIGPIPE
	// Some environments (e.g. OS X) support turning off SIGPIPE for a socket
	if (setsockopt(sd, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) < 0)
		{
		my_perror("ERROR: setsockopt - SO_NOSIGPIPE - aborting client");
		dnssd_close(sd);
		return;
		}
#endif

#if defined(_WIN32)
	if (ioctlsocket(sd, FIONBIO, &optval) != 0)
#else
	if (fcntl(sd, F_SETFL, fcntl(sd, F_GETFL, 0) | O_NONBLOCK) != 0)
#endif
		{
		my_perror("ERROR: fcntl(sd, F_SETFL, O_NONBLOCK) - aborting client");
		dnssd_close(sd);
		return;
		}
	else
		{
		request_state *request = NewRequest();
		request->ts = t_morecoming;
		request->sd = sd;
		LogOperation("%3d: Adding FD", request->sd);
		udsSupportAddFDToEventLoop(sd, request_callback, request);
		}
	}

mDNSexport int udsserver_init(dnssd_sock_t skt)
	{
	dnssd_sockaddr_t laddr;
	int ret;
#if defined(_WIN32)
	u_long opt = 1;
#endif

	LogOperation("udsserver_init");

	// If a particular platform wants to opt out of having a PID file, define PID_FILE to be ""
	if (PID_FILE[0])
		{
		FILE *fp = fopen(PID_FILE, "w");
		if (fp != NULL)
			{
			fprintf(fp, "%d\n", getpid());
			fclose(fp);
			}
		}

	if (dnssd_SocketValid(skt))
		listenfd = skt;
	else
		{
		listenfd = socket(AF_DNSSD, SOCK_STREAM, 0);
		if (!dnssd_SocketValid(listenfd))
			{
			my_perror("ERROR: socket(AF_DNSSD, SOCK_STREAM, 0); failed");
			goto error;
			}

		mDNSPlatformMemZero(&laddr, sizeof(laddr));

		#if defined(USE_TCP_LOOPBACK)
			{
			laddr.sin_family = AF_INET;
			laddr.sin_port = htons(MDNS_TCP_SERVERPORT);
			laddr.sin_addr.s_addr = inet_addr(MDNS_TCP_SERVERADDR);
			ret = bind(listenfd, (struct sockaddr *) &laddr, sizeof(laddr));
			if (ret < 0)
				{
				my_perror("ERROR: bind(listenfd, (struct sockaddr *) &laddr, sizeof(laddr)); failed");
				goto error;
				}
			}
		#else
			{
			mode_t mask = umask(0);
			unlink(MDNS_UDS_SERVERPATH);  //OK if this fails
			laddr.sun_family = AF_LOCAL;
			#ifndef NOT_HAVE_SA_LEN
			// According to Stevens (section 3.2), there is no portable way to
			// determine whether sa_len is defined on a particular platform.
			laddr.sun_len = sizeof(struct sockaddr_un);
			#endif
			mDNSPlatformStrCopy(laddr.sun_path, MDNS_UDS_SERVERPATH);
			ret = bind(listenfd, (struct sockaddr *) &laddr, sizeof(laddr));
			umask(mask);
			if (ret < 0)
				{
				my_perror("ERROR: bind(listenfd, (struct sockaddr *) &laddr, sizeof(laddr)); failed");
				goto error;
				}
			}
		#endif
		}

#if defined(_WIN32)
	// SEH: do we even need to do this on windows?
	// This socket will be given to WSAEventSelect which will automatically set it to non-blocking
	if (ioctlsocket(listenfd, FIONBIO, &opt) != 0)
#else
	if (fcntl(listenfd, F_SETFL, fcntl(listenfd, F_GETFL, 0) | O_NONBLOCK) != 0)
#endif
		{
		my_perror("ERROR: could not set listen socket to non-blocking mode");
		goto error;
		}

	if (listen(listenfd, LISTENQ) != 0)
		{
		my_perror("ERROR: could not listen on listen socket");
		goto error;
		}

	if (mStatus_NoError != udsSupportAddFDToEventLoop(listenfd, connect_callback, (void *) NULL))
		{
		my_perror("ERROR: could not add listen socket to event loop");
		goto error;
		}

#if !defined(PLATFORM_NO_RLIMIT)
	{
	// Set maximum number of open file descriptors
	#define MIN_OPENFILES 10240
	struct rlimit maxfds, newfds;

	// Due to bugs in OS X (<rdar://problem/2941095>, <rdar://problem/3342704>, <rdar://problem/3839173>)
	// you have to get and set rlimits once before getrlimit will return sensible values
	if (getrlimit(RLIMIT_NOFILE, &maxfds) < 0) { my_perror("ERROR: Unable to get file descriptor limit"); return 0; }
	if (setrlimit(RLIMIT_NOFILE, &maxfds) < 0) my_perror("ERROR: Unable to set maximum file descriptor limit");

	if (getrlimit(RLIMIT_NOFILE, &maxfds) < 0) { my_perror("ERROR: Unable to get file descriptor limit"); return 0; }
	newfds.rlim_max = (maxfds.rlim_max > MIN_OPENFILES) ? maxfds.rlim_max : MIN_OPENFILES;
	newfds.rlim_cur = (maxfds.rlim_cur > MIN_OPENFILES) ? maxfds.rlim_cur : MIN_OPENFILES;
	if (newfds.rlim_max != maxfds.rlim_max || newfds.rlim_cur != maxfds.rlim_cur)
		if (setrlimit(RLIMIT_NOFILE, &newfds) < 0) my_perror("ERROR: Unable to set maximum file descriptor limit");

	if (getrlimit(RLIMIT_NOFILE, &maxfds) < 0) { my_perror("ERROR: Unable to get file descriptor limit"); return 0; }
	debugf("maxfds.rlim_max %d", (long)maxfds.rlim_max);
	debugf("maxfds.rlim_cur %d", (long)maxfds.rlim_cur);
	}
#endif

	TrackLegacyBrowseDomains(&mDNSStorage);
	udsserver_handle_configchange(&mDNSStorage);
	return 0;

error:

	my_perror("ERROR: udsserver_init");
	return -1;
	}

mDNSexport int udsserver_exit(dnssd_sock_t skt)
	{
	// If the launching environment created no listening socket,
	// that means we created it ourselves, so we should clean it up on exit
	if (!dnssd_SocketValid(skt))
		{
		dnssd_close(listenfd);
#if !defined(USE_TCP_LOOPBACK)
		// Currently, we're unable to remove /var/run/mdnsd because we've changed to userid "nobody"
		// to give up unnecessary privilege, but we need to be root to remove this Unix Domain Socket.
		// It would be nice if we could find a solution to this problem
		if (unlink(MDNS_UDS_SERVERPATH))
			debugf("Unable to remove %s", MDNS_UDS_SERVERPATH);
#endif
		}

	if (PID_FILE[0]) unlink(PID_FILE);

	return 0;
	}

mDNSlocal void LogClientInfo(request_state *req)
	{
	if (!req->terminate)
		LogMsgNoIdent("%3d: No operation yet on this socket", req->sd);
	else if (req->terminate == connection_termination)
		LogMsgNoIdent("%3d: DNSServiceCreateConnection", req->sd);
	else if (req->terminate == regservice_termination_callback)
		{
		service_instance *ptr;
		for (ptr = req->u.servicereg.instances; ptr; ptr = ptr->next)
			LogMsgNoIdent("%3d: DNSServiceRegister         %##s %u", req->sd, ptr->srs.RR_SRV.resrec.name->c, SRS_PORT(&ptr->srs));
		}
	else if (req->terminate == browse_termination_callback)
		{
		browser_t *blist;
		for (blist = req->u.browser.browsers; blist; blist = blist->next)
			LogMsgNoIdent("%3d: DNSServiceBrowse           %##s", req->sd, blist->q.qname.c);
		}
	else if (req->terminate == resolve_termination_callback)
		LogMsgNoIdent("%3d: DNSServiceResolve          %##s", req->sd, req->u.resolve.qsrv.qname.c);
	else if (req->terminate == queryrecord_termination_callback)
		LogMsgNoIdent("%3d: DNSServiceQueryRecord      %##s", req->sd, req->u.queryrecord.q.qname.c);
	else if (req->terminate == enum_termination_callback)
		LogMsgNoIdent("%3d: DNSServiceEnumerateDomains %##s", req->sd, req->u.enumeration.q_all.qname.c);
	else if (req->terminate == port_mapping_termination_callback)
		LogMsgNoIdent("%3d: DNSServiceNATPortMapping   %s%s Int %d Ext %d Granted %d TTL %d Granted %d Addr %.4a",
			req->sd,
			req->u.pm.protocol & kDNSServiceProtocol_TCP ? "tcp" : "   ",
			req->u.pm.protocol & kDNSServiceProtocol_UDP ? "udp" : "   ",
			mDNSVal16(req->u.pm.privatePort),
			mDNSVal16(req->u.pm.requestedPub),
			mDNSVal16(req->u.pm.actualPub),
			req->u.pm.requestedTTL,
			req->u.pm.receivedTTL,
			&req->u.pm.addr);
	else if (req->terminate == addrinfo_termination_callback)
		LogMsgNoIdent("%3d: DNSServiceGetAddrInfo      %s%s %##s", req->sd,
			req->u.addrinfo.protocol & kDNSServiceProtocol_IPv4 ? "v4" : "  ",
			req->u.addrinfo.protocol & kDNSServiceProtocol_IPv6 ? "v6" : "  ",
			req->u.addrinfo.q4.qname.c);
	else
		LogMsgNoIdent("%3d: Unrecognized operation %p", req->sd, req->terminate);
	}

mDNSexport void udsserver_info(mDNS *const m)
	{
	mDNSs32 now = mDNS_TimeNow(m);
	mDNSu32 CacheUsed = 0, CacheActive = 0;
	mDNSu32 slot;
	CacheGroup *cg;
	DNSQuestion *q;
	CacheRecord *cr;
	AuthRecord *ar;
	request_state *req;

	LogMsgNoIdent("Timenow 0x%08lX (%ld)", (mDNSu32)now, now);
	LogMsgNoIdent("------------ Cache -------------");

	LogMsgNoIdent("Slt Q     TTL if    U Type rdlen");
	for (slot = 0; slot < CACHE_HASH_SLOTS; slot++)
		for (cg = m->rrcache_hash[slot]; cg; cg=cg->next)
			{
			CacheUsed++;	// Count one cache entity for the CacheGroup object
			for (cr = cg->members; cr; cr=cr->next)
				{
				mDNSs32 remain = cr->resrec.rroriginalttl - (now - cr->TimeRcvd) / mDNSPlatformOneSecond;
				NetworkInterfaceInfo *info = (NetworkInterfaceInfo *)cr->resrec.InterfaceID;
				CacheUsed++;
				if (cr->CRActiveQuestion) CacheActive++;
				LogMsgNoIdent("%3d %s%8ld %-6s%s %-6s%s",
					slot,
					cr->CRActiveQuestion ? "*" : " ",
					remain,
					info ? info->ifname : "-U-",
					(cr->resrec.RecordType == kDNSRecordTypePacketNegative)  ? "-" :
					(cr->resrec.RecordType & kDNSRecordTypePacketUniqueMask) ? " " : "+",
					DNSTypeName(cr->resrec.rrtype),
					CRDisplayString(m, cr));
				usleep(1000);	// Limit rate a little so we don't flood syslog too fast
				}
			}

	if (m->rrcache_totalused != CacheUsed)
		LogMsgNoIdent("Cache use mismatch: rrcache_totalused is %lu, true count %lu", m->rrcache_totalused, CacheUsed);
	if (m->rrcache_active != CacheActive)
		LogMsgNoIdent("Cache use mismatch: rrcache_active is %lu, true count %lu", m->rrcache_active, CacheActive);
	LogMsgNoIdent("Cache currently contains %lu records; %lu referenced by active questions", CacheUsed, CacheActive);

	LogMsgNoIdent("--------- Auth Records ---------");
	for (ar = m->ResourceRecords; ar; ar=ar->next)
		LogMsgNoIdent("%s", ARDisplayString(m, ar));

	LogMsgNoIdent("---------- Questions -----------");
	LogMsgNoIdent("   Int  Next if      Type");
	CacheUsed = 0;
	CacheActive = 0;
	for (q = m->Questions; q; q=q->next)
		{
		mDNSs32 i = q->ThisQInterval / mDNSPlatformOneSecond;
		mDNSs32 n = (q->LastQTime + q->ThisQInterval - now) / mDNSPlatformOneSecond;
		NetworkInterfaceInfo *info = (NetworkInterfaceInfo *)q->InterfaceID;
		CacheUsed++;
		if (q->ThisQInterval) CacheActive++;
		LogMsgNoIdent("%6d%6d %-6s%s %-6s%##s",
			i, n, info ? info->ifname : "",
			mDNSOpaque16IsZero(q->TargetQID) ? " " : q->LongLived ? "L" : "O", // mDNS, long-lived, or one-shot query?
			DNSTypeName(q->qtype), q->qname.c);
		usleep(1000);	// Limit rate a little so we don't flood syslog too fast
		}
	LogMsgNoIdent("%lu question%s; %lu active", CacheUsed, CacheUsed > 1 ? "s" : "", CacheActive);

	LogMsgNoIdent("---- Active Client Requests ----");
	for (req = all_requests; req; req=req->next)
		LogClientInfo(req);
	}

mDNSlocal int send_msg(reply_state *rep)
	{
	ssize_t nwriten;
	if (!rep->msgbuf) { LogMsg("ERROR: send_msg called with NULL message buffer"); return(rep->ts = t_error); }
	if (rep->request->no_reply) { freeL("reply_state msgbuf (no_reply)", rep->msgbuf); return(rep->ts = t_complete); }

	ConvertHeaderBytes(rep->mhdr);
	nwriten = send(rep->sd, rep->msgbuf + rep->nwriten, rep->len - rep->nwriten, 0);
	ConvertHeaderBytes(rep->mhdr);

	if (nwriten < 0)
		{
		if (dnssd_errno() == dnssd_EINTR || dnssd_errno() == dnssd_EWOULDBLOCK) nwriten = 0;
		else
			{
#if !defined(PLATFORM_NO_EPIPE)
			if (dnssd_errno() == EPIPE)
				return(rep->request->ts = rep->ts = t_terminated);
			else
#endif
				{ my_perror("ERROR: send\n"); return(rep->ts = t_error); }
			}
		}
	rep->nwriten += nwriten;
	if (rep->nwriten == rep->len) { freeL("reply_state msgbuf (t_complete)", rep->msgbuf); rep->ts = t_complete; }
	return rep->ts;
	}

mDNSexport mDNSs32 udsserver_idle(mDNSs32 nextevent)
	{
	mDNSs32 now = mDNS_TimeNow(&mDNSStorage);
	request_state **req = &all_requests;

	while (*req)
		{
		while ((*req)->replies)		// Send queued replies
			{
			transfer_state result;
			if ((*req)->replies->next) (*req)->replies->rhdr->flags |= dnssd_htonl(kDNSServiceFlagsMoreComing);
			result = send_msg((*req)->replies);	// Returns t_morecoming if buffer full because client is not reading
			if (result == t_complete)
				{
				reply_state *fptr = (*req)->replies;
				(*req)->replies = (*req)->replies->next;
				freeL("reply_state/udsserver_idle", fptr);
				(*req)->time_blocked = 0; // reset failure counter after successful send
				continue;
				}
			else if (result == t_terminated || result == t_error) abort_request(*req);
			break;
			}

		if ((*req)->replies)		// If we failed to send everything, check our time_blocked timer
			{
			if (!(*req)->time_blocked) (*req)->time_blocked = NonZeroTime(now);
			if (now - (*req)->time_blocked >= 60 * mDNSPlatformOneSecond)
				{
				LogMsg("Could not write data to client %d after %ld seconds - aborting connection",
					(*req)->sd, (now - (*req)->time_blocked) / mDNSPlatformOneSecond);
				LogClientInfo(*req);
				abort_request(*req);
				}
			else if (nextevent - now > mDNSPlatformOneSecond) nextevent = now + mDNSPlatformOneSecond;
			}

		if (!dnssd_SocketValid((*req)->sd)) // If this request is finished, unlink it from the list and free the memory
			{
			// Since we're already doing a list traversal, we unlink the request directly instead of using AbortUnlinkAndFree()
			request_state *tmp = *req;
			*req = tmp->next;
			freeL("request_state/udsserver_idle", tmp);
			}
		else
			req = &(*req)->next;
		}
	return nextevent;
	}
