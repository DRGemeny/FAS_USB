// FAS_USBDlg.h : header file
//

#if !defined(AFX_FAS_USBDLG_H__7991C3B5_87EE_4C9A_B1BA_6172B0880BFD__INCLUDED_)
#define AFX_FAS_USBDLG_H__7991C3B5_87EE_4C9A_B1BA_6172B0880BFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FT_USB.h"
/////////////////////////////////////////////////////////////////////////////
// CFAS_USBDlg dialog
union IO_data{

	unsigned __int16 nIO_Data[68];
	char cIO_Data[128];

};

union IO_RG{
	unsigned __int16 nIO_RG[6];
	char cIO_RG[12];


};
class CFAS_USBDlg : public CDialog
{
// Construction
public:

	CToolTipCtrl tt1, tt2, tt3,tt4, tt11, tt12, tt13,tt14, tt21, tt22, tt23,tt24, tt31, tt32, tt33,tt34,
	tt41,tt42, tt43, tt44, tt45, tt46, tt47;

	void Dyn_Record(int numb_rg, CString sEdit, CString sRG, int PriznakM);
	unsigned __int16 Forming_DN(int Count);
	unsigned __int16 Forming_DF(int count);
	CBitmap m_bPusk;
	IO_data io_d;
	IO_RG io_rg;

	unsigned int Reverse_Hi_Lo_Bytes(CString sData,int priznak);
	void Forming_IOData(void);
	int Forming_RGU(void);
	void Get_Rmax();
	BOOL IsCorrect(CString m_sEdit,int counter_razr);
//counter_razr=1 24 разрядный регистр, counter_razr=0 16 разрядный регистр.

	CFAS_USBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFAS_USBDlg)
	enum { IDD = IDD_FAS_USB_DIALOG };
	CEdit	m_cdR2;
	CEdit	m_cdR1;
	CEdit	m_cdR4;
	CEdit	m_cdR3;
	CEdit	m_cdN4;
	CEdit	m_cdN3;
	CEdit	m_cdN2;
	CEdit	m_cdN1;
	CEdit	m_cShSin;
	CEdit	m_ceddN;
	CEdit	m_cZD;
	CEdit	m_cNumOI;
	CEdit	m_cKOI;
	CEdit	m_cNOI;
	CEdit	m_cR;
	CEdit	m_cRmax;
	CComboBox	m_cDF;
	CComboBox	m_cA0;
	CComboBox	m_cdN;
	CEdit	m_cF1;
	CEdit	m_c2N1;
	CEdit	m_c1N1;
	CEdit	m_cRk1;
	CEdit	m_cRn1;
	CString	m_sRn1;
	CString	m_sRk1;
	CString	m_s1N1;
	CString	m_s2N1;
	CString	m_sF1;
	CEdit	m_cF2;
	CEdit	m_c2N2;
	CEdit	m_c1N2;
	CEdit	m_cRk2;
	CEdit	m_cRn2;
	CString	m_sRn2;
	CString	m_sRk2;
	CString	m_s1N2;
	CString	m_s2N2;
	CString	m_sF2;
	CEdit	m_cF3;
	CEdit	m_c2N3;
	CEdit	m_c1N3;
	CEdit	m_cRk3;
	CEdit	m_cRn3;
	CString	m_sRn3;
	CString	m_sRk3;
	CString	m_s1N3;
	CString	m_s2N3;
	CString	m_sF3;
	CEdit	m_cF4;
	CEdit	m_c2N4;
	CEdit	m_c1N4;
	CEdit	m_cRk4;
	CEdit	m_cRn4;
	CString	m_sRn4;
	CString	m_sRk4;
	CString	m_s1N4;
	CString	m_s2N4;
	CString	m_sF4;
	CString	m_sRmax;
	CString	m_sdN;
	CString	m_sA0;
	CString	m_sDF;
	CString	m_sR;
	CString	m_sNOI;
	CString	m_sKOI;
	CString	m_sNumOI;
	CString	m_sZD;
	CString	m_seddN;
	CString	m_sShSin;
	CString	m_sdN1;
	CString	m_sdN2;
	CString	m_sdN3;
	CString	m_sdN4;
	CString	m_sdR3;
	CString	m_sdR4;
	CString	m_sdR1;
	CString	m_sdR2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFAS_USBDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFAS_USBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPusk();
	afx_msg void OnKillfocusEdrn1();
	afx_msg void OnKillfocusEdrk1();
	afx_msg void OnKillfocusEd1n1();
	afx_msg void OnKillfocusEd2n1();
	afx_msg void OnKillfocusEdf1();
	afx_msg void OnKillfocusEdrn2();
	afx_msg void OnKillfocusEdrk2();
	afx_msg void OnKillfocusEd1n2();
	afx_msg void OnKillfocusEd2n2();
	afx_msg void OnKillfocusEdf2();
	afx_msg void OnKillfocusEdrn3();
	afx_msg void OnKillfocusEdrk3();
	afx_msg void OnKillfocusEd1n3();
	afx_msg void OnKillfocusEd2n3();
	afx_msg void OnKillfocusEdf3();
	afx_msg void OnKillfocusEdrn4();
	afx_msg void OnKillfocusEdrk4();
	afx_msg void OnKillfocusEd1n4();
	afx_msg void OnKillfocusEd2n4();
	afx_msg void OnKillfocusEdf4();
	afx_msg void OnKillfocusEdrmax();
	afx_msg void OnKillfocusEdr();
	afx_msg void OnKillfocusEdkoi();
	afx_msg void OnKillfocusEdnoi();
	afx_msg void OnClose();
	afx_msg void OnKillfocusEdnumoi();
	afx_msg void OnKillfocusCmba0();
	afx_msg void OnKillfocusCmbdf();
	afx_msg void OnKillfocusEdzd();
	afx_msg void OnKillfocusCmbDN();
	afx_msg void On5666();
	afx_msg void OnKillfocusEddn();
	afx_msg void OnKillfocusEdshsin();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChincr1();
	afx_msg void OnChincr2();
	afx_msg void OnChincr3();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnChvikl1();
	afx_msg void OnChvikl2();
	afx_msg void OnChvikl3();
	afx_msg void OnChvikl4();
	afx_msg void OnCHZDIKMtsek();
	afx_msg void OnChangeEdrmax();
	afx_msg void OnA0();
	afx_msg void OnKillfocusEddrn();
	afx_msg void OnKillfocusEddrk();
	afx_msg void OnKillfocusEddn1();
	afx_msg void OnKillfocusEddn2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAS_USBDLG_H__7991C3B5_87EE_4C9A_B1BA_6172B0880BFD__INCLUDED_)
