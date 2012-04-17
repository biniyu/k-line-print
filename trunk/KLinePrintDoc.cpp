
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
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	只考虑主力合约，确定分笔数据文件和日线文件
	string tmp;

	tmp = DataRepoUtil::GetMajorContractPath(targetCsvFile);

	if(!tmp.size()) return;

	m_CurCsvFile = tmp;
	m_CurDayFile = DataRepoUtil::GetDayLinePath(m_CurCsvFile);

	//	读入分笔数据
	m_TickData.clear();

	DWORD before = GetTickCount();
	m_TickReader.Read(m_CurCsvFile, m_TickData);
	DWORD after = GetTickCount();
	TRACE("\ntick file %s read, use %d ticks", m_CurCsvFile.c_str(), after - before);


	//	更新标题
	SetTitle(CString((m_CurCsvFile + "|" + m_CurDayFile).c_str()));
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

		//	显示所有
		DisplayTill(-1, -1);
	}
}

void CKLinePrintDoc::ReloadByDate(int nDate)
{
	string tmp = DataRepoUtil::GetPathByDate(m_CurCsvFile, nDate);

	if(!tmp.size()) return;

	LoadKLineGroup(tmp);
	DisplayTill(-1, -1);
}

void CKLinePrintDoc::ReloadDetailData(int second)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	m_15SecData.Clear();
	m_15SecData.Generate(m_TickData, second);
	pView->Set5SecData(&m_15SecData);
	pView->Render();
	this->UpdateAllViews(0);
}

void CKLinePrintDoc::ViewNeighborDate(BOOL bPrev)
{
	string tmp = DataRepoUtil::GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* 必须是主力合约 */);

	if(!tmp.size()) return;

	LoadKLineGroup(tmp);
	DisplayTill(-1, -1);
}

BOOL CKLinePrintDoc::LoadNextDay()
{
	string tmp = DataRepoUtil::GetNeighborCsvFile(m_CurCsvFile, FALSE, TRUE/* 必须是主力合约 */);

	if(!tmp.size()) return FALSE;

	LoadKLineGroup(tmp);
	int nDate = DataRepoUtil::GetDateByPath(tmp);
	DisplayTill(0, nDate);
	return TRUE;
}

int CKLinePrintDoc::GetCurrentTickTime()
{
	if(m_nCurrentTickIdx >= m_TickData.size()) return 0;
	else return m_TickData[m_nCurrentTickIdx].time;
}

BOOL CKLinePrintDoc::PlayTillTime(int nTillTime)
{
	int nDate = DataRepoUtil::GetDateByPath(m_CurCsvFile);

	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	while(m_nCurrentTickIdx < m_TickData.size())
	{
		// 需要override时间
		Tick tmp = m_TickData[m_nCurrentTickIdx];
		tmp.time = nDate;

		m_DayData.Quote(tmp);
		m_1MinData.Quote(m_TickData[m_nCurrentTickIdx]);
		m_15SecData.Quote(m_TickData[m_nCurrentTickIdx]);
		pView->SetTickData(m_TickData[m_nCurrentTickIdx]);

		m_nCurrentTickIdx++;

		if(m_nCurrentTickIdx >= m_TickData.size()) 
			break;

		if(m_TickData[m_nCurrentTickIdx].time >= nTillTime)
			break;
	}

	if(m_nCurrentTickIdx == m_TickData.size()) 
		return FALSE;
	else 
		return TRUE;
}

