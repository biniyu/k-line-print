
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

void CKLinePrintDoc::OnFileOpen()
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	CFileDialog dlg(TRUE); ///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal() == IDOK)
	{
		CString FilePathName = dlg.GetPathName(); 

		klc15s.clear();
		klc1min.clear();

		TickReader tr;

		TickCollection tcPrev;

		TickCollection tc;

		char InfoString[256];    
		  
		// ת��������ݴ����InfoString������   
		if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(FilePathName),-1,InfoString,100,NULL,NULL))    
		{    
			return;    
		} 

		m_CurCsvFile = InfoString;

		tr.Read(InfoString, tc);

		tr.Read(GetNeighborCsvFile(InfoString, TRUE, FALSE), tcPrev);

		KLine kline;

		kline.close = tcPrev.close;
		kline.open = tcPrev.close;
		kline.high = tcPrev.high;
		kline.low = tcPrev.low;
		kline.vol = tcPrev.avgvol;
	
		klc15s.Generate(tc, 15, kline);
		klc1min.Generate(tc, 60, kline);

		KLineReader klReader;

		string dayLineFile = DataRepoUtil::GetDayLinePath(m_CurCsvFile);

		if(dayLineFile != m_CurDayFile)
		{
			// ��ȡ��Ӧ����������
			klReader.Read(dayLineFile, klcday);
			m_CurDayFile = dayLineFile;
			pView->SetDayData(&klcday, DataRepoUtil::GetDateByPath(m_CurCsvFile));	
		}

		pView->Set1MinData(&klc1min);
		pView->Set5SecData(&klc15s);

		pView->Render();

		this->SetTitle(CString((m_CurCsvFile + "|" + m_CurDayFile).c_str()));
		
		this->UpdateAllViews(0);
	}
}

void CKLinePrintDoc::ReloadByDate(int nDate)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	��ǰ��Լ��ָ�����ڵķ�����
	klc15s.clear();
	klc1min.clear();

	TickReader tr;
	TickCollection tcPrev;
	TickCollection tc;

	string tmp = DataRepoUtil::GetPathByDate(m_CurCsvFile, nDate);

	if(tmp == m_CurCsvFile) return;

	m_CurCsvFile = tmp;

	tr.Read(m_CurCsvFile, tc);

	tr.Read(GetNeighborCsvFile(m_CurCsvFile, TRUE, FALSE), tcPrev);

	KLine kline;

	kline.close = tcPrev.close;
	kline.open = tcPrev.close;
	kline.high = tcPrev.high;
	kline.low = tcPrev.low;
	kline.vol = tcPrev.avgvol;

	klc15s.Generate(tc, 15, kline);
	klc1min.Generate(tc, 60, kline);

	pView->Set1MinData(&klc1min);
	pView->Set5SecData(&klc15s);

	pView->Render();

	this->SetTitle(CString((m_CurCsvFile + "|" + m_CurDayFile).c_str()));
	
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
		nNeighDate = ((CKLinePrintApp*)AfxGetApp())->cal.GetPrev(date);
	else
		nNeighDate = ((CKLinePrintApp*)AfxGetApp())->cal.GetNext(date); 

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
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	klc15s.clear();

	klc1min.clear();

	TickReader tr;

	TickCollection tcPrev;

	TickCollection tc;

	m_CurCsvFile = GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* ������������Լ */);

	tr.Read(m_CurCsvFile, tc);

	tr.Read(GetNeighborCsvFile(m_CurCsvFile, TRUE, FALSE/* ��������ͬ��Լ���������� */), tcPrev);

	KLine kline;

	kline.close = tcPrev.close;
	kline.open = tcPrev.close;
	kline.high = tcPrev.high;
	kline.low = tcPrev.low;
	kline.vol = tcPrev.avgvol;

	klc15s.Generate(tc, 15, kline);
	klc1min.Generate(tc, 60, kline);

	KLineReader klReader;

	string dayLineFile = DataRepoUtil::GetDayLinePath(m_CurCsvFile);

	if(dayLineFile != m_CurDayFile)
	{
		// ��ȡ��Ӧ����������
		klReader.Read(dayLineFile, klcday);
		m_CurDayFile = dayLineFile;
		pView->SetDayData(&klcday, DataRepoUtil::GetDateByPath(m_CurCsvFile));		
	}

	pView->Set1MinData(&klc1min);
	pView->Set5SecData(&klc15s);

	this->SetTitle(CString((m_CurCsvFile + "|" + m_CurDayFile).c_str()));
	
	this->UpdateAllViews(0);	
}
