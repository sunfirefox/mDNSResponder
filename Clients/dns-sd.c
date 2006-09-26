/* -*- Mode: C; tab-width: 4 -*-
 *
 * Copyright (c) 2002-2006 Apple Computer, Inc. All rights reserved.
 *
 * Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
 * ("Apple") in consideration of your agreement to the following terms, and your
 * use, installation, modification or redistribution of this Apple software
 * constitutes acceptance of these terms.  If you do not agree with these terms,
 * please do not use, install, modify or redistribute this Apple software.
 *
 * In consideration of your agreement to abide by the following terms, and subject
 * to these terms, Apple grants you a personal, non-exclusive license, under Apple's
 * copyrights in this original Apple software (the "Apple Software"), to use,
 * reproduce, modify and redistribute the Apple Software, with or without
 * modifications, in source and/or binary forms; provided that if you redistribute
 * the Apple Software in its entirety and without modifications, you must retain
 * this notice and the following text and disclaimers in all such redistributions of
 * the Apple Software.  Neither the name, trademarks, service marks or logos of
 * Apple Computer, Inc. may be used to endorse or promote products derived from the
 * Apple Software without specific prior written permission from Apple.  Except as
 * expressly stated in this notice, no other rights or licenses, express or implied,
 * are granted by Apple herein, including but not limited to any patent rights that
 * may be infringed by your derivative works or by other works in which the Apple
 * Software may be incorporated.
 *
 * The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 * COMBINATION WITH YOUR PRODUCTS.
 *
 * IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
 * OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Formatting notes:
 * This code follows the "Whitesmiths style" C indentation rules. Plenty of discussion
 * on C indentation can be found on the web, such as <http://www.kafejo.com/komp/1tbs.htm>,
 * but for the sake of brevity here I will say just this: Curly braces are not syntactially
 * part of an "if" statement; they are the beginning and ending markers of a compound statement;
 * therefore common sense dictates that if they are part of a compound statement then they
 * should be indented to the same level as everything else in that compound statement.
 * Indenting curly braces at the same level as the "if" implies that curly braces are
 * part of the "if", which is false. (This is as misleading as people who write "char* x,y;"
 * thinking that variables x and y are both of type "char*" -- and anyone who doesn't
 * understand why variable y is not of type "char*" just proves the point that poor code
 * layout leads people to unfortunate misunderstandings about how the C language really works.)

To build this tool, copy and paste the following into a command line:

OS X:
gcc dns-sd.c -o dns-sd

POSIX systems:
gcc dns-sd.c -o dns-sd -I../mDNSShared -ldns_sd

Windows:
cl dns-sd.c -I../mDNSShared -DNOT_HAVE_GETOPT ws2_32.lib ..\mDNSWindows\DLL\Release\dnssd.lib
(may require that you run a Visual Studio script such as vsvars32.bat first)
*/

// For testing changes to dnssd_clientstub.c, uncomment this line and the code will be compiled
// with an embedded copy of the client stub instead of linking the system library version at runtime.
//#define TEST_NEW_CLIENTSTUB 1

#include <ctype.h>
#include <stdio.h>			// For stdout, stderr
#include <stdlib.h>			// For exit()
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <errno.h>			// For errno, EINTR
#include <time.h>
#include <sys/types.h>		// For u_char

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
typedef int        pid_t;
#define getpid     _getpid
#define strcasecmp _stricmp
#define snprintf   _snprintf
static const char kFilePathSep = '\\';
#else
#include <unistd.h>			// For getopt() and optind
#include <netdb.h>			// For getaddrinfo()
#include <sys/time.h>		// For struct timeval
#include <sys/socket.h>		// For AF_INET
#include <netinet/in.h>		// For struct sockaddr_in()
#include <arpa/inet.h>		// For inet_addr()
static const char kFilePathSep = '/';
#endif

#define TEST_NEW_CLIENTSTUB 1

#if (TEST_NEW_CLIENTSTUB && !defined(__APPLE_API_PRIVATE))
#define __APPLE_API_PRIVATE 1
#endif

#include "dns_sd.h"

#ifdef TEST_NEW_CLIENTSTUB
#include "../mDNSShared/dnssd_clientstub.c"
#endif

//*************************************************************************************************************
// Globals

typedef union { unsigned char b[2]; unsigned short NotAnInteger; } Opaque16;

static int operation;
static uint32_t opinterface = kDNSServiceInterfaceIndexAny;
static DNSServiceRef client  = NULL;
static DNSServiceRef client2 = NULL;
static int num_printed;
static char addtest = 0;
static DNSRecordRef record = NULL;
static char myhinfoW[14] = "\002PC\012Windows XP";
static char myhinfoX[ 9] = "\003Mac\004OS X";
static char updatetest[3] = "\002AA";
static char bigNULL[8200];

// Note: the select() implementation on Windows (Winsock2) fails with any timeout much larger than this
#define LONG_TIME 100000000

static volatile int stopNow = 0;
static volatile int timeOut = LONG_TIME;

//*************************************************************************************************************
// Supporting Utility Function

