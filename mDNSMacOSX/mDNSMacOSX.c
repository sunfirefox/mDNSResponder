/* -*- Mode: C; tab-width: 4 -*-
 *
 * Copyright (c) 2002-2004 Apple Computer, Inc. All rights reserved.
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

$Log: mDNSMacOSX.c,v $
Revision 1.443  2007/07/20 23:23:11  cheshire
Rename out-of-date name "atq" (was AutoTunnelQuery) to simpler "tun"

Revision 1.442  2007/07/20 20:23:24  cheshire
<rdar://problem/4641118> Need separate SCPreferences for per-user .Mac settings
Fixed errors reading the Setup:/Network/BackToMyMac preferences

Revision 1.441  2007/07/20 16:46:45  mcguire
<rdar://problem/5345233> BTMM: Replace system() `route` calls to setup/teardown routes

Revision 1.440  2007/07/20 16:22:07  mcguire
<rdar://problem/5344584> BTMM: Replace system() `ifconfig` calls to setup/teardown IPv6 address

Revision 1.439  2007/07/20 01:14:56  cheshire
<rdar://problem/4641118> Need separate SCPreferences for per-user .Mac settings
Cleaned up log messages

Revision 1.438  2007/07/20 00:54:21  cheshire
<rdar://problem/4641118> Need separate SCPreferences for per-user .Mac settings

Revision 1.437  2007/07/19 22:01:27  cheshire
Added "#pragma mark" sections headings to divide code into related function groups

Revision 1.436  2007/07/18 03:25:25  cheshire
<rdar://problem/5304766> Register IPSec tunnel with IPv4-only hostname and create NAT port mappings
Bring up server-side tunnel on demand, when necessary

Revision 1.435  2007/07/18 01:05:08  cheshire
<rdar://problem/5303834> Automatically configure IPSec policy when resolving services
Add list of client tunnels so we can automatically reconfigure when local address changes

Revision 1.434  2007/07/16 20:16:00  vazquez
<rdar://problem/3867231> LegacyNATTraversal: Need complete rewrite
Remove unnecessary LNT init code

Revision 1.433  2007/07/14 00:36:07  cheshire
Remove temporary IPv4LL tunneling mode now that IPv6-over-IPv4 is working

Revision 1.432  2007/07/12 23:55:11  cheshire
<rdar://problem/5303834> Automatically configure IPSec policy when resolving services
Don't need two separate DNSQuestion structures when looking up tunnel endpoint

Revision 1.431  2007/07/12 23:34:48  cheshire
Removed 'LogOperation' message to reduce verbosity in syslog

Revision 1.430  2007/07/12 22:16:46  cheshire
Improved "could not convert shared secret from base64" log message so it doesn't reveal key data in syslog

Revision 1.429  2007/07/12 02:51:28  cheshire
<rdar://problem/5303834> Automatically configure IPSec policy when resolving services

Revision 1.428  2007/07/11 23:17:31  cheshire
<rdar://problem/5304766> Register IPSec tunnel with IPv4-only hostname and create NAT port mappings
Improve log message to indicate if we're starting or restarting racoon

Revision 1.427  2007/07/11 22:50:30  cheshire
<rdar://problem/5304766> Register IPSec tunnel with IPv4-only hostname and create NAT port mappings
Write /etc/racoon/remote/anonymous.conf configuration file and start up /usr/sbin/racoon

Revision 1.426  2007/07/11 20:40:49  cheshire
<rdar://problem/5304766> Register IPSec tunnel with IPv4-only hostname and create NAT port mappings
In mDNSPlatformGetPrimaryInterface(), prefer routable IPv4 address to IPv4LL

Revision 1.425  2007/07/11 19:24:19  cheshire
<rdar://problem/5303807> Register IPv6-only hostname and don't create port mappings for services
Configure internal AutoTunnel address
(For temporary testing we're faking up an IPv4LL address instead of IPv6 ULA, and we're
assigning it with "system(commandstring);" which probably isn't the most efficient way to do it)

Revision 1.424  2007/07/11 19:00:27  cheshire
Only need to set up m->AutoTunnelHostAddr first time through UpdateInterfaceList()

Revision 1.423  2007/07/11 03:00:59  cheshire
<rdar://problem/5303807> Register IPv6-only hostname and don't create port mappings for AutoTunnel services
Add AutoTunnel parameter to mDNS_SetSecretForDomain; Generate IPv6 ULA address for tunnel endpoint

Revision 1.422  2007/07/10 01:21:20  cheshire
Added (commented out) line for displaying key data for debugging

Revision 1.421  2007/06/25 20:58:11  cheshire
<rdar://problem/5234463> Write the Multicast DNS domains to the DynamicStore
Additional refinement: Add mDNS domain list new new DynamicStore entity "State:/Network/MulticastDNS"

Revision 1.420  2007/06/22 21:52:14  cheshire
<rdar://problem/5234463> Write the Multicast DNS domains to the DynamicStore

Revision 1.419  2007/06/22 21:32:00  cheshire
<rdar://problem/5239020> Use SecKeychainCopyDefault instead of SecKeychainOpen

Revision 1.418  2007/06/21 16:37:43  jgraessley
Bug #: 5280520
Reviewed by: Stuart Cheshire
Additional changes to get this compiling on the embedded platform.

Revision 1.417  2007/06/20 01:44:00  cheshire
More information in "Network Configuration Change" message

Revision 1.416  2007/06/20 01:10:12  cheshire
<rdar://problem/5280520> Sync iPhone changes into main mDNSResponder code

Revision 1.415  2007/06/15 19:23:38  cheshire
<rdar://problem/5254053> mDNSResponder renames my host without asking
Improve log messages, to distinguish user-initiated renames from automatic (name conflict) renames

Revision 1.414  2007/05/17 22:00:59  cheshire
<rdar://problem/5210966> Lower network change delay from two seconds to one second

Revision 1.413  2007/05/16 16:43:27  cheshire
Only log "bind" failures for our shared mDNS port and for binding to zero
-- other attempts to bind to a particular port may legitimately fail

Revision 1.412  2007/05/15 21:49:21  cheshire
Get rid of "#pragma unused"

Revision 1.411  2007/05/14 23:54:55  cheshire
Instead of sprintf, use safer length-limited mDNS_snprintf

Revision 1.410  2007/05/12 01:05:00  cheshire
Updated debugging messages

Revision 1.409  2007/05/10 22:39:48  cheshire
<rdar://problem/4118503> Share single socket instead of creating separate socket for each active interface
Only define CountMaskBits for builds with debugging messages

Revision 1.408  2007/05/10 22:19:00  cheshire
<rdar://problem/4118503> Share single socket instead of creating separate socket for each active interface
Don't deliver multicast packets for which we can't find an associated InterfaceID

Revision 1.407  2007/05/10 21:40:28  cheshire
Don't log unnecessary "Address already in use" errors when joining multicast groups

Revision 1.406  2007/05/08 00:56:17  cheshire
<rdar://problem/4118503> Share single socket instead of creating separate socket for each active interface

Revision 1.405  2007/05/04 20:21:39  cheshire
Improve "connect failed" error message

Revision 1.404  2007/05/02 19:41:53  cheshire
No need to alarm people with "Connection reset by peer" syslog message

Revision 1.403  2007/04/28 01:31:59  cheshire
Improve debugging support for catching memory corruption problems

Revision 1.402  2007/04/26 22:54:57  cheshire
Debugging messages to help track down <rdar://problem/5164206> mDNSResponder takes 50%+ CPU

Revision 1.401  2007/04/26 00:35:16  cheshire
<rdar://problem/5140339> uDNS: Domain discovery not working over VPN
Fixes to make sure results update correctly when connectivity changes (e.g. a DNS server
inside the firewall may give answers where a public one gives none, and vice versa.)

Revision 1.400  2007/04/24 21:50:27  cheshire
Debugging: Show list of changedKeys in NetworkChanged callback

Revision 1.399  2007/04/23 22:28:47  cheshire
Allan Nathanson informs us we should only be looking at the search list for resolver[0], not all of them

Revision 1.398  2007/04/23 04:57:00  cheshire
Log messages for debugging <rdar://problem/4570952> IPv6 multicast not working properly

Revision 1.397  2007/04/22 06:02:03  cheshire
<rdar://problem/4615977> Query should immediately return failure when no server

Revision 1.396  2007/04/21 21:47:47  cheshire
<rdar://problem/4376383> Daemon: Add watchdog timer

Revision 1.395  2007/04/18 20:58:34  cheshire
<rdar://problem/5140339> Domain discovery not working over VPN
Needed different code to handle the case where there's only a single search domain

Revision 1.394  2007/04/17 23:05:50  cheshire
<rdar://problem/3957358> Shouldn't send domain queries when we have 169.254 or loopback address

Revision 1.393  2007/04/17 19:21:29  cheshire
<rdar://problem/5140339> Domain discovery not working over VPN

Revision 1.392  2007/04/17 17:15:09  cheshire
Change NO_CFUSERNOTIFICATION code so it still logs to syslog

Revision 1.391  2007/04/07 01:01:48  cheshire
<rdar://problem/5095167> mDNSResponder periodically blocks in SSLRead

Revision 1.390  2007/04/06 18:45:02  cheshire
Fix SetupActiveInterfaces() -- accidentally changed SetupSocket parameter

Revision 1.389  2007/04/05 21:39:49  cheshire
Debugging messages to help diagnose <rdar://problem/5095167> mDNSResponder periodically blocks in SSLRead

Revision 1.388  2007/04/05 21:09:52  cheshire
Condense sprawling code

Revision 1.387  2007/04/05 20:40:37  cheshire
Remove unused mDNSPlatformTCPGetFlags()

Revision 1.386  2007/04/05 19:50:56  cheshire
Fixed memory leak: GetCertChain() was not releasing cert returned by SecIdentityCopyCertificate()

Revision 1.385  2007/04/03 19:39:19  cheshire
Fixed intel byte order bug in mDNSPlatformSetDNSServers()

Revision 1.384  2007/03/31 01:10:53  cheshire
Add debugging

Revision 1.383  2007/03/31 00:13:48  cheshire
Remove LogMsg

Revision 1.382  2007/03/28 21:01:29  cheshire
<rdar://problem/4743285> Remove inappropriate use of IsPrivateV4Addr()

Revision 1.381  2007/03/28 15:56:37  cheshire
<rdar://problem/5085774> Add listing of NAT port mapping and GetAddrInfo requests in SIGINFO output

Revision 1.380  2007/03/26 22:54:46  cheshire
Fix compile error

Revision 1.379  2007/03/22 18:31:48  cheshire
Put dst parameter first in mDNSPlatformStrCopy/mDNSPlatformMemCopy, like conventional Posix strcpy/memcpy

Revision 1.378  2007/03/22 00:49:20  cheshire
<rdar://problem/4848295> Advertise model information via Bonjour

Revision 1.377  2007/03/21 00:30:05  cheshire
<rdar://problem/4789455> Multiple errors in DNameList-related code

Revision 1.376  2007/03/20 17:07:15  cheshire
Rename "struct uDNS_TCPSocket_struct" to "TCPSocket", "struct uDNS_UDPSocket_struct" to "UDPSocket"

Revision 1.375  2007/03/20 00:50:57  cheshire
<rdar://problem/4530644> Remove logic to disable IPv6 discovery on interfaces which have a routable IPv4 address

Revision 1.374  2007/03/06 23:29:50  cheshire
<rdar://problem/4331696> Need to call IONotificationPortDestroy on shutdown

Revision 1.373  2007/02/28 01:51:20  cheshire
Added comment about reverse-order IP address

Revision 1.372  2007/02/28 01:06:48  cheshire
Use %#a format code instead of %d.%d.%d.%d

Revision 1.371  2007/02/08 21:12:28  cheshire
<rdar://problem/4386497> Stop reading /etc/mDNSResponder.conf on every sleep/wake

Revision 1.370  2007/01/16 22:59:58  cheshire
Error code ioErr is from wrong conceptual namespace; use errSSLClosedAbort instead

Revision 1.369  2007/01/10 02:09:32  cheshire
Better LogOperation record of keys read from System Keychain

Revision 1.368  2007/01/10 01:25:31  cheshire
Use symbol kDNSServiceCompPrivateDNS instead of fixed string "State:/Network/PrivateDNS"

Revision 1.367  2007/01/10 01:22:01  cheshire
Make sure c1, c2, c3 are initialized

Revision 1.366  2007/01/09 22:37:20  cheshire
Provide ten-second grace period for deleted keys, to give mDNSResponder
time to delete host name before it gives up access to the required key.

Revision 1.365  2007/01/09 21:09:20  cheshire
Need locking in KeychainChanged()

Revision 1.364  2007/01/09 20:17:04  cheshire
mDNSPlatformGetDNSConfig() needs to initialize fields even when no "Setup:/Network/DynamicDNS" entity exists

Revision 1.363  2007/01/09 02:41:18  cheshire
uDNS_SetupDNSConfig() shouldn't be called from mDNSMacOSX.c (platform support layer);
moved it to mDNS_Init() in mDNS.c (core code)

Revision 1.362  2007/01/08 23:54:01  cheshire
Made mDNSPlatformGetDNSConfig() more selective -- only reads prefs for non-null parameters

Revision 1.361  2007/01/05 08:30:48  cheshire
Trim excessive "$Log" checkin history from before 2006
(checkin history still available via "cvs log ..." of course)

Revision 1.360  2007/01/04 00:12:24  cheshire
<rdar://problem/4742742> Read *all* DNS keys from keychain,
 not just key for the system-wide default registration domain

Revision 1.359  2006/12/22 21:14:37  cheshire
Added comment explaining why we allow both "ddns" and "sndd" as valid item types
The Keychain APIs on Intel appear to store the four-character item type backwards (at least some of the time)

Revision 1.358  2006/12/22 20:59:50  cheshire
<rdar://problem/4742742> Read *all* DNS keys from keychain,
 not just key for the system-wide default registration domain

Revision 1.357  2006/12/21 00:09:45  cheshire
Use mDNSPlatformMemZero instead of bzero

Revision 1.356  2006/12/20 23:15:53  mkrochma
Fix the private domain list code so that it actually works

Revision 1.355  2006/12/20 23:04:36  mkrochma
Fix crash when adding private domain list to Dynamic Store

Revision 1.354  2006/12/19 22:43:55  cheshire
Fix compiler warnings

Revision 1.353  2006/12/14 22:08:29  cheshire
Fixed memory leak: need to call SecKeychainItemFreeAttributesAndData()
to release data allocated by SecKeychainItemCopyAttributesAndData()

Revision 1.352  2006/12/14 02:33:26  cheshire
<rdar://problem/4841422> uDNS: Wide-area registrations sometimes fail

Revision 1.351  2006/11/28 21:37:51  mkrochma
Tweak where the private DNS data is written

Revision 1.350  2006/11/28 07:55:02  herscher
<rdar://problem/4742743> dnsextd has a slow memory leak

Revision 1.349  2006/11/28 07:45:58  herscher
<rdar://problem/4787010> Daemon: Need to write list of private domain names to the DynamicStore

Revision 1.348  2006/11/16 21:47:20  mkrochma
<rdar://problem/4841422> uDNS: Wide-area registrations sometimes fail

Revision 1.347  2006/11/10 00:54:16  cheshire
<rdar://problem/4816598> Changing case of Computer Name doesn't work

Revision 1.346  2006/10/31 02:34:58  cheshire
<rdar://problem/4692130> Stop creating HINFO records

Revision 1.345  2006/09/21 20:04:38  mkrochma
Accidently changed function name while checking in previous fix

Revision 1.344  2006/09/21 19:04:13  mkrochma
<rdar://problem/4733803> uDNS: Update keychain format of DNS key to include prefix

Revision 1.343  2006/09/15 21:20:16  cheshire
Remove uDNS_info substructure from mDNS_struct

Revision 1.342  2006/08/16 00:31:50  mkrochma
<rdar://problem/4386944> Get rid of NotAnInteger references

Revision 1.341  2006/08/14 23:24:40  cheshire
Re-licensed mDNSResponder daemon source code under Apache License, Version 2.0

Revision 1.340  2006/07/29 19:11:13  mkrochma
Change GetUserSpecifiedDDNSConfig LogMsg to debugf

Revision 1.339  2006/07/27 03:24:35  cheshire
<rdar://problem/4049048> Convert mDNSResponder to use kqueue
Further refinement: Declare KQueueEntry parameter "const"

Revision 1.338  2006/07/27 02:59:25  cheshire
<rdar://problem/4049048> Convert mDNSResponder to use kqueue
Further refinements: CFRunLoop thread needs to explicitly wake the kqueue thread
after releasing BigMutex, in case actions it took have resulted in new work for the
kqueue thread (e.g. NetworkChanged events may result in the kqueue thread having to
add new active interfaces to its list, and consequently schedule queries to be sent).

Revision 1.337  2006/07/22 06:11:37  cheshire
<rdar://problem/4049048> Convert mDNSResponder to use kqueue

Revision 1.336  2006/07/15 02:01:32  cheshire
<rdar://problem/4472014> Add Private DNS client functionality to mDNSResponder
Fix broken "empty string" browsing

Revision 1.335  2006/07/14 05:25:11  cheshire
<rdar://problem/4472014> Add Private DNS client functionality to mDNSResponder
Fixed crash in mDNSPlatformGetDNSConfig() reading BrowseDomains array

Revision 1.334  2006/07/05 23:42:00  cheshire
<rdar://problem/4472014> Add Private DNS client functionality to mDNSResponder

Revision 1.333  2006/06/29 05:33:30  cheshire
<rdar://problem/4607043> mDNSResponder conditional compilation options

Revision 1.332  2006/06/28 09:10:36  cheshire
Extra debugging messages

Revision 1.331  2006/06/21 22:29:42  cheshire
Make _CFCopySystemVersionDictionary() call more defensive on systems that have no build information set

Revision 1.330  2006/06/20 23:06:00  cheshire
Fix some keychain API type mismatches (was mDNSu32 instead of UInt32)

Revision 1.329  2006/06/08 23:22:33  cheshire
Comment changes

Revision 1.328  2006/03/19 03:27:49  cheshire
<rdar://problem/4118624> Suppress "interface flapping" logic for loopback

Revision 1.327  2006/03/19 02:00:09  cheshire
<rdar://problem/4073825> Improve logic for delaying packets after repeated interface transitions

Revision 1.326  2006/03/08 22:42:23  cheshire
Fix spelling mistake: LocalReverseMapomain -> LocalReverseMapDomain

Revision 1.325  2006/01/10 00:39:17  cheshire
Add comments explaining how IPv6 link-local addresses sometimes have an embedded scope_id

Revision 1.324  2006/01/09 19:28:59  cheshire
<rdar://problem/4403128> Cap number of "sendto failed" messages we allow mDNSResponder to log

Revision 1.323  2006/01/05 21:45:27  cheshire
<rdar://problem/4400118> Fix uninitialized structure member in IPv6 code

Revision 1.322  2006/01/05 21:41:50  cheshire
<rdar://problem/4108164> Reword "mach_absolute_time went backwards" dialog

Revision 1.321  2006/01/05 21:35:06  cheshire
Add (commented out) trigger value for testing "mach_absolute_time went backwards" notice

*/

// ***************************************************************************
// mDNSMacOSX.c:
// Supporting routines to run mDNS on a CFRunLoop platform
// ***************************************************************************

// For debugging, set LIST_ALL_INTERFACES to 1 to display all found interfaces,
// including ones that mDNSResponder chooses not to use.
#define LIST_ALL_INTERFACES 0

// For enabling AAAA records over IPv4. Setting this to 0 sends only
// A records over IPv4 and AAAA over IPv6. Setting this to 1 sends both
// AAAA and A records over both IPv4 and IPv6.
#define AAAA_OVER_V4 1

// In Mac OS X 10.4 and earlier, to reduce traffic, we would send and receive using IPv6 only on interfaces that had no routable
// IPv4 address. Having a routable IPv4 address assigned is a reasonable indicator of being on a large configured network,
// which means there's a good chance that most or all the other devices on that network should also have IPv4.
// By doing this we lost the ability to talk to true IPv6-only devices on that link, but we cut the packet rate in half.
// At that time, reducing the packet rate was more important than v6-only devices on a large configured network,
// so were willing to make that sacrifice.
// In Mac OS X 10.5, in 2007, two things have changed:
// 1. IPv6-only devices are starting to become more common, so we can't ignore them.
// 2. Other efficiency improvements in the code mean that crude hacks like this should no longer be necessary.

#define USE_V6_ONLY_WHEN_NO_ROUTABLE_V4 0

#include "mDNSEmbeddedAPI.h"          // Defines the interface provided to the client layer above
#include "DNSCommon.h"
#include "uDNS.h"
#include "mDNSMacOSX.h"               // Defines the specific types needed to run mDNS on this platform
#include "../mDNSShared/uds_daemon.h" // Defines communication interface from platform layer up to UDS daemon
#include "PlatformCommon.h"


#include <stdio.h>
#include <stdarg.h>                 // For va_list support
#include <net/if.h>
#include <net/if_types.h>			// For IFT_ETHER
#include <net/if_dl.h>
#include <net/route.h>              // For struct rt_msghdr etc.
#include <sys/uio.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/event.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>                   // platform support for UTC time
#include <arpa/inet.h>              // for inet_aton
#include <pthread.h>

#include <netinet/in.h>             // For IP_RECVTTL
#ifndef IP_RECVTTL
#define IP_RECVTTL 24               // bool; receive reception TTL w/dgram
#endif

#include <netinet/in_systm.h>       // For n_long, required by <netinet/ip.h> below
#include <netinet/ip.h>             // For IPTOS_LOWDELAY etc.
#include <netinet6/in6_var.h>       // For IN6_IFF_NOTREADY etc.
#include <netinet6/nd6.h>           // For ND6_INFINITE_LIFETIME etc.

#ifndef NO_SECURITYFRAMEWORK
#include <Security/SecureTransport.h>
#include <Security/Security.h>
#endif /* NO_SECURITYFRAMEWORK */

#include <DebugServices.h>
#include "dnsinfo.h"

// Code contributed by Dave Heller:
// Define RUN_ON_PUMA_WITHOUT_IFADDRS to compile code that will
// work on Mac OS X 10.1, which does not have the getifaddrs call.
#define RUN_ON_PUMA_WITHOUT_IFADDRS 0
#if RUN_ON_PUMA_WITHOUT_IFADDRS
#include "mDNSMacOSXPuma.c"
#else
#include <ifaddrs.h>
#endif

#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <mach/mach_time.h>

#define kInterfaceSpecificOption "interface="

// ***************************************************************************
// Globals

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark - Globals
#endif

static mDNSu32 clockdivisor = 0;

mDNSexport int KQueueFD;

#ifndef NO_SECURITYFRAMEWORK
static CFArrayRef ServerCerts;
static SecKeychainRef ServerKC;
#endif /* NO_SECURITYFRAMEWORK */

#define DYNDNS_KEYCHAIN_SERVICE "DynDNS Shared Secret"

CFStringRef NetworkChangedKey_IPv4;
CFStringRef NetworkChangedKey_IPv6;
CFStringRef NetworkChangedKey_Hostnames;
CFStringRef NetworkChangedKey_Computername;
CFStringRef NetworkChangedKey_DNS;
CFStringRef NetworkChangedKey_DynamicDNS  = CFSTR("Setup:/Network/DynamicDNS");
CFStringRef NetworkChangedKey_BackToMyMac = CFSTR("Setup:/Network/BackToMyMac");

