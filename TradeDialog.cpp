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
{
	EXCHANGE.SetTick(Tick());
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
	m_AccountInfo.InsertColumn(2, CString("持仓保证金"), 0, 90);
	m_AccountInfo.InsertColumn(3, CString("总盈亏"), 0, 70);
	m_AccountInfo.InsertColumn(4, CString("总手续费"), 0, 70);

	m_PositionInfo.InsertColumn(0, CString("持仓量"), 0, 90);
	m_PositionInfo.InsertColumn(1, CString("开仓价格"), 0, 90);
	m_PositionInfo.InsertColumn(2, CString("当前价格"), 0, 90);
	m_PositionInfo.InsertColumn(3, CString("浮动盈亏"), 0, 90);

	m_nSlots = 1;
	m_nFee = EXCHANGE.m_nFee;
	m_nMargin = EXCHANGE.m_nMargin;
	m_nUnitsPerSlot = EXCHANGE.m_nUnitsPerSlot;

	UpdateData(FALSE);

	UpdateAccountInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTradeDialog::UpdateAccountInfo(void)
{
	m_AccountInfo.DeleteAllItems();
	m_AccountInfo.InsertItem(0, IntToCString(EXCHANGE.m_nBalance));
	m_AccountInfo.SetItemText(0, 1, IntToCString(EXCHANGE.m_nBalance));
	m_AccountInfo.SetItemText(0, 2, IntToCString(0));
	m_AccountInfo.SetItemText(0, 3, IntToCString(0));
	m_AccountInfo.SetItemText(0, 4, IntToCString(0));

	m_PositionInfo.DeleteAllItems();
	m_PositionInfo.InsertItem(0, IntToCString(EXCHANGE.m_nPosition.nSlot));
	m_PositionInfo.SetItemText(0, 1, IntToCString(EXCHANGE.m_nPosition.nPrice));
	m_PositionInfo.SetItemText(0, 2, IntToCString(EXCHANGE.m_nTick.price));
	m_PositionInfo.SetItemText(0, 3, IntToCString(EXCHANGE.m_nPosition.nProfit));

	Utility::WriteBalance(EXCHANGE.m_nBalance);
}

void CTradeDialog::OnBnClickedButtonUpdateParam()
{
	UpdateData();
	EXCHANGE.SetParam(m_nFee, m_nMargin, m_nUnitsPerSlot);
	Utility::WriteExchangeConfig(m_nFee, m_nMargin, m_nUnitsPerSlot);
}
