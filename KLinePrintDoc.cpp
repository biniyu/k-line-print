
// KLinePrintDoc.cpp : CKLinePrintDoc 类的实现
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


// CKLinePrintDoc 构造/析构

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
	  
	// 转换后的数据存放在InfoString数组中   
	if (!WideCharToMultiByte(CP_ACP,0, lpszPathName,-1, InfoString,100,NULL,NULL))    
	{    
		return FALSE;    
	} 

	LoadKLineGroup(InfoString);
	DisplayTill(-1, -1);
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

	//	如果不需要前一交易日K线数据
	if(!pbConfig.fGapPercentage 
		&& (!pbConfig.fLastDayFluctuationAbove)
		&& (!pbConfig.fLastDayFluctuationBelow)) 
		return TRUE;

	//	获取当前合约在该日期的文件名
	string tmp = Utility::GetPathByDate(m_CurCsvFile, nDate);

	//	获取主力合约文件名
	string major = Utility::GetMajorContractPath(tmp);

	//	无法获取主力合约文件，因该品种未上市
	if(!major.size()) return FALSE;

	//	获取日线数据文件名
	string dayfile = Utility::GetDayLinePath(major);

	KLine this_kline = m_KLineReader.GetKLineByTime(dayfile, nDate);
	KLine prev_kline = m_KLineReader.GetKLineByTime(dayfile, CALENDAR.GetPrev(nDate));

	//	无法获取上个交易日的数据
	if(!prev_kline.time) return FALSE;

	//	高低开条件
	int gap = 100 * abs(this_kline.open - prev_kline.close) / (float)prev_kline.close;

	if(pbConfig.fGapPercentage && gap < pbConfig.fGapPercentage)
		return FALSE;

	//	上一交易日的振幅
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
	//	未加载分笔数据
	if(!m_CurCsvFile.size()) return;

	m_FilteredCalendar.clear();

	//	根据配置生成过滤后的日历

	int nCurDate = CALENDAR.GetFirst();

	while(nCurDate > 0)
	{
		//	满足所有条件才加入
		if(ValidatePlaybackConfig(nCurDate, pbConfig))
			m_FilteredCalendar.Add(nCurDate);

		nCurDate = CALENDAR.GetNext(nCurDate);
	}
}

// CKLinePrintDoc 命令

void CKLinePrintDoc::LoadKLineGroup(string targetCsvFile)
{
	CKLinePrintView* pView = (CKLinePrintView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();

	//	只考虑主力合约，确定分笔数据文件和日线文件
	string tmp;

	tmp = Utility::GetMajorContractPath(targetCsvFile);

	if(!tmp.size()) return;

	m_CurCsvFile = tmp;
	m_CurDayFile = Utility::GetDayLinePath(m_CurCsvFile);

	//	读入分笔数据
	m_TickData.clear();

	DWORD before = GetTickCount();
	m_TickReader.Read(m_CurCsvFile, m_TickData);
	DWORD after = GetTickCount();
	TRACE("\ntick file %s read, use %d ticks", m_CurCsvFile.c_str(), after - before);

	int nWeekDay = Utility::GetWeekDayByDate(Utility::GetDateByPath(m_CurCsvFile));

	CString csvFileName(m_CurCsvFile.c_str());
	CString dayFileName(m_CurDayFile.c_str());
	CString title;

	title.Format(_T("%s | %s | 星期%d"), csvFileName, dayFileName, nWeekDay);

	//	更新标题
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
	string tmp = Utility::GetNeighborCsvFile(m_CurCsvFile, bPrev, TRUE/* 必须是主力合约 */);

	if(!tmp.size()) return;

	OnOpenDocument(CString(tmp.c_str()));
}

BOOL CKLinePrintDoc::LoadNextDay()
{
	// 根据回放配置决定下一个交易日的日期
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

		// 需要override时间
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

	//	当前日期
	int nDate = Utility::GetDateByPath(m_CurCsvFile);

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

	/* 前日日K */
	m_1MinData.AddToTail(prevDayKLine);
	
	//	生成本日第一条K线
	m_nCurrentTickIdx = 0;

	m_DayData.SetPeriod(36000);
	m_1MinData.SetPeriod(60);
	m_15SecData.SetPeriod(15);

	//	需要override时间
	Tick tmp = m_TickData[m_nCurrentTickIdx];
	tmp.time = nDate;

	m_DayData.StartQuote(tmp);
	m_1MinData.StartQuote(m_TickData[m_nCurrentTickIdx]);
	m_15SecData.StartQuote(m_TickData[m_nCurrentTickIdx]);

	//	继续生成
	PlayTillTime(nTillTime == -1 ? -1 : nTillTime * 1000);

	//	设置数据
	pView->SetDayData(&m_DayData, nDate);
	pView->Set1MinData(&m_1MinData);
	pView->Set5SecData(&m_15SecData);

	//	显示
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

	//	根据最新的日历搜索当前品种所有合约的日线数据，如果缺失则补全
	vector<string> contracts = Utility::GetAllContractPath(m_CurCsvFile);

	for(int i = 0; i < contracts.size(); i++)
	{
		string daylinePath = Utility::GetDayLinePath(contracts[i]);
		
		klc.Clear();
		klcorg.Clear();

		m_KLineReader.Read(daylinePath, klcorg);

		//	从该合约首个交易日开始检查缺失的日线
		//	但未必从首个交易日起就有分笔数据文件

		if(klcorg.size())
			nCurDate = klcorg[0].time;
		else
			nCurDate = CALENDAR.GetFirst();

		nCurKLineIdx = 0;

		//	寻找首个有分笔数据文件的交易日
		TickCollection ticks;

		while(nCurDate > 0 && nCurKLineIdx < klcorg.size())
		{
			string quotefile = Utility::GetPathByDate(contracts[i], nCurDate);

			m_TickReader.Read(quotefile, ticks);

			if(!ticks.size())
			{
				//	不存在分笔数据，只拷贝日线数据
				klc.AddToTail(klcorg[nCurKLineIdx]);
				nCurKLineIdx++;			
			}
			else
			{
				//	首个有分笔数据文件的交易日
				break;
			}

			nCurDate = CALENDAR.GetNext(nCurDate);
		}

		while(nCurDate > 0)
		{
			//	该交易日日线数据存在
			if(nCurKLineIdx < klcorg.size() && nCurDate == klcorg[nCurKLineIdx].time)
			{
				//	添加到最终的数据集中
				klc.AddToTail(klcorg[nCurKLineIdx]);
				nCurKLineIdx++;
			}
			else // 日线不存在
			{
//				TRACE("\ncontract %s missing %d ", contracts[i].c_str(), nCurDate);

				//	从分笔数据生成日线
				KLine kl = GenerateDayLineFromQuoteData(contracts[i], nCurDate);

				//	增加到数据集中
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

			//	检查各合约在该日是否有日线数据
			nCurDate = CALENDAR.GetNext(nCurDate);

			//	若没有日线数据，则从分笔数据中生成
		}

		//	保存各合约的日线数据到文件，备份原日线数据文件
		klineWriter.Write(daylinePath, klc);
	}

	AfxMessageBox(_T("日线数据补充生成完毕"));

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