// ***************************************************************************
// Functions

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - Utility Functions
#endif

// We only attempt to send and receive multicast packets on interfaces that are
// (a) flagged as multicast-capable
// (b) *not* flagged as point-to-point (e.g. modem)
// Typically point-to-point interfaces are modems (including mobile-phone pseudo-modems), and we don't want
// to run up the user's bill sending multicast traffic over a link where there's only a single device at the
// other end, and that device (e.g. a modem bank) is probably not answering Multicast DNS queries anyway.
#define MulticastInterface(i) ((i->ifa_flags & IFF_MULTICAST) && !(i->ifa_flags & IFF_POINTOPOINT))

mDNSexport void NotifyOfElusiveBug(const char *title, const char *msg)	// Both strings are UTF-8 text
	{
	static int notifyCount = 0;
	if (notifyCount) return;

	// If we display our alert early in the boot process, then it vanishes once the desktop appears.
	// To avoid this, we don't try to display alerts in the first three minutes after boot.
	if ((mDNSu32)(mDNSPlatformRawTime()) < (mDNSu32)(mDNSPlatformOneSecond * 180)) return;

	// Unless ForceAlerts is defined, we only show these bug report alerts on machines that have a 17.x.x.x address
	#if !ForceAlerts
		{
		// Determine if we're at Apple (17.*.*.*)
		extern mDNS mDNSStorage;
		NetworkInterfaceInfoOSX *i;
		for (i = mDNSStorage.p->InterfaceList; i; i = i->next)
			if (i->ifinfo.ip.type == mDNSAddrType_IPv4 && i->ifinfo.ip.ip.v4.b[0] == 17)
				break;
		if (!i) return;	// If not at Apple, don't show the alert
		}
	#endif

	LogMsg("%s", title);
	LogMsg("%s", msg);
	// Display a notification to the user
	notifyCount++;

#ifndef NO_CFUSERNOTIFICATION
	static const char footer[] = "(Note: This message only appears on machines with 17.x.x.x IP addresses — i.e. at Apple — not on customer machines.)";
	CFStringRef alertHeader  = CFStringCreateWithCString(NULL, title,  kCFStringEncodingUTF8);
	CFStringRef alertBody    = CFStringCreateWithCString(NULL, msg,    kCFStringEncodingUTF8);
	CFStringRef alertFooter  = CFStringCreateWithCString(NULL, footer, kCFStringEncodingUTF8);
	CFStringRef alertMessage = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@\r\r%@"), alertBody, alertFooter);
	CFUserNotificationDisplayNotice(0.0, kCFUserNotificationStopAlertLevel, NULL, NULL, NULL, alertHeader, alertMessage, NULL);
#endif /* NO_CFUSERNOTIFICATION */
	}

mDNSlocal struct ifaddrs *myGetIfAddrs(int refresh)
	{
	static struct ifaddrs *ifa = NULL;

	if (refresh && ifa)
		{
		freeifaddrs(ifa);
		ifa = NULL;
		}

	if (ifa == NULL) getifaddrs(&ifa);

	return ifa;
	}

mDNSlocal NetworkInterfaceInfoOSX *SearchForInterfaceByName(mDNS *const m, const char *ifname, int type)
	{
	NetworkInterfaceInfoOSX *i;
	for (i = m->p->InterfaceList; i; i = i->next)
		if (i->Exists && !strcmp(i->ifa_name, ifname) &&
			((AAAA_OVER_V4                                              ) ||
			 (type == AF_INET  && i->ifinfo.ip.type == mDNSAddrType_IPv4) ||
			 (type == AF_INET6 && i->ifinfo.ip.type == mDNSAddrType_IPv6))) return(i);
	return(NULL);
	}

mDNSlocal int myIfIndexToName(u_short index, char *name)
	{
	struct ifaddrs *ifa;
	for (ifa = myGetIfAddrs(0); ifa; ifa = ifa->ifa_next)
		if (ifa->ifa_addr->sa_family == AF_LINK)
			if (((struct sockaddr_dl*)ifa->ifa_addr)->sdl_index == index)
				{ strlcpy(name, ifa->ifa_name, IF_NAMESIZE); return 0; }
	return -1;
	}

mDNSexport mDNSInterfaceID mDNSPlatformInterfaceIDfromInterfaceIndex(mDNS *const m, mDNSu32 index)
	{
	NetworkInterfaceInfoOSX *i;
	if (index == kDNSServiceInterfaceIndexLocalOnly) return(mDNSInterface_LocalOnly);
	if (index == kDNSServiceInterfaceIndexAny      ) return(mDNSNULL);

	// Don't get tricked by inactive interfaces with no InterfaceID set
	for (i = m->p->InterfaceList; i; i = i->next)
		if (i->ifinfo.InterfaceID && i->scope_id == index) return(i->ifinfo.InterfaceID);

	// Not found. Make sure our interface list is up to date, then try again.
	LogOperation("InterfaceID for interface index %d not found; Updating interface list", index);
	mDNSMacOSXNetworkChanged(m);
	for (i = m->p->InterfaceList; i; i = i->next)
		if (i->ifinfo.InterfaceID && i->scope_id == index) return(i->ifinfo.InterfaceID);

	return(mDNSNULL);
	}

mDNSexport mDNSu32 mDNSPlatformInterfaceIndexfromInterfaceID(mDNS *const m, mDNSInterfaceID id)
	{
	NetworkInterfaceInfoOSX *i;
	if (id == mDNSInterface_LocalOnly) return(kDNSServiceInterfaceIndexLocalOnly);
	if (id == mDNSInterface_Any      ) return(0);

	// Don't use i->ifinfo.InterfaceID here, because we DO want to find inactive interfaces, which have no InterfaceID set
	for (i = m->p->InterfaceList; i; i = i->next)
		if ((mDNSInterfaceID)i == id) return(i->scope_id);

	// Not found. Make sure our interface list is up to date, then try again.
	LogOperation("Interface index for InterfaceID %p not found; Updating interface list", id);
	mDNSMacOSXNetworkChanged(m);
	for (i = m->p->InterfaceList; i; i = i->next)
		if ((mDNSInterfaceID)i == id) return(i->scope_id);

	return(0);
	}

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - UDP & TCP send & receive
#endif

mDNSlocal mDNSBool AddrRequiresPPPConnection(const struct sockaddr *addr)
	{
	mDNSBool result = mDNSfalse;
	SCNetworkConnectionFlags flags;
	SCNetworkReachabilityRef ReachRef = NULL;

	ReachRef = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, addr);
	if (!ReachRef) { LogMsg("ERROR: RequiresConnection - SCNetworkReachabilityCreateWithAddress"); goto end; }
	if (!SCNetworkReachabilityGetFlags(ReachRef, &flags)) { LogMsg("ERROR: AddrRequiresPPPConnection - SCNetworkReachabilityGetFlags"); goto end; }
	result = flags & kSCNetworkFlagsConnectionRequired;

	end:
	if (ReachRef) CFRelease(ReachRef);
	return result;
	}

// NOTE: If InterfaceID is NULL, it means, "send this packet through our anonymous unicast socket"
// NOTE: If InterfaceID is non-NULL it means, "send this packet through our port 5353 socket on the specified interface"
// OR send via our primary v4 unicast socket
mDNSexport mStatus mDNSPlatformSendUDP(const mDNS *const m, const void *const msg, const mDNSu8 *const end,
	mDNSInterfaceID InterfaceID, const mDNSAddr *dst, mDNSIPPort dstPort)
	{
	// Note: For this platform we've adopted the convention that InterfaceIDs are secretly pointers
	// to the NetworkInterfaceInfoOSX structure that holds the active sockets. The mDNSCore code
	// doesn't know that and doesn't need to know that -- it just treats InterfaceIDs as opaque identifiers.
	NetworkInterfaceInfoOSX *info = (NetworkInterfaceInfoOSX *)InterfaceID;
	char *ifa_name = info ? info->ifa_name : "unicast";
	struct sockaddr_storage to;
	int s = -1, err;
	mStatus result = mStatus_NoError;

	// Sanity check: Make sure that if we're sending a query via unicast, we're sending it using our
	// anonymous socket created for this purpose, so that we'll receive the response.
	// If we use one of the many multicast sockets bound to port 5353 then we may not receive responses reliably.
	if (InterfaceID && !mDNSAddrIsDNSMulticast(dst))
		{
		const DNSMessage *const m = (DNSMessage *)msg;
		if ((m->h.flags.b[0] & kDNSFlag0_QR_Mask) == kDNSFlag0_QR_Query)
			LogMsg("mDNSPlatformSendUDP: ERROR: Sending query OP from mDNS port to non-mDNS destination %#a:%d", dst, mDNSVal16(dstPort));
		}

	if (dst->type == mDNSAddrType_IPv4)
		{
		struct sockaddr_in *sin_to = (struct sockaddr_in*)&to;
		sin_to->sin_len            = sizeof(*sin_to);
		sin_to->sin_family         = AF_INET;
		sin_to->sin_port           = dstPort.NotAnInteger;
		sin_to->sin_addr.s_addr    = dst->ip.v4.NotAnInteger;
		s = m->p->permanentsockets.sktv4;
		if (info)	// Specify outgoing interface
			{
			if (!mDNSAddrIsDNSMulticast(dst))
				{
				#ifdef IP_FORCE_OUT_IFP
					setsockopt(s, IPPROTO_IP, IP_FORCE_OUT_IFP, ifa_name, strlen(ifa_name) + 1);
				#else
					{
					static int displayed = 0;
					if (!displayed)
						{
						displayed = 1;
						LogMsg("IP_FORCE_OUT_IFP Socket option not defined -- cannot specify interface for unicast packets");
						}
					}
				#endif
				}
			else
				{
				err = setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, &info->ifa_v4addr, sizeof(info->ifa_v4addr));
				if (err < 0) LogMsg("setsockopt - IP_MULTICAST_IF error %.4a %ld errno %d (%s)", &info->ifa_v4addr, err, errno, strerror(errno));
				}
			}
		}
	else if (dst->type == mDNSAddrType_IPv6)
		{
		struct sockaddr_in6 *sin6_to = (struct sockaddr_in6*)&to;
		sin6_to->sin6_len            = sizeof(*sin6_to);
		sin6_to->sin6_family         = AF_INET6;
		sin6_to->sin6_port           = dstPort.NotAnInteger;
		sin6_to->sin6_flowinfo       = 0;
		sin6_to->sin6_addr           = *(struct in6_addr*)&dst->ip.v6;
		sin6_to->sin6_scope_id       = info ? info->scope_id : 0;
		s = m->p->permanentsockets.sktv6;
		if (info && mDNSAddrIsDNSMulticast(dst))	// Specify outgoing interface
			{
			err = setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_IF, &info->scope_id, sizeof(info->scope_id));
			if (err < 0) LogMsg("setsockopt - IPV6_MULTICAST_IF error %ld errno %d (%s)", err, errno, strerror(errno));
			}
		}
	else
		{
		LogMsg("mDNSPlatformSendUDP: dst is not an IPv4 or IPv6 address!");
		return mStatus_BadParamErr;
		}

	// Don't send if it would cause dial-on-demand connection initiation.
	// As an optimization, don't bother consulting reachability API / routing
	// table when sending Multicast DNS since we ignore PPP interfaces for mDNS traffic.
	if (!info && !mDNSAddrIsDNSMulticast(dst) && AddrRequiresPPPConnection((struct sockaddr *)&to))
		{
		debugf("mDNSPlatformSendUDP: Surpressing sending to avoid dial-on-demand connection");
		return mStatus_NoError;
		}

	if (s >= 0)
		verbosedebugf("mDNSPlatformSendUDP: sending on InterfaceID %p %5s/%ld to %#a:%d skt %d",
			InterfaceID, ifa_name, dst->type, dst, mDNSVal16(dstPort), s);
	else
		verbosedebugf("mDNSPlatformSendUDP: NOT sending on InterfaceID %p %5s/%ld (socket of this type not available)",
			InterfaceID, ifa_name, dst->type, dst, mDNSVal16(dstPort));

	// Note: When sending, mDNSCore may often ask us to send both a v4 multicast packet and then a v6 multicast packet
	// If we don't have the corresponding type of socket available, then return mStatus_Invalid
	if (s < 0) return(mStatus_Invalid);

	err = sendto(s, msg, (UInt8*)end - (UInt8*)msg, 0, (struct sockaddr *)&to, to.ss_len);
	if (err < 0)
		{
		static int MessageCount = 0;
		// Don't report EHOSTDOWN (i.e. ARP failure), ENETDOWN, or no route to host for unicast destinations
		if (!mDNSAddressIsAllDNSLinkGroup(dst))
			if (errno == EHOSTDOWN || errno == ENETDOWN || errno == EHOSTUNREACH || errno == ENETUNREACH) return(mStatus_TransientErr);
		// Don't report EHOSTUNREACH in the first three minutes after boot
		// This is because mDNSResponder intentionally starts up early in the boot process (See <rdar://problem/3409090>)
		// but this means that sometimes it starts before configd has finished setting up the multicast routing entries.
		if (errno == EHOSTUNREACH && (mDNSu32)(mDNSPlatformRawTime()) < (mDNSu32)(mDNSPlatformOneSecond * 180)) return(mStatus_TransientErr);
		// Don't report EADDRNOTAVAIL ("Can't assign requested address") if we're in the middle of a network configuration change
		if (errno == EADDRNOTAVAIL && m->p->NetworkChanged) return(mStatus_TransientErr);
		if (MessageCount < 1000)
			{
			MessageCount++;
			LogMsg("mDNSPlatformSendUDP sendto failed to send packet on InterfaceID %p %5s/%ld to %#a:%d skt %d error %d errno %d (%s) %lu",
				InterfaceID, ifa_name, dst->type, dst, mDNSVal16(dstPort), s, err, errno, strerror(errno), (mDNSu32)(m->timenow));
			}
		result = mStatus_UnknownErr;
		}

#ifdef IP_FORCE_OUT_IFP
	if (dst->type == mDNSAddrType_IPv4 && info && !mDNSAddrIsDNSMulticast(dst))
		setsockopt(s, IPPROTO_IP, IP_FORCE_OUT_IFP, "", 1);
#endif

	return(result);
	}

mDNSlocal ssize_t myrecvfrom(const int s, void *const buffer, const size_t max,
	struct sockaddr *const from, size_t *const fromlen, mDNSAddr *dstaddr, char ifname[IF_NAMESIZE], mDNSu8 *ttl)
	{
	static unsigned int numLogMessages = 0;
	struct iovec databuffers = { (char *)buffer, max };
	struct msghdr   msg;
	ssize_t         n;
	struct cmsghdr *cmPtr;
	char            ancillary[1024];

	*ttl = 255;  // If kernel fails to provide TTL data (e.g. Jaguar doesn't) then assume the TTL was 255 as it should be

	// Set up the message
	msg.msg_name       = (caddr_t)from;
	msg.msg_namelen    = *fromlen;
	msg.msg_iov        = &databuffers;
	msg.msg_iovlen     = 1;
	msg.msg_control    = (caddr_t)&ancillary;
	msg.msg_controllen = sizeof(ancillary);
	msg.msg_flags      = 0;

	// Receive the data
	n = recvmsg(s, &msg, 0);
	if (n<0)
		{
		if (errno != EWOULDBLOCK && numLogMessages++ < 100) LogMsg("mDNSMacOSX.c: recvmsg(%d) returned error %d errno %d", s, n, errno);
		return(-1);
		}
	if (msg.msg_controllen < (int)sizeof(struct cmsghdr))
		{
		if (numLogMessages++ < 100) LogMsg("mDNSMacOSX.c: recvmsg(%d) returned %d msg.msg_controllen %d < sizeof(struct cmsghdr) %lu",
			s, n, msg.msg_controllen, sizeof(struct cmsghdr));
		return(-1);
		}
	if (msg.msg_flags & MSG_CTRUNC)
		{
		if (numLogMessages++ < 100) LogMsg("mDNSMacOSX.c: recvmsg(%d) msg.msg_flags & MSG_CTRUNC", s);
		return(-1);
		}

	*fromlen = msg.msg_namelen;

	// Parse each option out of the ancillary data.
	for (cmPtr = CMSG_FIRSTHDR(&msg); cmPtr; cmPtr = CMSG_NXTHDR(&msg, cmPtr))
		{
		// debugf("myrecvfrom cmsg_level %d cmsg_type %d", cmPtr->cmsg_level, cmPtr->cmsg_type);
		if (cmPtr->cmsg_level == IPPROTO_IP && cmPtr->cmsg_type == IP_RECVDSTADDR)
			{
			dstaddr->type = mDNSAddrType_IPv4;
			dstaddr->ip.v4 = *(mDNSv4Addr*)CMSG_DATA(cmPtr);
			}
		if (cmPtr->cmsg_level == IPPROTO_IP && cmPtr->cmsg_type == IP_RECVIF)
			{
			struct sockaddr_dl *sdl = (struct sockaddr_dl *)CMSG_DATA(cmPtr);
			if (sdl->sdl_nlen < IF_NAMESIZE)
				{
				mDNSPlatformMemCopy(ifname, sdl->sdl_data, sdl->sdl_nlen);
				ifname[sdl->sdl_nlen] = 0;
				// debugf("IP_RECVIF sdl_index %d, sdl_data %s len %d", sdl->sdl_index, ifname, sdl->sdl_nlen);
				}
			}
		if (cmPtr->cmsg_level == IPPROTO_IP && cmPtr->cmsg_type == IP_RECVTTL)
			*ttl = *(u_char*)CMSG_DATA(cmPtr);
		if (cmPtr->cmsg_level == IPPROTO_IPV6 && cmPtr->cmsg_type == IPV6_PKTINFO)
			{
			struct in6_pktinfo *ip6_info = (struct in6_pktinfo*)CMSG_DATA(cmPtr);
			dstaddr->type = mDNSAddrType_IPv6;
			dstaddr->ip.v6 = *(mDNSv6Addr*)&ip6_info->ipi6_addr;
			myIfIndexToName(ip6_info->ipi6_ifindex, ifname);
			}
		if (cmPtr->cmsg_level == IPPROTO_IPV6 && cmPtr->cmsg_type == IPV6_HOPLIMIT)
			*ttl = *(int*)CMSG_DATA(cmPtr);
		}

	return(n);
	}

mDNSlocal void myKQSocketCallBack(int s1, short filter, void *context)
	{
	const KQSocketSet *const ss = (const KQSocketSet *)context;
	mDNS *const m = ss->m;
	int err, count = 0;

	if (filter != EVFILT_READ)
		LogMsg("myKQSocketCallBack: Why is filter %d not EVFILT_READ (%d)?", filter, EVFILT_READ);

	if (s1 != ss->sktv4 && s1 != ss->sktv6)
		{
		LogMsg("myKQSocketCallBack: native socket %d", s1);
		LogMsg("myKQSocketCallBack: sktv4 %d", ss->sktv4);
		LogMsg("myKQSocketCallBack: sktv6 %d", ss->sktv6);
 		}

	while (1)
		{
		mDNSAddr senderAddr, destAddr;
		mDNSIPPort senderPort, destPort = MulticastDNSPort;
		struct sockaddr_storage from;
		size_t fromlen = sizeof(from);
		char packetifname[IF_NAMESIZE] = "";
		mDNSu8 ttl;
		err = myrecvfrom(s1, &m->imsg, sizeof(m->imsg), (struct sockaddr *)&from, &fromlen, &destAddr, packetifname, &ttl);
		if (err < 0) break;

		count++;
		if (from.ss_family == AF_INET)
			{
			struct sockaddr_in *sin = (struct sockaddr_in*)&from;
			senderAddr.type = mDNSAddrType_IPv4;
			senderAddr.ip.v4.NotAnInteger = sin->sin_addr.s_addr;
			senderPort.NotAnInteger = sin->sin_port;
			//LogOperation("myKQSocketCallBack received IPv4 packet from %#-15a to %#-15a on skt %d %s", &senderAddr, &destAddr, s1, packetifname);
			}
		else if (from.ss_family == AF_INET6)
			{
			struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)&from;
			senderAddr.type = mDNSAddrType_IPv6;
			senderAddr.ip.v6 = *(mDNSv6Addr*)&sin6->sin6_addr;
			senderPort.NotAnInteger = sin6->sin6_port;
			//LogOperation("myKQSocketCallBack received IPv6 packet from %#-15a to %#-15a on skt %d %s", &senderAddr, &destAddr, s1, packetifname);
			}
		else
			{
			LogMsg("myKQSocketCallBack from is unknown address family %d", from.ss_family);
			return;
			}

		// NOTE: When handling multiple packets in a batch, MUST reset InterfaceID before handling each packet
		mDNSInterfaceID InterfaceID = mDNSNULL;
		NetworkInterfaceInfo *intf = m->HostInterfaces;
		while (intf && strcmp(intf->ifname, packetifname)) intf = intf->next;
		// When going to sleep we deregister all our interfaces, but if the machine
		// takes a few seconds to sleep we may continue to receive multicasts
		// during that time, which would confuse mDNSCoreReceive, because as far
		// as it's concerned, we should have no active interfaces any more.
		// Hence we ignore multicasts for which we can find no matching InterfaceID.
		if (intf) InterfaceID = intf->InterfaceID;
		else if (mDNSAddrIsDNSMulticast(&destAddr)) continue;

//		LogMsg("myKQSocketCallBack got packet from %#a to %#a on interface %#a/%s",
//			&senderAddr, &destAddr, &ss->info->ifinfo.ip, ss->info->ifa_name);

		mDNSCoreReceive(m, &m->imsg, (unsigned char*)&m->imsg + err, &senderAddr, senderPort, &destAddr, destPort, InterfaceID);
		}

	if (err < 0 && (errno != EWOULDBLOCK || count == 0))
		{
		// Something is busted here.
		// kqueue says there is a packet, but myrecvfrom says there is not.
		// Try calling select() to get another opinion.
		// Find out about other socket parameter that can help understand why select() says the socket is ready for read
		// All of this is racy, as data may have arrived after the call to select()
		static unsigned int numLogMessages = 0;
		int save_errno = errno;
		int so_error = -1;
		int so_nread = -1;
		int fionread = -1;
		socklen_t solen = sizeof(int);
		fd_set readfds;
		struct timeval timeout;
		int selectresult;
		FD_ZERO(&readfds);
		FD_SET(s1, &readfds);
		timeout.tv_sec  = 0;
		timeout.tv_usec = 0;
		selectresult = select(s1+1, &readfds, NULL, NULL, &timeout);
		if (getsockopt(s1, SOL_SOCKET, SO_ERROR, &so_error, &solen) == -1)
			LogMsg("myKQSocketCallBack getsockopt(SO_ERROR) error %d", errno);
		if (getsockopt(s1, SOL_SOCKET, SO_NREAD, &so_nread, &solen) == -1)
			LogMsg("myKQSocketCallBack getsockopt(SO_NREAD) error %d", errno);
		if (ioctl(s1, FIONREAD, &fionread) == -1)
			LogMsg("myKQSocketCallBack ioctl(FIONREAD) error %d", errno);
		if (numLogMessages++ < 100)
			LogMsg("myKQSocketCallBack recvfrom skt %d error %d errno %d (%s) select %d (%spackets waiting) so_error %d so_nread %d fionread %d count %d",
				s1, err, save_errno, strerror(save_errno), selectresult, FD_ISSET(s1, &readfds) ? "" : "*NO* ", so_error, so_nread, fionread, count);
		if (numLogMessages > 5)
			NotifyOfElusiveBug("Flaw in Kernel (select/recvfrom mismatch)",
				"Congratulations, you've reproduced an elusive bug.\r"
				"Please contact the current assignee of <rdar://problem/3375328>.\r"
				"Alternatively, you can send email to radar-3387020@group.apple.com. (Note number is different.)\r"
				"If possible, please leave your machine undisturbed so that someone can come to investigate the problem.");

		sleep(1);		// After logging this error, rate limit so we don't flood syslog
		}
	}

