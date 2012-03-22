#include "StdAfx.h"
#include "TickReader.h"
#include <fstream>

TickReader::TickReader(void)
{
}

TickReader::~TickReader(void)
{
}

void TickReader::Read(string path, TickCollection& ticks)
{
	ifstream csvFile(path.c_str());


	/*
	����,ʱ��,�ɽ���,�ɽ���,����,����(�ֲ�����),B1��,B1��,B2��,B2��,B3��,B3��,S1��,S1��,S2��,S2��,S3��,S3��,BS
	2004-12-06,9:30:04,2520,10,10,1356,2520,8,0,0,0,0,2526,1,0,0,0,0,S

	*/
	int	year, month, day, hour, minute, second;
	int price, vol, totalvol, interest;
	int priceB1, volB1, priceB2, volB2, priceB3, volB3;
	int priceS1, volS1, priceS2, volS2, priceS3, volS3;
	char bs;

#define SZ 1024
	char buf[SZ];

	/* ���������� */
	csvFile.getline(buf,SZ);

	Tick tick;

	int totalVol = 0;

	/* ��һ���۸� */
	csvFile.getline(buf,SZ);
	sscanf(buf, "%d-%d-%d,%d:%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%c",
		&year, &month, &day, &hour, &minute, &second, &price, &vol, &totalvol, &interest, 
		&priceB1, &volB1, &priceB2, &volB2, &priceB3, &volB3, 
		&priceS1, &volS1, &priceS2, &volS2, &priceS3, &volS3, &bs);

	ticks.open = price;

	tick.price = price;
	tick.time = hour*3600 + minute* 60 + second;
	tick.vol = vol;

	totalVol += vol;

	ticks.high = price;
	ticks.low = price;

	ticks.push_back(tick);

	while(csvFile.getline(buf, SZ))
	{
		sscanf(buf, "%d-%d-%d,%d:%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%c",
			&year, &month, &day, &hour, &minute, &second, &price, &vol, &totalvol, &interest, 
			&priceB1, &volB1, &priceB2, &volB2, &priceB3, &volB3, 
			&priceS1, &volS1, &priceS2, &volS2, &priceS3, &volS3, &bs);

		tick.price = price;
		tick.time = hour*3600 + minute* 60 + second;
		tick.vol = vol;

		totalVol += vol;

		if(price > ticks.high)
			ticks.high = price;

		if(price < ticks.low)
			ticks.low = price;

		ticks.push_back(tick);
	}

	ticks.close = price;
	ticks.avgvol = totalVol / 255.0; /* 1������ */
}