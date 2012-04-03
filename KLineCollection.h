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
	int avg;		// 均价线
	double ma5;
	double ma10;
	double ma20;
	double ma60;
};

class KLineCollection : public vector<KLine>
{
private:

	//	根据时间建立索引，用于快速查找
	map<int, int>		m_mapTime2Idx;

	//	用于在分钟图上显示日线MA
	map<int, string>	m_mapKeyPrice;
	
public:
	KLineCollection(void);
	~KLineCollection(void);

	//	获取指定时间/日期的K线数据
	KLine GetKLineByTime(int nTime);

	//	统一接口用于创建索引
	void AddToTail(KLine kline);

	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol);

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds);

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);

	//	添加关键价格
	void AddKeyPrice(int price, string desc) { m_mapKeyPrice.insert(pair<int,string>(price, desc)); }

	map<int, string>& GetKeyPrices() { return m_mapKeyPrice; }

	void Clear();
};
