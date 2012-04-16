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

	int	year, month, day;
	int hour, minute, second;

#define SZ 1024
	char buf[SZ];

	/* 跳过标题行 */
	csvFile.getline(buf,SZ);

	Tick tick;

	while(csvFile.getline(buf, SZ))
	{
		sscanf(buf, "%d-%d-%d,%d:%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%c",
			&year, &month, &day, &hour, &minute, &second, 
			&tick.price, &tick.vol, &tick.totalvol, &tick.interest, 
			&tick.priceB1, &tick.volB1, &tick.priceB2, &tick.volB2, &tick.priceB3, &tick.volB3, 
			&tick.priceS1, &tick.volS1, &tick.priceS2, &tick.volS2, &tick.priceS3, &tick.volS3,
			&tick.bs);

		tick.time = hour*3600 + minute* 60 + second;
		ticks.push_back(tick);
	}
}