// TCP socket support

struct TCPSocket_struct
	{
	TCPConnectionCallback callback;
	int fd;
	KQueueEntry kqEntry;
	TCPSocketFlags flags;
#ifndef NO_SECURITYFRAMEWORK
	SSLContextRef tlsContext;
#endif /* NO_SECURITYFRAMEWORK */
	void *context;
	mDNSBool setup;
	mDNSBool handshakecomplete;
	mDNSBool connected;
	};

#ifndef NO_SECURITYFRAMEWORK

mDNSlocal OSStatus tlsWriteSock(SSLConnectionRef connection, const void *data, size_t *dataLength)
	{
	int ret = send(((TCPSocket *)connection)->fd, data, *dataLength, 0);
	//if (ret >= 0) LogMsg("tlsWriteSock: %d\n", ret);
	//else          LogMsg("tlsWriteSock: %d %d %s\n", ret, errno, strerror(errno));
	if (ret >= 0 && (size_t)ret < *dataLength) { *dataLength = ret; return(errSSLWouldBlock); }
	if (ret >= 0)                              { *dataLength = ret; return(noErr); }
	*dataLength = 0;
	if (errno == EAGAIN    ) return(errSSLWouldBlock);
	if (errno == ENOENT    ) return(errSSLClosedGraceful);
	if (errno == ECONNRESET) return(errSSLClosedAbort);
	LogMsg("ERROR: tlsWriteSock: error %d %s\n", errno, strerror(errno));
	return(errSSLClosedAbort);
	}

mDNSlocal OSStatus tlsReadSock(SSLConnectionRef connection, void *data, size_t *dataLength)
	{
	int ret = recv(((TCPSocket *)connection)->fd, data, *dataLength, 0);
	//if (ret >= 0) LogMsg("tlsSockRead: %d\n", ret);
	//else          LogMsg("tlsSockRead: %d %d %s\n", ret, errno, strerror(errno));
	if (ret >= 0 && (size_t)ret < *dataLength) { *dataLength = ret; return(errSSLWouldBlock); }
	if (ret >= 0)                              { *dataLength = ret; return(noErr); }
	*dataLength = 0;
	if (errno == EAGAIN    ) return(errSSLWouldBlock);
	if (errno == ENOENT    ) return(errSSLClosedGraceful);
	if (errno == ECONNRESET) return(errSSLClosedAbort);
	LogMsg("ERROR: tlsSockRead: error %d %s\n", errno, strerror(errno));
	return(errSSLClosedAbort);
	}

mDNSlocal OSStatus tlsSetupSock(TCPSocket *sock, mDNSBool server)
	{
	mStatus err = SSLNewContext(server, &sock->tlsContext);
	if (err) { LogMsg("ERROR: tlsSetupSock: SSLNewContext failed with error code: %d", err); return(err); }

	err = SSLSetIOFuncs(sock->tlsContext, tlsReadSock, tlsWriteSock);
	if (err) { LogMsg("ERROR: tlsSetupSock: SSLSetIOFuncs failed with error code: %d", err); return(err); }

	err = SSLSetConnection(sock->tlsContext, (SSLConnectionRef) sock);
	if (err) { LogMsg("ERROR: tlsSetupSock: SSLSetConnection failed with error code: %d", err); return(err); }

	return(err);
	}
#endif /* NO_SECURITYFRAMEWORK */

mDNSlocal void tcpKQSocketCallback(__unused int fd, short filter, void *context)
	{
	TCPSocket *sock = context;
	mStatus err = mStatus_NoError;

	//if (filter == EVFILT_READ ) LogMsg("myKQSocketCallBack: tcpKQSocketCallback %d is EVFILT_READ", filter);
	//if (filter == EVFILT_WRITE) LogMsg("myKQSocketCallBack: tcpKQSocketCallback %d is EVFILT_WRITE", filter);
	// EV_ONESHOT doesn't seem to work, so we add the filter with EV_ADD, and explicitly delete it here with EV_DELETE
	if (filter == EVFILT_WRITE) KQueueSet(sock->fd, EV_DELETE, EVFILT_WRITE, &sock->kqEntry);

	if (sock->flags & kTCPSocketFlags_UseTLS)
		{
#ifndef NO_SECURITYFRAMEWORK
		if (!sock->setup) { sock->setup = mDNStrue; tlsSetupSock(sock, mDNSfalse); }
		if (!sock->handshakecomplete)
			{
			//LogMsg("tcpKQSocketCallback Starting SSLHandshake");
			err = SSLHandshake(sock->tlsContext);
			//if (!err) LogMsg("tcpKQSocketCallback SSLHandshake complete");
			if (!err) sock->handshakecomplete = mDNStrue;
			else if (err == errSSLWouldBlock) return;
			else { LogMsg("KQ SSLHandshake failed: %d", err); SSLDisposeContext(sock->tlsContext); sock->tlsContext = NULL; }
			}
#else
		err = mStatus_UnsupportedErr;
#endif /* NO_SECURITYFRAMEWORK */
		}

	mDNSBool connect = !sock->connected;
	sock->connected = mDNStrue;
	sock->callback(sock, sock->context, connect, err);
	// NOTE: the callback may call CloseConnection here, which frees the context structure!
	}

mDNSexport int KQueueSet(int fd, u_short flags, short filter, const KQueueEntry *const entryRef)
	{
	struct kevent new_event;
	EV_SET(&new_event, fd, filter, flags, 0, 0, (void*)entryRef);
	return (kevent(KQueueFD, &new_event, 1, NULL, 0, NULL) < 0) ? errno : 0;
	}

mDNSexport void KQueueLock(mDNS *const m)
	{
	pthread_mutex_lock(&m->p->BigMutex);
	m->p->BigMutexStartTime = mDNSPlatformRawTime();
	}

mDNSexport void KQueueUnlock(mDNS *const m, const char const *task)
	{
	mDNSs32 end = mDNSPlatformRawTime();
	if (end - m->p->BigMutexStartTime >= WatchDogReportingThreshold)
		LogMsg("WARNING: %s took %dms to complete", task, end - m->p->BigMutexStartTime);

	pthread_mutex_unlock(&m->p->BigMutex);

	char wake = 1;
	if (send(m->p->WakeKQueueLoopFD, &wake, sizeof(wake), 0) == -1)
		LogMsg("ERROR: KQueueWake: send failed with error code: %d - %s", errno, strerror(errno));
	}

mDNSexport TCPSocket *mDNSPlatformTCPSocket(mDNS *const m, TCPSocketFlags flags, mDNSIPPort *port)
	{
	(void) m;

	TCPSocket *sock = mallocL("TCPSocket/mDNSPlatformTCPSocket", sizeof(TCPSocket));
	if (!sock) { LogMsg("mDNSPlatformTCPSocket: memory allocation failure"); return(mDNSNULL); }

	mDNSPlatformMemZero(sock, sizeof(TCPSocket));
	sock->callback          = mDNSNULL;
	sock->fd                = socket(AF_INET, SOCK_STREAM, 0);
	sock->kqEntry.KQcallback= tcpKQSocketCallback;
	sock->kqEntry.KQcontext = sock;
	sock->kqEntry.KQtask    = "mDNSPlatformTCPSocket";
	sock->flags             = flags;
	sock->context           = mDNSNULL;
	sock->setup             = mDNSfalse;
	sock->handshakecomplete = mDNSfalse;
	sock->connected         = mDNSfalse;
	
	if (sock->fd == -1)
		{
		LogMsg("mDNSPlatformTCPSocket: socket error %d errno %d (%s)", sock->fd, errno, strerror(errno));
		freeL("TCPSocket/mDNSPlatformTCPSocket", sock);
		return(mDNSNULL);
		}

	// Bind it
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = port->NotAnInteger;
	if (bind(sock->fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
		{ LogMsg("ERROR: bind %s", strerror(errno)); goto error; }

	// Receive interface identifiers
	const int on = 1;  // "on" for setsockopt
	if (setsockopt(sock->fd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on)) < 0)
		{ LogMsg("setsockopt IP_RECVIF - %s", strerror(errno)); goto error; }

	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	if (getsockname(sock->fd, (struct sockaddr*) &addr, &len) < 0)
		{ LogMsg("getsockname - %s", strerror(errno)); goto error; }

	port->NotAnInteger = addr.sin_port;
	return sock;

error:
	close(sock->fd);
	freeL("TCPSocket/mDNSPlatformTCPSocket", sock);
	return(mDNSNULL);
	}

mDNSexport mStatus mDNSPlatformTCPConnect(TCPSocket *sock, const mDNSAddr *dst, mDNSOpaque16 dstport, mDNSInterfaceID InterfaceID,
                                          TCPConnectionCallback callback, void *context)
	{
	struct sockaddr_in saddr;
	mStatus err = mStatus_NoError;

	sock->callback          = callback;
	sock->context           = context;
	sock->setup             = mDNSfalse;
	sock->handshakecomplete = mDNSfalse;
	sock->connected         = mDNSfalse;

	(void) InterfaceID;	//!!!KRS use this if non-zero!!!

	if (dst->type != mDNSAddrType_IPv4)
		{
		LogMsg("ERROR: mDNSPlatformTCPConnect - attempt to connect to an IPv6 address: opperation not supported");
		return mStatus_UnknownErr;
		}

	mDNSPlatformMemZero(&saddr, sizeof(saddr));
	saddr.sin_family      = AF_INET;
	saddr.sin_port        = dstport.NotAnInteger;
	saddr.sin_len         = sizeof(saddr);
	saddr.sin_addr.s_addr = dst->ip.v4.NotAnInteger;

	// Don't send if it would cause dial-on-demand connection initiation.
	if (AddrRequiresPPPConnection((struct sockaddr *)&saddr))
		{
		debugf("mDNSPlatformTCPConnect: Surpressing sending to avoid dial-on-demand connection");
		return mStatus_UnknownErr;
		}

	sock->kqEntry.KQcallback = tcpKQSocketCallback;
	sock->kqEntry.KQcontext  = sock;
	sock->kqEntry.KQtask     = "Outgoing TCP";

	// Watch for connect complete (write is ready)
	// EV_ONESHOT doesn't seem to work, so we add the filter with EV_ADD, and explicitly delete it in tcpKQSocketCallback using EV_DELETE
	if (KQueueSet(sock->fd, EV_ADD /* | EV_ONESHOT */, EVFILT_WRITE, &sock->kqEntry))
		{
		LogMsg("ERROR: mDNSPlatformTCPConnect - KQueueSet failed");
		close(sock->fd);
		return errno;
		}

	// Watch for incoming data
	if (KQueueSet(sock->fd, EV_ADD, EVFILT_READ, &sock->kqEntry))
		{
		LogMsg("ERROR: mDNSPlatformTCPConnect - KQueueSet failed");
		close(sock->fd); // Closing the descriptor removes all filters from the kqueue
		return errno;
 		}

	if (fcntl(sock->fd, F_SETFL, fcntl(sock->fd, F_GETFL, 0) | O_NONBLOCK) < 0) // set non-blocking
		{
		LogMsg("ERROR: setsockopt O_NONBLOCK - %s", strerror(errno));
		return mStatus_UnknownErr;
		}

	// initiate connection wth peer
	if (connect(sock->fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
		{
		if (errno == EINPROGRESS) return mStatus_ConnPending;
		LogMsg("ERROR: mDNSPlatformTCPConnect - connect failed: socket %d: Error %d %s", sock->fd, errno, strerror(errno));
		close(sock->fd);
		return mStatus_ConnFailed;
		}

	LogMsg("NOTE: mDNSPlatformTCPConnect completed synchronously");
	// kQueue should notify us, but this LogMsg is to help track down if it doesn't
	return err;
	}

mDNSexport mDNSBool mDNSPlatformTCPIsConnected(TCPSocket *sock)
	{
	return sock->connected;
	}

// Why doesn't mDNSPlatformTCPAccept actually call accept() ?
mDNSexport TCPSocket *mDNSPlatformTCPAccept(TCPSocketFlags flags, int fd)
	{
	mStatus err = mStatus_NoError;

	TCPSocket *sock = mallocL("TCPSocket/mDNSPlatformTCPAccept", sizeof(TCPSocket));
	if (!sock) return(mDNSNULL);

	memset(sock, 0, sizeof(*sock));
	sock->fd = fd;
	sock->flags = flags;

	if (flags & kTCPSocketFlags_UseTLS)
		{
#ifndef NO_SECURITYFRAMEWORK
		if (!ServerCerts) { LogMsg("ERROR: mDNSPlatformTCPAccept: unable to find TLS certificates"); err = mStatus_UnknownErr; goto exit; }

		err = tlsSetupSock(sock, mDNStrue);
		if (err) { LogMsg("ERROR: mDNSPlatformTCPAccept: tlsSetupSock failed with error code: %d", err); goto exit; }

		err = SSLSetCertificate(sock->tlsContext, ServerCerts);
		if (err) { LogMsg("ERROR: mDNSPlatformTCPAccept: SSLSetCertificate failed with error code: %d", err); goto exit; }
#else
		err = mStatus_UnsupportedErr;
#endif /* NO_SECURITYFRAMEWORK */
		}
#ifndef NO_SECURITYFRAMEWORK
exit:
#endif

	if (err) { freeL("TCPSocket/mDNSPlatformTCPAccept", sock); return(mDNSNULL); }
	return(sock);
	}

mDNSexport void mDNSPlatformTCPCloseConnection(TCPSocket *sock)
	{
	if (sock)
		{
#ifndef NO_SECURITYFRAMEWORK
		if (sock->tlsContext)
			{
			SSLClose(sock->tlsContext);
			SSLDisposeContext(sock->tlsContext);
			sock->tlsContext = NULL;
			}
#endif /* NO_SECURITYFRAMEWORK */
		if (sock->fd != -1)
			{
			shutdown(sock->fd, 2);
			close(sock->fd);
			sock->fd = -1;
			}

		freeL("mDNSPlatformTCPCloseConnection", sock);
		}
	}

mDNSexport long mDNSPlatformReadTCP(TCPSocket *sock, void *buf, unsigned long buflen, mDNSBool *closed)
	{
	long nread = 0;
	*closed = mDNSfalse;

	if (sock->flags & kTCPSocketFlags_UseTLS)
		{
#ifndef NO_SECURITYFRAMEWORK
		if (!sock->handshakecomplete)
			{
			//LogMsg("mDNSPlatformReadTCP Starting SSLHandshake");
			mStatus err = SSLHandshake(sock->tlsContext);
			//if (!err) LogMsg("mDNSPlatformReadTCP SSLHandshake complete");
			if (!err) sock->handshakecomplete = mDNStrue;
			else if (err == errSSLWouldBlock) return(0);
			else { LogMsg("Read SSLHandshake failed: %d", err); SSLDisposeContext(sock->tlsContext); sock->tlsContext = NULL; }
			}

		//LogMsg("Starting SSLRead %d %X", sock->fd, fcntl(sock->fd, F_GETFL, 0));
		mStatus err = SSLRead(sock->tlsContext, buf, buflen, (size_t*)&nread);
		//LogMsg("SSLRead returned %d (%d) nread %d buflen %d", err, errSSLWouldBlock, nread, buflen);
		if (err == errSSLClosedGraceful) { nread = 0; *closed = mDNStrue; }
		else if (err && err != errSSLWouldBlock)
			{ LogMsg("ERROR: mDNSPlatformReadTCP - SSLRead: %d", err); nread = -1; *closed = mDNStrue; }
#else
		nread = -1;
		*closed = mDNStrue;
#endif /* NO_SECURITYFRAMEWORK */
		}
	else
		{
		static int CLOSEDcount = 0;
		static int EAGAINcount = 0;
		nread = recv(sock->fd, buf, buflen, 0);

		if (nread > 0) { CLOSEDcount = 0; EAGAINcount = 0; } // On success, clear our error counters
		else if (nread == 0)
			{
			*closed = mDNStrue;
			if ((++CLOSEDcount % 1000) == 0) { LogMsg("ERROR: mDNSPlatformReadTCP - recv %d got CLOSED %d times", sock->fd, CLOSEDcount); sleep(1); }
			}
		// else nread is negative -- see what kind of error we got
		else if (errno == ECONNRESET) { nread = 0; *closed = mDNStrue; }
		else if (errno != EAGAIN) { LogMsg("ERROR: mDNSPlatformReadTCP - recv: %d %s", errno, strerror(errno)); nread = -1; }
		else // errno is EAGAIN (EWOULDBLOCK) -- no data available
			{
			nread = 0;
			if ((++EAGAINcount % 1000) == 0) { LogMsg("ERROR: mDNSPlatformReadTCP - recv %d got EAGAIN %d times", sock->fd, EAGAINcount); sleep(1); }
			}
		}

	return nread;
	}

mDNSexport long mDNSPlatformWriteTCP(TCPSocket *sock, const char *msg, unsigned long len)
	{
	int nsent;

	if (sock->flags & kTCPSocketFlags_UseTLS)
		{
#ifndef NO_SECURITYFRAMEWORK
		size_t	processed;
		mStatus	err = SSLWrite(sock->tlsContext, msg, len, &processed);

		if (!err) nsent = (int) processed;
		else if (err == errSSLWouldBlock) nsent = 0;
		else { LogMsg("ERROR: mDNSPlatformWriteTCP - SSLWrite returned %d", err); nsent = -1; }
#else
		nsent = -1;
#endif /* NO_SECURITYFRAMEWORK */
		}
	else
		{
		nsent = send(sock->fd, msg, len, 0);
		if (nsent < 0)
			{
			if (errno == EAGAIN) nsent = 0;
			else { LogMsg("ERROR: mDNSPlatformWriteTCP - send %s", strerror(errno)); nsent = -1; }
			}
		}

	return nsent;
	}

mDNSexport int mDNSPlatformTCPGetFD(TCPSocket *sock)
	{
	return sock->fd;
	}

// If mDNSIPPort port is non-zero, then it's a multicast socket on the specified interface
// If mDNSIPPort port is zero, then it's a randomly assigned port number, used for sending unicast queries
mDNSlocal mStatus SetupSocket(KQSocketSet *cp, const mDNSIPPort port, u_short sa_family)
	{
	const int ip_tosbits = IPTOS_LOWDELAY | IPTOS_THROUGHPUT;
	int         *s        = (sa_family == AF_INET) ? &cp->sktv4 : &cp->sktv6;
	KQueueEntry	*k        = (sa_family == AF_INET) ? &cp->kqsv4 : &cp->kqsv6;
	const int on = 1;
	const int twofivefive = 255;
	mStatus err = mStatus_NoError;
	char *errstr = mDNSNULL;

	int skt = socket(sa_family, SOCK_DGRAM, IPPROTO_UDP);
	if (skt < 3) { LogMsg("SetupSocket: socket error %d errno %d (%s)", skt, errno, strerror(errno)); return(skt); }

	// ... with a shared UDP port, if it's for multicast receiving
	if (mDNSSameIPPort(port, MulticastDNSPort)) err = setsockopt(skt, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	if (err < 0) { errstr = "setsockopt - SO_REUSEPORT"; goto fail; }

	if (sa_family == AF_INET)
		{
		// We want to receive destination addresses
		err = setsockopt(skt, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on));
		if (err < 0) { errstr = "setsockopt - IP_RECVDSTADDR"; goto fail; }

		// We want to receive interface identifiers
		err = setsockopt(skt, IPPROTO_IP, IP_RECVIF, &on, sizeof(on));
		if (err < 0) { errstr = "setsockopt - IP_RECVIF"; goto fail; }

		// We want to receive packet TTL value so we can check it
		err = setsockopt(skt, IPPROTO_IP, IP_RECVTTL, &on, sizeof(on));
		// We ignore errors here -- we already know Jaguar doesn't support this, but we can get by without it

		// Send unicast packets with TTL 255
		err = setsockopt(skt, IPPROTO_IP, IP_TTL, &twofivefive, sizeof(twofivefive));
		if (err < 0) { errstr = "setsockopt - IP_TTL"; goto fail; }

		// And multicast packets with TTL 255 too
		err = setsockopt(skt, IPPROTO_IP, IP_MULTICAST_TTL, &twofivefive, sizeof(twofivefive));
		if (err < 0) { errstr = "setsockopt - IP_MULTICAST_TTL"; goto fail; }

		// Mark packets as high-throughput/low-delay (i.e. lowest reliability) to get maximum 802.11 multicast rate
		err = setsockopt(skt, IPPROTO_IP, IP_TOS, &ip_tosbits, sizeof(ip_tosbits));
		if (err < 0) { errstr = "setsockopt - IP_TOS"; goto fail; }

		// And start listening for packets
		struct sockaddr_in listening_sockaddr;
		listening_sockaddr.sin_family      = AF_INET;
		listening_sockaddr.sin_port        = port.NotAnInteger;
		listening_sockaddr.sin_addr.s_addr = 0; // Want to receive multicasts AND unicasts on this socket
		err = bind(skt, (struct sockaddr *) &listening_sockaddr, sizeof(listening_sockaddr));
		if (err) { errstr = "bind"; goto fail; }
		}
	else if (sa_family == AF_INET6)
		{
		// We want to receive destination addresses and receive interface identifiers
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_PKTINFO, &on, sizeof(on));
		if (err < 0) { errstr = "setsockopt - IPV6_PKTINFO"; goto fail; }

		// We want to receive packet hop count value so we can check it
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_HOPLIMIT, &on, sizeof(on));
		if (err < 0) { errstr = "setsockopt - IPV6_HOPLIMIT"; goto fail; }

		// We want to receive only IPv6 packets. Without this option we get IPv4 packets too,
		// with mapped addresses of the form 0:0:0:0:0:FFFF:xxxx:xxxx, where xxxx:xxxx is the IPv4 address
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on));
		if (err < 0) { errstr = "setsockopt - IPV6_V6ONLY"; goto fail; }

		// Send unicast packets with TTL 255
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &twofivefive, sizeof(twofivefive));
		if (err < 0) { errstr = "setsockopt - IPV6_UNICAST_HOPS"; goto fail; }

		// And multicast packets with TTL 255 too
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &twofivefive, sizeof(twofivefive));
		if (err < 0) { errstr = "setsockopt - IPV6_MULTICAST_HOPS"; goto fail; }

		// Note: IPV6_TCLASS appears not to be implemented on OS X right now (or indeed on ANY version of Unix?)
		#ifdef IPV6_TCLASS
		// Mark packets as high-throughput/low-delay (i.e. lowest reliability) to get maximum 802.11 multicast rate
		int tclass = IPTOS_LOWDELAY | IPTOS_THROUGHPUT; // This may not be right (since tclass is not implemented on OS X, I can't test it)
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_TCLASS, &tclass, sizeof(tclass));
		if (err < 0) { errstr = "setsockopt - IPV6_TCLASS"; goto fail; }
		#endif

		// Want to receive our own packets
		err = setsockopt(skt, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &on, sizeof(on));
		if (err < 0) { errstr = "setsockopt - IPV6_MULTICAST_LOOP"; goto fail; }

		// And start listening for packets
		struct sockaddr_in6 listening_sockaddr6;
		mDNSPlatformMemZero(&listening_sockaddr6, sizeof(listening_sockaddr6));
		listening_sockaddr6.sin6_len         = sizeof(listening_sockaddr6);
		listening_sockaddr6.sin6_family      = AF_INET6;
		listening_sockaddr6.sin6_port        = port.NotAnInteger;
		listening_sockaddr6.sin6_flowinfo    = 0;
		listening_sockaddr6.sin6_addr        = in6addr_any; // Want to receive multicasts AND unicasts on this socket
		listening_sockaddr6.sin6_scope_id    = 0;
		err = bind(skt, (struct sockaddr *) &listening_sockaddr6, sizeof(listening_sockaddr6));
		if (err) { errstr = "bind"; goto fail; }
		}

	fcntl(skt, F_SETFL, fcntl(skt, F_GETFL, 0) | O_NONBLOCK); // set non-blocking
	fcntl(skt, F_SETFD, 1); // set close-on-exec
	*s = skt;
	k->KQcallback = myKQSocketCallBack;
	k->KQcontext  = cp;
	k->KQtask     = "UDP packet reception";
	KQueueSet(*s, EV_ADD, EVFILT_READ, k);

	return(err);

	fail:
	// For "bind" failures, only write log messages for our shared mDNS port, or for binding to zero
	if (strcmp(errstr, "bind") || mDNSSameIPPort(port, MulticastDNSPort) || mDNSIPPortIsZero(port))
		LogMsg("%s error %ld errno %d (%s)", errstr, err, errno, strerror(errno));

	// If we got a "bind" failure with an EADDRINUSE error for our shared mDNS port, display error alert
	if (!strcmp(errstr, "bind") && mDNSSameIPPort(port, MulticastDNSPort) && errno == EADDRINUSE)
		NotifyOfElusiveBug("Setsockopt SO_REUSEPORT failed",
			"Congratulations, you've reproduced an elusive bug.\r"
			"Please contact the current assignee of <rdar://problem/3814904>.\r"
			"Alternatively, you can send email to radar-3387020@group.apple.com. (Note number is different.)\r"
			"If possible, please leave your machine undisturbed so that someone can come to investigate the problem.");

	close(skt);
	return(err);
	}

