#pragma once

#include <vector>
#include <map>
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
private:

	//	根据时间建立索引，用于快速查找
	map<int, int> m_mapTime2Idx;
	
public:
	KLineCollection(void);
	~KLineCollection(void);

	//	获取指定时间/日期的K线数据
	KLine GetKLineByTime(int nTime);

	//	统一接口用于创建索引
	void AddToTail(KLine kline);

	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol);

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);
};
