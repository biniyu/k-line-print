
// KLinePrintDoc.cpp : CKLinePrintDoc ���ʵ��
//

#include "stdafx.h"
#include "KLinePrint.h"

#include "MainFrm.h"
#include "KLinePrintView.h"
#include "KLinePrintDoc.h"
#include "TickReader.h"
#include "KLineWriter.h"
#include "KLineCollection.h"
#include "CalendarGenerator.h"
#include "Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKLinePrintDoc

IMPLEMENT_DYNCREATE(CKLinePrintDoc, CDocument)

BEGIN_MESSAGE_MAP(CKLinePrintDoc, CDocument)
	ON_COMMAND(ID_GEN_DAYLINE, &CKLinePrintDoc::OnGenDayline)
END_MESSAGE_MAP()


// CKLinePrintDoc ����/����

CKLinePrintDoc::CKLinePrintDoc()
{
	m_nCurrentTickIdx = 0;
}

CKLinePrintDoc::~CKLinePrintDoc()
{
}

BOOL CKLinePrintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL CKLinePrintDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	char InfoString[256];    
	  
	// ת��������ݴ����InfoString������   
	if (!WideCharToMultiByte(CP_ACP,0, lpszPathName,-1, InfoString,100,NULL,NULL))    
	{    
		return FALSE;    
	} 

	LoadKLineGroup(InfoString);
	DisplayTill(-1, -1);
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


BOOL CKLinePrintDoc::ValidatePlaybackConfig(int nDate, PlaybackConfig pbConfig)
{
	if(pbConfig.nStartDate)
	{
		if(nDate < pbConfig.nStartDate) return FALSE;
	}

	if(pbConfig.nEndDate)
	{
		if(nDate > pbConfig.nEndDate) return FALSE;
	}

	int nWeekDay = Utility::GetWeekDayByDate(nDate);

	if(pbConfig.bDayOfWeek[nWeekDay] == FALSE)
		return FALSE;

	//	�������Ҫǰһ������K������
	if(!pbConfig.fGapPercentage 
		&& (!pbConfig.fLastDayFluctuationAbove)
		&& (!pbConfig.fLastDayFluctuationBelow)) 
		return TRUE;

	//	��ȡ��ǰ��Լ�ڸ����ڵ��ļ���
	string tmp = Utility::GetPathByDate(m_CurCsvFile, nDate);

	//	��ȡ������Լ�ļ���
	string major = Utility::GetMajorContractPath(tmp);

	//	�޷���ȡ������Լ�ļ������Ʒ��δ����
	if(!major.size()) return FALSE;

	//	��ȡ���������ļ���
	string dayfile = Utility::GetDayLinePath(major);

	KLine this_kline = m_KLineReader.GetKLineByTime(dayfile, nDate);
	KLine prev_kline = m_KLineReader.GetKLineByTime(dayfile, CALENDAR.GetPrev(nDate));

	//	�޷���ȡ�ϸ������յ�����
	if(!prev_kline.time) return FALSE;

	//	�ߵͿ�����
	int gap = 100 * abs(this_kline.open - prev_kline.close) / (float)prev_kline.close;

	if(pbConfig.fGapPercentage && gap < pbConfig.fGapPercentage)
		return FALSE;

	//	��һ�����յ����
	int lastFlunc =  100 * (prev_kline.high - prev_kline.low) 
					/ ((float)(prev_kline.high + prev_kline.low) / 2.0f);

	if(pbConfig.fLastDayFluctuationAbove && lastFlunc < pbConfig.fLastDayFluctuationAbove)
		return FALSE;

	if(pbConfig.fLastDayFluctuationBelow && lastFlunc > pbConfig.fLastDayFluctuationBelow)
		return FALSE;

	return TRUE;
}

