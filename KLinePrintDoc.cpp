
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

#define CALENDAR ((CKLinePrintApp*)AfxGetApp())->cal

// CKLinePrintDoc

IMPLEMENT_DYNCREATE(CKLinePrintDoc, CDocument)

BEGIN_MESSAGE_MAP(CKLinePrintDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CKLinePrintDoc::OnFileOpen)
END_MESSAGE_MAP()


// CKLinePrintDoc 构造/析构

CKLinePrintDoc::CKLinePrintDoc()
{
	// TODO: 在此添加一次性构造代码

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
		klcday.clear();
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

	tc.clear();
	klc15s.clear();
	klc1min.clear();

	TickReader tr;

	tr.Read(targetCsvFile, tc);

	klc15s.Generate(tc, 15);
	klc1min.Generate(tc, 60, prevDayKLine);

	pView->Set1MinData(&klc1min);
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

	klc15s.clear();
	klc15s.Generate(tc, second);
	pView->Set5SecData(&klc15s);
	pView->Render();
	this->UpdateAllViews(0);
}

string CKLinePrintDoc::GetNeighborCsvFile(string path, bool bPrev, bool bZhuLi)
{
	int date;
	char buf[512];
	string rootdir, contract, market;

	DataRepoUtil::GetInfoByPath(path, rootdir, market, contract, date);

	int nNeighDate;

	if(bPrev)
		nNeighDate = CALENDAR.GetPrev(date);
	else
		nNeighDate = CALENDAR.GetNext(date); 

	sprintf(buf, "%s\\%s\\%s%d\\%d\\%s_%d.csv", 
		rootdir.c_str(),
		market.c_str(),
		market.c_str(),
		nNeighDate/100,
		nNeighDate,
		contract.c_str(),
		nNeighDate);

	if(bZhuLi) /* 搜索主力合约 */
	{
		return DataRepoUtil::GetMajorContractPath(buf);
	}
	else
	{
		return string(buf);
	}
}

void CKLinePrintDoc::ViewNeighborDate(BOOL bPrev)
{
	string tmp = GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* 必须是主力合约 */);
	LoadKLineGroup(tmp);
}
