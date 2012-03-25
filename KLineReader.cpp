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

void KLineReader::Read(string path, KLineCollection& klines)
{
	char buf[SZ];
	int year, month, day;

	ifstream in (path.c_str());

	while(in.getline(buf, SZ))
	{
		KLine kline;

		kline.vol = 0;

		sscanf(buf, "%d/%d/%d %d %d %d %d ", 
			&year, &month, &day, &kline.open, &kline.high, &kline.low, &kline.close);

		kline.time = year*10000 + month*100 + day;

		klines.AddToTail(kline);
	}	
}
