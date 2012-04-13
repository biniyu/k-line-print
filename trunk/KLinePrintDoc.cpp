
// KLinePrintDoc.cpp : CKLinePrintDoc 类的实现
//

#include "stdafx.h"
#include "KLinePrint.h"

#include "MainFrm.h"
#include "KLinePrintView.h"
#include "KLinePrintDoc.h"
#include "TickReader.h"
#include "KLineCollection.h"
#include "CalendarGenerator.h"
#include "KLineReader.h"
#include "DataRepoUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKLinePrintDoc

IMPLEMENT_DYNCREATE(CKLinePrintDoc, CDocument)

BEGIN_MESSAGE_MAP(CKLinePrintDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CKLinePrintDoc::OnFileOpen)
END_MESSAGE_MAP()


// CKLinePrintDoc 构造/析构

CKLinePrintDoc::CKLinePrintDoc()
{
}

CKLinePrintDoc::~CKLinePrintDoc()
{
}

BOOL CKLinePrintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CKLinePrintDoc 序列化

void CKLinePrintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CKLinePrintDoc 诊断

#ifdef _DEBUG
void CKLinePrintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKLinePrintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKLinePrintDoc 命令

void CKLinePrintDoc::LoadKLineGroup(string targetCsvFile)
{
	// 只考虑主力合约
	targetCsvFile = DataRepoUtil::GetMajorContractPath(targetCsvFile);

	if(targetCsvFile == m_CurCsvFile) return;

	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	当前日期
	int nDate = DataRepoUtil::GetDateByPath(targetCsvFile);
	
	//	先加载日线数据(提供前一日的日K数据)
	KLineReader klReader;

	string dayLineFile = DataRepoUtil::GetDayLinePath(targetCsvFile);
	if(dayLineFile != m_CurDayFile)
	{
		// 读取相应的日线数据
		klcday.Clear();
		klReader.Read(dayLineFile, klcday);
		m_CurDayFile = dayLineFile;
	}

	pView->SetDayData(&klcday, nDate);

	//  获取前一交易日日K线
	KLine prevDayKLine;

	prevDayKLine = klcday.GetKLineByTime(CALENDAR.GetPrev(nDate));

	//	不关注前日的开盘价
	prevDayKLine.open = prevDayKLine.close;
	prevDayKLine.time = 0;
	prevDayKLine.vol = prevDayKLine.vol_acc = 0;

	tc.clear();
	klc15s.Clear();
	klc1min.Clear();

	TickReader tr;

	tr.Read(targetCsvFile, tc);

	klc15s.Generate(tc, 15);
	klc1min.Generate(tc, 60, prevDayKLine);

	klc1min.AddKeyPrice(prevDayKLine.ma5, "MA5");
	klc1min.AddKeyPrice(prevDayKLine.ma10, "MA10");
	klc1min.AddKeyPrice(prevDayKLine.ma20, "MA20");
	klc1min.AddKeyPrice(prevDayKLine.ma60, "MA60");
	pView->Set1MinData(&klc1min);

	klc15s.AddKeyPrice(prevDayKLine.ma5, "MA5");
	klc15s.AddKeyPrice(prevDayKLine.ma10, "MA10");
	klc15s.AddKeyPrice(prevDayKLine.ma20, "MA20");
	klc15s.AddKeyPrice(prevDayKLine.ma60, "MA60");
	pView->Set5SecData(&klc15s);

	pView->Render();

	m_CurCsvFile = targetCsvFile;

	this->SetTitle(CString((m_CurCsvFile + "|" + m_CurDayFile).c_str()));
	
	this->UpdateAllViews(0);
}

void CKLinePrintDoc::OnFileOpen()
{
	CFileDialog dlg(TRUE); ///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal() == IDOK)
	{
		CString FilePathName = dlg.GetPathName(); 
		char InfoString[256];    
		  
		// 转换后的数据存放在InfoString数组中   
		if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(FilePathName),-1,InfoString,100,NULL,NULL))    
		{    
			return;    
		} 

		LoadKLineGroup(InfoString);
	}
}

void CKLinePrintDoc::ReloadByDate(int nDate)
{
	string tmp = DataRepoUtil::GetPathByDate(m_CurCsvFile, nDate);
	LoadKLineGroup(tmp);
}

