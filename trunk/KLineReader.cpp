#include "StdAfx.h"
#include "KLineReader.h"
#include "KLineCollection.h"
#include <fstream>

#define SZ 1024

KLineReader::KLineReader(void)
{
}

KLineReader::~KLineReader(void)
{
}

KLine KLineReader::GetKLineByTime(string path, int time)
{
	KLineCollection kl;

	if(m_cachedData.find(path) == m_cachedData.end())
	{
		Read(path, kl);
	}

	return m_cachedData[path].GetKLineByTime(time);
}

void KLineReader::Read(string path, KLineCollection& klines, int nTillDate)
{
	char buf[SZ];
	int year, month, day;

	if((nTillDate == -1) && (m_cachedData.find(path)!= m_cachedData.end()))
	{
		klines = m_cachedData[path];
		return;
	}

	FastIndicatorComputer fic;

	ifstream in (path.c_str());

	while(in.getline(buf, SZ))
	{
		KLine kline;

		kline.vol = 0;

		sscanf(buf, "%d/%d/%d %d %d %d %d %d %d", 
			&year, &month, &day, 
			&kline.open, &kline.high, &kline.low, &kline.close,
			&kline.vol, &kline.avg);

		kline.time = year*10000 + month*100 + day;

		if(nTillDate == kline.time) break;

		fic.PushNewData(kline);

		kline.ma5 = fic.GetMA5();
		kline.ma10 = fic.GetMA10();
		kline.ma20 = fic.GetMA20();
		kline.ma60 = fic.GetMA60();

		fic.ComputeHighLow();

		kline.high5 = fic.high5;
		kline.high10 = fic.high10;
		kline.high20 = fic.high20;
		kline.high60 = fic.high60;

		kline.low5 = fic.low5;
		kline.low10 = fic.low10;
		kline.low20 = fic.low20;
		kline.low60 = fic.low60;

		klines.AddToTail(kline);
	}	

	//	完整的日线数据才cache
	if(nTillDate == -1)
		m_cachedData[path] = klines;
}
