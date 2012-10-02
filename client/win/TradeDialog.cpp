// TradeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeDialog.h"
#include "Utility.h"
#include "MainFrm.h"
#include "KLinePrintDoc.h"

// CTradeDialog 对话框

IMPLEMENT_DYNAMIC(CTradeDialog, CDialog)

CTradeDialog::CTradeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeDialog::IDD, pParent)
	, m_nFee(0)
	, m_nMargin(0)
	, m_nUnitsPerSlot(0)
	, m_nSlots(0)
	, m_nDefaultSlots(0)
	, m_nMaxLoss(0)
	, m_nMaxProfit(0)
	, m_nTimeStop(0)
	, m_nMaxLossPerDay(0)
	, m_nMaxOpenTimes(0)
{
	EXCHANGE.SetTick(Tick());
	m_bEnableTrade = FALSE;
}

CTradeDialog::~CTradeDialog()
{
}

void CTradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACCOUNT, m_AccountInfo);
	DDX_Control(pDX, IDC_LIST_POSITION, m_PositionInfo);

	DDX_Text(pDX, IDC_EDIT_FEE, m_nFee);
	DDX_Text(pDX, IDC_EDIT_MARGIN, m_nMargin);
	DDX_Text(pDX, IDC_EDIT_UNITS, m_nUnitsPerSlot);
	DDX_Text(pDX, IDC_EDIT_SLOTS, m_nSlots);
	DDX_Control(pDX, IDC_BUTTON_BUY, m_btnBuy);
	DDX_Control(pDX, IDC_BUTTON_SELL, m_btnSell);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_REVERSE, m_btnReverse);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_SLOTS, m_nDefaultSlots);
	DDX_Text(pDX, IDC_EDIT_MAXLOSS, m_nMaxLoss);
	DDX_Text(pDX, IDC_EDIT_MAXPROFIT, m_nMaxProfit);
	DDX_Text(pDX, IDC_EDIT_TIMESTOP, m_nTimeStop);
	DDX_Text(pDX, IDC_EDIT_MAXOPENTIMES, m_nMaxOpenTimes);
	DDX_Text(pDX, IDC_EDIT_MAXLOSSPERDAY, m_nMaxLossPerDay);
	DDX_Control(pDX, IDC_EDIT_MAX_SLOT, m_ctlMaxSlot);
}


BEGIN_MESSAGE_MAP(CTradeDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BUY, &CTradeDialog::OnBnClickedButtonBuy)
	ON_BN_CLICKED(IDC_BUTTON_SELL, &CTradeDialog::OnBnClickedButtonSell)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTradeDialog::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CTradeDialog::OnBnClickedButtonReverse)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_PARAM, &CTradeDialog::OnBnClickedButtonUpdateParam)
END_MESSAGE_MAP()



// CTradeDialog 消息处理程序
#define CURDOC	((CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument())

void CTradeDialog::OnBnClickedButtonBuy()
{
	UpdateData();
	CURDOC->AppendTradeRecord(EXCHANGE.Buy(m_nSlots));
	UpdateAccountInfo();
}

void CTradeDialog::OnBnClickedButtonSell()
{
	UpdateData();
	CURDOC->AppendTradeRecord(EXCHANGE.Sell(m_nSlots));
	UpdateAccountInfo();
}

void CTradeDialog::OnBnClickedButtonClose()
{
	CURDOC->AppendTradeRecord(EXCHANGE.Close());
	UpdateAccountInfo();
}

void CTradeDialog::OnBnClickedButtonReverse()
{
	TradeRecordCollection trs = EXCHANGE.Reverse();

	for(int i = 0 ; i < trs.size(); i++)
		CURDOC->AppendTradeRecord(trs[i]);

	UpdateAccountInfo();
}

BOOL CTradeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_AccountInfo.InsertColumn(0, CString("期初权益"), 0, 70);
	m_AccountInfo.InsertColumn(1, CString("当前权益"), 0, 70);
	m_AccountInfo.InsertColumn(2, CString("保证金"), 0, 60);
	m_AccountInfo.InsertColumn(3, CString("毛利"), 0, 60);
	m_AccountInfo.InsertColumn(4, CString("手续费"), 0, 60);
	m_AccountInfo.InsertColumn(5, CString("净利"), 0, 60);

	m_PositionInfo.InsertColumn(0, CString("时间"), 0, 60);
	m_PositionInfo.InsertColumn(1, CString("数量"), 0, 40);
	m_PositionInfo.InsertColumn(2, CString("开仓价格"), 0, 90);
	m_PositionInfo.InsertColumn(3, CString("当前价格"), 0, 90);
	m_PositionInfo.InsertColumn(4, CString("浮动盈亏"), 0, 90);

	m_nDefaultSlots = m_nSlots = TP.nDefaultSlots;
	m_nFee = TP.nFee;
	m_nMargin = TP.nMarginRate;
	m_nUnitsPerSlot = TP.nUnitsPerSlot;
	m_nMaxLoss = TP.nMaxLossStop;
	m_nMaxProfit = TP.nMaxProfitStop;
	m_nTimeStop = TP.nTimeStop;
	m_nMaxOpenTimes = TP.nMaxOpenTimes;
	m_nMaxLossPerDay = TP.nMaxLossPerDay;

	UpdateData(FALSE);

	UpdateAccountInfo();

	m_btnBuy.EnableWindow(FALSE);
	m_btnSell.EnableWindow(FALSE);
	m_btnClose.EnableWindow(FALSE);
	m_btnReverse.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTradeDialog::EnableTrade(BOOL bEnable)
{
	if(m_bEnableTrade && (!bEnable))
	{
		m_btnBuy.EnableWindow(FALSE);
		m_btnSell.EnableWindow(FALSE);
		m_btnClose.EnableWindow(FALSE);
		m_btnReverse.EnableWindow(FALSE);
	}
	else if((!m_bEnableTrade) && bEnable)
	{
		m_btnBuy.EnableWindow(TRUE);
		m_btnSell.EnableWindow(TRUE);
		m_btnClose.EnableWindow(TRUE);
		m_btnReverse.EnableWindow(TRUE);
	}

	m_bEnableTrade = bEnable;
}