void CKLinePrintDoc::LoadPlaybackCalendar(PlaybackConfig pbConfig)
{
	//	δ���طֱ�����
	if(!m_CurCsvFile.size()) return;

	m_FilteredCalendar.clear();

	//	�����������ɹ��˺������

	int nCurDate = CALENDAR.GetFirst();

	while(nCurDate > 0)
	{
		//	�������������ż���
		if(ValidatePlaybackConfig(nCurDate, pbConfig))
			m_FilteredCalendar.Add(nCurDate);

		nCurDate = CALENDAR.GetNext(nCurDate);
	}
}

// CKLinePrintDoc ����

void CKLinePrintDoc::LoadKLineGroup(string targetCsvFile)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	ֻ����������Լ��ȷ���ֱ������ļ��������ļ�
	string tmp;

	tmp = Utility::GetMajorContractPath(targetCsvFile);

	if(!tmp.size()) return;

	m_CurCsvFile = tmp;
	m_CurDayFile = Utility::GetDayLinePath(m_CurCsvFile);

	//	����ֱ�����
	m_TickData.clear();

	DWORD before = GetTickCount();
	m_TickReader.Read(m_CurCsvFile, m_TickData);
	DWORD after = GetTickCount();
	TRACE("\ntick file %s read, use %d ticks", m_CurCsvFile.c_str(), after - before);

	int nWeekDay = Utility::GetWeekDayByDate(Utility::GetDateByPath(m_CurCsvFile));

	CString csvFileName(m_CurCsvFile.c_str());
	CString dayFileName(m_CurDayFile.c_str());
	CString title;

	title.Format(_T("%s | %s | ����%d"), csvFileName, dayFileName, nWeekDay);

	//	���±���
	SetTitle(title);
	
	::WritePrivateProfileStringA("Files","Current", 
							m_CurCsvFile.c_str(), (Utility::GetProgramPath() + "klinep.ini").c_str()); 
}

void CKLinePrintDoc::ReloadByDate(int nDate)
{
	string tmp = Utility::GetPathByDate(m_CurCsvFile, nDate);

	if(!tmp.size()) return;

	OnOpenDocument(CString(tmp.c_str()));
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
	string tmp = Utility::GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* ������������Լ */);

	if(!tmp.size()) return;

	OnOpenDocument(CString(tmp.c_str()));
}

BOOL CKLinePrintDoc::LoadNextDay()
{
	// ���ݻط����þ�����һ�������յ�����
	int nCurDate, nNextDate;
	
	nCurDate = Utility::GetDateByPath(m_CurCsvFile);

	if(PBCONFIG.enPlaybackOrder == PlaybackConfig::PLAYBACK_SEQUENTIAL)
	{
		nNextDate = m_FilteredCalendar.GetNext(nCurDate);
	}
	else	//	PLAYBACK_RANDOM
	{	
		int nDayCnt = m_FilteredCalendar.size();
		nNextDate = m_FilteredCalendar.GetBySeq(rand() % nDayCnt);
	}

	string tmp = Utility::GetPathByDate(m_CurCsvFile, nNextDate);

	if(!tmp.size()) return FALSE;

	LoadKLineGroup(tmp);
	int nDate = Utility::GetDateByPath(tmp);
	DisplayTill(0, nDate);
	return TRUE;
}

Tick CKLinePrintDoc::GetTick(int nOffset)
{
	Tick tmp;

	if(m_nCurrentTickIdx < m_TickData.size())
		return m_TickData[m_nCurrentTickIdx + nOffset];
	else
	{
		tmp = m_TickData[m_TickData.size() - 1];
		tmp.time = 0;
		return tmp;
	}
}

