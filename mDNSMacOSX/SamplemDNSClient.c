/*
 * Copyright (c) 2002-2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.2 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
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
 *
 *  Change History (most recent first):
 *
 * $Log: SamplemDNSClient.c,v $
 * Revision 1.35  2003/07/11 01:57:18  cheshire
 * Add checkin history header
 *
 */

#include <libc.h>
#define BIND_8_COMPAT
#include <arpa/nameser.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <CoreFoundation/CoreFoundation.h>
#include <DNSServiceDiscovery/DNSServiceDiscovery.h>

//*************************************************************************************************************
// Globals

typedef union { unsigned char b[2]; unsigned short NotAnInteger; } Opaque16;

static char operation;
static dns_service_discovery_ref client = NULL;
static int num_printed;
static char addtest = 0;
static DNSRecordReference record;
static char myhinfo9[11] = "\003Mac\006OS 9.2";
static char myhinfoX[ 9] = "\003Mac\004OS X";
static char updatetest[2] = "\001A";
static char bigNULL[4096];

//*************************************************************************************************************
// Supporting Utility Functions
//
// This code takes care of:
// 1. Extracting the mach_port_t from the dns_service_discovery_ref
// 2. Making a CFMachPortRef from it
// 3. Making a CFRunLoopSourceRef from that
// 4. Adding that source to the current RunLoop
// 5. and passing the resulting messages back to DNSServiceDiscovery_handleReply() for processing
//
// Code that's not based around a CFRunLoop will need its own mechanism to receive Mach messages
// from the mDNSResponder daemon and pass them to the DNSServiceDiscovery_handleReply() routine.
// (There is no way to automate this, because it varies depending on the application's existing
// event handling model.)

static void MyHandleMachMessage(CFMachPortRef port, void *msg, CFIndex size, void *info)
	{
    (void)port;	// Unused
    (void)size;	// Unused
    (void)info;	// Unused
	DNSServiceDiscovery_handleReply(msg);
	}

static int AddDNSServiceClientToRunLoop(dns_service_discovery_ref client)
    {
	mach_port_t port = DNSServiceDiscoveryMachPort(client);
    if (!port)
        return(-1);
    else
        {
        CFMachPortContext  context    = { 0, 0, NULL, NULL, NULL };
        Boolean            shouldFreeInfo;
        CFMachPortRef      cfMachPort = CFMachPortCreateWithPort(kCFAllocatorDefault, port, MyHandleMachMessage, &context, &shouldFreeInfo);
        CFRunLoopSourceRef rls        = CFMachPortCreateRunLoopSource(NULL, cfMachPort, 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), rls, kCFRunLoopDefaultMode);
        CFRelease(rls);
        return(0);
        }
    }

//*************************************************************************************************************
// Sample callback functions for each of the operation types

static void printtimestamp(void)
	{
	struct timeval tv;
	struct tm tm;
	gettimeofday(&tv, NULL);
	localtime_r((time_t*)&tv.tv_sec, &tm);
	printf("%d:%02d:%02d.%03d  ", tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec/1000);
	}

#define DomainMsg(X) ((X) == DNSServiceDomainEnumerationReplyAddDomain        ? "Added"     :          \
                      (X) == DNSServiceDomainEnumerationReplyAddDomainDefault ? "(Default)" :          \
                      (X) == DNSServiceDomainEnumerationReplyRemoveDomain     ? "Removed"   : "Unknown")

static void regdom_reply(DNSServiceDomainEnumerationReplyResultType resultType, const char *replyDomain,
    DNSServiceDiscoveryReplyFlags flags, void *context)
	{
    (void)context; // Unused
	printtimestamp();
	printf("Recommended Registration Domain %s %s", replyDomain, DomainMsg(resultType));
	if (flags) printf(" Flags: %X", flags);
	printf("\n");
	}

static void browsedom_reply(DNSServiceDomainEnumerationReplyResultType resultType, const char *replyDomain,
    DNSServiceDiscoveryReplyFlags flags, void *context)
	{
    (void)context; // Unused
	printtimestamp();
	printf("Recommended Browsing Domain %s %s", replyDomain, DomainMsg(resultType));
	if (flags) printf(" Flags: %X", flags);
	printf("\n");
	}