struct UDPSocket_struct
	{
	KQSocketSet ss;
	};

mDNSexport UDPSocket *mDNSPlatformUDPSocket(mDNS *const m, const mDNSIPPort port)
	{
	mStatus err;
	UDPSocket *p = mallocL("UDPSocket", sizeof(UDPSocket));
	if (!p) { LogMsg("mDNSPlatformUDPSocket: memory exhausted"); return(mDNSNULL); }
	memset(p, 0, sizeof(UDPSocket));
	p->ss.m     = m;
	p->ss.sktv4 = -1;
	p->ss.sktv6 = -1;
	err = SetupSocket(&p->ss, port, AF_INET);
	if (err) { LogMsg("mDNSPlatformUDPSocket: SetupSocket %d failed", mDNSVal16(port)); freeL("UDPSocket", p); return(mDNSNULL); }
	return(p);
	}

mDNSlocal void CloseSocketSet(KQSocketSet *ss)
	{
	if (ss->sktv4 != -1)
		{
		close(ss->sktv4);
		ss->sktv4 = -1;
		}
	if (ss->sktv6 != -1)
		{
		close(ss->sktv6);
		ss->sktv6 = -1;
		}
	}

mDNSexport void mDNSPlatformUDPClose(UDPSocket *sock)
	{
	CloseSocketSet(&sock->ss);
	freeL("UDPSocket", sock);
	}

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - Key Management
#endif

#ifndef NO_SECURITYFRAMEWORK
mDNSlocal CFArrayRef GetCertChain(SecIdentityRef identity)
	{
	CFMutableArrayRef certChain = NULL;
	if (!identity) { LogMsg("getCertChain: identity is NULL"); return(NULL); }
	SecCertificateRef cert;
	OSStatus err = SecIdentityCopyCertificate(identity, &cert);
	if (err || !cert) LogMsg("getCertChain: SecIdentityCopyCertificate() returned %d", (int) err);
	else
		{
		SecPolicySearchRef searchRef;
		err = SecPolicySearchCreate(CSSM_CERT_X_509v3, &CSSMOID_APPLE_X509_BASIC, NULL, &searchRef);
		if (err || !searchRef) LogMsg("getCertChain: SecPolicySearchCreate() returned %d", (int) err);
		else
			{
			SecPolicyRef policy;
			err = SecPolicySearchCopyNext(searchRef, &policy);
			if (err || !policy) LogMsg("getCertChain: SecPolicySearchCopyNext() returned %d", (int) err);
			else
				{
				CFArrayRef wrappedCert = CFArrayCreate(NULL, (const void**) &cert, 1, &kCFTypeArrayCallBacks);
				if (!wrappedCert) LogMsg("getCertChain: wrappedCert is NULL");
				else
					{
					SecTrustRef trust;
					err = SecTrustCreateWithCertificates(wrappedCert, policy, &trust);
					if (err || !trust) LogMsg("getCertChain: SecTrustCreateWithCertificates() returned %d", (int) err);
					else
						{
						err = SecTrustEvaluate(trust, NULL);
						if (err) LogMsg("getCertChain: SecTrustEvaluate() returned %d", (int) err);
						else
							{
							CFArrayRef rawCertChain;
							CSSM_TP_APPLE_EVIDENCE_INFO *statusChain = NULL;
							err = SecTrustGetResult(trust, NULL, &rawCertChain, &statusChain);
							if (err || !rawCertChain || !statusChain) LogMsg("getCertChain: SecTrustGetResult() returned %d", (int) err);
							else
								{
								certChain = CFArrayCreateMutableCopy(NULL, 0, rawCertChain);
								if (!certChain) LogMsg("getCertChain: certChain is NULL");
								else
									{
									// Replace the SecCertificateRef at certChain[0] with a SecIdentityRef per documentation for SSLSetCertificate:
									// <http://devworld.apple.com/documentation/Security/Reference/secureTransportRef/index.html>
									CFArraySetValueAtIndex(certChain, 0, identity);
									// Remove root from cert chain, but keep any and all intermediate certificates that have been signed by the root certificate
									if (CFArrayGetCount(certChain) > 1) CFArrayRemoveValueAtIndex(certChain, CFArrayGetCount(certChain) - 1);
									}
								CFRelease(rawCertChain);
								// Do not free statusChain:
								// <http://developer.apple.com/documentation/Security/Reference/certifkeytrustservices/Reference/reference.html> says:
								// certChain: Call the CFRelease function to release this object when you are finished with it.
								// statusChain: Do not attempt to free this pointer; it remains valid until the trust management object is released...
								}
							}
						CFRelease(trust);
						}
					CFRelease(wrappedCert);
					}
				CFRelease(policy);
				}
			CFRelease(searchRef);
			}
		CFRelease(cert);
		}
	return certChain;
	}
#endif /* NO_SECURITYFRAMEWORK */

mDNSexport mStatus mDNSPlatformTLSSetupCerts(void)
	{
#ifdef NO_SECURITYFRAMEWORK
	return mStatus_UnsupportedErr;
#else
	SecIdentityRef			identity = nil;
	SecIdentitySearchRef	srchRef = nil;
	OSStatus				err;

	// Pick a keychain
	err = SecKeychainCopyDefault(&ServerKC);
	if (err) { LogMsg("ERROR: mDNSPlatformTLSSetupCerts: SecKeychainCopyDefault returned %d", (int) err); return err; }

	// search for "any" identity matching specified key use
	// In this app, we expect there to be exactly one
	err = SecIdentitySearchCreate(ServerKC, CSSM_KEYUSE_DECRYPT, &srchRef);
	if (err) { LogMsg("ERROR: mDNSPlatformTLSSetupCerts: SecIdentitySearchCreate returned %d", (int) err); return err; }

	err = SecIdentitySearchCopyNext(srchRef, &identity);
	if (err) { LogMsg("ERROR: mDNSPlatformTLSSetupCerts: SecIdentitySearchCopyNext returned %d", (int) err); return err; }

	if (CFGetTypeID(identity) != SecIdentityGetTypeID())
		{ LogMsg("ERROR: mDNSPlatformTLSSetupCerts: SecIdentitySearchCopyNext CFTypeID failure"); return mStatus_UnknownErr; }

	// Found one. Call getCertChain to create the correct certificate chain.
	ServerCerts = GetCertChain(identity);
	if (ServerCerts == nil) { LogMsg("ERROR: mDNSPlatformTLSSetupCerts: getCertChain error"); return mStatus_UnknownErr; }

	return mStatus_NoError;
#endif /* NO_SECURITYFRAMEWORK */
	}

mDNSexport  void  mDNSPlatformTLSTearDownCerts(void)
	{
#ifndef NO_SECURITYFRAMEWORK
	if (ServerCerts) { CFRelease(ServerCerts); ServerCerts = NULL; }
	if (ServerKC)    { CFRelease(ServerKC);    ServerKC    = NULL; }
#endif /* NO_SECURITYFRAMEWORK */
	}

// This gets the text of the field currently labelled "Computer Name" in the Sharing Prefs Control Panel
mDNSlocal void GetUserSpecifiedFriendlyComputerName(domainlabel *const namelabel)
	{
	CFStringEncoding encoding = kCFStringEncodingUTF8;
	CFStringRef cfs = SCDynamicStoreCopyComputerName(NULL, &encoding);
	if (cfs)
		{
		CFStringGetPascalString(cfs, namelabel->c, sizeof(*namelabel), kCFStringEncodingUTF8);
		CFRelease(cfs);
		}
	}

// This gets the text of the field currently labelled "Local Hostname" in the Sharing Prefs Control Panel
mDNSlocal void GetUserSpecifiedLocalHostName(domainlabel *const namelabel)
	{
	CFStringRef cfs = SCDynamicStoreCopyLocalHostName(NULL);
	if (cfs)
		{
		CFStringGetPascalString(cfs, namelabel->c, sizeof(*namelabel), kCFStringEncodingUTF8);
		CFRelease(cfs);
		}
	}

mDNSlocal mDNSBool DDNSSettingEnabled(CFDictionaryRef dict)
	{
	mDNSs32 val;
	CFNumberRef state = CFDictionaryGetValue(dict, CFSTR("Enabled"));
	if (!state) return mDNSfalse;
	if (!CFNumberGetValue(state, kCFNumberSInt32Type, &val)) { LogMsg("ERROR: DDNSSettingEnabled - CFNumberGetValue"); return mDNSfalse; }
	return val ? mDNStrue : mDNSfalse;
	}

mDNSlocal mStatus SetupAddr(mDNSAddr *ip, const struct sockaddr *const sa)
	{
	if (!sa) { LogMsg("SetupAddr ERROR: NULL sockaddr"); return(mStatus_Invalid); }

	if (sa->sa_family == AF_INET)
		{
		struct sockaddr_in *ifa_addr = (struct sockaddr_in *)sa;
		ip->type = mDNSAddrType_IPv4;
		ip->ip.v4.NotAnInteger = ifa_addr->sin_addr.s_addr;
		return(mStatus_NoError);
		}

	if (sa->sa_family == AF_INET6)
		{
		struct sockaddr_in6 *ifa_addr = (struct sockaddr_in6 *)sa;
		// Inside the BSD kernel they use a hack where they stuff the sin6->sin6_scope_id
		// value into the second word of the IPv6 link-local address, so they can just
		// pass around IPv6 address structures instead of full sockaddr_in6 structures.
		// Those hacked IPv6 addresses aren't supposed to escape the kernel in that form, but they do.
		// To work around this we always whack the second word of any IPv6 link-local address back to zero.
		if (IN6_IS_ADDR_LINKLOCAL(&ifa_addr->sin6_addr)) ifa_addr->sin6_addr.__u6_addr.__u6_addr16[1] = 0;
		ip->type = mDNSAddrType_IPv6;
		ip->ip.v6 = *(mDNSv6Addr*)&ifa_addr->sin6_addr;
		return(mStatus_NoError);
		}

	LogMsg("SetupAddr invalid sa_family %d", sa->sa_family);
	return(mStatus_Invalid);
	}

mDNSlocal mDNSEthAddr GetBSSID(char *ifa_name)
	{
	mDNSEthAddr eth = zeroEthAddr;
	SCDynamicStoreRef store = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:GetBSSID"), NULL, NULL);
	if (store)
		{
		CFStringRef entityname = CFStringCreateWithFormat(NULL, NULL, CFSTR("State:/Network/Interface/%s/AirPort"), ifa_name);
		if (entityname)
			{
			CFDictionaryRef dict = SCDynamicStoreCopyValue(store, entityname);
			if (dict)
				{
				CFRange range = { 0, 6 };		// Offset, length
				CFDataRef data = CFDictionaryGetValue(dict, CFSTR("BSSID"));
				if (data && CFDataGetLength(data) == 6) CFDataGetBytes(data, range, eth.b);
				CFRelease(dict);
				}
			CFRelease(entityname);
			}
		CFRelease(store);
		}
	return(eth);
	}

// Returns pointer to newly created NetworkInterfaceInfoOSX object, or
// pointer to already-existing NetworkInterfaceInfoOSX object found in list, or
// may return NULL if out of memory (unlikely) or parameters are invalid for some reason
// (e.g. sa_family not AF_INET or AF_INET6)
mDNSlocal NetworkInterfaceInfoOSX *AddInterfaceToList(mDNS *const m, struct ifaddrs *ifa, mDNSs32 utc)
	{
	mDNSu32 scope_id  = if_nametoindex(ifa->ifa_name);
	mDNSEthAddr bssid = GetBSSID(ifa->ifa_name);

	mDNSAddr ip, mask;
	if (SetupAddr(&ip,   ifa->ifa_addr   ) != mStatus_NoError) return(NULL);
	if (SetupAddr(&mask, ifa->ifa_netmask) != mStatus_NoError) return(NULL);

	NetworkInterfaceInfoOSX **p;
	for (p = &m->p->InterfaceList; *p; p = &(*p)->next)
		if (scope_id == (*p)->scope_id && mDNSSameAddress(&ip, &(*p)->ifinfo.ip) && mDNSSameEthAddress(&bssid, &(*p)->BSSID))
			{
			debugf("AddInterfaceToList: Found existing interface %lu %.6a with address %#a at %p", scope_id, &bssid, &ip, *p);
			(*p)->Exists = mDNStrue;
			// If interface was not in getifaddrs list last time we looked, but it is now, update 'AppearanceTime' for this record
			if ((*p)->LastSeen != utc) (*p)->AppearanceTime = utc;
			return(*p);
			}

	NetworkInterfaceInfoOSX *i = (NetworkInterfaceInfoOSX *)mallocL("NetworkInterfaceInfoOSX", sizeof(*i));
	debugf("AddInterfaceToList: Making   new   interface %lu %.6a with address %#a at %p", scope_id, &bssid, &ip, i);
	if (!i) return(mDNSNULL);
	mDNSPlatformMemZero(i, sizeof(NetworkInterfaceInfoOSX));
	i->ifa_name        = (char *)mallocL("NetworkInterfaceInfoOSX name", strlen(ifa->ifa_name) + 1);
	if (!i->ifa_name) { freeL("NetworkInterfaceInfoOSX", i); return(mDNSNULL); }
	strcpy(i->ifa_name, ifa->ifa_name);		// This is safe because we know we allocated i->ifa_name with sufficient space

	i->ifinfo.InterfaceID = mDNSNULL;
	i->ifinfo.ip          = ip;
	i->ifinfo.mask        = mask;
	strlcpy(i->ifinfo.ifname, ifa->ifa_name, sizeof(i->ifinfo.ifname));
	i->ifinfo.ifname[sizeof(i->ifinfo.ifname)-1] = 0;
	i->ifinfo.Advertise   = m->AdvertiseLocalAddresses;
	i->ifinfo.McastTxRx   = mDNSfalse; // For now; will be set up later at the end of UpdateInterfaceList

	i->next            = mDNSNULL;
	i->Exists          = mDNStrue;
	i->AppearanceTime  = utc;		// Brand new interface; AppearanceTime is now
	i->LastSeen        = utc;
	i->Flashing        = mDNSfalse;
	i->Occulting       = mDNSfalse;
	i->scope_id        = scope_id;
	i->BSSID           = bssid;
	i->sa_family       = ifa->ifa_addr->sa_family;
	i->ifa_flags       = ifa->ifa_flags;

	*p = i;
	return(i);
	}

#if USE_V6_ONLY_WHEN_NO_ROUTABLE_V4
mDNSlocal NetworkInterfaceInfoOSX *FindRoutableIPv4(mDNS *const m, mDNSu32 scope_id)
	{
	NetworkInterfaceInfoOSX *i;
	for (i = m->p->InterfaceList; i; i = i->next)
		if (i->Exists && i->scope_id == scope_id && i->ifinfo.ip.type == mDNSAddrType_IPv4)
			if (!mDNSv4AddressIsLinkLocal(&i->ifinfo.ip.ip.v4))
				return(i);
	return(mDNSNULL);
	}
#endif

#if APPLE_OSX_mDNSResponder

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - AutoTunnel
#endif

mDNSlocal void RestartRacoon(void)
	{
	mDNSBool startRacoon = true;
	
	FILE *fp = fopen("/var/run/racoon.pid", "r");
	if (fp)
		{
		int pid=0;
		if (fscanf(fp, "%d", &pid) && kill(pid,SIGHUP) == 0)
			{
			startRacoon = false;
			LogMsg("Sent SIGHUP to racoon (%d)", pid);
			}
		fclose(fp);
		}

	if (startRacoon)
		{
		pid_t pid = fork();
		if (!pid)
			{
			const char *racoon = "/usr/sbin/racoon";
			execl(racoon, racoon, (const char*) 0);
			}
		else
			{
			int stat=0;
			if (waitpid(pid, &stat, 0) != pid) LogMsg("Failed to start racoon (waitpid)");
			else if (!WIFEXITED(stat))         LogMsg("Failed to start racoon (!exited)");
			else if (WEXITSTATUS(stat))        LogMsg("Failed to start racoon (exit: %d)", WEXITSTATUS(stat));
			else                               LogMsg("Racoon started and listening on port %d", 500); // TEMP FOR AUTOTUNNEL TESTING: assume port 500
			}
		}
	}

mDNSlocal mDNSBool TunnelServers(mDNS *const m)
	{
	ServiceRecordSet *p;
	for (p = m->ServiceRegistrations; p; p = p->uDNS_next)
		{
		DomainAuthInfo *AuthInfo = GetAuthInfoForName(m, p->RR_SRV.resrec.name);
		if (AuthInfo && AuthInfo->AutoTunnel) return(mDNStrue);
		}
	return(mDNSfalse);
	}

#define kTunnelAddressInterface "lo0"

mDNSlocal void AliasTunnelAddress(mDNSv6Addr *const addr)
	{
	struct in6_aliasreq ifra_in6;
	int s = socket(AF_INET6, SOCK_DGRAM, 0);

	LogOperation("AliasTunnelAddress %.16a", addr);

	if (s < 0) { LogMsg("socket() failed trying to alias %.16a errno %d (%s)", addr, errno, strerror(errno)); return; }

	bzero(&ifra_in6, sizeof(ifra_in6));
	strncpy(ifra_in6.ifra_name, kTunnelAddressInterface, sizeof(ifra_in6.ifra_name));
	ifra_in6.ifra_lifetime.ia6t_vltime = ND6_INFINITE_LIFETIME;
	ifra_in6.ifra_lifetime.ia6t_pltime = ND6_INFINITE_LIFETIME;

	ifra_in6.ifra_addr.sin6_family = AF_INET6;
	ifra_in6.ifra_addr.sin6_len = sizeof(struct sockaddr_in6);
	memcpy(&(ifra_in6.ifra_addr.sin6_addr), addr, sizeof(mDNSv6Addr));

	ifra_in6.ifra_prefixmask.sin6_family = AF_INET6;
	ifra_in6.ifra_prefixmask.sin6_len = sizeof(struct sockaddr_in6);
	memset(&(ifra_in6.ifra_prefixmask.sin6_addr), 0xFF, sizeof(mDNSv6Addr));

	if (ioctl(s, SIOCAIFADDR_IN6, &ifra_in6) == -1) LogMsg("ioctl() failed to alias %.16a errno %d (%s)", addr, errno, strerror(errno));

	close(s);
	}

mDNSlocal void UnaliasTunnelAddress(mDNSv6Addr *const addr)
	{
	struct in6_ifreq ifr;
	int s = socket(AF_INET6, SOCK_DGRAM, 0);

	LogOperation("UnaliasTunnelAddress %.16a", addr);

	if (s < 0) { LogMsg("socket() failed trying to unalias %.16a errno %d (%s)", addr, errno, strerror(errno)); return; }

	bzero(&ifr, sizeof(ifr));
	strncpy(ifr.ifr_name, kTunnelAddressInterface, sizeof(ifr.ifr_name));
	ifr.ifr_ifru.ifru_addr.sin6_family = AF_INET6;
	ifr.ifr_ifru.ifru_addr.sin6_len = sizeof(struct sockaddr_in6);
	memcpy(&(ifr.ifr_ifru.ifru_addr.sin6_addr), addr, sizeof(mDNSv6Addr));

	if (ioctl(s, SIOCDIFADDR_IN6, &ifr) == -1) LogMsg("ioctl() failed to unalias %.16a errno %d (%s)", addr, errno, strerror(errno));

	close(s);
	}

