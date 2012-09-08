#pragma once

#include <vector>
#include <string>
#include <map>
using namespace std;

class Tick
{
public:

	int date;		//	日期	
	int time_ms;	//	毫秒级时间
	int price;
	int vol;
	int totalvol;
	int interest;	//	持仓量
	
	int priceB1;
	int volB1;
	int priceB2;
	int volB2;
	int priceB3;
	int volB3;

	int priceS1;
	int volS1;
	int priceS2;
	int volS2;
	int priceS3;
	int volS3;

	char bs;
};


class TickCollection : public vector<Tick> {};

class TickReader
{
private:

	map<string, TickCollection>	m_cachedData;

public:
	TickReader(void);
	~TickReader(void);

	void Read(string path, TickCollection& ticks);
};