void CKLinePrintDoc::PlayTillTime(int nTillMilliTime)
{
	int nDate = Utility::GetDateByPath(m_CurCsvFile);

	while(1)
	{
		m_nCurrentTickIdx++;

		if(m_nCurrentTickIdx >= m_TickData.size()) break;

		// ��Ҫoverrideʱ��
		Tick tmp = m_TickData[m_nCurrentTickIdx];

		tmp.time = nDate;
		m_DayData.Quote(tmp);

		m_1MinData.Quote(m_TickData[m_nCurrentTickIdx]);
		m_15SecData.Quote(m_TickData[m_nCurrentTickIdx]);

		int curMilliTime = m_TickData[m_nCurrentTickIdx].time * 1000 + m_TickData[m_nCurrentTickIdx].millisec; 

		if(nTillMilliTime != -1 && curMilliTime >= nTillMilliTime) 
			break;
	}
}

void CKLinePrintDoc::DisplayTill(int nTillTime, int nTillDate)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	m_DayData.Clear();
	m_15SecData.Clear();
	m_1MinData.Clear();

	//	��ǰ����
	int nDate = Utility::GetDateByPath(m_CurCsvFile);

	DWORD before = GetTickCount();
	m_KLineReader.Read(m_CurDayFile, m_DayData, nTillDate/**/);
	DWORD after = GetTickCount();
	TRACE("\nday line %s read, use %d ticks", m_CurDayFile.c_str(), after - before);

	//  ��ȡǰһ��������K��
	KLine prevDayKLine;
	prevDayKLine = m_DayData.GetKLineByTime(CALENDAR.GetPrev(nDate));

	//	����עǰ�յĿ��̼�
	prevDayKLine.open = prevDayKLine.close;
	prevDayKLine.time = 0;
	prevDayKLine.vol = prevDayKLine.vol_acc = 0;

	m_1MinData.AddKeyPrice(prevDayKLine.avg,  "AVG1");
	m_1MinData.AddKeyPrice(prevDayKLine.close, "C1");
	m_1MinData.AddKeyPrice(prevDayKLine.high, "H1");
	m_1MinData.AddKeyPrice(prevDayKLine.low, "L1");

	m_1MinData.AddKeyPrice(prevDayKLine.ma5, "MA5");
	m_1MinData.AddKeyPrice(prevDayKLine.ma10, "MA10");
	m_1MinData.AddKeyPrice(prevDayKLine.ma20, "MA20");
	m_1MinData.AddKeyPrice(prevDayKLine.ma60, "MA60");

	m_1MinData.AddKeyPrice(prevDayKLine.high5, "H5");
	m_1MinData.AddKeyPrice(prevDayKLine.high10, "H10");
	m_1MinData.AddKeyPrice(prevDayKLine.high20, "H20");
	m_1MinData.AddKeyPrice(prevDayKLine.high60, "H60");

	m_1MinData.AddKeyPrice(prevDayKLine.low5, "L5");
	m_1MinData.AddKeyPrice(prevDayKLine.low10, "L10");
	m_1MinData.AddKeyPrice(prevDayKLine.low20, "L20");
	m_1MinData.AddKeyPrice(prevDayKLine.low60, "L60");

	/* ǰ����K */
	m_1MinData.AddToTail(prevDayKLine);
	
	//	���ɱ��յ�һ��K��
	m_nCurrentTickIdx = 0;

	m_DayData.SetPeriod(36000);
	m_1MinData.SetPeriod(60);
	m_15SecData.SetPeriod(15);

	//	��Ҫoverrideʱ��
	Tick tmp = m_TickData[m_nCurrentTickIdx];
	tmp.time = nDate;

	m_DayData.StartQuote(tmp);
	m_1MinData.StartQuote(m_TickData[m_nCurrentTickIdx]);
	m_15SecData.StartQuote(m_TickData[m_nCurrentTickIdx]);

	//	��������
	PlayTillTime(nTillTime == -1 ? -1 : nTillTime * 1000);

	//	��������
	pView->SetDayData(&m_DayData, nDate);
	pView->Set1MinData(&m_1MinData);
	pView->Set5SecData(&m_15SecData);

	//	��ʾ
	pView->Render();	
	this->UpdateAllViews(0);
}

