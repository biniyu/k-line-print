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

	int	year, month, day;
	int hour, minute, second;

#define SZ 1024
	char buf[SZ];

	/* ���������� */
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