// demo32View.h : interface of the CDemo32View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO32VIEW_H__350971F9_A7BB_11D2_AF9D_444553540000__INCLUDED_)
#define AFX_DEMO32VIEW_H__350971F9_A7BB_11D2_AF9D_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDemo32View : public CView
{
protected: // create from serialization only
	CDemo32View();
	DECLARE_DYNCREATE(CDemo32View)

// Attributes
public:
	CDemo32Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo32View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo32View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo32View)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in demo32View.cpp
inline CDemo32Doc* CDemo32View::GetDocument()
   { return (CDemo32Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO32VIEW_H__350971F9_A7BB_11D2_AF9D_444553540000__INCLUDED_)