static void browse_reply(DNSServiceBrowserReplyResultType resultType,
    const char *replyName, const char *replyType, const char *replyDomain, DNSServiceDiscoveryReplyFlags flags, void *context)
	{
	char *op = (resultType == DNSServiceBrowserReplyAddInstance) ? "Add" : "Rmv";
    (void)context; // Unused
	if (num_printed++ == 0) printf("A/R Flags %-8s %-20s %s\n", "Domain", "Service Type", "Instance Name");
	printtimestamp();
	printf("%s%6X %-8s %-20s %s\n", op, flags, replyDomain, replyType, replyName);
	}

static void resolve_reply(struct sockaddr *interface, struct sockaddr *address, const char *txtRecord, DNSServiceDiscoveryReplyFlags flags, void *context)
	{
    (void)interface; // Unused
    (void)context; // Unused
	if (address->sa_family != AF_INET && address->sa_family != AF_INET6)
		printf("Unknown address family %d\n", address->sa_family);
	else
		{
        const char *src = txtRecord;
        printtimestamp();

        if (address->sa_family == AF_INET)
            {
            struct sockaddr_in *ip = (struct sockaddr_in *)address;
            union { uint32_t l; u_char b[4]; } addr = { ip->sin_addr.s_addr };
            union { uint16_t s; u_char b[2]; } port = { ip->sin_port };
            uint16_t PortAsNumber = ((uint16_t)port.b[0]) << 8 | port.b[1];
            char ipstring[16];
            sprintf(ipstring, "%d.%d.%d.%d", addr.b[0], addr.b[1], addr.b[2], addr.b[3]);
            printf("Service can be reached at   %-15s:%u", ipstring, PortAsNumber);
            }
        else if (address->sa_family == AF_INET6)
            {
            struct sockaddr_in6 *ip6 = (struct sockaddr_in6 *)address;
            u_int16_t *w = ip6->sin6_addr.__u6_addr.__u6_addr16;
            union { uint16_t s; u_char b[2]; } port = { ip6->sin6_port };
            uint16_t PortAsNumber = ((uint16_t)port.b[0]) << 8 | port.b[1];
            char ipstring[40];
            char ifname[IF_NAMESIZE + 1] = "";
            sprintf(ipstring, "%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X", w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7]);
            if (ip6->sin6_scope_id) { ifname[0] = '%';  if_indextoname(ip6->sin6_scope_id, &ifname[1]); }
            printf("%s%s:%u", ipstring, ifname, PortAsNumber);
            }
		if (flags) printf(" Flags: %X", flags);
        if (*src)
            {
            char txtInfo[64];								// Display at most first 64 characters of TXT record
            char *dst = txtInfo;
            const char *const lim = &txtInfo[sizeof(txtInfo)];
            while (*src && dst < lim-1)
            	{
            	if (*src == '\\') *dst++ = '\\';			// '\' displays as "\\"
            	if (*src >= ' ') *dst++ = *src++;			// Display normal characters as-is
            	else
            		{
            		*dst++ = '\\';							// Display a backslash
            		if (*src ==    1) *dst++ = ' ';			// String boundary displayed as "\ "
            		else									// Other chararacters displayed as "\0xHH"
            			{
            			static const char hexchars[16] = "0123456789ABCDEF";
            			*dst++ = '0';
            			*dst++ = 'x';
            			*dst++ = hexchars[*src >> 4];
            			*dst++ = hexchars[*src & 0xF];
            			}
					src++;
            		}
            	}
            *dst++ = 0;
            printf(" TXT %s", txtInfo);
            }
		printf("\n");
		}
	}

static void myCFRunLoopTimerCallBack(CFRunLoopTimerRef timer, void *info)
	{
	(void)timer;	// Parameter not used
	(void)info;		// Parameter not used
    
    switch (operation)
        {
        case 'A':
            {
            switch (addtest)
                {
                case 0: printf("Adding Test HINFO record\n");
                        record = DNSServiceRegistrationAddRecord(client, T_HINFO, sizeof(myhinfo9), &myhinfo9[0], 120);
                        addtest = 1;
                        break;
                case 1: printf("Updating Test HINFO record\n");
                        DNSServiceRegistrationUpdateRecord(client, record, sizeof(myhinfoX), &myhinfoX[0], 120);
                        addtest = 2;
                        break;
                case 2: printf("Removing Test HINFO record\n");
                        DNSServiceRegistrationRemoveRecord(client, record);
                        addtest = 0;
                        break;
                }
            }
            break;

        case 'U':
            {
            if (updatetest[1] != 'Z') updatetest[1]++;
            else                      updatetest[1] = 'A';
            printf("Updating Test TXT record to %c\n", updatetest[1]);
            DNSServiceRegistrationUpdateRecord(client, 0, sizeof(updatetest), &updatetest[0], 120);
            }
            break;

        case 'N':
            {
            printf("Adding big NULL record\n");
            DNSServiceRegistrationAddRecord(client, T_NULL, sizeof(bigNULL), &bigNULL[0], 120);
            CFRunLoopRemoveTimer(CFRunLoopGetCurrent(), timer, kCFRunLoopDefaultMode);
            }
            break;
        }
    }

