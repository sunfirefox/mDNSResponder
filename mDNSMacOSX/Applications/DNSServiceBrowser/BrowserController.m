/*
 *  BrowserController.m
 *  IP Browser
 *
 *  Created by Rod Lopez <rod@apple.com> on Thu Jul 19 2001.
 *  Copyright (c) 2001 Apple Computer, Inc. All rights reserved.
 *
 *  This is experimental proof-of-concept code. Please excuse the mess.
 */

#import "BrowserController.h"
#import <DNSServiceDiscovery/DNSServiceDiscovery.h>


void
MyHandleMachMessage ( CFMachPortRef port, void * msg, CFIndex size, void * info )
{
    DNSServiceDiscovery_handleReply(msg);
}

void browse_reply (
                   int 	resultType,		// One of DNSServiceBrowserReplyResultType
                   char  	*replyName,
                   char  	*replyType,
                   char  	*replyDomain,
                   int 	flags,			// DNS Service Discovery reply flags information
                   void	*context
                   )
{
    printf("Got a reply from the server %d %s %s %s %d\n", resultType, replyName, replyType, replyDomain, flags);
    [[NSApp delegate] updateBrowseWithResult:resultType name:[NSString stringWithUTF8String:replyName] type:[NSString stringWithUTF8String:replyType] domain:[NSString stringWithUTF8String:replyDomain] flags:flags];
    return;
}

void enum_reply (
                 int 	resultType,
                 char  	*replyDomain,
                 int 	flags,
                 void	*context
                 )
{
    // printf("Got a reply from the server %d %s %d\n", resultType, replyDomain, flags);

    [[NSApp delegate] updateEnumWithResult:resultType domain:[NSString stringWithUTF8String:replyDomain] flags:flags];

    return;
}

void resolve_reply (
                    struct sockaddr 	*interface,
                    struct sockaddr 	*address,
                    char 		*txtRecord,
                    int 		flags,
                    void		*context
                    )
{

    printf("interface length = %d, port = %d, family = %d, address = %s\n", ((struct sockaddr_in *)interface)->sin_len, ((struct sockaddr_in *)interface)->sin_port, ((struct sockaddr_in *)interface)->sin_family, inet_ntoa(((struct in_addr)((struct sockaddr_in *)interface)->sin_addr)));
    printf("address length = %d, port = %d, family = %d, address = %s\n", ((struct sockaddr_in *)address)->sin_len, ((struct sockaddr_in *)address)->sin_port, ((struct sockaddr_in *)address)->sin_family, inet_ntoa(((struct in_addr)((struct sockaddr_in *)address)->sin_addr)));

    [[NSApp delegate] resolveClientWithInterface:interface address:address txtRecord:[NSString stringWithUTF8String:txtRecord]];

    return;
}

@implementation BrowserController		//Begin implementation of BrowserController methods

- (void)registerDefaults
{
    NSMutableDictionary *regDict = [NSMutableDictionary dictionary];

    NSArray *typeArray = [NSArray arrayWithObjects:@"_ftp._tcp.", @"_printer._tcp.", @"_http._tcp.", @"_afp._tcp.", nil];
    NSArray *nameArray = [NSArray arrayWithObjects:@"File Transfer (ftp)", @"Printer (lpr)", @"Web Server (http)", @"AppleShare Server (afp)", nil];

    [regDict setObject:typeArray forKey:@"SrvTypeKeys"];
    [regDict setObject:nameArray forKey:@"SrvNameKeys"];

    [[NSUserDefaults standardUserDefaults] registerDefaults:regDict];
}
    

- (id)init
{
    [self registerDefaults];
    return [super init];
}

