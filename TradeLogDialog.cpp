// TradeLogDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeLogDialog.h"
#include "Utility.h"

// CTradeLogDialog 对话框

IMPLEMENT_DYNAMIC(CTradeLogDialog, CDialog)

CTradeLogDialog::CTradeLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeLogDialog::IDD, pParent)
{

}

CTradeLogDialog::~CTradeLogDialog()
{
}

void CTradeLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATE, m_ctlListDate);
	DDX_Control(pDX, IDC_LIST_FILE, m_ctlListFile);
	DDX_Control(pDX, IDC_LIST_LOG, m_ctlListLog);
}


BEGIN_MESSAGE_MAP(CTradeLogDialog, CDialog)
END_MESSAGE_MAP()


// CTradeLogDialog 消息处理程序

BOOL CTradeLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	Utility::ReadLog(m_trs);

	//	先填入练习日期

	m_ctlListLog.InsertColumn(0, _T("时间"), 0, 60);
	m_ctlListLog.InsertColumn(1, _T("买/卖"), 0, 60);
	m_ctlListLog.InsertColumn(2, _T("开/平"), 0, 60);
	m_ctlListLog.InsertColumn(3, _T("价格"), 0, 60);
	m_ctlListLog.InsertColumn(4, _T("数量"), 0, 60);
	m_ctlListLog.InsertColumn(5, _T("手续费"), 0, 70);
	m_ctlListLog.InsertColumn(6, _T("盈亏"), 0, 60);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