static void reg_reply(DNSServiceRegistrationReplyErrorType errorCode, void *context)
	{
    (void)context; // Unused
    printf("Got a reply from the server: ");
    switch (errorCode)
        {
        case kDNSServiceDiscoveryNoError:      printf("Name now registered and active\n"); break;
        case kDNSServiceDiscoveryNameConflict: printf("Name in use, please choose another\n"); exit(-1);
        default:                               printf("Error %d\n", errorCode); return;
        }

    if (operation == 'A' || operation == 'U' || operation == 'N')
        {
        CFRunLoopTimerContext myCFRunLoopTimerContext = { 0, 0, NULL, NULL, NULL };
        CFRunLoopTimerRef timer = CFRunLoopTimerCreate(kCFAllocatorDefault,
            CFAbsoluteTimeGetCurrent() + 5.0, 5.0, 0, 1,	// Next fire time, periodic interval, flags, and order
                                myCFRunLoopTimerCallBack, &myCFRunLoopTimerContext);
        CFRunLoopAddTimer(CFRunLoopGetCurrent(), timer, kCFRunLoopDefaultMode);
        }
	}

//*************************************************************************************************************
// The main test function

int main(int argc, char **argv)
	{
	char *dom;

	if (argc < 2) goto Fail;		// Minimum command line is the command name and one argument
    operation = getopt(argc, (char * const *)argv, "EFBLRAUNTM");
	if (operation == -1) goto Fail;

    switch (operation)
        {
        case 'E':	printf("Looking for recommended registration domains:\n");
                    client = DNSServiceDomainEnumerationCreate(1, regdom_reply, nil);
                    break;

        case 'F':	printf("Looking for recommended browsing domains:\n");
                    client = DNSServiceDomainEnumerationCreate(0, browsedom_reply, nil);
                    break;

        case 'B':	if (argc < optind+1) goto Fail;
                    dom = (argc < optind+2) ? "" : argv[optind+1];
                    if (dom[0] == '.' && dom[1] == 0) dom[0] = 0;	// We allow '.' on the command line as a synonym for empty string
                    printf("Browsing for %s%s\n", argv[optind+0], dom);
                    client = DNSServiceBrowserCreate(argv[optind+0], dom, browse_reply, nil);
                    break;

        case 'L':	if (argc < optind+2) goto Fail;
                    dom = (argc < optind+3) ? "" : argv[optind+2];
                    if (dom[0] == '.' && dom[1] == 0) dom[0] = 0;	// We allow '.' on the command line as a synonym for empty string
                    printf("Lookup %s.%s%s\n", argv[optind+0], argv[optind+1], dom);
                    client = DNSServiceResolverResolve(argv[optind+0], argv[optind+1], dom, resolve_reply, nil);
                    break;

        case 'R':	if (argc < optind+4) goto Fail;
                    {
                    char *nam = argv[optind+0];
                    char *typ = argv[optind+1];
                    char *dom = argv[optind+2];
                    uint16_t PortAsNumber = atoi(argv[optind+3]);
                    Opaque16 registerPort = { { PortAsNumber >> 8, PortAsNumber & 0xFF } };
                    char txt[2048];
                    char *ptr = txt;
                    int i;

                    if (nam[0] == '.' && nam[1] == 0) nam[0] = 0;	// We allow '.' on the command line as a synonym for empty string
                    if (dom[0] == '.' && dom[1] == 0) dom[0] = 0;	// We allow '.' on the command line as a synonym for empty string

					// Copy all the TXT strings into one C string separated by ASCII-1 delimiters                    
                    for (i = optind+4; i < argc; i++)
                    	{
                    	strcpy(ptr, argv[i]);
                    	ptr += strlen(argv[i]);
                    	*ptr++ = 1;
                    	}
                    if (ptr > txt) ptr--;
                    *ptr = 0;
                    
                    printf("Registering Service %s.%s%s port %s %s\n", nam, typ, dom, argv[optind+3], txt);
                    client = DNSServiceRegistrationCreate(nam, typ, dom, registerPort.NotAnInteger, txt, reg_reply, nil);
                    break;
                    }

        case 'A':
        case 'U':
        case 'N':	{
                    Opaque16 registerPort = { { 0x12, 0x34 } };
                    static const char TXT[] = "First String\001Second String\001Third String";
                    printf("Registering Service Test._testupdate._tcp.local.\n");
                    client = DNSServiceRegistrationCreate("Test", "_testupdate._tcp.", "", registerPort.NotAnInteger, TXT, reg_reply, nil);
                    break;
                    }

        case 'T':	{
                    Opaque16 registerPort = { { 0x23, 0x45 } };
                    char TXT[512];
                    unsigned int i;
                    for (i=0; i<sizeof(TXT)-1; i++)
                        if ((i & 0x1F) == 0x1F) TXT[i] = 1; else TXT[i] = 'A' + (i >> 5);
                    TXT[i] = 0;
                    printf("Registering Service Test._testlargetxt._tcp.local.\n");
                    client = DNSServiceRegistrationCreate("Test", "_testlargetxt._tcp.", "", registerPort.NotAnInteger, TXT, reg_reply, nil);
                    break;
                    }

        case 'M':	{
                    pid_t pid = getpid();
                    Opaque16 registerPort = { { pid >> 8, pid & 0xFF } };
                    static const char TXT1[] = "First String\001Second String\001Third String";
                    static const char TXT2[] = "\x0D" "Fourth String" "\x0C" "Fifth String" "\x0C" "Sixth String";
                    printf("Registering Service Test._testdualtxt._tcp.local.\n");
                    client = DNSServiceRegistrationCreate("", "_testdualtxt._tcp.", "", registerPort.NotAnInteger, TXT1, reg_reply, nil);
                    // use "sizeof(TXT2)-1" because we don't wan't the C compiler's null byte on the end of the string
                    record = DNSServiceRegistrationAddRecord(client, T_TXT, sizeof(TXT2)-1, TXT2, 120);
                    break;
                    }

        default: goto Exit;
        }

    if (!client) { fprintf(stderr, "DNSService call failed\n"); return (-1); }
    if (AddDNSServiceClientToRunLoop(client) != 0) { fprintf(stderr, "AddDNSServiceClientToRunLoop failed\n"); return (-1); }
    printf("Talking to DNS SD Daemon at Mach port %d\n", DNSServiceDiscoveryMachPort(client));
	CFRunLoopRun();
    
    // Be sure to deallocate the dns_service_discovery_ref when you're finished
    // Note: What other cleanup has to be done here?
    // We should probably invalidate, remove and release our CFRunLoopSourceRef?
    DNSServiceDiscoveryDeallocate(client);
    
Exit:
	return 0;

Fail:
	fprintf(stderr, "%s -E                  (Enumerate recommended registration domains)\n", argv[0]);
	fprintf(stderr, "%s -F                      (Enumerate recommended browsing domains)\n", argv[0]);
	fprintf(stderr, "%s -B        <Type> <Domain>        (Browse for services instances)\n", argv[0]);
	fprintf(stderr, "%s -L <Name> <Type> <Domain>           (Look up a service instance)\n", argv[0]);
	fprintf(stderr, "%s -R <Name> <Type> <Domain> <Port> [<TXT>...] (Register a service)\n", argv[0]);
	fprintf(stderr, "%s -A                      (Test Adding/Updating/Deleting a record)\n", argv[0]);
	fprintf(stderr, "%s -U                                  (Test updating a TXT record)\n", argv[0]);
	fprintf(stderr, "%s -N                             (Test adding a large NULL record)\n", argv[0]);
	fprintf(stderr, "%s -T                            (Test creating a large TXT record)\n", argv[0]);
	fprintf(stderr, "%s -M      (Test creating a registration with multiple TXT records)\n", argv[0]);
	return 0;
	}