- (void)awakeFromNib				//BrowserController startup procedure
{
    SrvType=NULL;
    Domain=NULL;
    srvtypeKeys = [NSMutableArray array];	//Define arrays for Type, Domain, and Name
    srvnameKeys = [NSMutableArray array];

    domainKeys = [NSMutableArray array];
    [domainKeys retain];

    nameKeys = [NSMutableArray array];
    [nameKeys retain];

    [srvtypeKeys retain];				//Keep arrays in memory until BrowserController closes
    [srvnameKeys retain];				//Keep arrays in memory until BrowserController closes
    [typeField setDataSource:self];		//Set application fields' data source to BrowserController
    [typeField sizeLastColumnToFit];		//and set column sizes to use their whole table's width.
    [nameField setDataSource:self];
    [nameField sizeLastColumnToFit];
    [domainField setDataSource:self];
    [domainField sizeLastColumnToFit];
    
    //[srvtypeKeys addObject:@"_ftp._tcp."];	//Add supported protocols and domains to their
    //[srvnameKeys addObject:@"File Transfer (ftp)"];
    //[srvtypeKeys addObject:@"_printer._tcp."];		//respective arrays
    //[srvnameKeys addObject:@"Printer (lpr)"];
    //[srvtypeKeys addObject:@"_http._tcp."];		//respective arrays
    //[srvnameKeys addObject:@"Web Server (http)"];
    //[srvtypeKeys addObject:@"_afp._tcp."];		//respective arrays
    //[srvnameKeys addObject:@"AppleShare Server (afp)"];

    [srvtypeKeys addObjectsFromArray:[[NSUserDefaults standardUserDefaults] arrayForKey:@"SrvTypeKeys"]];
    [srvnameKeys addObjectsFromArray:[[NSUserDefaults standardUserDefaults] arrayForKey:@"SrvNameKeys"]];

    
    [typeField reloadData];				//Reload (redraw) data in fields
    [domainField reloadData];

    [self loadDomains:self];

}

- (void)dealloc						//Deallocation method
{
    [srvtypeKeys release];
    [srvnameKeys release];
    [nameKeys release];
    [domainKeys release];
}

-(void)tableView:(NSTableView *)theTableView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
    if (row<0) return;
}

- (int)numberOfRowsInTableView:(NSTableView *)theTableView	//Begin mandatory TableView methods
{
    if (theTableView == typeField)
    {
        return [srvnameKeys count];
    }
    if (theTableView == domainField)
    {
        return [domainKeys count];
    }
    if (theTableView == nameField)
    {
        return [nameKeys count];
    }
    if (theTableView == serviceDisplayTable)
    {
        return [srvnameKeys count];
    }
    return 0;
}

- (id)tableView:(NSTableView *)theTableView objectValueForTableColumn:(NSTableColumn *)theColumn row:(int)rowIndex
{
    if (theTableView == typeField)
    {
        return [srvnameKeys objectAtIndex:rowIndex];
    }
    if (theTableView == domainField)
    {
        return [domainKeys objectAtIndex:rowIndex];
    }
    if (theTableView == nameField)
    {
        return [nameKeys objectAtIndex:rowIndex];
    }
    if (theTableView == serviceDisplayTable)
    {
        if (theColumn == typeColumn) {
            return [srvtypeKeys objectAtIndex:rowIndex];
        }
        if (theColumn == nameColumn) {
            return [srvnameKeys objectAtIndex:rowIndex];
        }
        return 0;
    }
    else
        return(0);
}						//End of mandatory TableView methods

- (IBAction)handleTypeClick:(id)sender		//Handle clicks for Type
{
    int index=[sender selectedRow];				//Find index of selected row
    if (index==-1) return;					//Error checking
    SrvType = [srvtypeKeys objectAtIndex:index];		//Save desired Type
    SrvName = [srvnameKeys objectAtIndex:index];		//Save desired Type
    if (Domain!=NULL) [self update:SrvType Domain:Domain];	//If Type and Domain are set, update records
}

- (IBAction)handleDomainClick:(id)sender			//Handle clicks for Domain
{
    int index=[sender selectedRow];				//Find index of selected row
    if (index==-1) return;					//Error checking
    Domain = [domainKeys objectAtIndex:index];			//Save desired Domain
    if (SrvType!=NULL) [self update:SrvType Domain:Domain];	//If Type and Domain are set, update records
}