static uint16_t GetRRType(const char *s)
	{
	if      (!strcasecmp(s, "A"       )) return(kDNSServiceType_A);
	else if (!strcasecmp(s, "NS"      )) return(kDNSServiceType_NS);
	else if (!strcasecmp(s, "MD"      )) return(kDNSServiceType_MD);
	else if (!strcasecmp(s, "MF"      )) return(kDNSServiceType_MF);
	else if (!strcasecmp(s, "CNAME"   )) return(kDNSServiceType_CNAME);
	else if (!strcasecmp(s, "SOA"     )) return(kDNSServiceType_SOA);
	else if (!strcasecmp(s, "MB"      )) return(kDNSServiceType_MB);
	else if (!strcasecmp(s, "MG"      )) return(kDNSServiceType_MG);
	else if (!strcasecmp(s, "MR"      )) return(kDNSServiceType_MR);
	else if (!strcasecmp(s, "NULL"    )) return(kDNSServiceType_NULL);
	else if (!strcasecmp(s, "WKS"     )) return(kDNSServiceType_WKS);
	else if (!strcasecmp(s, "PTR"     )) return(kDNSServiceType_PTR);
	else if (!strcasecmp(s, "HINFO"   )) return(kDNSServiceType_HINFO);
	else if (!strcasecmp(s, "MINFO"   )) return(kDNSServiceType_MINFO);
	else if (!strcasecmp(s, "MX"      )) return(kDNSServiceType_MX);
	else if (!strcasecmp(s, "TXT"     )) return(kDNSServiceType_TXT);
	else if (!strcasecmp(s, "RP"      )) return(kDNSServiceType_RP);
	else if (!strcasecmp(s, "AFSDB"   )) return(kDNSServiceType_AFSDB);
	else if (!strcasecmp(s, "X25"     )) return(kDNSServiceType_X25);
	else if (!strcasecmp(s, "ISDN"    )) return(kDNSServiceType_ISDN);
	else if (!strcasecmp(s, "RT"      )) return(kDNSServiceType_RT);
	else if (!strcasecmp(s, "NSAP"    )) return(kDNSServiceType_NSAP);
	else if (!strcasecmp(s, "NSAP_PTR")) return(kDNSServiceType_NSAP_PTR);
	else if (!strcasecmp(s, "SIG"     )) return(kDNSServiceType_SIG);
	else if (!strcasecmp(s, "KEY"     )) return(kDNSServiceType_KEY);
	else if (!strcasecmp(s, "PX"      )) return(kDNSServiceType_PX);
	else if (!strcasecmp(s, "GPOS"    )) return(kDNSServiceType_GPOS);
	else if (!strcasecmp(s, "AAAA"    )) return(kDNSServiceType_AAAA);
	else if (!strcasecmp(s, "LOC"     )) return(kDNSServiceType_LOC);
	else if (!strcasecmp(s, "NXT"     )) return(kDNSServiceType_NXT);
	else if (!strcasecmp(s, "EID"     )) return(kDNSServiceType_EID);
	else if (!strcasecmp(s, "NIMLOC"  )) return(kDNSServiceType_NIMLOC);
	else if (!strcasecmp(s, "SRV"     )) return(kDNSServiceType_SRV);
	else if (!strcasecmp(s, "ATMA"    )) return(kDNSServiceType_ATMA);
	else if (!strcasecmp(s, "NAPTR"   )) return(kDNSServiceType_NAPTR);
	else if (!strcasecmp(s, "KX"      )) return(kDNSServiceType_KX);
	else if (!strcasecmp(s, "CERT"    )) return(kDNSServiceType_CERT);
	else if (!strcasecmp(s, "A6"      )) return(kDNSServiceType_A6);
	else if (!strcasecmp(s, "DNAME"   )) return(kDNSServiceType_DNAME);
	else if (!strcasecmp(s, "SINK"    )) return(kDNSServiceType_SINK);
	else if (!strcasecmp(s, "OPT"     )) return(kDNSServiceType_OPT);
	else if (!strcasecmp(s, "TKEY"    )) return(kDNSServiceType_TKEY);
	else if (!strcasecmp(s, "TSIG"    )) return(kDNSServiceType_TSIG);
	else if (!strcasecmp(s, "IXFR"    )) return(kDNSServiceType_IXFR);
	else if (!strcasecmp(s, "AXFR"    )) return(kDNSServiceType_AXFR);
	else if (!strcasecmp(s, "MAILB"   )) return(kDNSServiceType_MAILB);
	else if (!strcasecmp(s, "MAILA"   )) return(kDNSServiceType_MAILA);
	else if (!strcasecmp(s, "ANY"     )) return(kDNSServiceType_ANY);
	else                                 return(atoi(s));
	}

//*************************************************************************************************************
// Sample callback functions for each of the operation types

