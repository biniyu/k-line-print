// demo32View.cpp : implementation of the CDemo32View class
//

#include "stdafx.h"
#include "demo32.h"

#include "demo32Doc.h"
#include "demo32View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo32View

IMPLEMENT_DYNCREATE(CDemo32View, CView)

BEGIN_MESSAGE_MAP(CDemo32View, CView)
	//{{AFX_MSG_MAP(CDemo32View)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo32View construction/destruction

CDemo32View::CDemo32View()
{
	// TODO: add construction code here

}

CDemo32View::~CDemo32View()
{
}

BOOL CDemo32View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDemo32View drawing

void CDemo32View::OnDraw(CDC* pDC)
{
	CDemo32Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDemo32View printing

BOOL CDemo32View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemo32View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemo32View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDemo32View diagnostics

#ifdef _DEBUG
void CDemo32View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo32View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo32Doc* CDemo32View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo32Doc)));
	return (CDemo32Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo32View message handlers
