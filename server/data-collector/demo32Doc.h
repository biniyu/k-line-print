// demo32Doc.h : interface of the CDemo32Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO32DOC_H__350971F7_A7BB_11D2_AF9D_444553540000__INCLUDED_)
#define AFX_DEMO32DOC_H__350971F7_A7BB_11D2_AF9D_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CDemo32Doc : public CDocument
{
protected: // create from serialization only
	CDemo32Doc();
	DECLARE_DYNCREATE(CDemo32Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo32Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo32Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo32Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO32DOC_H__350971F7_A7BB_11D2_AF9D_444553540000__INCLUDED_)
