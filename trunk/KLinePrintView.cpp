
// KLinePrintView.cpp : CKLinePrintView 类的实现
//

#include "stdafx.h"
#include "KLinePrint.h"

#include "KLinePrintDoc.h"
#include "KLinePrintView.h"
#include "KLineCollection.h"
#include "TickReader.h"
#include "KLineRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKLinePrintView

IMPLEMENT_DYNCREATE(CKLinePrintView, CView)

BEGIN_MESSAGE_MAP(CKLinePrintView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKLinePrintView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CKLinePrintView 构造/析构

CKLinePrintView::CKLinePrintView()
{
	// TODO: 在此处添加构造代码
	m_bDrawTrackingCrossLine = FALSE;
}

CKLinePrintView::~CKLinePrintView()
{
}

BOOL CKLinePrintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CKLinePrintView 绘制

void CKLinePrintView::OnDraw(CDC* pDC)
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rc;
	GetClientRect(&rc);

	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rc.Width(),rc.Height(),&m_MemDC,0,0,SRCCOPY);
}


// CKLinePrintView 打印


void CKLinePrintView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CKLinePrintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CKLinePrintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CKLinePrintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CKLinePrintView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKLinePrintView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CKLinePrintView 诊断

#ifdef _DEBUG
void CKLinePrintView::AssertValid() const
{
	CView::AssertValid();
}

void CKLinePrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKLinePrintDoc* CKLinePrintView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKLinePrintDoc)));
	return (CKLinePrintDoc*)m_pDocument;
}
#endif //_DEBUG


// CKLinePrintView 消息处理程序

void CKLinePrintView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(!pDoc->m_CurCsvFile.size()) return;

	if(nChar == VK_PRIOR )
	{
		pDoc->ViewNeighborDate(TRUE);
	}

	if(nChar == VK_NEXT )
	{
		pDoc->ViewNeighborDate(FALSE);
	}

	if(nChar == VK_UP)
	{
		klr_1min.ZoomIn();
		klr_day.ZoomIn();
		klr_5sec.ZoomIn();
	}

	if(nChar == VK_DOWN)
	{
		klr_1min.ZoomOut();
		klr_day.ZoomOut();
		klr_5sec.ZoomOut();
	}

	if(nChar == VK_LEFT)
	{
		klr_1min.MovePrev();
		klr_day.MovePrev();
		klr_5sec.MovePrev();
	}

	if(nChar == VK_RIGHT)
	{
		klr_1min.MoveNext();
		klr_day.MoveNext();
		klr_5sec.MoveNext();
	}

	if(nChar == VK_END)
	{
		klr_1min.SwitchMode();
		klr_day.SwitchMode();
		klr_5sec.SwitchMode();
	}

	if(klr_1min.IsSelected())
	{
		klr_5sec.SelectByTime(klr_1min.GetCurTime());
	}

	if(klr_day.IsSelected())
	{
		pDoc->ReloadByDate(klr_day.GetCurTime());
	}

	Render();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKLinePrintView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bDrawTrackingCrossLine)
	{
		cp = point;
		Render();
	}

	CView::OnMouseMove(nFlags, point);
}

void CKLinePrintView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bDrawTrackingCrossLine = !m_bDrawTrackingCrossLine;
	cp = point;
	Render();
	CView::OnLButtonDblClk(nFlags, point);
}

void CKLinePrintView::Render()
{
	CRect rc;
	GetClientRect(&rc);

	klr_1min.Render(&m_MemDC);
	klr_day.Render(&m_MemDC);
	klr_5sec.Render(&m_MemDC);

	if(m_bDrawTrackingCrossLine)
	{
		m_MemDC.MoveTo(rc.left, cp.y);
		m_MemDC.LineTo(rc.right, cp.y);

		m_MemDC.MoveTo(cp.x, rc.top);
		m_MemDC.LineTo(cp.x, rc.bottom);
	}

	Invalidate(FALSE);
}

void CKLinePrintView::OnSize(UINT nType, int cx, int cy)
{
	//	重建MEMDC

	CRect rc, rc1, rc2, rc3;
	GetClientRect(&rc);

	//绘图

	rc1 = rc;
	rc1.bottom /= 2;

	rc2 = rc;
	rc2.top = rc.bottom /2;
	rc2.right = rc.right / 2;

	rc3 = rc;
	rc3.top = rc.bottom / 2;
	rc3.left = rc.right / 2;

	m_MemBitmap.DeleteObject();
	m_MemDC.DeleteDC();

	m_MemDC.CreateCompatibleDC(GetDC());
	m_MemBitmap.CreateCompatibleBitmap(GetDC(),cx,cy);

	m_MemDC.SelectObject(&m_MemBitmap);
	m_MemDC.FillSolidRect(&rc,RGB(255,255,255));

	klr_1min.SetRect(rc1);
	klr_day.SetRect(rc2);
	klr_5sec.SetRect(rc3);
	
	Render();

	CView::OnSize(nType, cx, cy);
}

void CKLinePrintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	klr_1min.Select(point);

	if(klr_1min.IsSelected())
	{
		klr_5sec.SelectByTime(klr_1min.GetCurTime());
	}

	klr_5sec.Select(point);

	klr_day.Select(point);

	if(klr_day.IsSelected())
	{
		pDoc->ReloadByDate(klr_day.GetCurTime());
	}

	Render();

	CView::OnLButtonDown(nFlags, point);
}
