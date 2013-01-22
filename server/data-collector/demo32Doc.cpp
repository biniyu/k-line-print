// demo32Doc.cpp : implementation of the CDemo32Doc class
//

#include "stdafx.h"
#include "demo32.h"

#include "demo32Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo32Doc

IMPLEMENT_DYNCREATE(CDemo32Doc, CDocument)

BEGIN_MESSAGE_MAP(CDemo32Doc, CDocument)
	//{{AFX_MSG_MAP(CDemo32Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo32Doc construction/destruction

CDemo32Doc::CDemo32Doc()
{
	// TODO: add one-time construction code here

}

CDemo32Doc::~CDemo32Doc()
{
}

BOOL CDemo32Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemo32Doc serialization

void CDemo32Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDemo32Doc diagnostics

#ifdef _DEBUG
void CDemo32Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemo32Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo32Doc commands
