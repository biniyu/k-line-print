#include "StdAfx.h"
#include "StkUI.h"
#include "AccountWindow.h"
#include "Resource.h"

CAccountWindow::CAccountWindow(void)
{
}

CAccountWindow::~CAccountWindow(void)
{
}

BEGIN_MESSAGE_MAP(CAccountWindow, CGuiControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CAccountWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_TabWnd.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,0x9999))
		return -1;

	const DWORD dwStyle =LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_AccountInfo.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 1))
		return -1;

	SetDefaultListCtrlStyle(m_AccountInfo);

	m_AccountInfo.InsertColumn( 0, "账户", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 1, "昨日结存", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 2, "浮动盈亏", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 3, "平仓盈亏", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 4, "动态权益", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 5, "可用资金", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 6, "持仓保证金", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 7, "冻结资金", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 8, "手续费", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 9, "风险", LVCFMT_CENTER, 80 );

	int nRow = m_AccountInfo.InsertItem(0, "10800110");
    m_AccountInfo.SetItemText(nRow, 1, "jacky");
    m_AccountInfo.SetItemText(nRow, 2, "jacky");
    m_AccountInfo.SetItemText(nRow, 3, "jacky");
    m_AccountInfo.SetItemText(nRow, 4, "jacky");
    m_AccountInfo.SetItemText(nRow, 5, "jacky");

	if (!m_Positions.Create(dwStyle,CRect(0,0,0,0), &m_TabWnd, 2))
		return -1;

	SetDefaultListCtrlStyle(m_Positions);

	m_Positions.InsertColumn( 0, "账户", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 1, "商品", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 2, "总持仓", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 3, "均价", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 4, "现价", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 5, "浮动盈亏", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 6, "买持仓", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 7, "买均价", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 8, "买盈亏", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 9, "今买量", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 10, "卖持仓", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 11, "卖均价", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 12, "卖盈亏", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 13, "今卖量", LVCFMT_CENTER, 80 );

	if (!m_Trades.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 3))
		return -1;

	SetDefaultListCtrlStyle(m_Trades);

	m_Trades.InsertColumn( 0, "账户", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 1, "合同号", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 2, "商品", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 3, "类型", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 4, "状态", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 5, "开平标志", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 6, "数量", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 7, "委托价格", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 8, "委托时间", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 9, "成交数量", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 10, "成交价格", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 11, "未成数量", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 12, "滑价", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 13, "止损价", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 14, "获利价", LVCFMT_CENTER, 80 );

	if (!m_Triggers.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 4))
		return -1;

	SetDefaultListCtrlStyle(m_Triggers);

	m_Triggers.InsertColumn( 0, "账户", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 1, "过期时间", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 2, "商品", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 3, "数量", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 4, "状态", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 5, "开平标志", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 6, "触发类型", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 7, "触发价格", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 8, "执行价格", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 9, "止损价", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 10, "获利价", LVCFMT_CENTER, 80 );

	if (!m_Stops.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 5))
		return -1;

	SetDefaultListCtrlStyle(m_Stops);

	m_Stops.InsertColumn( 0, "账户", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 1, "商品", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 2, "类型", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 3, "数量", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 4, "状态", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 5, "止损价", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 6, "获利价", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 7, "成交价格", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 8, "成交时间", LVCFMT_CENTER, 80 );

	if (!m_History.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 6))
		return -1;

	SetDefaultListCtrlStyle(m_History);

	m_History.InsertColumn( 0, "时间", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 1, "当日结存", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 2, "前日结存", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 3, "当日盈亏", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 4, "可用资金", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 5, "手续费", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 6, "当日出金", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 7, "当日入金", LVCFMT_CENTER, 80 );

	m_TabWnd.SetImageList(IDB_DBPROJECTS, 16,8, RGB (255, 0, 0));
	m_TabWnd.Addtab(&m_AccountInfo,"账户汇总",1);
	m_TabWnd.Addtab(&m_Positions,"持仓统计",2);
	m_TabWnd.Addtab(&m_Trades,"当日交易",3);
	m_TabWnd.Addtab(&m_Triggers,"触发单",4);
	m_TabWnd.Addtab(&m_Stops,"止损获利单",5);
	m_TabWnd.Addtab(&m_History,"历史明细",6);
	
//	SetIcon(IDB_BITMAPHELP,16,5,RGB(255,0,0),3);

	return 0;
}

void CAccountWindow::SetDefaultListCtrlStyle(CListCtrl& ctlList)
{
	LONG lStyle;
	lStyle = GetWindowLong(ctlList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(ctlList.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle2 = ctlList.GetExtendedStyle();
	dwStyle2 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle2 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle2 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	ctlList.SetExtendedStyle(dwStyle2); //设置扩展风格
}