// Before SetupLocalAutoTunnelInterface_internal is called,
// m->AutoTunnelHostAddr.b[0] must be non-zero, and there must be at least one TunnelClient or TunnelServer
// Must be called with the lock held
mDNSexport void SetupLocalAutoTunnelInterface_internal(mDNS *const m)
	{
	LogOperation("SetupLocalAutoTunnelInterface");

	// 1. Configure the local IPv6 address
	AliasTunnelAddress(&m->AutoTunnelHostAddr);

	// 2. If we have at least one server listening, publish our records
	if (TunnelServers(m))
		{
		DomainAuthInfo *info;
		for (info = m->AuthInfoList; info; info = info->next)
			{
			if (info->AutoTunnel && !info->AutoTunnelHostRecord.namestorage.c[0])
				{
				// 1. Set up our address record for the internal tunnel address
				// (User-visible user-friendly host name, used as target in AutoTunnel SRV records)
				mDNS_SetupResourceRecord(&info->AutoTunnelHostRecord, mDNSNULL, mDNSInterface_Any, kDNSType_AAAA, kHostNameTTL, kDNSRecordTypeKnownUnique, mDNSNULL, mDNSNULL);
				info->AutoTunnelHostRecord.namestorage.c[0] = 0;
				AppendDomainLabel(&info->AutoTunnelHostRecord.namestorage, &m->hostlabel);
				AppendDomainLabel(&info->AutoTunnelHostRecord.namestorage, (const domainlabel *)"\x04" "btmm");
				AppendDomainName (&info->AutoTunnelHostRecord.namestorage, &info->domain);
				info->AutoTunnelHostRecord.resrec.rdata->u.ipv6 = m->AutoTunnelHostAddr;
				mDNS_Register_internal(m, &info->AutoTunnelHostRecord);
		
				// 2. Set up our address record for the external tunnel address
				// (Constructed name, not generally user-visible, used as target in IKE tunnel's SRV record)
				mDNS_SetupResourceRecord(&info->AutoTunnelTarget, mDNSNULL, mDNSInterface_Any, kDNSType_A, kHostNameTTL, kDNSRecordTypeKnownUnique, mDNSNULL, mDNSNULL);
				info->AutoTunnelTarget.namestorage.c[0] = 0;
				AppendDomainLabel(&info->AutoTunnelTarget.namestorage, &m->AutoTunnelLabel);
				AppendDomainName (&info->AutoTunnelTarget.namestorage, &info->domain);
				mDNS_AddDynDNSHostName(m, &info->AutoTunnelTarget.namestorage, mDNSNULL, mDNSNULL);
		
				// 3. Set up IKE tunnel's SRV record: "AutoTunnelHostRecord SRV 0 0 port AutoTunnelTarget"
				mDNS_SetupResourceRecord(&info->AutoTunnelService, mDNSNULL, mDNSInterface_Any, kDNSType_SRV, kHostNameTTL, kDNSRecordTypeKnownUnique, mDNSNULL, mDNSNULL);
				AssignDomainName(&info->AutoTunnelService.namestorage, (const domainname*) "\x0B" "_autotunnel" "\x04" "_udp");
				AppendDomainName(&info->AutoTunnelService.namestorage, &info->AutoTunnelHostRecord.namestorage);
				info->AutoTunnelService.resrec.rdata->u.srv.priority = 0;
				info->AutoTunnelService.resrec.rdata->u.srv.weight   = 0;
				info->AutoTunnelService.resrec.rdata->u.srv.port     = mDNSOpaque16fromIntVal(500); // TEMP FOR AUTOTUNNEL TESTING: assume port 500
				AssignDomainName(&info->AutoTunnelService.resrec.rdata->u.srv.target, &info->AutoTunnelTarget.namestorage);
				mDNS_Register_internal(m, &info->AutoTunnelService);

				// 4. Create configuration file, and start (or SIGHUP) Racoon
				static const char RacoonConfig1[] =
					"remote anonymous\n"
						"{\n"
						"exchange_mode aggressive; doi ipsec_doi; situation identity_only; verify_identifier off; generate_policy on; shared_secret use \"";
				static const char RacoonConfig2[] =
						"\"; nonce_size 16; lifetime time 5 min; initial_contact on; support_proxy on; proposal_check claim;\n"
						"proposal { encryption_algorithm aes; hash_algorithm sha1; authentication_method pre_shared_key; dh_group 2; lifetime time 5 min; }\n"
						"}\n"
					"sainfo anonymous { pfs_group 2; lifetime time 60 min; encryption_algorithm aes; authentication_algorithm hmac_sha1; compression_algorithm deflate; }\n";
			
				FILE *f = fopen("/etc/racoon/remote/anonymous.conf", "w");
				fchmod(fileno(f), S_IRUSR | S_IWUSR);
				fwrite(RacoonConfig1, sizeof(RacoonConfig1)-1, 1, f);
				fwrite(info->b64keydata, strlen(info->b64keydata), 1, f);
				fwrite(RacoonConfig2, sizeof(RacoonConfig2)-1, 1, f);
				fclose(f);
				RestartRacoon();
				}
			}
		}
	}

mDNSlocal void SetupLocalAutoTunnelInterface(mDNS *const m)
	{
	mDNS_Lock(m);
	SetupLocalAutoTunnelInterface_internal(m);
	mDNS_Unlock(m);
	}

static unsigned int routeSeq = 1;

mDNSlocal void SetupTunnelRoute(mDNSv6Addr *const local, mDNSv6Addr *const remote)
	{
	int s = socket(PF_ROUTE, SOCK_RAW, AF_INET);
	struct
		{
		struct rt_msghdr    hdr;
		struct sockaddr_in6 dst;
		struct sockaddr_in6 gtwy;
		} msg;
	
	LogOperation("SetupTunnelRoute %.16a --> %.16a", local, remote);
	
	if (s < 0) { LogMsg("socket() failed trying to add route errno %d (%s)", errno, strerror(errno)); return; }
	
	memset(&msg, 0, sizeof(msg));
	
	msg.hdr.rtm_msglen = sizeof(msg);
	msg.hdr.rtm_type = RTM_ADD;
	// The following flags are set by `route add -inet6 -host ...`
	msg.hdr.rtm_flags = RTF_UP | RTF_GATEWAY | RTF_HOST | RTF_STATIC;
	msg.hdr.rtm_version = RTM_VERSION;
	msg.hdr.rtm_seq = routeSeq++;
	msg.hdr.rtm_addrs = RTA_DST | RTA_GATEWAY;
	
	msg.dst.sin6_len = sizeof(msg.dst);
	msg.dst.sin6_family = AF_INET6;
	memcpy(&msg.dst.sin6_addr, remote, sizeof(mDNSv6Addr));
	
	msg.gtwy.sin6_len = sizeof(msg.gtwy);
	msg.gtwy.sin6_family = AF_INET6;
	memcpy(&msg.gtwy.sin6_addr, local, sizeof(mDNSv6Addr));
	
	// send message, ignore error when route already exists
	if (write(s, &msg, msg.hdr.rtm_msglen) == -1 && errno != EEXIST) LogMsg("write() failed trying to add route errno %d (%s)", errno, strerror(errno));
	
	close(s);
	}

mDNSlocal void TeardownTunnelRoute(mDNSv6Addr *const remote)
	{
	int s = socket(PF_ROUTE, SOCK_RAW, AF_INET);
	struct
		{
		struct rt_msghdr    hdr;
		struct sockaddr_in6 dst;
		} msg;
	
	LogOperation("TeardownTunnelRoute %.16a", remote);
	
	if (s < 0) { LogMsg("socket() failed trying to delete route errno %d (%s)", errno, strerror(errno)); return; }
	
	memset(&msg, 0, sizeof(msg));
	
	msg.hdr.rtm_msglen = sizeof(msg);
	msg.hdr.rtm_type = RTM_DELETE;
	msg.hdr.rtm_version = RTM_VERSION;
	msg.hdr.rtm_seq = routeSeq++;
	msg.hdr.rtm_addrs = RTA_DST;
	
	msg.dst.sin6_len = sizeof(msg.dst);
	msg.dst.sin6_family = AF_INET6;
	memcpy(&msg.dst.sin6_addr, remote, sizeof(mDNSv6Addr));
	
	// send message, ignore error when route doesn't exist
	if (write(s, &msg, msg.hdr.rtm_msglen) == -1 && errno != ESRCH) LogMsg("write() failed trying to delete route errno %d (%s)", errno, strerror(errno));
	
	close(s);
	}

mDNSlocal const char RacoonClientConfig[] = "remote %s\n"
	"{ exchange_mode aggressive; doi ipsec_doi; situation identity_only; verify_identifier off; shared_secret use \"%s\";\n"
	"nonce_size 16; lifetime time 5 min; initial_contact on; support_proxy on; proposal_check claim;\n"
	"proposal { encryption_algorithm aes; hash_algorithm sha1; authentication_method pre_shared_key; dh_group 2; lifetime time 5 min; }\n"
	"}\n"
	"sainfo address %s any address %s any\n"
	"{ pfs_group 2; lifetime time 60 min; encryption_algorithm aes; authentication_algorithm hmac_sha1; compression_algorithm deflate; }\n"
	"sainfo address %s any address %s any\n"
	"{ pfs_group 2; lifetime time 60 min; encryption_algorithm aes; authentication_algorithm hmac_sha1; compression_algorithm deflate; }\n";

mDNSlocal void AutoTunnelSetKeys(mDNS *const m, ClientTunnel *tun, mDNSBool AddNew)
	{
	char li[40], lo[40], ri[40], ro[40];		// Enough for IPv6 address plus nul on the end

	tun->loc_inner = m->AutoTunnelHostAddr;
	tun->loc_outer = m->AdvertisedV4.ip.v4;

	mDNS_snprintf(li, sizeof(li), "%.16a", &tun->loc_inner);
	mDNS_snprintf(lo, sizeof(lo), "%.4a",  &tun->loc_outer);
	mDNS_snprintf(ri, sizeof(ri), "%.16a", &tun->rmt_inner);
	mDNS_snprintf(ro, sizeof(ro), "%.4a",  &tun->rmt_outer);

	FILE *fp = popen("/usr/sbin/setkey -c", "r+");
	if (!fp)
		LogMsg("popen(\"/usr/sbin/setkey -c\", \"r+\"); failed");
	else
		{
		if (fprintf(fp, "spddelete %s/128 %s/128 any -P out;\n", li, ri) < 0) LogMsg("spddelete loc->rmt failed");
		if (fprintf(fp, "spddelete %s/128 %s/128 any -P  in;\n", ri, li) < 0) LogMsg("spddelete rmt->loc failed");
		if (AddNew)
			{
			if (fprintf(fp, "spdadd %s/128 %s/128 any -P out ipsec esp/tunnel/%s-%s/require;\n", li, ri, lo, ro) < 0) LogMsg("spdadd loc->rmt failed");
			if (fprintf(fp, "spdadd %s/128 %s/128 any -P in  ipsec esp/tunnel/%s-%s/require;\n", ri, li, ro, lo) < 0) LogMsg("spdadd rmt->loc failed");
			}
		pclose(fp);

		TeardownTunnelRoute(&tun->rmt_inner);
		if (AddNew)
			{
			char filename[64];
			mDNS_snprintf(filename, sizeof(filename), "/etc/racoon/remote/%s.conf", ro);
			FILE *f = fopen(filename, "w");
			fchmod(fileno(f), S_IRUSR | S_IWUSR);
			char filedata[1024];
			int len = mDNS_snprintf(filedata, sizeof(filedata), RacoonClientConfig, ro, tun->b64keydata, ri, li, li, ri);
			fwrite(filedata, len, 1, f);
			fclose(f);
			RestartRacoon();

			SetupTunnelRoute(&tun->loc_inner, &tun->rmt_inner);
			}
		}
	}

// If the EUI-64 part of the IPv6 ULA matches, then that means the two addresses point to the same machine
#define mDNSSameClientTunnel(A,B) ((A)->l[2] == (B)->l[2] && (A)->l[3] == (B)->l[3])

mDNSlocal void AutoTunnelCallback(mDNS *const m, DNSQuestion *question, const ResourceRecord *const answer, mDNSBool AddRecord)
	{
	ClientTunnel *tun = (ClientTunnel *)question->QuestionContext;
	if (!AddRecord || !answer->rdlength) return;
	if (question->qtype == kDNSType_SRV)
		{
		LogOperation("AutoTunnelCallback: SRV target name %##s", answer->rdata->u.srv.target.c);
		mDNS_StopQuery(m, question);
		AssignDomainName(&tun->q.qname, &answer->rdata->u.srv.target);
		question->qtype = kDNSType_A;
		mDNS_StartQuery(m, &tun->q);
		}
	else if (question->qtype == kDNSType_A)
		{
		LogOperation("AutoTunnelCallback: SRV target addr %.4a", &answer->rdata->u.ipv4);
		mDNS_StopQuery(m, question);
		question->ThisQInterval = -1;		// So we know this tunnel setup has completed
		tun->rmt_outer = answer->rdata->u.ipv4;
		AutoTunnelSetKeys(m, tun, mDNStrue);
		}
	else
		LogMsg("AutoTunnelCallback: Unknown question %p", question);
	}

// Must be called with the lock held
mDNSexport void AddNewClientTunnel(mDNS *const m, DNSQuestion *const originalquestion, const ResourceRecord *const dsthost)
	{
	// If this is our own Tunnel address query, don't want to loop endlessly
	// (Should never happen -- we only do AutoTunnel setups for IPv6 destinations, and at present
	// our AutoTunnel endpoints are always IPv4 addresses, but it doesn't hurt to be cautious.)
	if (originalquestion->QuestionCallback == AutoTunnelCallback) return;
	if (originalquestion->AuthInfo == mDNSNULL) return;

	ClientTunnel **p = &m->TunnelClients;
	while (*p && !mDNSSameClientTunnel(&(*p)->rmt_inner, &dsthost->rdata->u.ipv6)) p = &(*p)->next;

	if (*p)
		{
		LogOperation("AddNewClientTunnel: Already have AutoTunnel for %##s %.16a", dsthost->name->c, &dsthost->rdata->u.ipv6);
		if ((*p)->q.ThisQInterval >= 0) mDNS_StopQuery_internal(m, &(*p)->q);
		AutoTunnelSetKeys(m, (*p), mDNSfalse);		// Clear the old tunnel endpoint state before installing new state
		}
	else
		{
		LogOperation("AddNewClientTunnel: New AutoTunnel for %##s %.16a", dsthost->name->c, &dsthost->rdata->u.ipv6);
		ClientTunnel *tun = mallocL("ClientTunnel", sizeof(ClientTunnel));
		if (!tun) return;
		// If this is our first tunnel client, bring interface up now
		if (m->AutoTunnelHostAddr.b[0] && !m->TunnelClients) SetupLocalAutoTunnelInterface_internal(m);
		tun->next = mDNSNULL;
		tun->rmt_inner = dsthost->rdata->u.ipv6;	// tun->rmt_outer unknown at this stage -- SRV query will discover that
		*p = tun;
		}

	(*p)->q.InterfaceID      = mDNSInterface_Any;
	(*p)->q.Target           = zeroAddr;
	AssignDomainName(&(*p)->q.qname, (const domainname*) "\x0B" "_autotunnel" "\x04" "_udp");
	AppendDomainName(&(*p)->q.qname, dsthost->name);
	(*p)->q.qtype            = kDNSType_SRV;
	(*p)->q.qclass           = kDNSClass_IN;
	(*p)->q.LongLived        = mDNSfalse;
	(*p)->q.ExpectUnique     = mDNStrue;
	(*p)->q.ForceMCast       = mDNSfalse;
	(*p)->q.ReturnIntermed   = mDNSfalse;
	(*p)->q.QuestionCallback = AutoTunnelCallback;
	(*p)->q.QuestionContext  = (*p);

	mDNS_snprintf((*p)->b64keydata, sizeof((*p)->b64keydata), "%s", originalquestion->AuthInfo->b64keydata);

	mDNS_StartQuery_internal(m, &(*p)->q);
	}

#endif // APPLE_OSX_mDNSResponder

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - Power State & Configuration Change Management
#endif

mDNSlocal mStatus UpdateInterfaceList(mDNS *const m, mDNSs32 utc)
	{
	mDNSBool foundav4           = mDNSfalse;
	mDNSBool foundav6           = mDNSfalse;
	struct ifaddrs *ifa         = myGetIfAddrs(1);
	struct ifaddrs *v4Loopback  = NULL;
	struct ifaddrs *v6Loopback  = NULL;
	mDNSEthAddr PrimaryMAC      = zeroEthAddr;
	char defaultname[32];
#ifndef NO_IPV6
	int InfoSocket              = socket(AF_INET6, SOCK_DGRAM, 0);
	if (InfoSocket < 3) LogMsg("UpdateInterfaceList: InfoSocket error %d errno %d (%s)", InfoSocket, errno, strerror(errno));
#endif
	if (m->SleepState) ifa = NULL;

	while (ifa)
		{
#if LIST_ALL_INTERFACES
		if (ifa->ifa_addr->sa_family == AF_APPLETALK)
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d is AF_APPLETALK",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
		else if (ifa->ifa_addr->sa_family == AF_LINK)
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d is AF_LINK",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
		else if (ifa->ifa_addr->sa_family != AF_INET && ifa->ifa_addr->sa_family != AF_INET6)
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d not AF_INET (2) or AF_INET6 (30)",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
		if (!(ifa->ifa_flags & IFF_UP))
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d Interface not IFF_UP",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
		if (!(ifa->ifa_flags & IFF_MULTICAST))
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d Interface not IFF_MULTICAST",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
		if (ifa->ifa_flags & IFF_POINTOPOINT)
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d Interface IFF_POINTOPOINT",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
		if (ifa->ifa_flags & IFF_LOOPBACK)
			LogMsg("UpdateInterfaceList: %5s(%d) Flags %04X Family %2d Interface IFF_LOOPBACK",
				ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family);
#endif

		if (ifa->ifa_addr->sa_family == AF_LINK)
			{
			struct sockaddr_dl *sdl = (struct sockaddr_dl *)ifa->ifa_addr;
			if (sdl->sdl_type == IFT_ETHER && sdl->sdl_alen == sizeof(PrimaryMAC) && mDNSSameEthAddress(&PrimaryMAC, &zeroEthAddr))
				mDNSPlatformMemCopy(PrimaryMAC.b, sdl->sdl_data + sdl->sdl_nlen, 6);
			}

		if (ifa->ifa_flags & IFF_UP && ifa->ifa_addr)
			if (ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6)
				{
				if (!ifa->ifa_netmask)
					{
					mDNSAddr ip;
					SetupAddr(&ip, ifa->ifa_addr);
					LogMsg("getifaddrs: ifa_netmask is NULL for %5s(%d) Flags %04X Family %2d %#a",
						ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family, &ip);
					}
				else if (ifa->ifa_addr->sa_family != ifa->ifa_netmask->sa_family)
					{
					mDNSAddr ip;
					SetupAddr(&ip, ifa->ifa_addr);
					LogMsg("getifaddrs ifa_netmask for %5s(%d) Flags %04X Family %2d %#a has different family: %d",
						ifa->ifa_name, if_nametoindex(ifa->ifa_name), ifa->ifa_flags, ifa->ifa_addr->sa_family, &ip, ifa->ifa_netmask->sa_family);
					}
				else
					{
					int ifru_flags6 = 0;
#ifndef NO_IPV6
					if (ifa->ifa_addr->sa_family == AF_INET6 && InfoSocket >= 0)
						{
						struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)ifa->ifa_addr;
						struct in6_ifreq ifr6;
						mDNSPlatformMemZero((char *)&ifr6, sizeof(ifr6));
						strlcpy(ifr6.ifr_name, ifa->ifa_name, sizeof(ifr6.ifr_name));
						ifr6.ifr_addr = *sin6;
						if (ioctl(InfoSocket, SIOCGIFAFLAG_IN6, &ifr6) != -1)
							ifru_flags6 = ifr6.ifr_ifru.ifru_flags6;
						verbosedebugf("%s %.16a %04X %04X", ifa->ifa_name, &sin6->sin6_addr, ifa->ifa_flags, ifru_flags6);
						}
#endif
					if (!(ifru_flags6 & (IN6_IFF_NOTREADY | IN6_IFF_DETACHED | IN6_IFF_DEPRECATED | IN6_IFF_TEMPORARY)))
						{
						if (ifa->ifa_flags & IFF_LOOPBACK)
							if (ifa->ifa_addr->sa_family == AF_INET) v4Loopback = ifa;
							else                                     v6Loopback = ifa;
						else
							{
							NetworkInterfaceInfoOSX *i = AddInterfaceToList(m, ifa, utc);
							if (i && MulticastInterface(i))
								{
								if (ifa->ifa_addr->sa_family == AF_INET) foundav4 = mDNStrue;
								else                                     foundav6 = mDNStrue;
								}
							}
						}
					}
				}
		ifa = ifa->ifa_next;
		}

	// For efficiency, we don't register a loopback interface when other interfaces of that family are available
	if (!foundav4 && v4Loopback) AddInterfaceToList(m, v4Loopback, utc);
	if (!foundav6 && v6Loopback) AddInterfaceToList(m, v6Loopback, utc);

	// Now the list is complete, set the McastTxRx setting for each interface.
	NetworkInterfaceInfoOSX *i;
	for (i = m->p->InterfaceList; i; i = i->next)
		if (i->Exists)
			{
			mDNSBool txrx = MulticastInterface(i);
#if USE_V6_ONLY_WHEN_NO_ROUTABLE_V4
			txrx = txrx && ((i->ifinfo.ip.type == mDNSAddrType_IPv4) || !FindRoutableIPv4(m, i->scope_id));
#endif
			if (i->ifinfo.McastTxRx != txrx)
				{
				i->ifinfo.McastTxRx = txrx;
				i->Exists = 2; // State change; need to deregister and reregister this interface
				}
			}

#ifndef NO_IPV6
	if (InfoSocket >= 0) close(InfoSocket);
#endif

	// If we haven't set up AutoTunnelHostAddr yet, do it now
	if (!mDNSSameEthAddress(&PrimaryMAC, &zeroEthAddr) && m->AutoTunnelHostAddr.b[0] == 0)
		{
		m->AutoTunnelHostAddr.b[0x0] = 0xFD;		// Required prefix for "locally assigned" ULA (See RFC 4193)
		m->AutoTunnelHostAddr.b[0x1] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x2] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x3] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x4] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x5] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x6] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x7] = mDNSRandom(255);
		m->AutoTunnelHostAddr.b[0x8] = PrimaryMAC.b[0] ^ 0x02;	// See RFC 3513, Appendix A for explanation
		m->AutoTunnelHostAddr.b[0x9] = PrimaryMAC.b[1];
		m->AutoTunnelHostAddr.b[0xA] = PrimaryMAC.b[2];
		m->AutoTunnelHostAddr.b[0xB] = 0xFF;
		m->AutoTunnelHostAddr.b[0xC] = 0xFE;
		m->AutoTunnelHostAddr.b[0xD] = PrimaryMAC.b[3];
		m->AutoTunnelHostAddr.b[0xE] = PrimaryMAC.b[4];
		m->AutoTunnelHostAddr.b[0xF] = PrimaryMAC.b[5];
		m->AutoTunnelLabel.c[0] = mDNS_snprintf((char*)m->AutoTunnelLabel.c+1, 254, "AutoTunnel-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
			m->AutoTunnelHostAddr.b[0x8], m->AutoTunnelHostAddr.b[0x9], m->AutoTunnelHostAddr.b[0xA], m->AutoTunnelHostAddr.b[0xB],
			m->AutoTunnelHostAddr.b[0xC], m->AutoTunnelHostAddr.b[0xD], m->AutoTunnelHostAddr.b[0xE], m->AutoTunnelHostAddr.b[0xF]);
		LogOperation("m->AutoTunnelLabel %#s", m->AutoTunnelLabel.c);
		}

	#ifndef kDefaultLocalHostNamePrefix
	#define kDefaultLocalHostNamePrefix "Macintosh"
	#endif
	mDNS_snprintf(defaultname, sizeof(defaultname), kDefaultLocalHostNamePrefix "-%02X%02X%02X%02X%02X%02X",
		PrimaryMAC.b[0], PrimaryMAC.b[1], PrimaryMAC.b[2], PrimaryMAC.b[3], PrimaryMAC.b[4], PrimaryMAC.b[5]);

	// Set up the nice label
	domainlabel nicelabel;
	nicelabel.c[0] = 0;
	GetUserSpecifiedFriendlyComputerName(&nicelabel);
	if (nicelabel.c[0] == 0)
		{
		LogMsg("Couldn't read user-specified Computer Name; using default “%s” instead", defaultname);
		MakeDomainLabelFromLiteralString(&nicelabel, defaultname);
		}

	// Set up the RFC 1034-compliant label
	domainlabel hostlabel;
	hostlabel.c[0] = 0;
	GetUserSpecifiedLocalHostName(&hostlabel);
	if (hostlabel.c[0] == 0)
		{
		LogMsg("Couldn't read user-specified local hostname; using default “%s.local” instead", defaultname);
		MakeDomainLabelFromLiteralString(&hostlabel, defaultname);
		}

	// We use a case-sensitive comparison here because even though changing the capitalization
	// of the name alone is not significant to DNS, it's still a change from the user's point of view
	if (SameDomainLabelCS(m->p->usernicelabel.c, nicelabel.c))
		debugf("Usernicelabel (%#s) unchanged since last time; not changing m->nicelabel (%#s)", m->p->usernicelabel.c, m->nicelabel.c);
	else
		{
		if (m->p->usernicelabel.c[0])	// Don't show message first time through, when we first read name from prefs on boot
			LogMsg("User updated Computer Name from %#s to %#s", m->p->usernicelabel.c, nicelabel.c);
		m->p->usernicelabel = m->nicelabel = nicelabel;
		}

	if (SameDomainLabelCS(m->p->userhostlabel.c, hostlabel.c))
		debugf("Userhostlabel (%#s) unchanged since last time; not changing m->hostlabel (%#s)", m->p->userhostlabel.c, m->hostlabel.c);
	else
		{
		if (m->p->userhostlabel.c[0])	// Don't show message first time through, when we first read name from prefs on boot
			LogMsg("User updated Local Hostname from %#s to %#s", m->p->userhostlabel.c, hostlabel.c);
		m->p->userhostlabel = m->hostlabel = hostlabel;
		mDNS_SetFQDN(m);
		}

	return(mStatus_NoError);
	}

