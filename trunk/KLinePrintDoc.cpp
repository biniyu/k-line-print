
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

#define CALENDAR ((CKLinePrintApp*)AfxGetApp())->cal

// CKLinePrintDoc

IMPLEMENT_DYNCREATE(CKLinePrintDoc, CDocument)

BEGIN_MESSAGE_MAP(CKLinePrintDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CKLinePrintDoc::OnFileOpen)
END_MESSAGE_MAP()


// CKLinePrintDoc ����/����

CKLinePrintDoc::CKLinePrintDoc()
{
	// TODO: �ڴ����һ���Թ������

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
	// ֻ����������Լ
	targetCsvFile = DataRepoUtil::GetMajorContractPath(targetCsvFile);

	if(targetCsvFile == m_CurCsvFile) return;

	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	��ǰ����
	int nDate = DataRepoUtil::GetDateByPath(targetCsvFile);
	
	//	�ȼ�����������(�ṩǰһ�յ���K����)
	KLineReader klReader;

	string dayLineFile = DataRepoUtil::GetDayLinePath(targetCsvFile);
	if(dayLineFile != m_CurDayFile)
	{
		// ��ȡ��Ӧ����������
		klcday.clear();
		klReader.Read(dayLineFile, klcday);
		m_CurDayFile = dayLineFile;
	}

	pView->SetDayData(&klcday, nDate);

	//  ��ȡǰһ��������K��
	KLine prevDayKLine;

	prevDayKLine = klcday.GetKLineByTime(CALENDAR.GetPrev(nDate));

	//	����עǰ�յĿ��̼�
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

	if(bZhuLi) /* ����������Լ */
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
	string tmp = GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* ������������Լ */);
	LoadKLineGroup(tmp);
}
