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
	日期,时间,成交价,成交量,总量,属性(持仓增减),B1价,B1量,B2价,B2量,B3价,B3量,S1价,S1量,S2价,S2量,S3价,S3量,BS
	2004-12-06,9:30:04,2520,10,10,1356,2520,8,0,0,0,0,2526,1,0,0,0,0,S

	*/
	int	year, month, day, hour, minute, second;
	int price, vol, totalvol, interest;
	int priceB1, volB1, priceB2, volB2, priceB3, volB3;
	int priceS1, volS1, priceS2, volS2, priceS3, volS3;
	char bs;

#define SZ 1024
	char buf[SZ];

	/* 跳过标题行 */
	csvFile.getline(buf,SZ);

	Tick tick;

	int totalVol = 0;

	/* 第一个价格 */
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
	ticks.avgvol = totalVol / 255.0; /* 1分钟线 */
}