static void printtimestamp(void)
	{
	struct tm tm;
	int ms;
#ifdef _WIN32
	SYSTEMTIME sysTime;
	time_t uct = time(NULL);
	tm = *localtime(&uct);
	GetLocalTime(&sysTime);
	ms = sysTime.wMilliseconds;
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	localtime_r((time_t*)&tv.tv_sec, &tm);
	ms = tv.tv_usec/1000;
#endif
	printf("%2d:%02d:%02d.%03d  ", tm.tm_hour, tm.tm_min, tm.tm_sec, ms);
	}

#define DomainMsg(X) (((X) & kDNSServiceFlagsDefault) ? "(Default)" : \
                      ((X) & kDNSServiceFlagsAdd)     ? "Added"     : "Removed")

static const char *GetNextLabel(const char *cstr, char label[64])
	{
	char *ptr = label;
	while (*cstr && *cstr != '.')								// While we have characters in the label...
		{
		char c = *cstr++;
		if (c == '\\')
			{
			c = *cstr++;
			if (isdigit(cstr[-1]) && isdigit(cstr[0]) && isdigit(cstr[1]))
				{
				int v0 = cstr[-1] - '0';						// then interpret as three-digit decimal
				int v1 = cstr[ 0] - '0';
				int v2 = cstr[ 1] - '0';
				int val = v0 * 100 + v1 * 10 + v2;
				if (val <= 255) { c = (char)val; cstr += 2; }	// If valid three-digit decimal value, use it
				}
			}
		*ptr++ = c;
		if (ptr >= label+64) return(NULL);
		}
	if (*cstr) cstr++;											// Skip over the trailing dot (if present)
	*ptr++ = 0;
	return(cstr);
	}

