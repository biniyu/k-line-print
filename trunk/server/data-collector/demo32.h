// demo32.h : main header file for the DEMO32 application
//

#if !defined(AFX_DEMO32_H__350971F1_A7BB_11D2_AF9D_444553540000__INCLUDED_)
#define AFX_DEMO32_H__350971F1_A7BB_11D2_AF9D_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemo32App:
// See demo32.cpp for the implementation of this class
//

class CDemo32App : public CWinApp
{
public:
	CDemo32App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemo32App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO32_H__350971F1_A7BB_11D2_AF9D_444553540000__INCLUDED_)
