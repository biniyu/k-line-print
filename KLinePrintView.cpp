
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
END_MESSAGE_MAP()

// CKLinePrintView 构造/析构

CKLinePrintView::CKLinePrintView()
{
	// TODO: 在此处添加构造代码

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
	KLineRenderer klr;

	GetClientRect(&rc);

	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有地方画 ^_^
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小，也可以自己定义（如：有滚动条时就要大于当前窗口的大小，在BitBlt时决定拷贝内存的哪部分到屏幕上）
	MemBitmap.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());

	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
	MemDC.FillSolidRect(0,0,rc.Width(),rc.Height(),RGB(255,255,255));
	//绘图

	klr.Render(&MemDC, rc, pDoc->klc);

	if(m_bDrawTrackingCrossLine)
	{
		MemDC.MoveTo(rc.left, cp.y);
		MemDC.LineTo(rc.right, cp.y);

		MemDC.MoveTo(cp.x, rc.top);
		MemDC.LineTo(cp.x, rc.bottom);
	}

	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rc.Width(),rc.Height(),&MemDC,0,0,SRCCOPY);

	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
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

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKLinePrintView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bDrawTrackingCrossLine)
	{
		cp = point;
		Invalidate(FALSE);
	}

	CView::OnMouseMove(nFlags, point);
}

void CKLinePrintView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bDrawTrackingCrossLine = !m_bDrawTrackingCrossLine;
	Invalidate(FALSE);
	CView::OnLButtonDblClk(nFlags, point);
}