void CKLinePrintDoc::DisplayTill(int nTillTime, int nTillDate)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	m_DayData.Clear();
	m_15SecData.Clear();
	m_1MinData.Clear();

	//	当前日期
	int nDate = DataRepoUtil::GetDateByPath(m_CurCsvFile);

	DWORD before = GetTickCount();
	m_KLineReader.Read(m_CurDayFile, m_DayData, nTillDate/**/);
	DWORD after = GetTickCount();
	TRACE("\nday line %s read, use %d ticks", m_CurDayFile.c_str(), after - before);

	//  获取前一交易日日K线
	KLine prevDayKLine;
	prevDayKLine = m_DayData.GetKLineByTime(CALENDAR.GetPrev(nDate));

	//	不关注前日的开盘价
	prevDayKLine.open = prevDayKLine.close;
	prevDayKLine.time = 0;
	prevDayKLine.vol = prevDayKLine.vol_acc = 0;

	m_1MinData.AddKeyPrice(prevDayKLine.ma5, "MA5");
	m_1MinData.AddKeyPrice(prevDayKLine.ma10, "MA10");
	m_1MinData.AddKeyPrice(prevDayKLine.ma20, "MA20");
	m_1MinData.AddKeyPrice(prevDayKLine.ma60, "MA60");

	m_1MinData.AddKeyPrice(prevDayKLine.high, "H1");
	m_1MinData.AddKeyPrice(prevDayKLine.high5, "H5");
	m_1MinData.AddKeyPrice(prevDayKLine.high10, "H10");
	m_1MinData.AddKeyPrice(prevDayKLine.high20, "H20");
	m_1MinData.AddKeyPrice(prevDayKLine.high60, "H60");

	m_1MinData.AddKeyPrice(prevDayKLine.low, "L1");
	m_1MinData.AddKeyPrice(prevDayKLine.low5, "L5");
	m_1MinData.AddKeyPrice(prevDayKLine.low10, "L10");
	m_1MinData.AddKeyPrice(prevDayKLine.low20, "L20");
	m_1MinData.AddKeyPrice(prevDayKLine.low60, "L60");

	m_15SecData.AddKeyPrice(prevDayKLine.ma5, "MA5");
	m_15SecData.AddKeyPrice(prevDayKLine.ma10, "MA10");
	m_15SecData.AddKeyPrice(prevDayKLine.ma20, "MA20");
	m_15SecData.AddKeyPrice(prevDayKLine.ma60, "MA60");

	m_15SecData.AddKeyPrice(prevDayKLine.high, "H1");
	m_15SecData.AddKeyPrice(prevDayKLine.high5, "H5");
	m_15SecData.AddKeyPrice(prevDayKLine.high10, "H10");
	m_15SecData.AddKeyPrice(prevDayKLine.high20, "H20");
	m_15SecData.AddKeyPrice(prevDayKLine.high60, "H60");

	m_15SecData.AddKeyPrice(prevDayKLine.low, "L1");
	m_15SecData.AddKeyPrice(prevDayKLine.low5, "L5");
	m_15SecData.AddKeyPrice(prevDayKLine.low10, "L10");
	m_15SecData.AddKeyPrice(prevDayKLine.low20, "L20");
	m_15SecData.AddKeyPrice(prevDayKLine.low60, "L60");


	/* 前日日K */
	m_1MinData.AddToTail(prevDayKLine);
	
	m_nCurrentTickIdx = 0;

	m_DayData.SetPeriod(36000);
	m_1MinData.SetPeriod(60);
	m_15SecData.SetPeriod(15);

	before = GetTickCount();

	while(m_nCurrentTickIdx < m_TickData.size())
	{
		// 需要override时间
		Tick tmp = m_TickData[m_nCurrentTickIdx];
		tmp.time = nDate;

		if(0 == m_nCurrentTickIdx)
		{
			m_DayData.StartQuote(tmp);
			m_1MinData.StartQuote(m_TickData[m_nCurrentTickIdx]);
			m_15SecData.StartQuote(m_TickData[m_nCurrentTickIdx]);
		}
		else
		{
			m_DayData.Quote(tmp);
			m_1MinData.Quote(m_TickData[m_nCurrentTickIdx]);
			m_15SecData.Quote(m_TickData[m_nCurrentTickIdx]);
		}

		pView->SetTickData(m_TickData[m_nCurrentTickIdx]);

		m_nCurrentTickIdx++;

		if(nTillTime == 0 && m_nCurrentTickIdx == 1) 
			break;

		if(nTillTime != -1 && m_TickData[m_nCurrentTickIdx].time > nTillTime)
			break;

	}

	after = GetTickCount();
	TRACE("\n1min/15s generated, use %d ticks", after - before);


	//	设置数据
	pView->SetDayData(&m_DayData, nDate);
	pView->Set1MinData(&m_1MinData);
	pView->Set5SecData(&m_15SecData);

	//	显示
	pView->Render();	
	this->UpdateAllViews(0);
}
