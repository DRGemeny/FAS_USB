// FAS_USB.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FAS_USB.h"
#include "FAS_USBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFAS_USBApp

BEGIN_MESSAGE_MAP(CFAS_USBApp, CWinApp)
	//{{AFX_MSG_MAP(CFAS_USBApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFAS_USBApp construction

CFAS_USBApp::CFAS_USBApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFAS_USBApp object

CFAS_USBApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFAS_USBApp initialization

BOOL CFAS_USBApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFAS_USBDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		AfxMessageBox("1212121");
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