- (IBAction)handleNameClick:(id)sender				//Handle clicks for Name
{
    int index=[sender selectedRow];				//Find index of selected row
    if (index==-1) return;					//Error checking
    Name=[nameKeys objectAtIndex:index];			//Save desired name

    {
        CFMachPortRef           cfMachPort;
        CFMachPortContext       context;
        Boolean                 shouldFreeInfo;
        dns_service_discovery_ref 	dns_client;
        mach_port_t			port;
        CFRunLoopSourceRef		rls;

        context.version                 = 1;
        context.info                    = 0;
        context.retain                  = NULL;
        context.release                 = NULL;
        context.copyDescription 	    = NULL;

        // start an enumerator on the local server
        dns_client = DNSServiceResolverResolve
            (
             [Name UTF8String],
             [SrvType UTF8String],
             [Domain UTF8String],
             resolve_reply,
             nil
             );

        port = DNSServiceDiscoveryMachPort(dns_client);

        if (port) {
            cfMachPort = CFMachPortCreateWithPort ( kCFAllocatorDefault, port, ( CFMachPortCallBack ) MyHandleMachMessage,&context,&shouldFreeInfo );

            /* Create and add a run loop source for the port */
            rls = CFMachPortCreateRunLoopSource(NULL, cfMachPort, 0);
            CFRunLoopAddSource(CFRunLoopGetCurrent(), rls, kCFRunLoopDefaultMode);
            CFRelease(rls);
        } else {
            printf("Could not obtain client port\n");
            return;
        }
    }
}

- (IBAction)loadDomains:(id)sender
{
    CFMachPortRef           cfMachPort;
    CFMachPortContext       context;
    Boolean                 shouldFreeInfo;
    dns_service_discovery_ref 	dns_client;
    mach_port_t			port;
    CFRunLoopSourceRef		rls;

    context.version                 = 1;
    context.info                    = 0;
    context.retain                  = NULL;
    context.release                 = NULL;
    context.copyDescription 	    = NULL;

    // start an enumerator on the local server
    dns_client =  DNSServiceDomainEnumerationCreate
        (
         0,
         enum_reply,
         nil
         );

    port = DNSServiceDiscoveryMachPort(dns_client);

    if (port) {
        cfMachPort = CFMachPortCreateWithPort ( kCFAllocatorDefault, port, ( CFMachPortCallBack ) MyHandleMachMessage,&context,&shouldFreeInfo );

        /* Create and add a run loop source for the port */
        rls = CFMachPortCreateRunLoopSource(NULL, cfMachPort, 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), rls, kCFRunLoopDefaultMode);
        CFRelease(rls);
    } else {
        printf("Could not obtain client port\n");
        return;
    }
}

- (IBAction)update:theType Domain:theDomain;		//The Big Kahuna: Fetch PTR records and update application
{
    const char * const DomainC=[theDomain UTF8String];	//Domain in C string format
    const char * const TypeC=[theType UTF8String];		//Type in C string format

    [nameKeys removeAllObjects];	//Get rid of displayed records if we're going to go get new ones
    [nameField reloadData];		//Reload (redraw) names to show the old data is gone

    // now create a browser to return the values for the nameField ...
    {
        CFMachPortRef           cfMachPort;
        CFMachPortContext       context;
        Boolean                 shouldFreeInfo;
        dns_service_discovery_ref 	dns_client;
        mach_port_t			port;
        CFRunLoopSourceRef		rls;

        context.version                 = 1;
        context.info                    = 0;
        context.retain                  = NULL;
        context.release                 = NULL;
        context.copyDescription 	    = NULL;

        // start an enumerator on the local server
        dns_client = DNSServiceBrowserCreate
            (
             TypeC,
             DomainC,
             browse_reply,
             nil
             );

        port = DNSServiceDiscoveryMachPort(dns_client);

        if (port) {
            cfMachPort = CFMachPortCreateWithPort ( kCFAllocatorDefault, port, ( CFMachPortCallBack ) MyHandleMachMessage,&context,&shouldFreeInfo );

            /* Create and add a run loop source for the port */
            rls = CFMachPortCreateRunLoopSource(NULL, cfMachPort, 0);
            CFRunLoopAddSource(CFRunLoopGetCurrent(), rls, kCFRunLoopDefaultMode);
            CFRelease(rls);
        } else {
            printf("Could not obtain client port\n");
            return;
        }
    }

}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication //Quit when main window is closed
{
    return YES;
}

