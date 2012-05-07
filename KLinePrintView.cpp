
// KLinePrintView.cpp : CKLinePrintView 类的实现
//

#include "stdafx.h"
#include "KLinePrint.h"

#include "KLinePrintDoc.h"
#include "KLinePrintView.h"
#include "KLineCollection.h"
#include "TickReader.h"
#include "KLineRenderer.h"
#include "Utility.h"
#include "PlaybackConfDialog.h"

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
	ON_COMMAND(ID_PLAYBACK_CONF, &CKLinePrintView::OnPlaybackConf)
END_MESSAGE_MAP()

// CKLinePrintView 构造/析构

CKLinePrintView::CKLinePrintView()
{
	m_bLocked = TRUE;
	m_enViewMode = ViewModeAll;
	m_pTradeDialog = 0;
}

CKLinePrintView::~CKLinePrintView()
{

}

void CKLinePrintView::SetTickData(Tick tick)
{
	tick_render.SetTick(tick);

	if(m_pTradeDialog) 
		m_pTradeDialog->SetTick(tick);
}

void CKLinePrintView::Set5SecData(KLineCollection* pData) 
{ 
	klr_5sec.SetKLineData(pData, 120); 
	klr_5sec.SetOpenIndex(0);
}

void CKLinePrintView::Set1MinData(KLineCollection* pData) 
{ 
	klr_1min.SetKLineData(pData, 240); 
	klr_1min.SetRenderMode(KLineRenderer::enAxisMode);
	klr_1min.SetShowAvg(true);
	klr_1min.SetShowCriticalTime(true);
	klr_1min.SetShowMaxMin(true);	//	显示日内最高最低价
	klr_1min.SetOpenIndex(1);
}

void CKLinePrintView::SetDayData(KLineCollection* pData, int nDate)  
{ 
	klr_day.SetKLineData(pData, 120);
	klr_day.SelectByTime(nDate);
	klr_day.SetShowVol(false);
	klr_day.SetShowMA(true);
	klr_day.SetShowHighLow(true);	//	显示n日最高最低价
	klr_day.SetOpenIndex(-1);
	klr_day.SetShowDate(true);
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
	tick_render.Render(&m_MemDC);
	Invalidate(FALSE);
}

void CKLinePrintView::Layout()
{
	CRect rc;
	CRect rc_tick, rc_1min, rc_day, rc_detail;
	
	GetClientRect(&rc);

	int horiSplit = rc.bottom / 5 * 3;

	int vertSplitUp = rc.right / 9;
	int vertSplitDown = rc.right / 2;

	//绘图
	if(m_enViewMode == ViewModeAll)
	{
		rc_tick = rc;
		rc_tick.right = vertSplitUp;
		rc_tick.bottom = horiSplit;

		rc_1min = rc;
		rc_1min.left = vertSplitUp;
		rc_1min.bottom = horiSplit;

		rc_day = rc;
		rc_day.top = horiSplit;
		rc_day.right = vertSplitDown;

		rc_detail = rc;
		rc_detail.top = horiSplit;
		rc_detail.left = vertSplitDown;
	}
	else if(m_enViewMode == ViewMode1Min)
	{
		rc_tick = rc;
		rc_tick.right = vertSplitUp;

		rc_1min = rc;
		rc_1min.left = vertSplitUp;

		rc_day = CRect(0,0,0,0);
		rc_detail = CRect(0,0,0,0);
	}

	tick_render.SetRect(rc_tick);
	klr_1min.SetRect(rc_1min);
	klr_day.SetRect(rc_day);
	klr_5sec.SetRect(rc_detail);
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

	Layout();

	Render();
}

void CKLinePrintView::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	GetClientRect(&rc);

	//	重建MEMDC
	m_MemBitmap.DeleteObject();
	m_MemDC.DeleteDC();

	m_MemDC.CreateCompatibleDC(GetDC());
	m_MemBitmap.CreateCompatibleBitmap(GetDC(),cx,cy);

	m_MemDC.SelectObject(&m_MemBitmap);
	m_MemDC.FillSolidRect(&rc,RGB(255,255,255));

	Layout();
	
	Render();

	CView::OnSize(nType, cx, cy);
}

void CKLinePrintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	tick_render.Select(point);

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
	CKLinePrintDoc* pDoc = GetDocument();

	m_nPlaybackSpeed = 1;

	pDoc->DisplayTill(klr_1min.GetCurTime(), klr_day.GetCurTime());

	SetTimer(1,1000,NULL); 

	if(!m_pTradeDialog)
	{
		m_pTradeDialog = new CTradeDialog;
		m_pTradeDialog->Create(IDD_TRADE,NULL);
		m_pTradeDialog->ShowWindow(SW_SHOW); 
	}
	else
	{
		m_pTradeDialog->ShowWindow(SW_SHOW); 
	}
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

		//  根据回放配置决定下一个交易日的日期
		if(pDoc->LoadNextDay())
			SetTimer(1,1000,NULL); 

		//	TODO :	完成数据加载后不自动开始，等用户点播放键再开始	

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

void CKLinePrintView::OnPlaybackConf()
{
	CKLinePrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	PlaybackConfig pc;
	CPlaybackConfDialog dlg;

	if(IDOK == dlg.DoModal())
	{
		if(dlg.m_nPlaybackOrder == 0)
			pc.enPlaybackOrder = PlaybackConfig::PLAYBACK_SEQUENTIAL;
		else
			pc.enPlaybackOrder = PlaybackConfig::PLAYBACK_RANDOM;
		
		pc.bDayOfWeek[1] = dlg.m_bMonday;
		pc.bDayOfWeek[2] = dlg.m_bTuesday;
		pc.bDayOfWeek[3] = dlg.m_bWednesday;
		pc.bDayOfWeek[4] = dlg.m_bThursday;
		pc.bDayOfWeek[5] = dlg.m_bFriday;

		if(dlg.m_bGap)
			pc.nGapPercentage = dlg.m_nGap;

		if(dlg.m_bFluncAbove)
			pc.nLastDayFluctuationAbove = dlg.m_nFluncAbove;

		if(dlg.m_bFluncBelow)
			pc.nLastDayFluctuationBelow = dlg.m_nFluncBelow;

		pDoc->SetPlaybackConfig(pc);
	}
}
