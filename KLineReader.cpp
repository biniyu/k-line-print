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

	FastIndicatorComputer fic;

	ifstream in (path.c_str());

	while(in.getline(buf, SZ))
	{
		KLine kline;

		kline.vol = 0;

		sscanf(buf, "%d/%d/%d %d %d %d %d ", 
			&year, &month, &day, &kline.open, &kline.high, &kline.low, &kline.close);

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
}