#if LogAllOperations || MDNS_DEBUGMSGS
// Returns number of leading one-bits in mask: 0-32 for IPv4, 0-128 for IPv6
// Returns -1 if all the one-bits are not contiguous
mDNSlocal int CountMaskBits(mDNSAddr *mask)
	{
	int i = 0, bits = 0;
	int bytes = mask->type == mDNSAddrType_IPv4 ? 4 : mask->type == mDNSAddrType_IPv6 ? 16 : 0;
	while (i < bytes)
		{
		mDNSu8 b = mask->ip.v6.b[i++];
		while (b & 0x80) { bits++; b <<= 1; }
		if (b) return(-1);
		}
	while (i < bytes) if (mask->ip.v6.b[i++]) return(-1);
	return(bits);
	}
#endif

// returns count of non-link local V4 addresses registered
mDNSlocal int SetupActiveInterfaces(mDNS *const m, mDNSs32 utc)
	{
	NetworkInterfaceInfoOSX *i;
	int count = 0;
	for (i = m->p->InterfaceList; i; i = i->next)
		if (i->Exists)
			{
			NetworkInterfaceInfo *n = &i->ifinfo;
			NetworkInterfaceInfoOSX *primary = SearchForInterfaceByName(m, i->ifa_name, i->sa_family);
			if (!primary) LogMsg("SetupActiveInterfaces ERROR! SearchForInterfaceByName didn't find %s", i->ifa_name);

			if (n->InterfaceID && n->InterfaceID != (mDNSInterfaceID)primary)	// Sanity check
				{
				LogMsg("SetupActiveInterfaces ERROR! n->InterfaceID %p != primary %p", n->InterfaceID, primary);
				n->InterfaceID = mDNSNULL;
				}

			if (!n->InterfaceID)
				{
				// NOTE: If n->InterfaceID is set, that means we've called mDNS_RegisterInterface() for this interface,
				// so we need to make sure we call mDNS_DeregisterInterface() before disposing it.
				// If n->InterfaceID is NOT set, then we haven't registered it and we should not try to deregister it
				n->InterfaceID = (mDNSInterfaceID)primary;
				// If i->LastSeen == utc, then this is a brand-new interface, just created, or an interface that never went away.
				// If i->LastSeen != utc, then this is an old interface, previously seen, that went away for (utc - i->LastSeen) seconds.
				// If the interface is an old one that went away and came back in less than a minute, then we're in a flapping scenario.
				i->Occulting = !(i->ifa_flags & IFF_LOOPBACK) && (utc - i->LastSeen > 0 && utc - i->LastSeen < 60);
				mDNS_RegisterInterface(m, n, i->Flashing && i->Occulting);
				if (!mDNSAddressIsLinkLocal(&i->ifinfo.ip)) count++;
				LogOperation("SetupActiveInterfaces:   Registered    %5s(%lu) %.6a InterfaceID %p %#a/%d%s%s%s",
					i->ifa_name, i->scope_id, &i->BSSID, primary, &n->ip, CountMaskBits(&n->mask),
					i->Flashing        ? " (Flashing)"  : "",
					i->Occulting       ? " (Occulting)" : "",
					n->InterfaceActive ? " (Primary)"   : "");
				}

			if (!n->McastTxRx)
				debugf("SetupActiveInterfaces:   No Tx/Rx on   %5s(%lu) %.6a InterfaceID %p %#a", i->ifa_name, i->scope_id, &i->BSSID, primary, &n->ip);
			else
				{
				if (i->sa_family == AF_INET)
					{
					struct ip_mreq imr;
					primary->ifa_v4addr.s_addr = i->ifinfo.ip.ip.v4.NotAnInteger;
					imr.imr_multiaddr.s_addr = AllDNSLinkGroup_v4.ip.v4.NotAnInteger;
					imr.imr_interface        = primary->ifa_v4addr;
					mStatus err = setsockopt(m->p->permanentsockets.sktv4, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr));
					// Joining same group twice can give "Address already in use" error -- no need to report that
					if (err < 0 && errno != EADDRINUSE)
						LogMsg("setsockopt - IP_ADD_MEMBERSHIP error %ld errno %d (%s)", err, errno, strerror(errno));
					}
#ifndef NO_IPV6
				if (i->sa_family == AF_INET6)
					{
					struct ipv6_mreq i6mr;
					i6mr.ipv6mr_interface = primary->scope_id;
					i6mr.ipv6mr_multiaddr = *(struct in6_addr*)&AllDNSLinkGroup_v6.ip.v6;
					mStatus err = setsockopt(m->p->permanentsockets.sktv6, IPPROTO_IPV6, IPV6_JOIN_GROUP, &i6mr, sizeof(i6mr));
					// Joining same group twice can give "Address already in use" error -- no need to report that
					if (err < 0 && errno != EADDRINUSE)
						LogMsg("setsockopt - IPV6_JOIN_GROUP error %ld errno %d (%s)", err, errno, strerror(errno));
					}
#endif
				}
			}
	return count;
	}

mDNSlocal void MarkAllInterfacesInactive(mDNS *const m, mDNSs32 utc)
	{
	NetworkInterfaceInfoOSX *i;
	for (i = m->p->InterfaceList; i; i = i->next)
		{
		if (i->Exists) i->LastSeen = utc;
		i->Exists = mDNSfalse;
		}
	}

// returns count of non-link local V4 addresses deregistered
mDNSlocal int ClearInactiveInterfaces(mDNS *const m, mDNSs32 utc)
	{
	// First pass:
	// If an interface is going away, then deregister this from the mDNSCore.
	// We also have to deregister it if the primary interface that it's using for its InterfaceID is going away.
	// We have to do this because mDNSCore will use that InterfaceID when sending packets, and if the memory
	// it refers to has gone away we'll crash.
	// Don't actually close the sockets or free the memory yet: When the last representative of an interface goes away
	// mDNSCore may want to send goodbye packets on that interface. (Not yet implemented, but a good idea anyway.)
	NetworkInterfaceInfoOSX *i;
	int count = 0;
	for (i = m->p->InterfaceList; i; i = i->next)
		{
		// If this interface is no longer active, or its InterfaceID is changing, deregister it
		NetworkInterfaceInfoOSX *primary = SearchForInterfaceByName(m, i->ifa_name, i->sa_family);
		if (i->ifinfo.InterfaceID)
			if (i->Exists == 0 || i->Exists == 2 || i->ifinfo.InterfaceID != (mDNSInterfaceID)primary)
				{
				i->Flashing = !(i->ifa_flags & IFF_LOOPBACK) && (utc - i->AppearanceTime < 60);
				LogOperation("ClearInactiveInterfaces: Deregistering %5s(%lu) %.6a InterfaceID %p %#a/%d%s%s%s",
					i->ifa_name, i->scope_id, &i->BSSID, i->ifinfo.InterfaceID,
					&i->ifinfo.ip, CountMaskBits(&i->ifinfo.mask),
					i->Flashing               ? " (Flashing)"  : "",
					i->Occulting              ? " (Occulting)" : "",
					i->ifinfo.InterfaceActive ? " (Primary)"   : "");
				mDNS_DeregisterInterface(m, &i->ifinfo, i->Flashing && i->Occulting);
				if (!mDNSAddressIsLinkLocal(&i->ifinfo.ip)) count++;
				i->ifinfo.InterfaceID = mDNSNULL;
				// NOTE: If n->InterfaceID is set, that means we've called mDNS_RegisterInterface() for this interface,
				// so we need to make sure we call mDNS_DeregisterInterface() before disposing it.
				// If n->InterfaceID is NOT set, then it's not registered and we should not call mDNS_DeregisterInterface() on it.
				}
		}

	// Second pass:
	// Now that everything that's going to deregister has done so, we can close sockets and free the memory
	NetworkInterfaceInfoOSX **p = &m->p->InterfaceList;
	while (*p)
		{
		i = *p;
		// If no longer active, delete interface from list and free memory
		if (!i->Exists)
			{
			if (i->LastSeen == utc) i->LastSeen = utc - 1;
			mDNSBool delete = (NumCacheRecordsForInterfaceID(m, (mDNSInterfaceID)i) == 0) && (utc - i->LastSeen >= 60);
			LogOperation("ClearInactiveInterfaces: %-13s %5s(%lu) %.6a InterfaceID %p %#a/%d Age %d%s", delete ? "Deleting" : "Holding",
				i->ifa_name, i->scope_id, &i->BSSID, i->ifinfo.InterfaceID,
				&i->ifinfo.ip, CountMaskBits(&i->ifinfo.mask), utc - i->LastSeen,
				i->ifinfo.InterfaceActive ? " (Primary)" : "");
			if (delete)
				{
				*p = i->next;
				if (i->ifa_name) freeL("NetworkInterfaceInfoOSX name", i->ifa_name);
				freeL("NetworkInterfaceInfoOSX", i);
				continue;	// After deleting this object, don't want to do the "p = &i->next;" thing at the end of the loop
				}
			}
		p = &i->next;
		}
	return count;
	}

mDNSlocal void AppendDNameListElem(DNameListElem ***List, mDNSu32 uid, domainname *name)
	{
	DNameListElem *dnle = (DNameListElem*) mallocL("DNameListElem/AppendDNameListElem", sizeof(DNameListElem));
	if (!dnle) LogMsg("ERROR: AppendDNameListElem: memory exhausted");
	else
		{
		dnle->next = mDNSNULL;
		dnle->uid  = uid;
		AssignDomainName(&dnle->name, name);
		**List = dnle;
		*List = &dnle->next;
		}
	}

mDNSexport void mDNSPlatformSetDNSConfig(mDNS *const m, mDNSBool setservers, mDNSBool setsearch, domainname *const fqdn, DNameListElem **RegDomains, DNameListElem **BrowseDomains)
	{
	int i;
	char buf[MAX_ESCAPED_DOMAIN_NAME];	// Max legal C-string name, including terminating NUL
	domainname d;

	// Need to set these here because we need to do this even if SCDynamicStoreCreate() or SCDynamicStoreCopyValue() below don't succeed
	if (fqdn)          fqdn->c[0]      = 0;
	if (RegDomains   ) *RegDomains     = NULL;
	if (BrowseDomains) *BrowseDomains  = NULL;

	LogOperation("mDNSPlatformSetDNSConfig%s%s%s%s%s",
		setservers    ? " setservers"    : "",
		setsearch     ? " setsearch"     : "",
		fqdn          ? " fqdn"          : "",
		RegDomains    ? " RegDomains"    : "",
		BrowseDomains ? " BrowseDomains" : "");

	// Add the inferred address-based configuration discovery domains
	// (should really be in core code I think, not platform-specific)
	if (setsearch)
		{
		struct ifaddrs *ifa = myGetIfAddrs(1);
		while (ifa)
			{
			mDNSAddr a, n;
			if (ifa->ifa_addr->sa_family == AF_INET	&&
				ifa->ifa_netmask                    &&
				!(ifa->ifa_flags & IFF_LOOPBACK)	&&
				!SetupAddr(&a, ifa->ifa_addr)		&&
				!SetupAddr(&n, ifa->ifa_netmask)	&&
				!mDNSv4AddressIsLinkLocal(&a.ip.v4)  )
				{
				// Note: This is reverse order compared to a normal dotted-decimal IP address, so we can't use our customary "%.4a" format code
				mDNS_snprintf(buf, sizeof(buf), "%d.%d.%d.%d.in-addr.arpa.", a.ip.v4.b[3] & n.ip.v4.b[3],
																			 a.ip.v4.b[2] & n.ip.v4.b[2],
																			 a.ip.v4.b[1] & n.ip.v4.b[1],
																			 a.ip.v4.b[0] & n.ip.v4.b[0]);
				mDNS_AddSearchDomain_CString(buf);
				}
			ifa = ifa->ifa_next;
			}
		}

#ifndef MDNS_NO_DNSINFO
	if (setservers || setsearch)
		{
		dns_config_t *config = dns_configuration_copy();
		if (!config)
			{
			// When running on 10.3 (build 7xxx) and earlier, we don't expect dns_configuration_copy() to succeed
			// On 10.4, calls to dns_configuration_copy() early in the boot process often fail.
			// Apparently this is expected behaviour -- "not a bug".
			// Accordingly, we suppress syslog messages for the first three minutes after boot.
			// If we are still getting failures after three minutes, then we log them.
			if (mDNSMacOSXSystemBuildNumber(NULL) > 7 && (mDNSu32)mDNSPlatformRawTime() > (mDNSu32)(mDNSPlatformOneSecond * 180))
				LogMsg("GetDNSConfig: Error: dns_configuration_copy returned NULL");
			}
		else
			{
			LogOperation("mDNSPlatformSetDNSConfig: Registering %d resolvers", config->n_resolver);
			if (setservers)
				{
				for (i = 0; i < config->n_resolver; i++)
					{
					int j, n;
					dns_resolver_t *r = config->resolver[i];
					// Ignore dnsinfo entries for mDNS domains (indicated by the fact that the resolver port is 5353, the mDNS port)
					// Note: Unlike the BSD Sockets APIs (where TCP and UDP port numbers are universally in network byte order)
					// in Apple's "dnsinfo.h" API the port number is declared to be a "uint16_t in host byte order"
					if (r->port == 5353) continue;
					if (r->search_order == DEFAULT_SEARCH_ORDER || !r->domain || !*r->domain) d.c[0] = 0; // we ignore domain for "default" resolver
					else if (!MakeDomainNameFromDNSNameString(&d, r->domain)) { LogMsg("RegisterSplitDNS: bad domain %s", r->domain); continue; }

					for (j = 0; j < config->n_resolver; j++)  // check if this is the lowest-weighted server for the domain
						{
						dns_resolver_t *p = config->resolver[j];
						if (p->port == 5353) continue; // Note: dns_resolver_t port is defined to be "uint16_t in host byte order"
						if (p->search_order <= r->search_order)
							{
							domainname tmp;
							if (p->search_order == DEFAULT_SEARCH_ORDER || !p->domain || !*p->domain) tmp.c[0] = '\0';
							else if (!MakeDomainNameFromDNSNameString(&tmp, p->domain)) { LogMsg("RegisterSplitDNS: bad domain %s", p->domain); continue; }
							if (SameDomainName(&d, &tmp))
								if (p->search_order < r->search_order || j < i) break;  // if equal weights, pick first in list, otherwise pick lower-weight (p)
							}
						}
					if (j < config->n_resolver) // found a lower-weighted resolver for this domain
						debugf("Rejecting DNS server in slot %d domain %##s (slot %d outranks)", i, d.c, j);
					else
						{
						mDNSInterfaceID interface = mDNSInterface_Any;
						int disabled  = 0;
						
						// DNS server option parsing
						if (r->options != NULL)
							{
							char *nextOption = r->options;
							char *currentOption = NULL;
							while ((currentOption = strsep(&nextOption, " ")) != NULL && currentOption[0] != 0)
								{
								// The option may be in the form of interface=xxx where xxx is an interface name.
								if (strncmp(currentOption, kInterfaceSpecificOption, sizeof(kInterfaceSpecificOption) - 1) == 0)
									{
									NetworkInterfaceInfoOSX *i;
									char	ifname[IF_NAMESIZE+1];
									mDNSu32	ifindex = 0;
									// If something goes wrong finding the interface, create the server entry anyhow but mark it as disabled.
									// This allows us to block these special queries from going out on the wire.
									strlcpy(ifname, currentOption + sizeof(kInterfaceSpecificOption)-1, sizeof(ifname));
									ifindex = if_nametoindex(ifname);
									if (ifindex == 0) { disabled = 1; LogMsg("RegisterSplitDNS: interfaceSpecific - interface %s not found", ifname); continue; }
									LogOperation("%s: Interface specific entry: %s on %s (%d)", __FUNCTION__, r->domain, ifname, ifindex);
									// Find the interface, can't use mDNSPlatformInterfaceIDFromInterfaceIndex
									// because that will call mDNSMacOSXNetworkChanged if the interface doesn't exist
									for (i = m->p->InterfaceList; i; i = i->next)
										if (i->ifinfo.InterfaceID && i->scope_id == ifindex) break;
									if (i != NULL) interface = i->ifinfo.InterfaceID;
									if (interface == mDNSNULL) { disabled = 1; LogMsg("RegisterSplitDNS: interfaceSpecific - index %d (%s) not found", ifindex, ifname); continue; }
									}
								}
							}
						for (n = 0; n < r->n_nameserver; n++)
							if (r->nameserver[n]->sa_family == AF_INET && (interface || disabled || !AddrRequiresPPPConnection(r->nameserver[n])))
								{
								mDNSAddr saddr;
								// mDNSAddr saddr = { mDNSAddrType_IPv4, { { { 192, 168, 1, 1 } } } }; // for testing
								debugf("Adding dns server from slot %d %#a for domain %##s", i, &saddr, d.c);
								if (SetupAddr(&saddr, r->nameserver[n])) LogMsg("RegisterSplitDNS: bad IP address");
								else
									{
									DNSServer *s = mDNS_AddDNSServer(m, &d, mDNSInterface_Any, &saddr, r->port ? mDNSOpaque16fromIntVal(r->port) : UnicastDNSPort);
									if (s && disabled) s->teststate = DNSServer_Disabled;
									}
								}
						}
					}
				}
			if (setsearch)
				{
				// Due to the vagaries of Apple's SystemConfiguration and dnsinfo.h APIs, if there are no search domains
				// listed, then you're supposed to interpret the "domain" field as also being the search domain, but if
				// there *are* search domains listed, then you're supposed to ignore the "domain" field completely and
				// instead use the search domain list as the sole authority for what domains to search and in what order
				// (and the domain from the "domain" field will also appear somewhere in that list).
				// Also, all search domains get added to the search list for resolver[0], so the domains and/or
				// search lists for other resolvers in the list need to be ignored.
				if (config->resolver[0]->n_search == 0) mDNS_AddSearchDomain_CString(config->resolver[0]->domain);
				else for (i = 0; i < config->resolver[0]->n_search; i++) mDNS_AddSearchDomain_CString(config->resolver[0]->search[i]);
				}
			dns_configuration_free(config);
			setservers = mDNSfalse;  // Done these now -- no need to fetch the same data from SCDynamicStore
			setsearch  = mDNSfalse;
			}
		}
#endif // MDNS_NO_DNSINFO

	SCDynamicStoreRef store = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:mDNSPlatformSetDNSConfig"), NULL, NULL);
	if (store)
		{
		CFDictionaryRef dict = SCDynamicStoreCopyValue(store, NetworkChangedKey_DynamicDNS);
		if (dict)
			{
			if (fqdn)
				{
				CFArrayRef fqdnArray = CFDictionaryGetValue(dict, CFSTR("HostNames"));
				if (fqdnArray && CFArrayGetCount(fqdnArray) > 0)
					{
					// for now, we only look at the first array element.  if we ever support multiple configurations, we will walk the list
					CFDictionaryRef fqdnDict = CFArrayGetValueAtIndex(fqdnArray, 0);
					if (fqdnDict && DDNSSettingEnabled(fqdnDict))
						{
						CFStringRef name = CFDictionaryGetValue(fqdnDict, CFSTR("Domain"));
						if (name)
							{
							if (!CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8) ||
								!MakeDomainNameFromDNSNameString(fqdn, buf) || !fqdn->c[0])
								LogMsg("GetUserSpecifiedDDNSConfig SCDynamicStore bad DDNS host name: %s", buf[0] ? buf : "(unknown)");
							else debugf("GetUserSpecifiedDDNSConfig SCDynamicStore DDNS host name: %s", buf);
							}
						}
					}
				}

			if (RegDomains)
				{
				CFArrayRef regArray = CFDictionaryGetValue(dict, CFSTR("RegistrationDomains"));
				if (regArray && CFArrayGetCount(regArray) > 0)
					{
					CFDictionaryRef regDict = CFArrayGetValueAtIndex(regArray, 0);
					if (regDict && DDNSSettingEnabled(regDict))
						{
						CFStringRef name = CFDictionaryGetValue(regDict, CFSTR("Domain"));
						if (name)
							{
							if (!CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8) ||
								!MakeDomainNameFromDNSNameString(&d, buf) || !d.c[0])
								LogMsg("GetUserSpecifiedDDNSConfig SCDynamicStore bad DDNS registration domain: %s", buf[0] ? buf : "(unknown)");
							else
								{
								debugf("GetUserSpecifiedDDNSConfig SCDynamicStore DDNS registration domain: %s", buf);
								AppendDNameListElem(&RegDomains, 0, &d);
								}
							}
						}
					}

				CFDictionaryRef btmm = SCDynamicStoreCopyValue(store, NetworkChangedKey_BackToMyMac);
				if (btmm)
					{
					CFIndex size = CFDictionaryGetCount(btmm);
					const void *key[size];
					const void *val[size];
					CFDictionaryGetKeysAndValues(btmm, key, val);
					for (i = 0; i < size; i++)
						{
						LogOperation("BackToMyMac %d", i);
						if (!CFStringGetCString(key[i], buf, sizeof(buf), kCFStringEncodingUTF8))
							LogMsg("Can't read BackToMyMac %d key %s", i, buf);
						else
							{
							mDNSu32 uid = atoi(buf);
							if (!CFStringGetCString(val[i], buf, sizeof(buf), kCFStringEncodingUTF8))
								LogMsg("Can't read BackToMyMac %d val %s", i, buf);
							else if (MakeDomainNameFromDNSNameString(&d, buf) && d.c[0])
								{
								LogOperation("BackToMyMac %d %d %##s", i, uid, d.c);
								AppendDNameListElem(&RegDomains, uid, &d);
								}
							}
						}
					CFRelease(btmm);
					}
				}

			if (BrowseDomains)
				{
				CFArrayRef browseArray = CFDictionaryGetValue(dict, CFSTR("BrowseDomains"));
				if (browseArray)
					{
					for (i = 0; i < CFArrayGetCount(browseArray); i++)
						{
						CFDictionaryRef browseDict = CFArrayGetValueAtIndex(browseArray, i);
						if (browseDict && DDNSSettingEnabled(browseDict))
							{
							CFStringRef name = CFDictionaryGetValue(browseDict, CFSTR("Domain"));
							if (name)
								{
								if (!CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8) ||
									!MakeDomainNameFromDNSNameString(&d, buf) || !d.c[0])
									LogMsg("GetUserSpecifiedDDNSConfig SCDynamicStore bad DDNS browsing domain: %s", buf[0] ? buf : "(unknown)");
								else
									{
									debugf("GetUserSpecifiedDDNSConfig SCDynamicStore DDNS browsing domain: %s", buf);
									AppendDNameListElem(&BrowseDomains, 0, &d);
									}
								}
							}
						}
					}
				}
			CFRelease(dict);
			}

		if (setservers || setsearch)
			{
			CFStringRef key = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL, kSCDynamicStoreDomainState, kSCEntNetDNS);
			if (key)
				{
				CFDictionaryRef dict = SCDynamicStoreCopyValue(store, key);
				if (dict)
					{
					if (setservers)
						{
						CFArrayRef values = CFDictionaryGetValue(dict, kSCPropNetDNSServerAddresses);
						if (values)
							{
							for (i = 0; i < CFArrayGetCount(values); i++)
								{
								CFStringRef s = CFArrayGetValueAtIndex(values, i);
								char buf[256];
								mDNSAddr addr = { mDNSAddrType_IPv4, { { { 0 } } } };
								if (s && CFStringGetCString(s, buf, 256, kCFStringEncodingUTF8) &&
									inet_aton(buf, (struct in_addr *) &addr.ip.v4))
									mDNS_AddDNSServer(m, mDNSNULL, mDNSInterface_Any, &addr, UnicastDNSPort);
								}
							}
						}
					if (setsearch)
						{
						// Add the manual and/or DHCP-dicovered search domains
						CFArrayRef searchDomains = CFDictionaryGetValue(dict, kSCPropNetDNSSearchDomains);
						if (searchDomains)
							{
							for (i = 0; i < CFArrayGetCount(searchDomains); i++)
								{
								CFStringRef s = CFArrayGetValueAtIndex(searchDomains, i);
								if (s && CFStringGetCString(s, buf, sizeof(buf), kCFStringEncodingUTF8))
									mDNS_AddSearchDomain_CString(buf);
								}
							}
						else	// No kSCPropNetDNSSearchDomains, so use kSCPropNetDNSDomainName
							{
							// Due to the vagaries of Apple's SystemConfiguration and dnsinfo.h APIs, if there are no search domains
							// listed, then you're supposed to interpret the "domain" field as also being the search domain, but if
							// there *are* search domains listed, then you're supposed to ignore the "domain" field completely and
							// instead use the search domain list as the sole authority for what domains to search and in what order
							// (and the domain from the "domain" field will also appear somewhere in that list).
							CFStringRef string = CFDictionaryGetValue(dict, kSCPropNetDNSDomainName);
							if (string && CFStringGetCString(string, buf, sizeof(buf), kCFStringEncodingUTF8))
								mDNS_AddSearchDomain_CString(buf);
							}
						}
					CFRelease(dict);
					}
				CFRelease(key);
				}
			}

		CFRelease(store);
		}
	}

