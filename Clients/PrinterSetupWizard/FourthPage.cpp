/*
 * Copyright (c) 1997-2004 Apple Computer, Inc. All rights reserved.
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
    
$Log: FourthPage.cpp,v $
Revision 1.1  2004/06/18 04:36:57  rpantos
First checked in


*/

#include "stdafx.h"
#include "PrinterSetupWizardApp.h"
#include "PrinterSetupWizardSheet.h"
#include "FourthPage.h"


// CFourthPage dialog

IMPLEMENT_DYNAMIC(CFourthPage, CPropertyPage)
CFourthPage::CFourthPage()
	: CPropertyPage(CFourthPage::IDD),
		m_initialized(false)
{
	m_psp.dwFlags &= ~(PSP_HASHELP);
	m_psp.dwFlags |= PSP_DEFAULT|PSP_HIDEHEADER;

	// create the large font
	m_largeFont.CreateFont(-16, 0, 0, 0, 
		FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("MS Sans Serif"));
}

CFourthPage::~CFourthPage()
{
}

void CFourthPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GOODBYE, m_goodbye);
	DDX_Control(pDX, IDC_PRINTER_NAME, m_printerNameCtrl);
	DDX_Control(pDX, IDC_PRINTER_MANUFACTURER, m_printerManufacturerCtrl);
	DDX_Control(pDX, IDC_PRINTER_MODEL, m_printerModelCtrl);
	DDX_Control(pDX, IDC_PRINTER_PORT, m_printerPortCtrl);
	DDX_Control(pDX, IDC_PRINTER_DEFAULT, m_printerDefault);
}


BEGIN_MESSAGE_MAP(CFourthPage, CPropertyPage)
END_MESSAGE_MAP()


// CFourthPage message handlers
OSStatus 
CFourthPage::OnInitPage()
{
	return kNoErr;
}


BOOL
CFourthPage::OnSetActive()
{
	CPrinterSetupWizardSheet	*	psheet;
	CString							goodbyeText;
	Printer						*	printer;

	psheet = reinterpret_cast<CPrinterSetupWizardSheet*>(GetParent());
	require_quiet( psheet, exit );
   
	printer = psheet->GetSelectedPrinter();
	require_quiet( psheet, exit );
	
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	if (m_initialized == false)
	{
		m_initialized = true;
		OnInitPage();
	}

	m_goodbye.SetFont(&m_largeFont);

	goodbyeText.LoadString(IDS_GOODBYE);
	m_goodbye.SetWindowText(goodbyeText);

	m_printerNameCtrl.SetWindowText( printer->displayName );
	m_printerManufacturerCtrl.SetWindowText ( printer->manufacturer );
	m_printerModelCtrl.SetWindowText ( printer->model );
	m_printerPortCtrl.SetWindowText ( printer->portName );

	m_printerDefault.SetWindowText ( printer->deflt ? L"Yes" : L"No" );

exit:

	return CPropertyPage::OnSetActive();
}
