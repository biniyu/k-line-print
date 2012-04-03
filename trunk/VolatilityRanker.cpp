#include "StdAfx.h"
#include "VolatilityRanker.h"
#include "DataRepoUtil.h"
#include "CalendarGenerator.h"
#include "KLineCollection.h"
#include "KLineReader.h"

VolatilityRanker::~VolatilityRanker(void)
{

}

vector<string> VolatilityRanker::Rank()
{
	vector<string> res;

	// 处理当天的
	string curFile = DataRepoUtil::GetMajorContractPath(m_strFileName);
	string dayLineFile = DataRepoUtil::GetDayLinePath(curFile);

	KLineCollection& klcday = GetDayLineData(dayLineFile);
	KLine kline = klcday.GetKLineByTime(DataRepoUtil::GetDateByPath(curFile));

	AddToRankList(kline.high - kline.low, curFile);

	//	向后处理
	while(1)
	{
		curFile = DataRepoUtil::GetNeighborCsvFile(curFile, false, true);
		if(!curFile.size()) break;

		string dayLineFile = DataRepoUtil::GetDayLinePath(curFile);
		KLineCollection& klcday = GetDayLineData(dayLineFile);
		KLine kline = klcday.GetKLineByTime(DataRepoUtil::GetDateByPath(curFile));

		AddToRankList(kline.high - kline.low, curFile);
	}

	curFile = DataRepoUtil::GetMajorContractPath(m_strFileName);

	//	向前处理
	while(1)
	{
		curFile = DataRepoUtil::GetNeighborCsvFile(curFile, true, true);
		if(!curFile.size()) break;

		string dayLineFile = DataRepoUtil::GetDayLinePath(curFile);
		KLineCollection& klcday = GetDayLineData(dayLineFile);
		KLine kline = klcday.GetKLineByTime(DataRepoUtil::GetDateByPath(curFile));

		AddToRankList(kline.high - kline.low, curFile);
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