mDNSexport mStatus mDNSPlatformGetPrimaryInterface(mDNS *const m, mDNSAddr *v4, mDNSAddr *v6, mDNSAddr *r)
	{
	SCDynamicStoreRef	store	= NULL;
	CFDictionaryRef		dict	= NULL;
	CFStringRef			key		= NULL;
	CFStringRef			string	= NULL;
	int					nAdditions = 0;
	int					nDeletions = 0;
	char				buf[256];
	mStatus				err		= 0;

	// get IPv4 settings

	store = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:mDNSPlatformGetPrimaryInterface"), NULL, NULL);
	require_action(store, exit, err = mStatus_UnknownErr);

	key = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL, kSCDynamicStoreDomainState, kSCEntNetIPv4);
	require_action(key, exit, err = mStatus_UnknownErr);

	dict = SCDynamicStoreCopyValue(store, key);
	require_action(dict, exit, err = mStatus_UnknownErr);

	// handle router changes

	r->type  = mDNSAddrType_IPv4;
	r->ip.v4 = zerov4Addr;
	 
	string = CFDictionaryGetValue(dict, kSCPropNetIPv4Router);

	if (string)
		{
		struct sockaddr_in saddr;

		if (!CFStringGetCString(string, buf, 256, kCFStringEncodingUTF8))
			LogMsg("Could not convert router to CString");
		else
			{
			saddr.sin_len = sizeof(saddr);
			saddr.sin_family = AF_INET;
			saddr.sin_port = 0;
			inet_aton(buf, &saddr.sin_addr);

			if (AddrRequiresPPPConnection((struct sockaddr *)&saddr)) debugf("Ignoring router %s (requires PPP connection)", buf);
			else *(in_addr_t *)&r->ip.v4 = saddr.sin_addr.s_addr;
			}
		}

	// handle primary interface changes
	// if we gained or lost DNS servers (e.g. logged into VPN) "toggle" primary address so it gets re-registered even if it is unchanged
	if (nAdditions || nDeletions) mDNS_SetPrimaryInterfaceInfo(m, NULL, NULL, NULL);

	string = CFDictionaryGetValue(dict, kSCDynamicStorePropNetPrimaryInterface);

	if (string)
		{
		mDNSBool HavePrimaryGlobalv6 = mDNSfalse;  // does the primary interface have a global v6 address?
		struct ifaddrs *ifa = myGetIfAddrs(1);

		*v4 = *v6 = zeroAddr;

		if (!CFStringGetCString(string, buf, 256, kCFStringEncodingUTF8)) { LogMsg("Could not convert router to CString"); goto exit; }

		// find primary interface in list
		while (ifa && (mDNSIPv4AddressIsZero(v4->ip.v4) || mDNSv4AddressIsLinkLocal(&v4->ip.v4) || !HavePrimaryGlobalv6))
			{
			mDNSAddr tmp6 = zeroAddr;
			if (!strcmp(buf, ifa->ifa_name))
				{
				if (ifa->ifa_addr->sa_family == AF_INET)
					{
					if (mDNSIPv4AddressIsZero(v4->ip.v4) || mDNSv4AddressIsLinkLocal(&v4->ip.v4)) SetupAddr(v4, ifa->ifa_addr);
					}
				else if (ifa->ifa_addr->sa_family == AF_INET6)
					{
					SetupAddr(&tmp6, ifa->ifa_addr);
					if (tmp6.ip.v6.b[0] >> 5 == 1)   // global prefix: 001
						{ HavePrimaryGlobalv6 = mDNStrue; *v6 = tmp6; }
					}
				}
			else
				{
				// We'll take a V6 address from the non-primary interface if the primary interface doesn't have a global V6 address
				if (!HavePrimaryGlobalv6 && ifa->ifa_addr->sa_family == AF_INET6 && !v6->ip.v6.b[0])
					{
					SetupAddr(&tmp6, ifa->ifa_addr);
					if (tmp6.ip.v6.b[0] >> 5 == 1) *v6 = tmp6;
					}
				}
			ifa = ifa->ifa_next;
			}

		// Note that while we advertise v6, we still require v4 (possibly NAT'd, but not link-local) because we must use
		// V4 to communicate w/ our DNS server
		}

	exit:
	if (dict) CFRelease(dict);
	if (key) CFRelease(key);
	if (store) CFRelease(store);
	return err;
	}

mDNSexport void mDNSPlatformDynDNSHostNameStatusChanged(const domainname *const dname, const mStatus status)
	{
	SCDynamicStoreRef store = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:mDNSPlatformDynDNSHostNameStatusChanged"), NULL, NULL);
	if (store)
		{
		char uname[MAX_ESCAPED_DOMAIN_NAME];	// Max legal C-string name, including terminating NUL
		char *p;

		ConvertDomainNameToCString(dname, uname);
		p = uname;

		while (*p)
			{
			*p = tolower(*p);
			if (!(*(p+1)) && *p == '.') *p = 0; // if last character, strip trailing dot
			p++;
			}

		// We need to make a CFDictionary called "State:/Network/DynamicDNS" containing (at present) a single entity.
		// That single entity is a CFDictionary with name "HostNames".
		// The "HostNames" CFDictionary contains a set of name/value pairs, where the each name is the FQDN
		// in question, and the corresponding value is a CFDictionary giving the state for that FQDN.
		// (At present we only support a single FQDN, so this dictionary holds just a single name/value pair.)
		// The CFDictionary for each FQDN holds (at present) a single name/value pair,
		// where the name is "Status" and the value is a CFNumber giving an errror code (with zero meaning success).

			{
			const CFStringRef StateKeys [1] = { CFSTR("HostNames") };
			const CFStringRef HostKeys  [1] = { CFStringCreateWithCString(NULL, uname, kCFStringEncodingUTF8) };
			const CFStringRef StatusKeys[1] = { CFSTR("Status") };
			if (!HostKeys[0]) LogMsg("SetDDNSNameStatus: CFStringCreateWithCString(%s) failed", uname);
			else
				{
				const CFNumberRef StatusVals[1] = { CFNumberCreate(NULL, kCFNumberSInt32Type, &status) };
				if (!StatusVals[0]) LogMsg("SetDDNSNameStatus: CFNumberCreate(%ld) failed", status);
				else
					{
					const CFDictionaryRef HostVals[1] = { CFDictionaryCreate(NULL, (void*)StatusKeys, (void*)StatusVals, 1, NULL, NULL) };
					if (HostVals[0])
						{
						const CFDictionaryRef StateVals[1] = { CFDictionaryCreate(NULL, (void*)HostKeys, (void*)HostVals, 1, NULL, NULL) };
						if (StateVals[0])
							{
							CFDictionaryRef StateDict = CFDictionaryCreate(NULL, (void*)StateKeys, (void*)StateVals, 1, NULL, NULL);
							if (StateDict)
								{
								SCDynamicStoreSetValue(store, CFSTR("State:/Network/DynamicDNS"), StateDict);
								CFRelease(StateDict);
								}
							CFRelease(StateVals[0]);
							}
						CFRelease(HostVals[0]);
						}
					CFRelease(StatusVals[0]);
					}
				CFRelease(HostKeys[0]);
				}
			CFRelease(store);
			}
		}
	}

mDNSlocal mDNSBool IsTunnelModeDomain(const domainname *d)
	{
	static const domainname *mmc = (const domainname*) "\x7" "members" "\x3" "mac" "\x3" "com";
	const domainname *d1 = mDNSNULL;	// TLD
	const domainname *d2 = mDNSNULL;	// SLD
	const domainname *d3 = mDNSNULL;
	while (d->c[0]) { d3 = d2; d2 = d1; d1 = d; d = (const domainname*)(d->c + 1 + d->c[0]); }
	return(d3 && SameDomainName(d3, mmc));
	}

mDNSlocal void SetDomainSecrets(mDNS *m)
	{
#ifdef NO_SECURITYFRAMEWORK
	(void)m;
	LogMsg("Note: SetDomainSecrets: no keychain support");
#else

	LogOperation("SetDomainSecrets");

	// Rather than immediately deleting all keys now, we mark them for deletion in ten seconds.
	// In the case where the user simultaneously removes their DDNS host name and the key
	// for it, this gives mDNSResponder ten seconds to gracefully delete the name from the
	// server before it loses access to the necessary key. Otherwise, we'd leave orphaned
	// address records behind that we no longer have permission to delete.
	DomainAuthInfo *ptr = m->AuthInfoList;
	for (ptr = m->AuthInfoList; ptr; ptr = ptr->next)
		ptr->deltime = NonZeroTime(m->timenow + mDNSPlatformOneSecond*10);

	CFMutableArrayRef sa = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
	if (!sa) { LogMsg("SetDomainSecrets: CFArrayCreateMutable failed"); return; }

	SecKeychainRef skc;
	OSStatus err = SecKeychainCopyDefault(&skc);
	if (err) LogMsg("SetDomainSecrets: SecKeychainCopyDefault %d", err);
	else
		{
		SecKeychainSearchRef searchRef;
		err = SecKeychainSearchCreateFromAttributes(skc, kSecGenericPasswordItemClass, NULL, &searchRef);
		if (err) LogMsg("SetDomainSecrets: SecKeychainSearchCreateFromAttributes %d", err);
		else
			{
			while (1)
				{
				// Iterate through keychain items
				SecKeychainItemRef itemRef = NULL;
				err = SecKeychainSearchCopyNext(searchRef, &itemRef);
				if (err) break;

				// Get attributes and data out of itemRef
				UInt32 tags[3] = { kSecTypeItemAttr, kSecServiceItemAttr, kSecAccountItemAttr };
				SecKeychainAttributeInfo attrInfo = { 3, tags, NULL };	// Count, array of tags, array of formats
				SecKeychainAttributeList *a = NULL;
				void *secret = NULL;
				err = SecKeychainItemCopyAttributesAndData(itemRef,  &attrInfo, NULL, &a, NULL, NULL);

				// Validate that results look reasonable
				// LogMsg("* %.*s", a->attr[1].length, a->attr[1].data);
				if (err || !a)                 { LogMsg("SetSecretForDomain: SecKeychainItemCopyAttributesAndData error %d %p", err, a); goto nextitem; }
				if (a->count != 3)             { LogMsg("SetSecretForDomain: SecKeychainItemCopyAttributesAndData got wrong count %d", a->count); goto nextitem; }
				if (a->attr[0].tag != tags[0]) { LogMsg("SetSecretForDomain: SecKeychainItemCopyAttributesAndData got wrong attribute %d", 0); goto nextitem; }
				if (a->attr[1].tag != tags[1]) { LogMsg("SetSecretForDomain: SecKeychainItemCopyAttributesAndData got wrong attribute %d", 1); goto nextitem; }
				if (a->attr[2].tag != tags[2]) { LogMsg("SetSecretForDomain: SecKeychainItemCopyAttributesAndData got wrong attribute %d", 2); goto nextitem; }

				// Validate that attributes are not too large
				char dstring[4 + MAX_ESCAPED_DOMAIN_NAME];  // Extra 4 is for the "dns:" prefix
				char keynamebuf[MAX_ESCAPED_DOMAIN_NAME];   // Max legal C-string name, including terminating NUL
				if (a->attr[1].length > sizeof(dstring   )-1) { LogMsg("SetSecretForDomain: Bad kSecServiceItemAttr length %d", a->attr[1].length); goto nextitem; }
				if (a->attr[2].length > sizeof(keynamebuf)-1) { LogMsg("SetSecretForDomain: Bad kSecAccountItemAttr length %d", a->attr[2].length); goto nextitem; }

				// Get domain name for which this key applies
				// On Mac OS X on Intel, the four-character string seems to be stored backwards, at least sometimes.
				// I suspect some overenthusiastic inexperienced engineer said, "On Intel everything's backwards,
				// therefore I need to add some byte swapping in this API to make this four-character string backwards too."
				// To cope with this we allow *both* "ddns" and "sndd" as valid item types.
				if (a->attr[0].length == 4 && (!strncasecmp(a->attr[0].data, "ddns", 4) || !strncasecmp(a->attr[0].data, "sndd", 4)))
					{ memcpy(dstring, a->attr[1].data, a->attr[1].length); dstring[a->attr[1].length] = 0; }
				else if (a->attr[1].length >= 4 && strncasecmp(a->attr[1].data, "dns:", 4) == 0)
					{ memcpy(dstring, a->attr[1].data + 4, a->attr[1].length - 4); dstring[a->attr[1].length - 4] = 0; }
				else goto nextitem;

				domainname domain;
				if (!MakeDomainNameFromDNSNameString(&domain, dstring)) { LogMsg("SetSecretForDomain: bad key domain %s", dstring); goto nextitem; }

				// Get DNS key name
				memcpy(keynamebuf, a->attr[2].data, a->attr[2].length);
				keynamebuf[a->attr[2].length] = 0;
				domainname keyname;
				if (!MakeDomainNameFromDNSNameString(&keyname, keynamebuf)) { LogMsg("SetSecretForDomain: bad key name %s", keynamebuf); goto nextitem; }

				// Get DNS key data
				UInt32 secretlen;
				char keystring[1024];
				err = SecKeychainItemCopyAttributesAndData(itemRef,  NULL, NULL, NULL, &secretlen, &secret);
				if (err)                               { LogMsg("SetSecretForDomain: SecKeychainItemCopyAttributesAndData error %d", err); goto nextitem; }
				if (!secretlen || !secret)             { LogMsg("SetSecretForDomain: No shared secret"); goto nextitem; }
				if (secretlen > sizeof(keystring) - 1) { LogMsg("SetSecretForDomain: Shared secret too long: %d", secretlen); goto nextitem; }
				memcpy(keystring, secret, secretlen);
				keystring[secretlen] = 0;	// mDNS_SetSecretForDomain requires NULL-terminated C string for key

				for (ptr = m->AuthInfoList; ptr; ptr = ptr->next)
					if (SameDomainName(&ptr->domain, &domain)) break;

				// Uncomment the line below to view the keys as they're read out of the system keychain
				// DO NOT SHIP CODE THIS WAY OR YOU'LL LEAK SECRET DATA INTO A PUBLICLY READABLE FILE!
				//LogOperation("SetDomainSecrets: %##s %##s %s", &domain.c, &keyname.c, keystring);

				// If didn't find desired domain in the list, make a new entry
				if (!ptr)
					{
					ptr = (DomainAuthInfo*)mallocL("DomainAuthInfo", sizeof(*ptr));
					if (!ptr) { LogMsg("SetSecretForDomain: No memory"); goto nextitem; }
					}

				if (mDNS_SetSecretForDomain(m, ptr, &domain, &keyname, keystring, IsTunnelModeDomain(&domain)) == mStatus_BadParamErr)
					{ mDNSPlatformMemFree(ptr); goto nextitem; }

				CFStringRef cfs = CFStringCreateWithCString(NULL, dstring, kCFStringEncodingUTF8);
				if (cfs) { CFArrayAppendValue(sa, cfs); CFRelease(cfs); }

				nextitem:
				SecKeychainItemFreeAttributesAndData(a, secret);
				CFRelease(itemRef);
				}
			CFRelease(searchRef);
			}
		CFRelease(skc);
		}
	SCDynamicStoreRef store = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:SetDomainSecrets"), NULL, NULL);
	if (!store) LogMsg("SetDomainSecrets: SCDynamicStoreCreate failed");
	else { SCDynamicStoreSetValue(store, CFSTR("State:/Network/" kDNSServiceCompPrivateDNS), sa); CFRelease(store); }
	CFRelease(sa);

	#if APPLE_OSX_mDNSResponder
		{
		if (m->AutoTunnelHostAddr.b[0])
			if (m->TunnelClients || TunnelServers(m))
				SetupLocalAutoTunnelInterface(m);
		}
	#endif APPLE_OSX_mDNSResponder

#endif /* NO_SECURITYFRAMEWORK */
	}

mDNSlocal void SetLocalDomains(void)
	{
	CFMutableArrayRef sa = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
	if (!sa) { LogMsg("SetLocalDomains: CFArrayCreateMutable failed"); return; }

	CFArrayAppendValue(sa, CFSTR("local"));
	CFArrayAppendValue(sa, CFSTR("254.169.in-addr.arpa"));
	CFArrayAppendValue(sa, CFSTR("8.e.f.ip6.arpa"));
	CFArrayAppendValue(sa, CFSTR("9.e.f.ip6.arpa"));
	CFArrayAppendValue(sa, CFSTR("a.e.f.ip6.arpa"));
	CFArrayAppendValue(sa, CFSTR("b.e.f.ip6.arpa"));

	SCDynamicStoreRef store = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:SetLocalDomains"), NULL, NULL);
	if (!store) LogMsg("SetLocalDomains: SCDynamicStoreCreate failed");
	else { SCDynamicStoreSetValue(store, CFSTR("State:/Network/" kDNSServiceCompMulticastDNS), sa); CFRelease(store); }
	CFRelease(sa);
	}

mDNSexport void mDNSMacOSXNetworkChanged(mDNS *const m)
	{
	LogOperation("***   Network Configuration Change   ***  (%d)%s",
		m->p->NetworkChanged ? mDNS_TimeNow(m) - m->p->NetworkChanged : 0,
		m->p->NetworkChanged ? "": " (no scheduled configuration change)");
	m->p->NetworkChanged = 0;		// If we received a network change event and deferred processing, we're now dealing with it
	mDNSs32 utc = mDNSPlatformUTC();
	MarkAllInterfacesInactive(m, utc);
	UpdateInterfaceList(m, utc);
	int nDeletions = ClearInactiveInterfaces(m, utc);
	int nAdditions = SetupActiveInterfaces(m, utc);

	#if APPLE_OSX_mDNSResponder
		{
		if (m->AutoTunnelHostAddr.b[0])
			if (m->TunnelClients || TunnelServers(m))
				SetupLocalAutoTunnelInterface(m);
		
		// Scan to find client tunnels whose questions have completed,
		// but whose local inner/outer addresses have changed since the tunnel was set up
		ClientTunnel *p;
		for (p = m->TunnelClients; p; p = p->next)
			if (p->q.ThisQInterval < 0)
				if (!mDNSSameIPv6Address(p->loc_inner, m->AutoTunnelHostAddr) ||
					!mDNSSameIPv4Address(p->loc_outer, m->AdvertisedV4.ip.v4)) AutoTunnelSetKeys(m, p, mDNStrue);
		}
	#endif APPLE_OSX_mDNSResponder

	uDNS_SetupDNSConfig(m);                           // note - call DynDNSConfigChanged *before* mDNS_UpdateLLQs
	if (nDeletions || nAdditions) mDNS_UpdateLLQs(m); // so that LLQs are restarted against the up to date name servers

	if (m->MainCallback)
		m->MainCallback(m, mStatus_ConfigChanged);
	}

mDNSlocal void NetworkChanged(SCDynamicStoreRef store, CFArrayRef changedKeys, void *context)
	{
	(void)store;        // Parameter not used
	(void)changedKeys;  // Parameter not used
	mDNS *const m = (mDNS *const)context;
	KQueueLock(m);
	mDNS_Lock(m);

	mDNSs32 delay = mDNSPlatformOneSecond;					// Start off assuming a one-second delay

	int c = CFArrayGetCount(changedKeys);					// Count changes
	CFRange range = { 0, c };
	int c1 = (CFArrayContainsValue(changedKeys, range, NetworkChangedKey_Hostnames   ) != 0);
	int c2 = (CFArrayContainsValue(changedKeys, range, NetworkChangedKey_Computername) != 0);
	int c3 = (CFArrayContainsValue(changedKeys, range, NetworkChangedKey_DynamicDNS  ) != 0);
	if (c && c - c1 - c2 - c3 == 0) delay = mDNSPlatformOneSecond/20;	// If these were the only changes, shorten delay

#if LogAllOperations
	int i;
	for (i=0; i<c; i++)
		{
		char buf[256];
		if (!CFStringGetCString(CFArrayGetValueAtIndex(changedKeys, i), buf, sizeof(buf), kCFStringEncodingUTF8)) buf[0] = 0;
		LogOperation("***   NetworkChanged SC key: %s", buf);
		}
	LogOperation("***   NetworkChanged   *** %d change%s %s%s%sdelay %d",
		c, c>1?"s":"",
		c1 ? "(Local Hostname) " : "",
		c2 ? "(Computer Name) "  : "",
		c3 ? "(DynamicDNS) "     : "",
		delay);
#endif

	if (!m->p->NetworkChanged ||
		m->p->NetworkChanged - NonZeroTime(m->timenow + delay) < 0)
		m->p->NetworkChanged = NonZeroTime(m->timenow + delay);

	if (!m->SuppressSending ||
		m->SuppressSending - m->p->NetworkChanged < 0)
		m->SuppressSending = m->p->NetworkChanged;

	mDNS_Unlock(m);
	KQueueUnlock(m, "NetworkChanged");
	}