void CKLinePrintDoc::OnGenDayline()
{
	int nCurDate, nCurKLineIdx;

	KLineWriter klineWriter;
	KLineCollection klcorg, klc;

	if(!m_CurCsvFile.size()) return;

	if(!CALENDAR.size()) return;

	//	�������µ�����������ǰƷ�����к�Լ���������ݣ����ȱʧ��ȫ
	vector<string> contracts = Utility::GetAllContractPath(m_CurCsvFile);

	for(int i = 0; i < contracts.size(); i++)
	{
		string daylinePath = Utility::GetDayLinePath(contracts[i]);
		
		klc.Clear();
		klcorg.Clear();

		m_KLineReader.Read(daylinePath, klcorg);

		//	�Ӹú�Լ�׸������տ�ʼ���ȱʧ������
		//	��δ�ش��׸�����������зֱ������ļ�

		if(klcorg.size())
			nCurDate = klcorg[0].time;
		else
			nCurDate = CALENDAR.GetFirst();

		nCurKLineIdx = 0;

		//	Ѱ���׸��зֱ������ļ��Ľ�����
		TickCollection ticks;

		while(nCurDate > 0 && nCurKLineIdx < klcorg.size())
		{
			string quotefile = Utility::GetPathByDate(contracts[i], nCurDate);

			m_TickReader.Read(quotefile, ticks);

			if(!ticks.size())
			{
				//	�����ڷֱ����ݣ�ֻ������������
				klc.AddToTail(klcorg[nCurKLineIdx]);
				nCurKLineIdx++;			
			}
			else
			{
				//	�׸��зֱ������ļ��Ľ�����
				break;
			}

			nCurDate = CALENDAR.GetNext(nCurDate);
		}

		while(nCurDate > 0)
		{
			//	�ý������������ݴ���
			if(nCurKLineIdx < klcorg.size() && nCurDate == klcorg[nCurKLineIdx].time)
			{
				//	��ӵ����յ����ݼ���
				klc.AddToTail(klcorg[nCurKLineIdx]);
				nCurKLineIdx++;
			}
			else // ���߲�����
			{
//				TRACE("\ncontract %s missing %d ", contracts[i].c_str(), nCurDate);

				//	�ӷֱ�������������
				KLine kl = GenerateDayLineFromQuoteData(contracts[i], nCurDate);

				//	���ӵ����ݼ���
				if(kl.time)
				{
					TRACE("\ncontract %s missing %d ", contracts[i].c_str(), nCurDate);
					klc.AddToTail(kl);
					TRACE("regenerated.");
				}
				else
				{
//					TRACE("generation failed!");
				}
			}

			//	������Լ�ڸ����Ƿ�����������
			nCurDate = CALENDAR.GetNext(nCurDate);

			//	��û���������ݣ���ӷֱ�����������
		}

		//	�������Լ���������ݵ��ļ�������ԭ���������ļ�
		klineWriter.Write(daylinePath, klc);
	}

	AfxMessageBox(_T("�������ݲ����������"));

}

KLine CKLinePrintDoc::GenerateDayLineFromQuoteData(string path, int date)
{
	string quotefile = Utility::GetPathByDate(path, date);

	KLine kline;
	TickCollection ticks;
	float totalPrice = 0;

	m_TickReader.Read(quotefile, ticks);

	if(!ticks.size())
	{
		kline.time = 0;
		return kline;
	}

	kline.time = date;
	kline.open = kline.high = kline.low = ticks[0].price;
	kline.close = ticks[ticks.size()-1].price;
	kline.vol = 0;

	for(int i = 0; i < ticks.size(); i++)
	{
		kline.vol += ticks[i].vol;
		totalPrice += (ticks[i].price * ticks[i].vol);

		if(ticks[i].price > kline.high)
			kline.high = ticks[i].price;

		if(ticks[i].price < kline.low)
			kline.low = ticks[i].price;
	}

	kline.avg = (int)(totalPrice / kline.vol);

	return kline;
}
