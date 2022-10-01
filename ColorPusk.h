#if !defined(AFX_COLORPUSK_H__B669A5C3_4FC1_4BC3_9503_2BBF7B2C5CC5__INCLUDED_)
#define AFX_COLORPUSK_H__B669A5C3_4FC1_4BC3_9503_2BBF7B2C5CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorPusk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorPusk window

class CColorPusk : public CButton
{
// Construction
public:
	CColorPusk();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPusk)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorPusk();
virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPusk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPUSK_H__B669A5C3_4FC1_4BC3_9503_2BBF7B2C5CC5__INCLUDED_)