mDNSlocal mStatus WatchForNetworkChanges(mDNS *const m)
	{
	mStatus err = -1;
	SCDynamicStoreContext context = { 0, m, NULL, NULL, NULL };
	SCDynamicStoreRef     store    = SCDynamicStoreCreate(NULL, CFSTR("mDNSResponder:WatchForNetworkChanges"), NetworkChanged, &context);
	CFMutableArrayRef     keys     = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
	CFStringRef           pattern1 = SCDynamicStoreKeyCreateNetworkInterfaceEntity(NULL, kSCDynamicStoreDomainState, kSCCompAnyRegex, kSCEntNetIPv4);
	CFStringRef           pattern2 = SCDynamicStoreKeyCreateNetworkInterfaceEntity(NULL, kSCDynamicStoreDomainState, kSCCompAnyRegex, kSCEntNetIPv6);
	CFMutableArrayRef     patterns = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);

	if (!store) { LogMsg("SCDynamicStoreCreate failed: %s", SCErrorString(SCError())); goto error; }
	if (!keys || !pattern1 || !pattern2 || !patterns) goto error;

	CFArrayAppendValue(keys, NetworkChangedKey_IPv4);
	CFArrayAppendValue(keys, NetworkChangedKey_IPv6);
	CFArrayAppendValue(keys, NetworkChangedKey_Hostnames);
	CFArrayAppendValue(keys, NetworkChangedKey_Computername);
	CFArrayAppendValue(keys, NetworkChangedKey_DNS);
	CFArrayAppendValue(keys, NetworkChangedKey_DynamicDNS);
	CFArrayAppendValue(keys, NetworkChangedKey_BackToMyMac);
	CFArrayAppendValue(patterns, pattern1);
	CFArrayAppendValue(patterns, pattern2);
	CFArrayAppendValue(patterns, CFSTR("State:/Network/Interface/[^/]+/AirPort"));
	if (!SCDynamicStoreSetNotificationKeys(store, keys, patterns))
		{ LogMsg("SCDynamicStoreSetNotificationKeys failed: %s", SCErrorString(SCError())); goto error; }

	m->p->StoreRLS = SCDynamicStoreCreateRunLoopSource(NULL, store, 0);
	if (!m->p->StoreRLS) { LogMsg("SCDynamicStoreCreateRunLoopSource failed: %s", SCErrorString(SCError())); goto error; }

	CFRunLoopAddSource(CFRunLoopGetCurrent(), m->p->StoreRLS, kCFRunLoopDefaultMode);
	m->p->Store = store;
	err = 0;
	goto exit;

	error:
	if (store)    CFRelease(store);

	exit:
	if (patterns) CFRelease(patterns);
	if (pattern2) CFRelease(pattern2);
	if (pattern1) CFRelease(pattern1);
	if (keys)     CFRelease(keys);

	return(err);
	}

#ifndef NO_SECURITYFRAMEWORK
mDNSlocal OSStatus KeychainChanged(SecKeychainEvent keychainEvent, SecKeychainCallbackInfo *info, void *context)
	{
	(void) keychainEvent;
	mDNS *const m = (mDNS *const)context;
	SecKeychainRef skc;
	OSStatus err = SecKeychainCopyDefault(&skc);
	if (!err)
		{
		if (info->keychain == skc)
			{
			LogOperation("***   Keychain Changed   ***");
			KQueueLock(m);
			mDNS_Lock(m);
			SetDomainSecrets((mDNS*)context);
			mDNS_Unlock(m);
			KQueueUnlock(m, "KeychainChanged");
			}
		CFRelease(skc);
		}
	return 0;
	}
#endif

#ifndef NO_IOPOWER
mDNSlocal void PowerChanged(void *refcon, io_service_t service, natural_t messageType, void *messageArgument)
	{
	mDNS *const m = (mDNS *const)refcon;
	KQueueLock(m);
	(void)service;    // Parameter not used
	switch(messageType)
		{
		case kIOMessageCanSystemPowerOff:		debugf      ("PowerChanged kIOMessageCanSystemPowerOff (no action)");				break; // E0000240
		case kIOMessageSystemWillPowerOff:		LogOperation("PowerChanged kIOMessageSystemWillPowerOff");
												mDNSCoreMachineSleep(m, true); mDNSMacOSXNetworkChanged(m);							break; // E0000250
		case kIOMessageSystemWillNotPowerOff:	debugf      ("PowerChanged kIOMessageSystemWillNotPowerOff (no action)");			break; // E0000260
		case kIOMessageCanSystemSleep:			debugf      ("PowerChanged kIOMessageCanSystemSleep (no action)");					break; // E0000270
		case kIOMessageSystemWillSleep:			LogOperation("PowerChanged kIOMessageSystemWillSleep");
												mDNSCoreMachineSleep(m, true); mDNSMacOSXNetworkChanged(m);							break; // E0000280
		case kIOMessageSystemWillNotSleep:		debugf      ("PowerChanged kIOMessageSystemWillNotSleep (no action)");				break; // E0000290
		case kIOMessageSystemHasPoweredOn:		LogOperation("PowerChanged kIOMessageSystemHasPoweredOn");
												// If still sleeping (didn't get 'WillPowerOn' message for some reason?) wake now
												if (m->SleepState) mDNSCoreMachineSleep(m, false);
												// Just to be safe, also make sure our interface list is fully up to date, in case we
												// haven't yet received the System Configuration Framework "network changed" event that
												// we expect to receive some time shortly after the kIOMessageSystemWillPowerOn message
												mDNSMacOSXNetworkChanged(m);														break; // E0000300
		case kIOMessageSystemWillRestart:		debugf      ("PowerChanged kIOMessageSystemWillRestart (no action)");				break; // E0000310
		case kIOMessageSystemWillPowerOn:		LogOperation("PowerChanged kIOMessageSystemWillPowerOn");
												// Make sure our interface list is cleared to the empty state, then tell mDNSCore to wake
												mDNSMacOSXNetworkChanged(m); mDNSCoreMachineSleep(m, false);						break; // E0000320
		default:								LogOperation("PowerChanged unknown message %X", messageType);						break;
		}
	IOAllowPowerChange(m->p->PowerConnection, (long)messageArgument);
	KQueueUnlock(m, "Sleep/Wake");
	}
#endif /* NO_IOPOWER */

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - Initialization & Teardown
#endif

CF_EXPORT CFDictionaryRef _CFCopySystemVersionDictionary(void);
CF_EXPORT const CFStringRef _kCFSystemVersionProductNameKey;
CF_EXPORT const CFStringRef _kCFSystemVersionProductVersionKey;
CF_EXPORT const CFStringRef _kCFSystemVersionBuildVersionKey;

// Major version 6 is 10.2.x (Jaguar)
// Major version 7 is 10.3.x (Panther)
// Major version 8 is 10.4.x (Tiger)
mDNSexport int mDNSMacOSXSystemBuildNumber(char *HINFO_SWstring)
	{
	int major = 0, minor = 0;
	char letter = 0, prodname[256]="<Unknown>", prodvers[256]="<Unknown>", buildver[256]="<Unknown>";
	CFDictionaryRef vers = _CFCopySystemVersionDictionary();
	if (vers)
		{
		CFStringRef cfprodname = CFDictionaryGetValue(vers, _kCFSystemVersionProductNameKey);
		CFStringRef cfprodvers = CFDictionaryGetValue(vers, _kCFSystemVersionProductVersionKey);
		CFStringRef cfbuildver = CFDictionaryGetValue(vers, _kCFSystemVersionBuildVersionKey);
		if (cfprodname) CFStringGetCString(cfprodname, prodname, sizeof(prodname), kCFStringEncodingUTF8);
		if (cfprodvers) CFStringGetCString(cfprodvers, prodvers, sizeof(prodvers), kCFStringEncodingUTF8);
		if (cfbuildver) CFStringGetCString(cfbuildver, buildver, sizeof(buildver), kCFStringEncodingUTF8);
		sscanf(buildver, "%d%c%d", &major, &letter, &minor);
		CFRelease(vers);
		}
	if (!major) { major=8; LogMsg("Note: No Major Build Version number found; assuming 8"); }
	if (HINFO_SWstring) mDNS_snprintf(HINFO_SWstring, 256, "%s %s (%s), %s", prodname, prodvers, buildver, mDNSResponderVersionString);
	return(major);
	}

// Test to see if we're the first client running on UDP port 5353, by trying to bind to 5353 without using SO_REUSEPORT.
// If we fail, someone else got here first. That's not a big problem; we can share the port for multicast responses --
// we just need to be aware that we shouldn't expect to successfully receive unicast UDP responses.
mDNSlocal mDNSBool mDNSPlatformInit_CanReceiveUnicast(void)
	{
	int err = -1;
	int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 3)
		LogMsg("mDNSPlatformInit_CanReceiveUnicast: socket error %d errno %d (%s)", s, errno, strerror(errno));
	else
		{
		struct sockaddr_in s5353;
		s5353.sin_family      = AF_INET;
		s5353.sin_port        = MulticastDNSPort.NotAnInteger;
		s5353.sin_addr.s_addr = 0;
		err = bind(s, (struct sockaddr *)&s5353, sizeof(s5353));
		close(s);
		}

	if (err) LogMsg("No unicast UDP responses");
	else     debugf("Unicast UDP responses okay");
	return(err == 0);
	}

// Construction of Default Browse domain list (i.e. when clients pass NULL) is as follows:
// 1) query for b._dns-sd._udp.local on LocalOnly interface
//    (.local manually generated via explicit callback)
// 2) for each search domain (from prefs pane), query for b._dns-sd._udp.<searchdomain>.
// 3) for each result from (2), register LocalOnly PTR record b._dns-sd._udp.local. -> <result>
// 4) result above should generate a callback from question in (1).  result added to global list
// 5) global list delivered to client via GetSearchDomainList()
// 6) client calls to enumerate domains now go over LocalOnly interface
//    (!!!KRS may add outgoing interface in addition)

mDNSlocal mStatus mDNSPlatformInit_setup(mDNS *const m)
	{
	mStatus err;

	// In 10.4, mDNSResponder is launched very early in the boot process, while other subsystems are still in the process of starting up.
	// If we can't read the user's preferences, then we sleep a bit and try again, for up to five seconds before we give up.
	int i;
	for (i=0; i<100; i++)
		{
		domainlabel testlabel;
		testlabel.c[0] = 0;
		GetUserSpecifiedLocalHostName(&testlabel);
		if (testlabel.c[0]) break;
		usleep(50000);
		}

	m->hostlabel.c[0]        = 0;

	char *HINFO_HWstring = "Macintosh";
	char HINFO_HWstring_buffer[256];
	int    get_model[2] = { CTL_HW, HW_MODEL };
	size_t len_model = sizeof(HINFO_HWstring_buffer);
	if (sysctl(get_model, 2, HINFO_HWstring_buffer, &len_model, NULL, 0) == 0)
		HINFO_HWstring = HINFO_HWstring_buffer;

	char HINFO_SWstring[256] = "";
	if (mDNSMacOSXSystemBuildNumber(HINFO_SWstring) < 7) m->KnownBugs |= mDNS_KnownBug_PhantomInterfaces;
	if (mDNSPlatformInit_CanReceiveUnicast())            m->CanReceiveUnicastOn5353 = mDNStrue;

	mDNSu32 hlen = mDNSPlatformStrLen(HINFO_HWstring);
	mDNSu32 slen = mDNSPlatformStrLen(HINFO_SWstring);
	if (hlen + slen < 254)
		{
		m->HIHardware.c[0] = hlen;
		m->HISoftware.c[0] = slen;
		mDNSPlatformMemCopy(&m->HIHardware.c[1], HINFO_HWstring, hlen);
		mDNSPlatformMemCopy(&m->HISoftware.c[1], HINFO_SWstring, slen);
		}

 	m->p->permanentsockets.m     = m;
	m->p->permanentsockets.sktv4 = m->p->permanentsockets.sktv6 = -1;
	m->p->permanentsockets.kqsv4.KQcallback = m->p->permanentsockets.kqsv6.KQcallback = myKQSocketCallBack;
	m->p->permanentsockets.kqsv4.KQcontext  = m->p->permanentsockets.kqsv6.KQcontext  = &m->p->permanentsockets;
	m->p->permanentsockets.kqsv4.KQtask     = m->p->permanentsockets.kqsv6.KQtask     = "UDP packet reception";

	err = SetupSocket(&m->p->permanentsockets, MulticastDNSPort, AF_INET);
#ifndef NO_IPV6
	err = SetupSocket(&m->p->permanentsockets, MulticastDNSPort, AF_INET6);
#endif

	struct sockaddr_in s4;
	socklen_t n4 = sizeof(s4);
	if (getsockname(m->p->permanentsockets.sktv4, (struct sockaddr *)&s4, &n4) < 0) LogMsg("getsockname v4 error %d (%s)", errno, strerror(errno));
	else m->UnicastPort4.NotAnInteger = s4.sin_port;
#ifndef NO_IPV6
	struct sockaddr_in6 s6;
	socklen_t n6 = sizeof(s6);
	if (getsockname(m->p->permanentsockets.sktv6, (struct sockaddr *)&s6, &n6) < 0) LogMsg("getsockname v6 error %d (%s)", errno, strerror(errno));
	else m->UnicastPort6.NotAnInteger = s6.sin6_port;
#endif

	m->p->InterfaceList      = mDNSNULL;
	m->p->userhostlabel.c[0] = 0;
	m->p->usernicelabel.c[0] = 0;
	m->p->NotifyUser         = 0;

	m->AutoTunnelHostAddr.b[0] = 0;		// Zero out AutoTunnelHostAddr so UpdateInterfaceList() know it has to set it up

	mDNSs32 utc = mDNSPlatformUTC();
	UpdateInterfaceList(m, utc);
	SetupActiveInterfaces(m, utc);

	NetworkChangedKey_IPv4         = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL, kSCDynamicStoreDomainState, kSCEntNetIPv4);
	NetworkChangedKey_IPv6         = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL, kSCDynamicStoreDomainState, kSCEntNetIPv6);
	NetworkChangedKey_Hostnames    = SCDynamicStoreKeyCreateHostNames(NULL);
	NetworkChangedKey_Computername = SCDynamicStoreKeyCreateComputerName(NULL);
	NetworkChangedKey_DNS          = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL, kSCDynamicStoreDomainState, kSCEntNetDNS);
	if (!NetworkChangedKey_IPv4 || !NetworkChangedKey_IPv6 || !NetworkChangedKey_Hostnames || !NetworkChangedKey_Computername || !NetworkChangedKey_DNS)
		return(mStatus_NoMemoryErr);
	err = WatchForNetworkChanges(m);
	if (err) return(err);

	mDNS_Lock(m);
	SetDomainSecrets(m);
	SetLocalDomains();
	mDNS_Unlock(m);

#ifndef NO_SECURITYFRAMEWORK
	err = SecKeychainAddCallback(KeychainChanged, kSecAddEventMask|kSecDeleteEventMask|kSecUpdateEventMask, m);
	if (err) return(err);
#endif

#ifndef NO_IOPOWER
	m->p->PowerConnection = IORegisterForSystemPower(m, &m->p->PowerPortRef, PowerChanged, &m->p->PowerNotifier);
	if (!m->p->PowerConnection) return(-1);
	else CFRunLoopAddSource(CFRunLoopGetCurrent(), IONotificationPortGetRunLoopSource(m->p->PowerPortRef), kCFRunLoopDefaultMode);
#endif /* NO_IOPOWER */

	return(mStatus_NoError);
	}

mDNSexport mStatus mDNSPlatformInit(mDNS *const m)
	{
#if MDNS_NO_DNSINFO
	LogMsg("Note: Compiled without Apple-specific Split-DNS support");
#endif

	mStatus result = mDNSPlatformInit_setup(m);

	// We don't do asynchronous initialization on OS X, so by the time we get here the setup will already
	// have succeeded or failed -- so if it succeeded, we should just call mDNSCoreInitComplete() immediately
	if (result == mStatus_NoError) mDNSCoreInitComplete(m, mStatus_NoError);
	return(result);
	}

mDNSexport void mDNSPlatformClose(mDNS *const m)
	{
	if (m->p->PowerConnection)
		{
		CFRunLoopRemoveSource(CFRunLoopGetCurrent(), IONotificationPortGetRunLoopSource(m->p->PowerPortRef), kCFRunLoopDefaultMode);
#ifndef NO_IOPOWER
		// According to <http://developer.apple.com/qa/qa2004/qa1340.html>, a single call
		// to IORegisterForSystemPower creates *three* objects that need to be disposed individually:
		IODeregisterForSystemPower(&m->p->PowerNotifier);
		IOServiceClose            ( m->p->PowerConnection);
		IONotificationPortDestroy ( m->p->PowerPortRef);
#endif /* NO_IOPOWER */
		m->p->PowerConnection = 0;
		}

	if (m->p->Store)
		{
		CFRunLoopRemoveSource(CFRunLoopGetCurrent(), m->p->StoreRLS, kCFRunLoopDefaultMode);
		CFRunLoopSourceInvalidate(m->p->StoreRLS);
		CFRelease(m->p->StoreRLS);
		CFRelease(m->p->Store);
		m->p->Store    = NULL;
		m->p->StoreRLS = NULL;
		}

	mDNSs32 utc = mDNSPlatformUTC();
	MarkAllInterfacesInactive(m, utc);
	ClearInactiveInterfaces(m, utc);
	CloseSocketSet(&m->p->permanentsockets);

	#if APPLE_OSX_mDNSResponder
	if (m->AutoTunnelHostAddr.b[0])
		UnaliasTunnelAddress(&m->AutoTunnelHostAddr);
	#endif // APPLE_OSX_mDNSResponder
	}

#if COMPILER_LIKES_PRAGMA_MARK
#pragma mark -
#pragma mark - General Platform Support Layer functions
#endif

mDNSexport mDNSu32 mDNSPlatformRandomSeed(void)
	{
	return(mach_absolute_time());
	}

mDNSexport mDNSs32 mDNSPlatformOneSecond = 1000;

mDNSexport mStatus mDNSPlatformTimeInit(void)
	{
	// Notes: Typical values for mach_timebase_info:
	// tbi.numer = 1000 million
	// tbi.denom =   33 million
	// These are set such that (mach_absolute_time() * numer/denom) gives us nanoseconds;
	//          numer  / denom = nanoseconds per hardware clock tick (e.g. 30);
	//          denom  / numer = hardware clock ticks per nanosecond (e.g. 0.033)
	// (denom*1000000) / numer = hardware clock ticks per millisecond (e.g. 33333)
	// So: mach_absolute_time() / ((denom*1000000)/numer) = milliseconds
	//
	// Arithmetic notes:
	// tbi.denom is at least 1, and not more than 2^32-1.
	// Therefore (tbi.denom * 1000000) is at least one million, but cannot overflow a uint64_t.
	// tbi.denom is at least 1, and not more than 2^32-1.
	// Therefore clockdivisor should end up being a number roughly in the range 10^3 - 10^9.
	// If clockdivisor is less than 10^3 then that means that the native clock frequency is less than 1MHz,
	// which is unlikely on any current or future Macintosh.
	// If clockdivisor is greater than 10^9 then that means the native clock frequency is greater than 1000GHz.
	// When we ship Macs with clock frequencies above 1000GHz, we may have to update this code.
	struct mach_timebase_info tbi;
	kern_return_t result = mach_timebase_info(&tbi);
	if (result == KERN_SUCCESS) clockdivisor = ((uint64_t)tbi.denom * 1000000) / tbi.numer;
	return(result);
	}

mDNSexport mDNSs32 mDNSPlatformRawTime(void)
	{
	if (clockdivisor == 0) { LogMsg("mDNSPlatformRawTime called before mDNSPlatformTimeInit"); return(0); }

	static uint64_t last_mach_absolute_time = 0;
	//static uint64_t last_mach_absolute_time = 0x8000000000000000LL;	// Use this value for testing the alert display
	uint64_t this_mach_absolute_time = mach_absolute_time();
	if ((int64_t)this_mach_absolute_time - (int64_t)last_mach_absolute_time < 0)
		{
		LogMsg("mDNSPlatformRawTime: last_mach_absolute_time %08X%08X", last_mach_absolute_time);
		LogMsg("mDNSPlatformRawTime: this_mach_absolute_time %08X%08X", this_mach_absolute_time);
		// Update last_mach_absolute_time *before* calling NotifyOfElusiveBug()
		last_mach_absolute_time = this_mach_absolute_time;
		// Only show "mach_absolute_time went backwards" notice on 10.4 (build 8xyyy) or later.
		// (This bug happens all the time on 10.3, and we know that's not going to be fixed.)
		if (mDNSMacOSXSystemBuildNumber(NULL) >= 8)
			NotifyOfElusiveBug("mach_absolute_time went backwards!",
				"This error occurs from time to time, often on newly released hardware, "
				"and usually the exact cause is different in each instance.\r\r"
				"Please file a new Radar bug report with the title “mach_absolute_time went backwards” "
				"and assign it to Radar Component “Kernel” Version “X”.");
		}
	last_mach_absolute_time = this_mach_absolute_time;

	return((mDNSs32)(this_mach_absolute_time / clockdivisor));
	}

mDNSexport mDNSs32 mDNSPlatformUTC(void)
	{
	return time(NULL);
	}

// Locking is a no-op here, because we're single-threaded with a CFRunLoop, so we can never interrupt ourselves
mDNSexport void     mDNSPlatformLock   (const mDNS *const m) { (void)m; }
mDNSexport void     mDNSPlatformUnlock (const mDNS *const m) { (void)m; }
mDNSexport void     mDNSPlatformStrCopy(      void *dst, const void *src)              { strcpy((char *)dst, (char *)src); }
mDNSexport mDNSu32  mDNSPlatformStrLen (                 const void *src)              { return(strlen((char*)src)); }
mDNSexport void     mDNSPlatformMemCopy(      void *dst, const void *src, mDNSu32 len) { memcpy(dst, src, len); }
mDNSexport mDNSBool mDNSPlatformMemSame(const void *dst, const void *src, mDNSu32 len) { return(memcmp(dst, src, len) == 0); }
mDNSexport void     mDNSPlatformMemZero(      void *dst,                  mDNSu32 len) { bzero(dst, len); }
#if !(APPLE_OSX_mDNSResponder && MACOSX_MDNS_MALLOC_DEBUGGING)
mDNSexport void *   mDNSPlatformMemAllocate(mDNSu32 len) { return(mallocL("mDNSPlatformMemAllocate", len)); }
#endif
mDNSexport void     mDNSPlatformMemFree    (void *mem)   { freeL("mDNSPlatformMemFree", mem); }