void CKLinePrintDoc::ReloadDetailData(int second)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	klc15s.Clear();
	klc15s.Generate(tc, second);
	pView->Set5SecData(&klc15s);
	pView->Render();
	this->UpdateAllViews(0);
}

void CKLinePrintDoc::ViewNeighborDate(BOOL bPrev)
{
	string tmp = DataRepoUtil::GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* 必须是主力合约 */);
	LoadKLineGroup(tmp);
}

int CKLinePrintDoc::GetCurrentTickTime()
{
	if(m_nCurrentTickIdx >= tc.size()) return 0;
	else return tc[m_nCurrentTickIdx].time;
}

BOOL CKLinePrintDoc::PlayTillTime(int nTillTime)
{
	int nDate = DataRepoUtil::GetDateByPath(m_CurCsvFile);

	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	while(m_nCurrentTickIdx < tc.size())
	{
		// 需要override时间
		Tick tmp = tc[m_nCurrentTickIdx];
		tmp.time = nDate;

		klcday.Quote(tmp);
		klc1min.Quote(tc[m_nCurrentTickIdx]);
		klc15s.Quote(tc[m_nCurrentTickIdx]);

		m_nCurrentTickIdx++;

		if(m_nCurrentTickIdx >= tc.size()) 
			break;

		if(tc[m_nCurrentTickIdx].time > nTillTime)
			break;
	}

	pView->Render();	
	this->UpdateAllViews(0);

	if(m_nCurrentTickIdx == tc.size()) 
		return FALSE;
	else 
		return TRUE;
}

void CKLinePrintDoc::DisplayTillTime(int nTillTime)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	klcday.Clear();

	//	当前日期
	int nDate = DataRepoUtil::GetDateByPath(m_CurCsvFile);

	//	当日的日线不读入	
	KLineReader klReader;
	klReader.Read(m_CurDayFile, klcday, nDate/**/);

	//  获取前一交易日日K线
	KLine prevDayKLine;
	prevDayKLine = klcday.GetKLineByTime(CALENDAR.GetPrev(nDate));

	//	不关注前日的开盘价
	prevDayKLine.open = prevDayKLine.close;
	prevDayKLine.time = 0;
	prevDayKLine.vol = prevDayKLine.vol_acc = 0;

	klc15s.Clear();
	klc1min.Clear();

	klc1min.AddKeyPrice(prevDayKLine.ma5, "MA5");
	klc1min.AddKeyPrice(prevDayKLine.ma10, "MA10");
	klc1min.AddKeyPrice(prevDayKLine.ma20, "MA20");
	klc1min.AddKeyPrice(prevDayKLine.ma60, "MA60");

	klc15s.AddKeyPrice(prevDayKLine.ma5, "MA5");
	klc15s.AddKeyPrice(prevDayKLine.ma10, "MA10");
	klc15s.AddKeyPrice(prevDayKLine.ma20, "MA20");
	klc15s.AddKeyPrice(prevDayKLine.ma60, "MA60");

	/* 前日日K */
	klc1min.AddToTail(prevDayKLine);
	
	m_nCurrentTickIdx = 0;

	klcday.SetPeriod(36000);
	klc1min.SetPeriod(60);
	klc15s.SetPeriod(15);

	while(m_nCurrentTickIdx < tc.size())
	{
		// 需要override时间
		Tick tmp = tc[m_nCurrentTickIdx];
		tmp.time = nDate;

		if(0 == m_nCurrentTickIdx)
		{
			klcday.StartQuote(tmp);
			klc1min.StartQuote(tc[m_nCurrentTickIdx]);
			klc15s.StartQuote(tc[m_nCurrentTickIdx]);
		}
		else
		{
			klcday.Quote(tmp);
			klc1min.Quote(tc[m_nCurrentTickIdx]);
			klc15s.Quote(tc[m_nCurrentTickIdx]);
		}

		m_nCurrentTickIdx++;

		if(nTillTime == 0 && m_nCurrentTickIdx == 1) 
			break;

		if(nTillTime != -1 && tc[m_nCurrentTickIdx].time > nTillTime)
			break;

	}

	pView->Render();	
	this->UpdateAllViews(0);
}
