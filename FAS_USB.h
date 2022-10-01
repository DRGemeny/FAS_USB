// FAS_USB.h : main header file for the FAS_USB application
//

#if !defined(AFX_FAS_USB_H__FFA0F3B4_FBDF_4325_8003_E6B93C9E3987__INCLUDED_)
#define AFX_FAS_USB_H__FFA0F3B4_FBDF_4325_8003_E6B93C9E3987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFAS_USBApp:
// See FAS_USB.cpp for the implementation of this class
//

class CFAS_USBApp : public CWinApp
{
public:
	CFAS_USBApp();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CFAS_USBApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFAS_USBApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAS_USB_H__FFA0F3B4_FBDF_4325_8003_E6B93C9E3987__INCLUDED_)
