
// KLinePrintDoc.cpp : CKLinePrintDoc ���ʵ��
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


// CKLinePrintDoc ����/����

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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CKLinePrintDoc ���л�

void CKLinePrintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CKLinePrintDoc ���

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


// CKLinePrintDoc ����

void CKLinePrintDoc::LoadKLineGroup(string targetCsvFile)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	ֻ����������Լ��ȷ���ֱ������ļ��������ļ�
	m_CurCsvFile = DataRepoUtil::GetMajorContractPath(targetCsvFile);
	m_CurDayFile = DataRepoUtil::GetDayLinePath(m_CurCsvFile);

	//	����ֱ�����
	tc.clear();
	TickReader tr;
	tr.Read(m_CurCsvFile, tc);

	//	���±���
	SetTitle(CString((m_CurCsvFile + "|" + m_CurDayFile).c_str()));

	//	��ʾK��
	DisplayTill(-1, -1);
}

void CKLinePrintDoc::OnFileOpen()
{
	CFileDialog dlg(TRUE); ///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal() == IDOK)
	{
		CString FilePathName = dlg.GetPathName(); 
		char InfoString[256];    
		  
		// ת��������ݴ����InfoString������   
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
	string tmp = DataRepoUtil::GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* ������������Լ */);
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
		// ��Ҫoverrideʱ��
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

	if(m_nCurrentTickIdx == tc.size()) 
		return FALSE;
	else 
		return TRUE;
}

void CKLinePrintDoc::DisplayTill(int nTillTime, int nTillDate)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	klcday.Clear();
	klc15s.Clear();
	klc1min.Clear();

	//	��ǰ����
	int nDate = DataRepoUtil::GetDateByPath(m_CurCsvFile);

	//	���յ����߲�����	
	KLineReader klReader;
	klReader.Read(m_CurDayFile, klcday, nTillDate/**/);

	//  ��ȡǰһ��������K��
	KLine prevDayKLine;
	prevDayKLine = klcday.GetKLineByTime(CALENDAR.GetPrev(nDate));

	//	����עǰ�յĿ��̼�
	prevDayKLine.open = prevDayKLine.close;
	prevDayKLine.time = 0;
	prevDayKLine.vol = prevDayKLine.vol_acc = 0;

	klc1min.AddKeyPrice(prevDayKLine.ma5, "MA5");
	klc1min.AddKeyPrice(prevDayKLine.ma10, "MA10");
	klc1min.AddKeyPrice(prevDayKLine.ma20, "MA20");
	klc1min.AddKeyPrice(prevDayKLine.ma60, "MA60");

	klc1min.AddKeyPrice(prevDayKLine.high, "HIGH1");
	klc1min.AddKeyPrice(prevDayKLine.high5, "HIGH5");
	klc1min.AddKeyPrice(prevDayKLine.high10, "HIGH10");
	klc1min.AddKeyPrice(prevDayKLine.high20, "HIGH20");
	klc1min.AddKeyPrice(prevDayKLine.high60, "HIGH60");

	klc1min.AddKeyPrice(prevDayKLine.low, "LOW1");
	klc1min.AddKeyPrice(prevDayKLine.low5, "LOW5");
	klc1min.AddKeyPrice(prevDayKLine.low10, "LOW10");
	klc1min.AddKeyPrice(prevDayKLine.low20, "LOW20");
	klc1min.AddKeyPrice(prevDayKLine.low60, "LOW60");

	klc15s.AddKeyPrice(prevDayKLine.ma5, "MA5");
	klc15s.AddKeyPrice(prevDayKLine.ma10, "MA10");
	klc15s.AddKeyPrice(prevDayKLine.ma20, "MA20");
	klc15s.AddKeyPrice(prevDayKLine.ma60, "MA60");

	/* ǰ����K */
	klc1min.AddToTail(prevDayKLine);
	
	m_nCurrentTickIdx = 0;

	klcday.SetPeriod(36000);
	klc1min.SetPeriod(60);
	klc15s.SetPeriod(15);

	while(m_nCurrentTickIdx < tc.size())
	{
		// ��Ҫoverrideʱ��
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

	//	��������
	pView->SetDayData(&klcday, nDate);
	pView->Set1MinData(&klc1min);
	pView->Set5SecData(&klc15s);

	//	��ʾ
	pView->Render();	
	this->UpdateAllViews(0);
}
