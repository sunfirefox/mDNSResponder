/*
 * Copyright (c) 2003-2004 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@

    Change History (most recent first):

$Log: AssemblyInfo.cpp,v $
Revision 1.3  2004/07/14 19:54:57  shersche
use version file to get version numbers

Revision 1.2  2004/07/14 15:42:47  shersche
remove entries for strong authentication

Revision 1.1  2004/06/26 04:01:22  shersche
Initial revision


 */
    
#include "stdafx.h"
#include "WinVersRes.h"

using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;

//
// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
[assembly:AssemblyTitleAttribute("dnssd.NET")];
[assembly:AssemblyDescriptionAttribute(".NET wrapper for DNS-SD services")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("Apple Computer, Inc.")];
[assembly:AssemblyProductAttribute("")];
[assembly:AssemblyCopyrightAttribute("Apple Computer, Inc.")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];		

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the value or you can default the Revision and Build Numbers 
// by using the '*' as shown below:

[assembly:AssemblyVersionAttribute(MASTER_PROD_VERS_STR2)];

