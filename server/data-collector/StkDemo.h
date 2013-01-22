#if !defined(AFX_STKDEMO_H__58975303_DC58_11D1_A883_00C04FCCA334__INCLUDED_)
#define AFX_STKDEMO_H__58975303_DC58_11D1_A883_00C04FCCA334__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StkDemo.h : header file
//


#define	 My_Msg_StkData			WM_APP + 1

/////////////////////////////////////////////////////////////////////////////
// CStkDemo window

class CStkDemo : public CWnd
{
// Construction
public:
	CStkDemo();
	
	enum {StkBufNum = 8};
// Data
public:
	CString m_Stock[StkBufNum];		// 股票数据
	int		m_StkPtr;				// 数据指针
	CString	m_Min;					// 分时数据
	CString	m_File;					// 文件数据
	BOOL	m_bRunFlag;				// 
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStkDemo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStkDemo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStkDemo)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LONG OnStkDataOK(UINT wFileType,LONG lPara);
	DECLARE_MESSAGE_MAP()
public:
	int MyCreate(CWnd* pWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STKDEMO_H__58975303_DC58_11D1_A883_00C04FCCA334__INCLUDED_)
