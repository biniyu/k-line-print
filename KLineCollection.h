#pragma once

#include "TickReader.h"
#include <vector>
#include <map>
using namespace std;

class KLine
{
public:
	int time;				//	K线当前时间
	int start_time;			//	K线起始时间
	int high;
	int low;
	int open;
	int close;
	int vol;				//	当前K线的成交量
	int interest;			//  持仓量
	
	//	日内
	int		avg;			//	均价线
	int		avg_devi;		//	均价偏离值
	float	vol_acc;		//	累积成交量
	double	price_acc;		//	累积成交价格
	
	//	均线
	double ma5;
	double ma10;
	double ma20;
	double ma60;

	//	最高最低价
	double high5;
	double low5;
	double high10;
	double low10;
	double high20;
	double low20;
	double high60;
	double low60;

};

class KLineCollection : public vector<KLine>
{
private:

	//	根据时间建立索引，用于快速查找
	map<int, int>		m_mapTime2Idx;

	//	用于在分钟图上显示日线MA
	map<int, string>	m_mapKeyPrice;

	//	K线周期(以秒为单位, -1为日线)
	int					m_nKLinePeriod;

public:

	int					m_nMaxPrice;			//	日内最高价
	int					m_nMinPrice;			//	日内最低价

	
public:
	KLineCollection(void);
	~KLineCollection(void);

	//	获取指定时间/日期的K线数据
	KLine GetKLineByTime(int nTime);

	//	设置K线周期(以秒为单位, -1为日线)
	void SetPeriod(int nPeriod) { m_nKLinePeriod = nPeriod; }

	//	统一接口用于创建索引
	void AddToTail(KLine kline);

	//	获取价格和成交量的最大最小值
	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol, int& nMaxInterest);

	//	开始接收分笔数据
	void StartQuote(Tick tick);

	//	接收分笔数据
	void Quote(Tick tick);

	//	计算均线偏离值
	int GetAvgDevi(KLine kline);

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds);

	//	从分笔数据生成指定周期(以秒为单位)的K线数据
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);

	//	添加关键价格
	void AddKeyPrice(int price, string desc) { m_mapKeyPrice[price] = desc; }

	map<int, string>& GetKeyPrices() { return m_mapKeyPrice; }

	void Clear();
};
