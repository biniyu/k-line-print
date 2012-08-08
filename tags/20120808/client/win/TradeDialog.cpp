// TradeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeDialog.h"
#include "Utility.h"
#include "MainFrm.h"
#include "KLinePrintDoc.h"

// CTradeDialog �Ի���

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
}


BEGIN_MESSAGE_MAP(CTradeDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BUY, &CTradeDialog::OnBnClickedButtonBuy)
	ON_BN_CLICKED(IDC_BUTTON_SELL, &CTradeDialog::OnBnClickedButtonSell)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTradeDialog::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CTradeDialog::OnBnClickedButtonReverse)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_PARAM, &CTradeDialog::OnBnClickedButtonUpdateParam)
END_MESSAGE_MAP()



// CTradeDialog ��Ϣ�������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_AccountInfo.InsertColumn(0, CString("�ڳ�Ȩ��"), 0, 70);
	m_AccountInfo.InsertColumn(1, CString("��ǰȨ��"), 0, 70);
	m_AccountInfo.InsertColumn(2, CString("��֤��"), 0, 60);
	m_AccountInfo.InsertColumn(3, CString("ë��"), 0, 60);
	m_AccountInfo.InsertColumn(4, CString("������"), 0, 60);
	m_AccountInfo.InsertColumn(5, CString("����"), 0, 60);

	m_PositionInfo.InsertColumn(0, CString("ʱ��"), 0, 60);
	m_PositionInfo.InsertColumn(1, CString("����"), 0, 40);
	m_PositionInfo.InsertColumn(2, CString("���ּ۸�"), 0, 90);
	m_PositionInfo.InsertColumn(3, CString("��ǰ�۸�"), 0, 90);
	m_PositionInfo.InsertColumn(4, CString("����ӯ��"), 0, 90);

	m_nDefaultSlots = m_nSlots = EXCHANGE.m_nDefaultSlots;
	m_nFee = EXCHANGE.m_nFee;
	m_nMargin = EXCHANGE.m_nMargin;
	m_nUnitsPerSlot = EXCHANGE.m_nUnitsPerSlot;
	m_nMaxLoss = EXCHANGE.m_nMaxLoss;
	m_nMaxProfit = EXCHANGE.m_nMaxProfit;
	m_nTimeStop = EXCHANGE.m_nTimeStop;

	UpdateData(FALSE);

	UpdateAccountInfo();

	m_btnBuy.EnableWindow(FALSE);
	m_btnSell.EnableWindow(FALSE);
	m_btnClose.EnableWindow(FALSE);
	m_btnReverse.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// �����ǰӯ���������ֹ��ֹӯֵ��ƽ�ֲ���ʾ

	TradeFacility& tf = EXCHANGE;

	if(m_nMaxLoss && (EXCHANGE.m_nPosition.nProfit <= -m_nMaxLoss))
	{
		CURDOC->AppendTradeRecord(EXCHANGE.Close());
		AfxMessageBox(_T("ֹ��ƽ��!"));
	}

	if(m_nMaxProfit && (EXCHANGE.m_nPosition.nProfit >= m_nMaxProfit))
	{
		CURDOC->AppendTradeRecord(EXCHANGE.Close());
		AfxMessageBox(_T("ֹӯƽ��!"));
	}

	//	�����ǰʱ�䳬��ʱ��ֹ���һ����ڿ���ǿ��ƽ��
	if(m_nTimeStop && tf.m_nPosition.nTime)
	{
		if(tf.m_nTick.time_ms - tf.m_nPosition.nTime > m_nTimeStop * 1000
			&& tf.m_nPosition.nProfit < 0 )
		{
			CURDOC->AppendTradeRecord(EXCHANGE.Close());
			AfxMessageBox(_T("ʱ��ֹ��ƽ��!"));
		}
	}

	if(m_AccountInfo.GetItemCount() == 0)
		m_AccountInfo.InsertItem(0, IntToCString(0));
	
	m_AccountInfo.SetItemText(0, 0, IntToCString(EXCHANGE.m_nIntialBalance));
	m_AccountInfo.SetItemText(0, 1, IntToCString(EXCHANGE.m_nBalance));
	m_AccountInfo.SetItemText(0, 2, IntToCString(0));
	m_AccountInfo.SetItemText(0, 3, IntToCString(EXCHANGE.m_nTotalProfit));
	m_AccountInfo.SetItemText(0, 4, IntToCString(EXCHANGE.m_nTotalFee));
	m_AccountInfo.SetItemText(0, 5, IntToCString(EXCHANGE.m_nTotalProfit - EXCHANGE.m_nTotalFee));

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

	Utility::WriteBalance(EXCHANGE.m_nBalance);
}

void CTradeDialog::OnBnClickedButtonUpdateParam()
{
	UpdateData();
	EXCHANGE.SetParam(m_nFee, m_nMargin, m_nUnitsPerSlot);
	EXCHANGE.m_nDefaultSlots = m_nDefaultSlots;
	EXCHANGE.m_nMaxLoss = m_nMaxLoss;
	EXCHANGE.m_nMaxProfit = m_nMaxProfit;
	EXCHANGE.m_nTimeStop = m_nTimeStop;
	Utility::WriteExchangeConfig(m_nFee, m_nMargin, m_nUnitsPerSlot, 
								m_nDefaultSlots, m_nMaxLoss, m_nMaxProfit, m_nTimeStop);
}
