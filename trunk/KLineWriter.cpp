#include "StdAfx.h"
#include "KLineWriter.h"
#include "KLineCollection.h"
#include <fstream>

#define SZ 1024

KLineWriter::KLineWriter(void)
{
}

KLineWriter::~KLineWriter(void)
{
}

void KLineWriter::Write(string path, KLineCollection& klines)
{
	char buf[512];
	ofstream outf (path.c_str());

	for(int i = 0; i < klines.size(); i++)
	{
		KLine kline = klines[i];

		int year = kline.time / 10000;
		int month = kline.time % 10000 / 100;
		int day = kline.time % 10000 % 100;

		sprintf(buf,"%d/%02d/%02d %d %d %d %d %.2f", 
			year, month, day, 
			kline.open, kline.high, kline.low, kline.close,
			(float)kline.vol);

		outf << buf << endl;
	}
}
