
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
	ON_COMMAND(ID_PLAYBACK_BEGIN, &CKLinePrintView::OnPlaybackBegin)
	ON_COMMAND(ID_PLAYBACK_END, &CKLinePrintView::OnPlaybackEnd)
	ON_COMMAND(ID_PLAYBACK_FORWARD, &CKLinePrintView::OnPlaybackForward)
	ON_WM_TIMER()
	ON_COMMAND(ID_PLAYBACK_PAUSE, &CKLinePrintView::OnPlaybackPause)
	ON_COMMAND(ID_PLAYBACK_FASTFW, &CKLinePrintView::OnPlaybackFastfw)
	ON_COMMAND(ID_PLAYBACK_FASTREV, &CKLinePrintView::OnPlaybackFastrev)
	ON_COMMAND(ID_PLAYBACK_STOP, &CKLinePrintView::OnPlaybackStop)
END_MESSAGE_MAP()

// CKLinePrintView 构造/析构

CKLinePrintView::CKLinePrintView()
{
	m_bLocked = TRUE;
	m_enViewMode = ViewModeAll;
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

#define RADIUS 5

	if(m_bLocked)
		m_MemDC.Ellipse(rc.right - 2 * RADIUS, rc.top, rc.right, rc.top + 2*RADIUS);

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

	int origdate = klr_day.GetCurTime();

	if(nChar == VK_RETURN)
		ToggleViewMode();


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

	if(nChar == VK_HOME)
	{
		klr_1min.ToggleTrackingMode();
		klr_day.ToggleTrackingMode();
		klr_5sec.ToggleTrackingMode();
	}

	if(nChar == VK_END)
	{
		klr_1min.ToggleRenderMode();
		klr_day.ToggleRenderMode();
		klr_5sec.ToggleRenderMode();
	}

	if(klr_5sec.IsSelected() && nChar > '0' && nChar <'9')
	{
		int tmptime = klr_5sec.GetCurTime(); 
		switch(nChar)
		{
		case '1':
			pDoc->ReloadDetailData(1);
			break;
		case '2':
			pDoc->ReloadDetailData(5);
			break;
		case '3':
			pDoc->ReloadDetailData(15);
			break;
		}
		klr_5sec.SelectByTime(tmptime);
	}

	if(nChar == VK_SPACE)
	{
		m_bLocked = !m_bLocked;
	}

	if(klr_1min.IsSelected() && m_bLocked)
	{
		klr_5sec.SelectByTime(klr_1min.GetCurTime());
		klr_day.SetSelectedPrice(klr_1min.GetSelectedClosePrice());
	}

	if(klr_5sec.IsSelected() && m_bLocked)
	{
		klr_1min.SelectByTime(klr_5sec.GetCurTime());
		klr_1min.SetSelectedPrice(klr_5sec.GetSelectedClosePrice());
		klr_day.SetSelectedPrice(klr_5sec.GetSelectedClosePrice());
	}

	if(klr_day.IsSelected() && m_bLocked && (origdate != klr_day.GetCurTime()))
	{
		pDoc->ReloadByDate(klr_day.GetCurTime());
	}

	Render();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKLinePrintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 如果1min图处于Mouse跟踪模式，获取Mouse所在的价格，在其他图上选择该价格

	if(klr_1min.GetTrackingMode() == KLineRenderer::enMouseTMode)
	{
		int price = klr_1min.GetMousePrice(point);

		if(price)
		{
			klr_day.SetSelectedPrice(price);
			klr_5sec.SetSelectedPrice(price);
		}

		Render();
	}

	CView::OnMouseMove(nFlags, point);
}

void CKLinePrintView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CView::OnLButtonDblClk(nFlags, point);
}

void CKLinePrintView::Render()
{
	klr_1min.Render(&m_MemDC);
	klr_day.Render(&m_MemDC);
	klr_5sec.Render(&m_MemDC);
	Invalidate(FALSE);
}

void CKLinePrintView::ToggleViewMode()		//	切换视图
{
	switch(m_enViewMode)
	{
	case ViewModeAll:
		m_enViewMode = ViewMode1Min;
		break;
	case ViewMode1Min:
		m_enViewMode = ViewModeAll;
		break;
	default:
		break;
	}

	CRect rc, rc1, rc2, rc3;
	GetClientRect(&rc);

	//绘图
	if(m_enViewMode == ViewModeAll)
	{
		rc1 = rc;
		rc1.bottom /= 2;

		rc2 = rc;
		rc2.top = rc.bottom /2;
		rc2.right = rc.right / 2;

		rc3 = rc;
		rc3.top = rc.bottom / 2;
		rc3.left = rc.right / 2;
	}
	else if(m_enViewMode == ViewMode1Min)
	{
		rc1 = rc;
		rc2 = CRect(0,0,0,0);
		rc3 = CRect(0,0,0,0);
	}

	klr_1min.SetRect(rc1);
	klr_day.SetRect(rc2);
	klr_5sec.SetRect(rc3);
	
	Render();

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
	if(klr_1min.IsSelected() && m_bLocked)
	{
		klr_5sec.SelectByTime(klr_1min.GetCurTime());
		klr_day.SetSelectedPrice(klr_1min.GetSelectedClosePrice());
	}

	klr_5sec.Select(point);
	if(klr_5sec.IsSelected() && m_bLocked)
	{
		klr_1min.SelectByTime(klr_5sec.GetCurTime());
		klr_1min.SetSelectedPrice(klr_5sec.GetSelectedClosePrice());
	}

	klr_day.Select(point);
	if(klr_day.IsSelected() && m_bLocked)
	{
		pDoc->ReloadByDate(klr_day.GetCurTime());
	}

	Render();

	CView::OnLButtonDown(nFlags, point);
}

void CKLinePrintView::OnPlaybackBegin()
{
	KillTimer(1);

	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->DisplayTill(0, klr_day.GetCurTime());
}

void CKLinePrintView::OnPlaybackEnd()
{
	KillTimer(1);

	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->DisplayTill(-1, klr_day.GetCurTime());
}

void CKLinePrintView::OnPlaybackForward()
{
	m_nPlaybackSpeed = 1;
	SetTimer(1,1000,NULL); 
}

void CKLinePrintView::OnTimer(UINT_PTR nIDEvent)
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int time = pDoc->GetCurrentTickTime();

	if(!time)
	{
		KillTimer(1);
		//  加载次日的数据
		if(pDoc->LoadNextDay())
			SetTimer(1,1000,NULL); 
	}
	else
	{
		int nTillTime = pDoc->GetCurrentTickTime() + m_nPlaybackSpeed;
		pDoc->PlayTillTime(nTillTime);
		klr_1min.SelectLastK();
		klr_5sec.SelectLastK();
		klr_day.SetSelectedPrice(klr_1min.GetSelectedClosePrice());
		Render();
		Invalidate(FALSE);
	}

	CView::OnTimer(nIDEvent);
}

void CKLinePrintView::OnPlaybackPause()
{
	KillTimer(1);
}

void CKLinePrintView::OnPlaybackFastfw()
{
	m_nPlaybackSpeed += 30;
}

void CKLinePrintView::OnPlaybackFastrev()
{
	if(m_nPlaybackSpeed - 30 > 1)
		m_nPlaybackSpeed -= 30;

	else
		m_nPlaybackSpeed = 1;
}

void CKLinePrintView::OnPlaybackStop()
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	KillTimer(1);
	pDoc->DisplayTill(-1, -1);
}
