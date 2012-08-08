#include "StdAfx.h"
#include "VolatilityRanker.h"
#include "Utility.h"
#include "KLinePrint.h"
#include "KLineCollection.h"
#include "KLineReader.h"

VolatilityRanker::~VolatilityRanker(void)
{

}

vector<string> VolatilityRanker::Rank()
{
	vector<string> res;

	// 处理当天的
	string curFile = Utility::GetMajorContractPath(m_strFileName);
	string dayLineFile = Utility::GetDayLinePath(curFile);

	KLineCollection& klcday = GetDayLineData(dayLineFile);
	KLine kline = klcday.GetKLineByTime(Utility::GetDateByPath(curFile));

	if(m_enRankMode == en_RankModeHighLow)
	{
		AddToRankList(kline.high - kline.low, curFile);
	}
	else if(m_enRankMode == en_RankModeOpenClose)
	{
		AddToRankList(abs(kline.open - kline.close), curFile);
	}
	else if(m_enRankMode == en_RankModeGap)
	{
		KLine prev = klcday.GetKLineByTime(CALENDAR.GetPrev(Utility::GetDateByPath(curFile)));
		if(prev.open) AddToRankList(abs(prev.close - kline.open), curFile);	
	}

	//	向后处理
	while(1)
	{
		curFile = Utility::GetNeighborCsvFile(curFile, false, true);
		if(!curFile.size()) break;

		string dayLineFile = Utility::GetDayLinePath(curFile);
		KLineCollection& klcday = GetDayLineData(dayLineFile);
		KLine kline = klcday.GetKLineByTime(Utility::GetDateByPath(curFile));

		if(m_enRankMode == en_RankModeHighLow)
		{
			AddToRankList(kline.high - kline.low, curFile);
		}
		else if(m_enRankMode == en_RankModeOpenClose)
		{
			AddToRankList(abs(kline.open - kline.close), curFile);
		}
		else if(m_enRankMode == en_RankModeGap)
		{
			KLine prev = klcday.GetKLineByTime(CALENDAR.GetPrev(Utility::GetDateByPath(curFile)));
			if(prev.open)AddToRankList(abs(prev.close - kline.open), curFile);	
		}
	}

	curFile = Utility::GetMajorContractPath(m_strFileName);

	//	向前处理
	while(1)
	{
		curFile = Utility::GetNeighborCsvFile(curFile, true, true);
		if(!curFile.size()) break;

		string dayLineFile = Utility::GetDayLinePath(curFile);
		KLineCollection& klcday = GetDayLineData(dayLineFile);
		KLine kline = klcday.GetKLineByTime(Utility::GetDateByPath(curFile));

		if(m_enRankMode == en_RankModeHighLow)
		{
			AddToRankList(kline.high - kline.low, curFile);
		}
		else if(m_enRankMode == en_RankModeOpenClose)
		{
			AddToRankList(abs(kline.open - kline.close), curFile);
		}
		else if(m_enRankMode == en_RankModeGap)
		{
			KLine prev = klcday.GetKLineByTime(CALENDAR.GetPrev(Utility::GetDateByPath(curFile)));
			if(prev.open)AddToRankList(abs(prev.close - kline.open), curFile);	
		}
	}

	multimap <int, string>::reverse_iterator rit;

	for(rit = m_mapRankList.rbegin(); rit != m_mapRankList.rend(); rit++)
	{
		res.push_back(rit->second);
	}
	
	return res;
}

KLineCollection& VolatilityRanker::GetDayLineData(string dayLineFile)
{
	if(m_mapDayLines.find(dayLineFile) == m_mapDayLines.end())
	{
		KLineReader klReader;
		klReader.Read(dayLineFile, m_mapDayLines[dayLineFile]);
	}

	return m_mapDayLines[dayLineFile];
}

void VolatilityRanker::AddToRankList(int volatility, string file)
{
	m_mapRankList.insert(pair<int,string>(volatility, file));
}