void CTradeDialog::UpdateAccountInfo(void)
{
	// 如果当前盈亏超出最大止损止盈值，平仓并提示

	TradeFacility& tf = EXCHANGE;

	if(m_nMaxLoss && (EXCHANGE.m_nPosition.nProfit <= -m_nMaxLoss))
	{
		CURDOC->AppendTradeRecord(EXCHANGE.Close());
		AfxMessageBox(_T("止损平仓!"));
	}

	if(m_nMaxProfit && (EXCHANGE.m_nPosition.nProfit >= m_nMaxProfit))
	{
		CURDOC->AppendTradeRecord(EXCHANGE.Close());
		AfxMessageBox(_T("止盈平仓!"));
	}

	//	如果当前时间超过时间止损，且还处于亏损，强制平仓
	if(m_nTimeStop && tf.m_nPosition.nTime)
	{
		if(tf.m_nTick.time_ms - tf.m_nPosition.nTime > m_nTimeStop * 1000
			&& tf.m_nPosition.nProfit < 0 )
		{
			CURDOC->AppendTradeRecord(EXCHANGE.Close());
			AfxMessageBox(_T("时间止损平仓!"));
		}
	}

	if(m_AccountInfo.GetItemCount() == 0)
		m_AccountInfo.InsertItem(0, IntToCString(0));
	
	m_AccountInfo.SetItemText(0, 0, IntToCString(EXCHANGE.m_nIntialBalance));
	m_AccountInfo.SetItemText(0, 1, IntToCString(EXCHANGE.m_nBalance + EXCHANGE.m_nPosition.nProfit));
	m_AccountInfo.SetItemText(0, 2, IntToCString(EXCHANGE.m_nMargin));
	m_AccountInfo.SetItemText(0, 3, IntToCString(EXCHANGE.m_nTotalProfit + EXCHANGE.m_nPosition.nProfit));
	m_AccountInfo.SetItemText(0, 4, IntToCString(EXCHANGE.m_nTotalFee));
	m_AccountInfo.SetItemText(0, 5, IntToCString(EXCHANGE.m_nTotalProfit - EXCHANGE.m_nTotalFee + EXCHANGE.m_nPosition.nProfit));

	if(m_PositionInfo.GetItemCount() == 0)
		m_PositionInfo.InsertItem(0, IntToCString(0));

	int nDispTime = Utility::ConvContTimeToDispTime(EXCHANGE.m_nPosition.nTime / 1000);

	CString tmp;
	tmp.Format(_T("%02d:%02d:%02d"), 
			nDispTime / 10000, 
			nDispTime % 10000 / 100, 
			nDispTime % 10000 % 100);

	m_PositionInfo.SetItemText(0, 0, tmp);

	m_PositionInfo.SetItemText(0, 1, IntToCString(EXCHANGE.m_nPosition.nSlot));
	m_PositionInfo.SetItemText(0, 2, IntToCString(EXCHANGE.m_nPosition.nPrice));
	m_PositionInfo.SetItemText(0, 3, IntToCString(EXCHANGE.m_nTick.price));
	m_PositionInfo.SetItemText(0, 4, IntToCString(EXCHANGE.m_nPosition.nProfit));

	//	计算最大可开仓手数
	int maxSlot = (tf.m_nBalance + tf.m_nPosition.nProfit - tf.m_nMargin) 
		/ ((float)tf.m_nTick.price * TP.nUnitsPerSlot * TP.nMarginRate / 100);

	m_ctlMaxSlot.SetWindowTextW(IntToCString(maxSlot));

	Utility::WriteBalance(EXCHANGE.m_nBalance);
}

void CTradeDialog::OnBnClickedButtonUpdateParam()
{
	UpdateData();

	TP.nFee = m_nFee;
	TP.nMarginRate = m_nMargin;
	TP.nUnitsPerSlot = m_nUnitsPerSlot;

	TP.nDefaultSlots = m_nDefaultSlots;
	TP.nMaxLossStop = m_nMaxLoss;
	TP.nMaxProfitStop = m_nMaxProfit;
	TP.nTimeStop = m_nTimeStop;
	TP.nMaxOpenTimes = m_nMaxOpenTimes;
	TP.nMaxLossPerDay = m_nMaxLossPerDay;

	Utility::WriteExchangeConfig(TP);
}
