#pragma once

#include <vector>

using namespace std;

class TickCollection;

class KLine
{
public:
	int time;
	int high;
	int low;
	int open;
	int close;
	int vol;
	int avg;	// 均价线
};

class KLineCollection : public vector<KLine>
{
public:
	KLineCollection(void);
	~KLineCollection(void);

	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol);

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);
};
