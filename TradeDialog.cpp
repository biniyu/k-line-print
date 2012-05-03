// TradeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeDialog.h"
#include "Utility.h"

// CTradeDialog 对话框

IMPLEMENT_DYNAMIC(CTradeDialog, CDialog)

CTradeDialog::CTradeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeDialog::IDD, pParent)
	, m_nSlots(_T(""))
{
	m_tf.SetBalance(50000);
	m_tf.SetParam(10, 12, 5);
}

CTradeDialog::~CTradeDialog()
{
}

void CTradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SLOTS, m_nSlots);
	DDX_Control(pDX, IDC_LIST_ACCOUNT, m_AccountInfo);
	DDX_Control(pDX, IDC_LIST_POSITION, m_PositionInfo);
}


BEGIN_MESSAGE_MAP(CTradeDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BUY, &CTradeDialog::OnBnClickedButtonBuy)
	ON_BN_CLICKED(IDC_BUTTON_SELL, &CTradeDialog::OnBnClickedButtonSell)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTradeDialog::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CTradeDialog::OnBnClickedButtonReverse)
END_MESSAGE_MAP()



// CTradeDialog 消息处理程序

void CTradeDialog::OnBnClickedButtonBuy()
{
	UpdateData();
	m_tf.Buy(CStringToInt(m_nSlots));
	UpdateAccountInfo();

}

void CTradeDialog::OnBnClickedButtonSell()
{
	UpdateData();
	m_tf.Sell(CStringToInt(m_nSlots));
	UpdateAccountInfo();
}

void CTradeDialog::OnBnClickedButtonClose()
{
	m_tf.Close();
	UpdateAccountInfo();
}

void CTradeDialog::OnBnClickedButtonReverse()
{
	m_tf.Reverse();
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
	m_PositionInfo.InsertColumn(2, CString("浮动盈亏"), 0, 90);

	m_nSlots = CString("1");
	UpdateData(FALSE);

	UpdateAccountInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTradeDialog::SetTick(Tick tick) 
{ 
	m_tf.SetTick(tick);
	UpdateAccountInfo();
}


void CTradeDialog::UpdateAccountInfo(void)
{
	m_AccountInfo.DeleteAllItems();
	m_AccountInfo.InsertItem(0, IntToCString(m_tf.m_nBalance));
	m_AccountInfo.SetItemText(0, 1, IntToCString(m_tf.m_nBalance));
	m_AccountInfo.SetItemText(0, 2, IntToCString(0));
	m_AccountInfo.SetItemText(0, 3, IntToCString(0));
	m_AccountInfo.SetItemText(0, 4, IntToCString(0));

	m_PositionInfo.DeleteAllItems();
	m_PositionInfo.InsertItem(0, IntToCString(m_tf.m_nPosition.nSlot));
	m_PositionInfo.SetItemText(0, 1, IntToCString(m_tf.m_nPosition.nPrice));
	m_PositionInfo.SetItemText(0, 2, IntToCString(m_tf.m_nPosition.nProfit));
}


