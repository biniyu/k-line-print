// TradeLogDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeLogDialog.h"
#include "Utility.h"
#include "MainFrm.h"
#include "KLinePrintDoc.h"

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
	ON_LBN_SELCHANGE(IDC_LIST_DATE, &CTradeLogDialog::OnLbnSelchangeListDate)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, &CTradeLogDialog::OnLbnSelchangeListFile)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOG, &CTradeLogDialog::OnNMClickListLog)
END_MESSAGE_MAP()


// CTradeLogDialog 消息处理程序

BOOL CTradeLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	vector<TradeRecord> logs;
	Utility::ReadLog(logs);

	for(int i = 0; i < logs.size(); i++)
	{
		TradeRecord tmp = logs[i];
		m_trs[tmp.nRealDate][tmp.sFileName].push_back(tmp);
	}

	//	先填入练习日期
	map<int, map<string, vector<TradeRecord>>>::reverse_iterator it;
	for(it = m_trs.rbegin(); it != m_trs.rend(); it++)
	{
		CString tmp;
		tmp.Format(_T("%d"),it->first); 
		m_ctlListDate.AddString(tmp);
	}

	m_ctlListLog.InsertColumn(0, _T("时间"), 0, 60);
	m_ctlListLog.InsertColumn(1, _T("买/卖"), 0, 40);
	m_ctlListLog.InsertColumn(2, _T("开/平"), 0, 40);
	m_ctlListLog.InsertColumn(3, _T("价格"), 0, 60);
	m_ctlListLog.InsertColumn(4, _T("数量"), 0, 40);
	m_ctlListLog.InsertColumn(5, _T("费"), 0, 40);
	m_ctlListLog.InsertColumn(6, _T("盈亏"), 0, 60);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTradeLogDialog::OnLbnSelchangeListDate()
{
	int nSelIdx;
	//	填入该日的文件
	CString strSelText;

	nSelIdx = m_ctlListDate.GetCurSel();

	if(nSelIdx < 0) return;

	for(int i = m_ctlListFile.GetCount() - 1; i >=0; i--)
	{
		m_ctlListFile.DeleteString(i);
	}

	m_ctlListLog.DeleteAllItems();

	m_ctlListDate.GetText(nSelIdx, strSelText);

	int nDate = CStringToInt(strSelText);

	map<string, vector<TradeRecord>> trs;

	trs = m_trs[nDate];

	map<string, vector<TradeRecord>>::iterator it;
	for(it = trs.begin(); it != trs.end(); it++)
	{
		CString tmp(it->first.c_str()); 
		m_ctlListFile.AddString(tmp);
	}
}

void CTradeLogDialog::OnLbnSelchangeListFile()
{
	//	打开文件并显示交易记号
	CString strSelText, tmp;
	int nSelIdx, nSelDate = 0;

	nSelIdx = m_ctlListDate.GetCurSel();

	if(nSelIdx < 0) return;

	m_ctlListDate.GetText(nSelIdx, strSelText);
	int nDate = CStringToInt(strSelText);

	nSelIdx = m_ctlListFile.GetCurSel();

	if(nSelIdx < 0) return;

	m_ctlListFile.GetText(nSelIdx, strSelText);

	m_ctlListLog.DeleteAllItems();

	//	加载文件
	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	pDoc->OnOpenDocument(strSelText);

	string filename = CStringTostring(strSelText);

	vector<TradeRecord> trs;
	trs = m_trs[nDate][filename];

	int i, nTotalFee = 0, nTotalProfit = 0;

	for(i = 0; i < trs.size(); i++)
	{
		int nDispTime = Utility::ConvContTimeToDispTime(trs[i].nSimuTime / 1000);
		tmp.Format(_T("%d"), nDispTime);
		m_ctlListLog.InsertItem(i, tmp);

		if(trs[i].bBuy)
			m_ctlListLog.SetItemText(i, 1, _T("买"));
		else
			m_ctlListLog.SetItemText(i, 1, _T("卖"));

		if(trs[i].bOpen)
			m_ctlListLog.SetItemText(i, 2, _T("开"));
		else
			m_ctlListLog.SetItemText(i, 2, _T("平"));

		tmp.Format(_T("%d"), trs[i].nPrice);
		m_ctlListLog.SetItemText(i, 3, tmp);

		tmp.Format(_T("%d"), trs[i].nSlot);
		m_ctlListLog.SetItemText(i, 4, tmp);

		tmp.Format(_T("%d"), trs[i].nFee);
		m_ctlListLog.SetItemText(i, 5, tmp);

		nTotalFee += trs[i].nFee;

		tmp.Format(_T("%d"), trs[i].nProfit);
		m_ctlListLog.SetItemText(i, 6, tmp);	

		nTotalProfit += trs[i].nProfit;
	}

	//	最后一行显示汇总信息
	m_ctlListLog.InsertItem(i, _T("汇总"));
	m_ctlListLog.SetItemText(i, 1, _T("-"));
	m_ctlListLog.SetItemText(i, 2, _T("-"));
	m_ctlListLog.SetItemText(i, 3, _T("-"));
	m_ctlListLog.SetItemText(i, 4, _T("-"));

	tmp.Format(_T("%d"), nTotalFee);
	m_ctlListLog.SetItemText(i, 5, tmp);

	tmp.Format(_T("%d"), nTotalProfit);
	m_ctlListLog.SetItemText(i, 6, tmp);
}

void CTradeLogDialog::OnNMClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//	高亮交易记号
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