- (BOOL)windowShouldClose:(NSWindow *)sender	//Save domains to our domain file when quitting
{
    [domainField reloadData];
    //[NSArchiver archiveRootObject:domainKeys toFile:@"domains.dms"];
    return YES;
}

- (void)updateEnumWithResult:(int)resultType domain:(NSString *)domain flags:(int)flags
{
    // new domain received
    if (DNSServiceDomainEnumerationReplyAddDomain == resultType || DNSServiceDomainEnumerationReplyAddDomainDefault == resultType) {
        // add the domain to the list
        [domainKeys addObject:domain];
    } else {
        // remove the domain from the list
        NSEnumerator *dmnEnum = [domainKeys objectEnumerator];
        NSString *aDomain = nil;

        while (aDomain = [dmnEnum nextObject]) {
            if ([aDomain isEqualToString:domain]) {
                [domainKeys removeObject:domain];
                break;
            }
        }
    }
    // update the domain table
    [domainField reloadData];
    return;
}



- (void)updateBrowseWithResult:(int)type name:(NSString *)name type:(NSString *)resulttype domain:(NSString *)domain flags:(int)flags
{
    if ([domain isEqualToString:Domain] && [resulttype isEqualToString:SrvType]) {

        if (type == DNSServiceBrowserReplyRemoveInstance) {
            if ([nameKeys containsObject:name]) {
                [nameKeys removeObject:name];
            }
        }
        if (type == DNSServiceBrowserReplyAddInstance) {
            [nameKeys addObject:name];
        }
        
        [nameField reloadData];		//Reload (redraw) Name TableView with newly found data
    }
    return;
}

- (void)resolveClientWithInterface:(struct sockaddr *)interface address:(struct sockaddr *)address txtRecord:(NSString *)txtRecord
{
    // printf("Got a reply from the server %s, %d\n", txtRecord, flags);

    printf("Length = %d\n", strlen(interface));

    printf("interface length = %d, port = %d, family = %d, address = %s\n", ((struct sockaddr_in *)interface)->sin_len, ((struct sockaddr_in *)interface)->sin_port, ((struct sockaddr_in *)interface)->sin_family, inet_ntoa(((struct in_addr)((struct sockaddr_in *)interface)->sin_addr)));
    printf("address length = %d, port = %d, family = %d, address = %s\n", ((struct sockaddr_in *)address)->sin_len, ((struct sockaddr_in *)address)->sin_port, ((struct sockaddr_in *)address)->sin_family, inet_ntoa(((struct in_addr)((struct sockaddr_in *)address)->sin_addr)));
    //[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:txtRecord]];
    return;
}

- (IBAction)handleTableClick:(id)sender
{
    //populate the text fields
}

- (IBAction)removeSelected:(id)sender
{
    // remove the selected row and force a refresh

    int selectedRow = [serviceDisplayTable selectedRow];

    if (selectedRow) {

        [srvtypeKeys removeObjectAtIndex:selectedRow];
        [srvnameKeys removeObjectAtIndex:selectedRow];

        [[NSUserDefaults standardUserDefaults] setObject:srvtypeKeys forKey:@"SrvTypeKeys"];
        [[NSUserDefaults standardUserDefaults] setObject:srvnameKeys forKey:@"SrvNameKeys"];
        
        [typeField reloadData];
        [serviceDisplayTable reloadData];
    }
}

- (IBAction)addNewService:(id)sender
{
    // add new entries from the edit fields to the arrays for the defaults

    if ([[serviceTypeField stringValue] length] && [[serviceNameField stringValue] length]) {
        [srvtypeKeys addObject:[serviceTypeField stringValue]];
        [srvnameKeys addObject:[serviceNameField stringValue]];

        [[NSUserDefaults standardUserDefaults] setObject:srvtypeKeys forKey:@"SrvTypeKeys"];
        [[NSUserDefaults standardUserDefaults] setObject:srvnameKeys forKey:@"SrvNameKeys"];

        [typeField reloadData];
        [serviceDisplayTable reloadData];
    }

}



@end