// TradeLogDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeLogDialog.h"
#include "Utility.h"
#include "MainFrm.h"
#include "KLinePrintDoc.h"
#include "KLinePrintView.h"

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
	DDX_Control(pDX, IDC_COMBO_LOGFILE, m_comboLogFiles);
}


BEGIN_MESSAGE_MAP(CTradeLogDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_DATE, &CTradeLogDialog::OnLbnSelchangeListDate)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, &CTradeLogDialog::OnLbnSelchangeListFile)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOG, &CTradeLogDialog::OnNMClickListLog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOG, &CTradeLogDialog::OnLvnItemchangedListLog)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGFILE, &CTradeLogDialog::OnCbnSelchangeComboLogfile)
	ON_WM_PAINT()
	ON_CBN_DROPDOWN(IDC_COMBO_LOGFILE, &CTradeLogDialog::OnCbnDropdownComboLogfile)
END_MESSAGE_MAP()


// CTradeLogDialog 消息处理程序

BOOL CTradeLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	m_ctlListFile.ResetContent();
	m_ctlListLog.DeleteAllItems();

	m_ctlListDate.GetText(nSelIdx, strSelText);

	int nDate = CStringToInt(strSelText);

	map<string, TradeRecordCollection>::reverse_iterator rit;
	map<string, TradeRecordCollection> trs;

	trs = m_trs[nDate];

	for(rit = trs.rbegin(); rit != trs.rend(); rit++)
	{
		m_ctlListFile.AddString(CString(rit->first.c_str()));
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

	m_curfile = CStringTostring(strSelText);

	TradeRecordCollection trs;
	trs = m_trs[nDate][m_curfile];

	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pDoc->OnOpenDocument(strSelText);
	pDoc->DisplayTill();
	pDoc->SetTradeRecord(trs);
	pView->Render();
	pDoc->UpdateAllViews(0);

	int i, nTotalFee = 0, nTotalProfit = 0;

	for(i = 0; i < trs.size(); i++)
	{
		int nDispTime = Utility::ConvContTimeToDispTime(trs[i].nSimuTime / 1000);
		
		tmp.Format(_T("%02d:%02d:%02d"), 
					nDispTime / 10000, 
					nDispTime % 10000 / 100, 
					nDispTime % 10000 % 100);

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

		if(trs[i].bOpen)
		{
			m_ctlListLog.SetItemText(i, 6, _T("-"));			
		}
		else
		{
			tmp.Format(_T("%d"), trs[i].nProfit);
			m_ctlListLog.SetItemText(i, 6, tmp);	
		}

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

	Invalidate(FALSE);
}

void CTradeLogDialog::OnNMClickListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//	高亮交易记号
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pDoc->SetSelTradeIdx(pNMItemActivate->iItem);

	pView->Render();
	pDoc->UpdateAllViews(0);

	*pResult = 0;
}

void CTradeLogDialog::OnLvnItemchangedListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pDoc->SetSelTradeIdx(pNMLV->iItem);

	pView->Render();
	pDoc->UpdateAllViews(0);

	*pResult = 0;
}

void CTradeLogDialog::OnCbnSelchangeComboLogfile()
{
	CString tmp;

	m_comboLogFiles.GetWindowTextW(tmp);

	string filename = CStringTostring(tmp);

	filename = Utility::GetProgramPath() + "log\\" + filename;

	TradeRecordCollection logs;

	Utility::ReadLog(filename, logs);

	m_trs.clear();

	m_ctlListDate.ResetContent();
	m_ctlListFile.ResetContent();
	m_ctlListLog.DeleteAllItems();

	int capital = 0;
	string lastfile;

	m_capital.clear();

	int date = 0;

	for(int i = 0; i < logs.size(); i++)
	{
		TradeRecord tmp = logs[i];
		m_trs[tmp.nRealDate][tmp.sFileName].push_back(tmp);

		if((tmp.sFileName != lastfile) && lastfile.size())
		{
			date = Utility::GetDateByPath(lastfile);		
			m_capital[date] = capital;
		}

		if(!tmp.bOpen)
		{
			capital += tmp.nProfit;
		}

		capital -= tmp.nFee;

		lastfile = tmp.sFileName;
	}

	date = Utility::GetDateByPath(lastfile);		
	m_capital[date] = capital;

	//	先填入练习日期
	map<int, map<string, TradeRecordCollection>>::reverse_iterator it;
	for(it = m_trs.rbegin(); it != m_trs.rend(); it++)
	{
		CString tmp;
		tmp.Format(_T("%d"),it->first); 
		m_ctlListDate.AddString(tmp);
	}	

	this->Invalidate(FALSE);
 }

void CTradeLogDialog::OnPaint()
{
	CRect rc;
	CPaintDC dc(this); // device context for painting

	CWnd * pWnd = GetDlgItem(IDC_STATIC_CAPITAL);   //IDC_STATIC1 specified in the dialog editor

#define DEFAULT_DAYS  100
	
	int maxabs = 0;
	int ndays = DEFAULT_DAYS;

	map<int,int>::iterator it2;
	for(it2 = m_capital.begin(); it2!= m_capital.end(); it2++)
	{
		TRACE("\n%d %d", it2->first, it2->second);

		if(abs(it2->second) > maxabs) 
			maxabs = abs(it2->second);
	}

	CDC * pControlDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pWnd->GetClientRect(&rc);
	pControlDC->SelectStockObject(GRAY_BRUSH);
	pControlDC->Rectangle(rc);

	if(m_capital.size() > DEFAULT_DAYS) 
		ndays = m_capital.size();

	pControlDC->SelectStockObject(BLACK_PEN);

	pControlDC->MoveTo(0, rc.Height() / 2);
	pControlDC->LineTo(rc.Width(), rc.Height() / 2);
	
	float pixPerProfit = rc.Height() / (-2.0f * maxabs);
	float pixPerDay = rc.Width() / ndays;

	float x, y, lastx, lasty;
	pControlDC->SelectStockObject(WHITE_PEN);

	ndays = 0;
	for(it2 = m_capital.begin(); it2!= m_capital.end(); it2++)
	{
		x = pixPerDay * ndays;
		y = rc.Height() / 2 + it2->second * pixPerProfit;

		if(it2->first == Utility::GetDateByPath(m_curfile))
		{
			pControlDC->SelectStockObject(BLACK_PEN);
			pControlDC->MoveTo(x, 0);
			pControlDC->LineTo(x, rc.Height());
		}

		if(it2 !=  m_capital.begin())
		{
			pControlDC->SelectStockObject(WHITE_PEN);
			pControlDC->MoveTo(lastx, lasty);
			pControlDC->LineTo(x, y);
		}

		lastx = x;
		lasty = y;

		ndays++;
	}

	pWnd->ReleaseDC(pControlDC);
}

void CTradeLogDialog::OnCbnDropdownComboLogfile()
{
	vector<string> vecFiles;

	string logPath = Utility::GetProgramPath() + "log\\";

	//	所有日志文件
	vecFiles = GetFiles(logPath, "*.log.txt", false);

	m_comboLogFiles.ResetContent();

	for(int i = 0; i < vecFiles.size(); i++)
	{
		m_comboLogFiles.AddString(CString(vecFiles[i].c_str()));
	}

}