static void DNSSD_API enum_reply(DNSServiceRef client, const DNSServiceFlags flags, uint32_t ifIndex,
	DNSServiceErrorType errorCode, const char *replyDomain, void *context)
	{
	DNSServiceFlags partialflags = flags & ~(kDNSServiceFlagsMoreComing | kDNSServiceFlagsAdd | kDNSServiceFlagsDefault);
	int labels = 0, depth = 0, i, initial = 0;
	char text[64];
	const char *label[128];
	
	(void)client;       // Unused
	(void)ifIndex;      // Unused
	(void)context;      // Unused

	// 1. Print the header
	if (num_printed++ == 0) printf("Timestamp     Recommended %s domain\n", operation == 'E' ? "Registration" : "Browsing");
	printtimestamp();
	if (errorCode)
		printf("Error code %d\n", errorCode);
	else if (!*replyDomain)
		printf("Error: No reply domain\n");
	else
		{
		printf("%-10s", DomainMsg(flags));
		printf("%-8s", (flags & kDNSServiceFlagsMoreComing) ? "(More)" : "");
		if (partialflags) printf("Flags: %4X  ", partialflags);
		else printf("             ");
		
		// 2. Count the labels
		while (*replyDomain)
			{
			label[labels++] = replyDomain;
			replyDomain = GetNextLabel(replyDomain, text);
			}
		
		// 3. Decide if we're going to clump the last two or three labels (e.g. "apple.com", or "nicta.com.au")
		if      (labels >= 3 && replyDomain - label[labels-1] <= 3 && label[labels-1] - label[labels-2] <= 4) initial = 3;
		else if (labels >= 2 && replyDomain - label[labels-1] <= 4) initial = 2;
		else initial = 1;
		labels -= initial;
	
		// 4. Print the initial one-, two- or three-label clump
		for (i=0; i<initial; i++)
			{
			GetNextLabel(label[labels+i], text);
			if (i>0) printf(".");
			printf("%s", text);
			}
		printf("\n");
	
		// 5. Print the remainder of the hierarchy
		for (depth=0; depth<labels; depth++)
			{
			printf("                                             ");
			for (i=0; i<=depth; i++) printf("- ");
			GetNextLabel(label[labels-1-depth], text);
			printf("> %s\n", text);
			}
		}

	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static void DNSSD_API browse_reply(DNSServiceRef client, const DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode,
	const char *replyName, const char *replyType, const char *replyDomain, void *context)
	{
	char *op = (flags & kDNSServiceFlagsAdd) ? "Add" : "Rmv";
	(void)client;       // Unused
	(void)context;      // Unused
	if (num_printed++ == 0) printf("Timestamp     A/R Flags if %-25s %-25s %s\n", "Domain", "Service Type", "Instance Name");
	printtimestamp();
	if (errorCode) printf("Error code %d\n", errorCode);
	else printf("%s%6X%3d %-25s %-25s %s\n", op, flags, ifIndex, replyDomain, replyType, replyName);
	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static void ShowTXTRecord(uint16_t txtLen, const unsigned char *txtRecord)
	{
	const unsigned char *ptr = txtRecord;
	const unsigned char *max = txtRecord + txtLen;
	while (ptr < max)
		{
		const unsigned char *const end = ptr + 1 + ptr[0];
		if (end > max) { printf("<< invalid data >>"); break; }
		if (++ptr < end) printf(" ");   // As long as string is non-empty, begin with a space
		while (ptr<end)
			{
			// We'd like the output to be shell-friendly, so that it can be copied and pasted unchanged into a "dns-sd -R" command.
			// However, this is trickier than it seems. Enclosing a string in double quotes doesn't necessarily make it
			// shell-safe, because shells still expand variables like $foo even when they appear inside quoted strings.
			// Enclosing a string in single quotes is better, but when using single quotes even backslash escapes are ignored,
			// meaning there's simply no way to represent a single quote (or apostrophe) inside a single-quoted string.
			// The only remaining solution is not to surround the string with quotes at all, but instead to use backslash
			// escapes to encode spaces and all other known shell metacharacters.
			// (If we've missed any known shell metacharacters, please let us know.)
			// In addition, non-printing ascii codes (0-31) are displayed as \xHH, using a two-digit hex value.
			// Because '\' is itself a shell metacharacter (the shell escape character), it has to be escaped as "\\" to survive
			// the round-trip to the shell and back. This means that a single '\' is represented here as EIGHT backslashes:
			// The C compiler eats half of them, resulting in four appearing in the output.
			// The shell parses those four as a pair of "\\" sequences, passing two backslashes to the "dns-sd -R" command.
			// The "dns-sd -R" command interprets this single "\\" pair as an escaped literal backslash. Sigh.
			if (strchr(" &;`'\"|*?~<>^()[]{}$", *ptr)) printf("\\");
			if      (*ptr == '\\') printf("\\\\\\\\");
			else if (*ptr >= ' ' ) printf("%c",        *ptr);
			else                   printf("\\\\x%02X", *ptr);
			ptr++;
			}
		}
	}

static void DNSSD_API resolve_reply(DNSServiceRef client, const DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode,
	const char *fullname, const char *hosttarget, uint16_t opaqueport, uint16_t txtLen, const unsigned char *txtRecord, void *context)
	{
	union { uint16_t s; u_char b[2]; } port = { opaqueport };
	uint16_t PortAsNumber = ((uint16_t)port.b[0]) << 8 | port.b[1];

	(void)client;       // Unused
	(void)ifIndex;      // Unused
	(void)context;      // Unused

	printtimestamp();
	if (errorCode) printf("Error code %d\n", errorCode);
	else
		{
		printf("%s can be reached at %s:%u", fullname, hosttarget, PortAsNumber);
		if (flags) printf(" Flags: %X", flags);
		// Don't show degenerate TXT records containing nothing but a single empty string
		if (txtLen > 1) { printf("\n"); ShowTXTRecord(txtLen, txtRecord); }
		printf("\n");
		}

	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static void myTimerCallBack(void)
	{
	DNSServiceErrorType err = kDNSServiceErr_Unknown;

	switch (operation)
		{
		case 'A':
			{
			switch (addtest)
				{
				case 0: printf("Adding Test HINFO record\n");
						err = DNSServiceAddRecord(client, &record, 0, kDNSServiceType_HINFO, sizeof(myhinfoW), &myhinfoW[0], 0);
						addtest = 1;
						break;
				case 1: printf("Updating Test HINFO record\n");
						err = DNSServiceUpdateRecord(client, record, 0, sizeof(myhinfoX), &myhinfoX[0], 0);
						addtest = 2;
						break;
				case 2: printf("Removing Test HINFO record\n");
						err = DNSServiceRemoveRecord(client, record, 0);
						addtest = 0;
						break;
				}
			}
			break;

		case 'U':
			{
			if (updatetest[1] != 'Z') updatetest[1]++;
			else                      updatetest[1] = 'A';
			updatetest[0] = 3 - updatetest[0];
			updatetest[2] = updatetest[1];
			printf("Updating Test TXT record to %c\n", updatetest[1]);
			err = DNSServiceUpdateRecord(client, NULL, 0, 1+updatetest[0], &updatetest[0], 0);
			}
			break;

		case 'N':
			{
			printf("Adding big NULL record\n");
			err = DNSServiceAddRecord(client, &record, 0, kDNSServiceType_NULL, sizeof(bigNULL), &bigNULL[0], 0);
			if (err) printf("Failed: %d\n", err); else printf("Succeeded\n");
			timeOut = LONG_TIME;
			}
			break;
		}

	if (err != kDNSServiceErr_NoError)
		{
		fprintf(stderr, "DNSService call failed %ld\n", (long int)err);
		stopNow = 1;
		}
	}

static void DNSSD_API reg_reply(DNSServiceRef client, const DNSServiceFlags flags, DNSServiceErrorType errorCode,
	const char *name, const char *regtype, const char *domain, void *context)
	{
	(void)client;   // Unused
	(void)flags;    // Unused
	(void)context;  // Unused

	printf("Got a reply for %s.%s%s: ", name, regtype, domain);

	if (errorCode == kDNSServiceErr_NoError)
		{
		printf("Name now registered and active\n");
		if (operation == 'A' || operation == 'U' || operation == 'N') timeOut = 5;
		}
	else if (errorCode == kDNSServiceErr_NameConflict)
		{
		printf("Name in use, please choose another\n");
		exit(-1);
		}
	else
		printf("Error %d\n", errorCode);

	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static void DNSSD_API qr_reply(DNSServiceRef sdRef, const DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode,
	const char *fullname, uint16_t rrtype, uint16_t rrclass, uint16_t rdlen, const void *rdata, uint32_t ttl, void *context)
	{
	char *op = (flags & kDNSServiceFlagsAdd) ? "Add" : "Rmv";
	const unsigned char *rd  = rdata;
	const unsigned char *end = (const unsigned char *) rdata + rdlen;
	char rdb[1000];
	int unknowntype = 0;

	(void)sdRef;    // Unused
	(void)flags;    // Unused
	(void)ifIndex;  // Unused
	(void)ttl;      // Unused
	(void)context;  // Unused

	if (num_printed++ == 0) printf("Timestamp     A/R Flags if %-30s%4s%4s Rdata\n", "Name", "T", "C");
	printtimestamp();
	if (errorCode)
		printf("Error code %d\n", errorCode);
	else
		{
		switch (rrtype)
			{
			case kDNSServiceType_A: sprintf(rdb, "%d.%d.%d.%d", rd[0], rd[1], rd[2], rd[3]); break;
			case kDNSServiceType_AAAA: sprintf(rdb, "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
				rd[0x0], rd[0x1], rd[0x2], rd[0x3], rd[0x4], rd[0x5], rd[0x6], rd[0x7],
				rd[0x8], rd[0x9], rd[0xA], rd[0xB], rd[0xC], rd[0xD], rd[0xE], rd[0xF]); break;
				break;
			default : sprintf(rdb, "%d bytes%s", rdlen, rdlen ? ":" : ""); unknowntype = 1; break;
			}

		printf("%s%6X%3d %-30s%4d%4d %s", op, flags, ifIndex, fullname, rrtype, rrclass, rdb);
		if (unknowntype) while (rd < end) printf(" %02X", *rd++);
		printf("\n");
	
		if (operation == 'C')
			if (flags & kDNSServiceFlagsAdd)
				DNSServiceReconfirmRecord(flags, ifIndex, fullname, rrtype, rrclass, rdlen, rdata);
		}

	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static void DNSSD_API port_mapping_create_reply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode, uint32_t publicAddress, uint8_t protocol, uint16_t privatePort, uint16_t publicPort, uint32_t ttl, void * context)
	{
	(void)sdRef;       // Unused
	(void)context;     // Unused
	(void)flags;       // Unused
	
	if (num_printed++ == 0) printf("Timestamp     if   %-20s %-15s %-15s %-15s %s\n", "Public Address", "Protocol", "Private Port", "Public Port", "TTL");
	printtimestamp();
	if (errorCode) printf("Error code %d\n", errorCode);
	else
		{
		const unsigned char * digits = ( const unsigned char* ) &publicAddress;
		char                  addr[256];

		sprintf(addr, "%d.%d.%d.%d", digits[0], digits[1], digits[2], digits[3]);
		printf("%-4d %-20s %-15d %-15d %-15d %d\n", ifIndex, addr, protocol, privatePort, publicPort, ttl);
		}
	fflush(stdout);
	}

//*************************************************************************************************************
// The main test function

static void HandleEvents(void)
	{
	int dns_sd_fd  = client  ? DNSServiceRefSockFD(client ) : -1;
	int dns_sd_fd2 = client2 ? DNSServiceRefSockFD(client2) : -1;
	int nfds = dns_sd_fd + 1;
	fd_set readfds;
	struct timeval tv;
	int result;
	
	if (dns_sd_fd2 > dns_sd_fd) nfds = dns_sd_fd2 + 1;

	while (!stopNow)
		{
		// 1. Set up the fd_set as usual here.
		// This example client has no file descriptors of its own,
		// but a real application would call FD_SET to add them to the set here
		FD_ZERO(&readfds);

		// 2. Add the fd for our client(s) to the fd_set
		if (client ) FD_SET(dns_sd_fd , &readfds);
		if (client2) FD_SET(dns_sd_fd2, &readfds);

		// 3. Set up the timeout.
		tv.tv_sec  = timeOut;
		tv.tv_usec = 0;

		result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
		if (result > 0)
			{
			DNSServiceErrorType err = kDNSServiceErr_NoError;
			if      (client  && FD_ISSET(dns_sd_fd , &readfds)) err = DNSServiceProcessResult(client );
			else if (client2 && FD_ISSET(dns_sd_fd2, &readfds)) err = DNSServiceProcessResult(client2);
			if (err) { fprintf(stderr, "DNSServiceProcessResult returned %d\n", err); stopNow = 1; }
			}
		else if (result == 0)
			myTimerCallBack();
		else
			{
			printf("select() returned %d errno %d %s\n", result, errno, strerror(errno));
			if (errno != EINTR) stopNow = 1;
			}
		}
	}

static int getfirstoption( int argc, char **argv, const char *optstr, int *pOptInd)
// Return the recognized option in optstr and the option index of the next arg.
#if NOT_HAVE_GETOPT
	{
	int i;
	for ( i=1; i < argc; i++)
		{
		if ( argv[i][0] == '-' && &argv[i][1] && 
			 NULL != strchr( optstr, argv[i][1]))
			{
			*pOptInd = i + 1;
			return argv[i][1];
			}
		}
	return -1;
	}
#else
	{
	int operation = getopt(argc, (char * const *)argv, optstr);
	*pOptInd = optind;
	return operation;
	}
#endif

static void DNSSD_API MyRegisterRecordCallback(DNSServiceRef service, DNSRecordRef record, const DNSServiceFlags flags,
    DNSServiceErrorType errorCode, void * context)
	{
	char *name = (char *)context;
	
	(void)service;	// Unused
	(void)record;	// Unused
	(void)flags;	// Unused
	
	printf("Got a reply for %s: ", name);
	switch (errorCode)
		{
		case kDNSServiceErr_NoError:      printf("Name now registered and active\n"); break;
		case kDNSServiceErr_NameConflict: printf("Name in use, please choose another\n"); exit(-1);
		default:                          printf("Error %d\n", errorCode); break;
		}
	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static unsigned long getip(const char *const name)
	{
	unsigned long ip = 0;
	struct addrinfo hints;
	struct addrinfo * addrs = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	
	if (getaddrinfo(name, NULL, &hints, &addrs) == 0)
		{
		ip = ((struct sockaddr_in*) addrs->ai_addr)->sin_addr.s_addr;
		}

	if (addrs)
		{
		freeaddrinfo(addrs);
		}

	return(ip);
	}

static DNSServiceErrorType RegisterProxyAddressRecord(DNSServiceRef *sdRef, const char *host, const char *ip)
	{
	// Call getip() after the call DNSServiceCreateConnection().
	// On the Win32 platform, WinSock must be initialized for getip() to succeed.
	// Any DNSService* call will initialize WinSock for us, so we make sure
	// DNSServiceCreateConnection() is called before getip() is.
	unsigned long addr = 0;
	DNSServiceErrorType err = DNSServiceCreateConnection(sdRef);
	if (err) { fprintf(stderr, "DNSServiceCreateConnection returned %d\n", err); return(err); }
	addr = getip(ip);
	return(DNSServiceRegisterRecord(*sdRef, &record, kDNSServiceFlagsUnique, kDNSServiceInterfaceIndexAny, host,
		kDNSServiceType_A, kDNSServiceClass_IN, sizeof(addr), &addr, 240, MyRegisterRecordCallback, (void*)host));
	// Note, should probably add support for creating proxy AAAA records too, one day
	}

#define HexVal(X) ( ((X) >= '0' && (X) <= '9') ? ((X) - '0'     ) :  \
					((X) >= 'A' && (X) <= 'F') ? ((X) - 'A' + 10) :  \
					((X) >= 'a' && (X) <= 'f') ? ((X) - 'a' + 10) : 0)

#define HexPair(P) ((HexVal((P)[0]) << 4) | HexVal((P)[1]))

static DNSServiceErrorType RegisterService(DNSServiceRef *sdRef,
	const char *nam, const char *typ, const char *dom, const char *host, const char *port, int argc, char **argv)
	{
	DNSServiceFlags flags = 0;
	uint16_t PortAsNumber = atoi(port);
	Opaque16 registerPort = { { PortAsNumber >> 8, PortAsNumber & 0xFF } };
	unsigned char txt[2048] = "";
	unsigned char *ptr = txt;
	int i;
	
	if (nam[0] == '.' && nam[1] == 0) nam = "";   // We allow '.' on the command line as a synonym for empty string
	if (dom[0] == '.' && dom[1] == 0) dom = "";   // We allow '.' on the command line as a synonym for empty string
	
	printf("Registering Service %s.%s%s%s", nam[0] ? nam : "<<Default>>", typ, dom[0] ? "." : "", dom);
	if (host && *host) printf(" host %s", host);
	printf(" port %s\n", port);

	if (argc)
		{
		for (i = 0; i < argc; i++)
			{
			const char *p = argv[i];
			*ptr = 0;
			while (*p && *ptr < 255 && ptr + 1 + *ptr < txt+sizeof(txt))
				{
				if      (p[0] != '\\' || p[1] == 0)                       { ptr[++*ptr] = *p;           p+=1; }
				else if (p[1] == 'x' && isxdigit(p[2]) && isxdigit(p[3])) { ptr[++*ptr] = HexPair(p+2); p+=4; }
				else                                                      { ptr[++*ptr] = p[1];         p+=2; }
				}
			ptr += 1 + *ptr;
			}
		ShowTXTRecord(ptr-txt, txt);
		printf("\n");
		}
	
	//flags |= kDNSServiceFlagsAllowRemoteQuery;
	//flags |= kDNSServiceFlagsNoAutoRename;
	
	return(DNSServiceRegister(sdRef, flags, opinterface, nam, typ, dom, host, registerPort.NotAnInteger, (uint16_t) (ptr-txt), txt, reg_reply, NULL));
	}

int main(int argc, char **argv)
	{
	DNSServiceErrorType err;
	char *dom;
	int optind;

	// Extract the program name from argv[0], which by convention contains the path to this executable.
	// Note that this is just a voluntary convention, not enforced by the kernel --
	// the process calling exec() can pass bogus data in argv[0] if it chooses to.
	const char *a0 = strrchr(argv[0], kFilePathSep) + 1;
	if (a0 == (const char *)1) a0 = argv[0];

	if (sizeof(argv) == 8) printf("Running in 64-bit mode\n");

	if (argc > 1 && !strcmp(argv[1], "-lo"))
		{
		argc--;
		argv++;
		opinterface = kDNSServiceInterfaceIndexLocalOnly;
		printf("Using LocalOnly\n");
		}

	if (argc > 2 && !strcmp(argv[1], "-i") && atoi(argv[2]))
		{
		opinterface = atoi(argv[2]);
		argc -= 2;
		argv += 2;
		printf("Using interface %d\n", opinterface);
		}

	if (argc < 2) goto Fail;        // Minimum command line is the command name and one argument
	operation = getfirstoption( argc, argv, "EFBLRPQCAUNTMIG", &optind);
	if (operation == -1) goto Fail;

	switch (operation)
		{
		case 'E':	printf("Looking for recommended registration domains:\n");
					err = DNSServiceEnumerateDomains(&client, kDNSServiceFlagsRegistrationDomains, opinterface, enum_reply, NULL);
					break;

		case 'F':	printf("Looking for recommended browsing domains:\n");
					err = DNSServiceEnumerateDomains(&client, kDNSServiceFlagsBrowseDomains, opinterface, enum_reply, NULL);
					//enum_reply(client, kDNSServiceFlagsAdd, 0, 0, "nicta.com.au.", NULL);
					//enum_reply(client, kDNSServiceFlagsAdd, 0, 0, "bonjour.nicta.com.au.", NULL);
					//enum_reply(client, kDNSServiceFlagsAdd, 0, 0, "ibm.com.", NULL);
					//enum_reply(client, kDNSServiceFlagsAdd, 0, 0, "dns-sd.ibm.com.", NULL);
					break;

		case 'B':	if (argc < optind+1) goto Fail;
					dom = (argc < optind+2) ? "" : argv[optind+1];
					if (dom[0] == '.' && dom[1] == 0) dom[0] = 0;   // We allow '.' on the command line as a synonym for empty string
					printf("Browsing for %s%s%s\n", argv[optind+0], dom[0] ? "." : "", dom);
					err = DNSServiceBrowse(&client, 0, opinterface, argv[optind+0], dom, browse_reply, NULL);
					break;

		case 'L':	if (argc < optind+2) goto Fail;
					dom = (argc < optind+3) ? "local" : argv[optind+2];
					if (dom[0] == '.' && dom[1] == 0) dom = "local";   // We allow '.' on the command line as a synonym for "local"
					printf("Lookup %s.%s.%s\n", argv[optind+0], argv[optind+1], dom);
					err = DNSServiceResolve(&client, 0, opinterface, argv[optind+0], argv[optind+1], dom, (DNSServiceResolveReply)resolve_reply, NULL);
					break;

		case 'R':	if (argc < optind+4) goto Fail;
					err = RegisterService(&client, argv[optind+0], argv[optind+1], argv[optind+2], NULL, argv[optind+3], argc-(optind+4), argv+(optind+4));
					break;

		case 'P':	if (argc < optind+6) goto Fail;
					err = RegisterProxyAddressRecord(&client2, argv[optind+4], argv[optind+5]);
					if (err) break;
					err = RegisterService(&client, argv[optind+0], argv[optind+1], argv[optind+2], argv[optind+4], argv[optind+3], argc-(optind+6), argv+(optind+6));
					break;

		case 'Q':
		case 'C':	{
					uint16_t rrtype, rrclass;
					DNSServiceFlags flags = kDNSServiceFlagsReturnCNAME;
					if (argc < optind+1) goto Fail;
					rrtype = (argc <= optind+1) ? kDNSServiceType_A  : GetRRType(argv[optind+1]);
					rrclass = (argc <= optind+2) ? kDNSServiceClass_IN : atoi(argv[optind+2]);
					if (rrtype == kDNSServiceType_TXT || rrtype == kDNSServiceType_PTR) flags |= kDNSServiceFlagsLongLivedQuery;
					err = DNSServiceQueryRecord(&client, flags, opinterface, argv[optind+0], rrtype, rrclass, qr_reply, NULL);
					break;
					}

		case 'A':
		case 'U':
		case 'N':	{
					Opaque16 registerPort = { { 0x12, 0x34 } };
					static const char TXT[] = "\xC" "First String" "\xD" "Second String" "\xC" "Third String";
					printf("Registering Service Test._testupdate._tcp.local.\n");
					err = DNSServiceRegister(&client, 0, opinterface, "Test", "_testupdate._tcp.", "", NULL, registerPort.NotAnInteger, sizeof(TXT)-1, TXT, reg_reply, NULL);
					break;
					}

		case 'T':	{
					Opaque16 registerPort = { { 0x23, 0x45 } };
					char TXT[1024];
					unsigned int i;
					for (i=0; i<sizeof(TXT); i++)
						if ((i & 0x1F) == 0) TXT[i] = 0x1F; else TXT[i] = 'A' + (i >> 5);
					printf("Registering Service Test._testlargetxt._tcp.local.\n");
					err = DNSServiceRegister(&client, 0, opinterface, "Test", "_testlargetxt._tcp.", "", NULL, registerPort.NotAnInteger, sizeof(TXT), TXT, reg_reply, NULL);
					break;
					}

		case 'M':	{
					pid_t pid = getpid();
					Opaque16 registerPort = { { pid >> 8, pid & 0xFF } };
					static const char TXT1[] = "\xC" "First String"  "\xD" "Second String" "\xC" "Third String";
					static const char TXT2[] = "\xD" "Fourth String" "\xC" "Fifth String"  "\xC" "Sixth String";
					printf("Registering Service Test._testdualtxt._tcp.local.\n");
					err = DNSServiceRegister(&client, 0, opinterface, "Test", "_testdualtxt._tcp.", "", NULL, registerPort.NotAnInteger, sizeof(TXT1)-1, TXT1, reg_reply, NULL);
					if (!err) err = DNSServiceAddRecord(client, &record, 0, kDNSServiceType_TXT, sizeof(TXT2)-1, TXT2, 0);
					break;
					}

		case 'I':	{
					pid_t pid = getpid();
					Opaque16 registerPort = { { pid >> 8, pid & 0xFF } };
					static const char TXT[] = "\x09" "Test Data";
					printf("Registering Service Test._testtxt._tcp.local.\n");
					err = DNSServiceRegister(&client, 0, opinterface, "Test", "_testtxt._tcp.", "", NULL, registerPort.NotAnInteger, 0, NULL, reg_reply, NULL);
					if (!err) err = DNSServiceUpdateRecord(client, NULL, 0, sizeof(TXT)-1, TXT, 0);
					break;
					}

		case 'G':   {
					if (argc != optind+4) goto Fail;
					else
						{
						uint16_t PrivatePortAsNumber = atoi(argv[optind+1]);
						uint16_t PublicPortAsNumber = atoi(argv[optind+2]);
						Opaque16 mapPrivatePort = { { PrivatePortAsNumber >> 8, PrivatePortAsNumber & 0xFF } };
						Opaque16 mapPublicPort = { { PublicPortAsNumber >> 8, PublicPortAsNumber & 0xFF } };

						err = DNSServiceNATPortMappingCreate(&client, 0, 0, atoi(argv[optind+0]), mapPrivatePort.NotAnInteger, mapPublicPort.NotAnInteger, atoi(argv[optind+3]), port_mapping_create_reply, NULL);
						}
					break;
		            }

		default: goto Fail;
		}

	if (!client || err != kDNSServiceErr_NoError) { fprintf(stderr, "DNSService call failed %ld\n", (long int)err); return (-1); }
	HandleEvents();

	// Be sure to deallocate the DNSServiceRef when you're finished
	if (client ) DNSServiceRefDeallocate(client );
	if (client2) DNSServiceRefDeallocate(client2);
	return 0;

Fail:
	fprintf(stderr, "%s -E                              (Enumerate recommended registration domains)\n", a0);
	fprintf(stderr, "%s -F                                  (Enumerate recommended browsing domains)\n", a0);
	fprintf(stderr, "%s -B        <Type> <Domain>                    (Browse for services instances)\n", a0);
	fprintf(stderr, "%s -L <Name> <Type> <Domain>                       (Look up a service instance)\n", a0);
	fprintf(stderr, "%s -R <Name> <Type> <Domain> <Port> [<TXT>...]             (Register a service)\n", a0);
	fprintf(stderr, "%s -P <Name> <Type> <Domain> <Port> <Host> <IP> [<TXT>...]              (Proxy)\n", a0);
	fprintf(stderr, "%s -Q <FQDN> <rrtype> <rrclass>             (Generic query for any record type)\n", a0);
	fprintf(stderr, "%s -C <FQDN> <rrtype> <rrclass>               (Query; reconfirming each result)\n", a0);
	fprintf(stderr, "%s -A                                  (Test Adding/Updating/Deleting a record)\n", a0);
	fprintf(stderr, "%s -U                                              (Test updating a TXT record)\n", a0);
	fprintf(stderr, "%s -N                                         (Test adding a large NULL record)\n", a0);
	fprintf(stderr, "%s -T                                        (Test creating a large TXT record)\n", a0);
	fprintf(stderr, "%s -M                  (Test creating a registration with multiple TXT records)\n", a0);
	fprintf(stderr, "%s -I               (Test registering and then immediately updating TXT record)\n", a0);
	fprintf(stderr, "%s -G <Protocol> <Private Port> <Public Port> <TTL> (Create a NAT Port Mapping)\n", a0);
	return 0;
	}
