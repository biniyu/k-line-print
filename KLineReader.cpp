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

void KLineReader::Read(string path, KLineCollection& klines, int nTillDate)
{
	char buf[SZ];
	int year, month, day;

	FastMovingAverageComputer fmac;

	ifstream in (path.c_str());

	while(in.getline(buf, SZ))
	{
		KLine kline;

		kline.vol = 0;

		sscanf(buf, "%d/%d/%d %d %d %d %d ", 
			&year, &month, &day, &kline.open, &kline.high, &kline.low, &kline.close);

		kline.time = year*10000 + month*100 + day;

		if(nTillDate == kline.time) break;

		fmac.PushNewData(kline.close);

		kline.ma5 = fmac.GetMA5();
		kline.ma10 = fmac.GetMA10();
		kline.ma20 = fmac.GetMA20();
		kline.ma60 = fmac.GetMA60();

		klines.AddToTail(kline);
	}	
}
