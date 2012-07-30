#pragma once

#include <string>
#include <deque>
#include <map>
#include "KLineCollection.h"

using namespace std;

class FastIndicatorComputer
{
private:

	double sum5;
	double sum10;
	double sum20;
	double sum60;

	deque<KLine> que_5;
	deque<KLine> que_10;
	deque<KLine> que_20;
	deque<KLine> que_60;

public:

	double high5;
	double high10;
	double high20;
	double high60;

	double low5;
	double low10;
	double low20;
	double low60;

public:

	FastIndicatorComputer()
	{
		sum5 = sum10 = sum20 = sum60 = 0;
		high5 = high10 = high20 = high60 = 0;
		low5 = low10 = low20 = low60 = 0;
	}


	void PushNewData(KLine kline)
	{
		que_5.push_back(kline);
		que_10.push_back(kline);
		que_20.push_back(kline);
		que_60.push_back(kline);

		sum5 += kline.close;
		sum10 += kline.close;
		sum20 += kline.close;
		sum60 += kline.close;

		if(que_5.size() > 5 ) 
		{
			sum5 -= que_5.front().close;
			que_5.pop_front();
		}
		
		if(que_10.size() > 10 ) 
		{
			sum10 -= que_10.front().close;
			que_10.pop_front();
		}

		if(que_20.size() > 20 ) 
		{
			sum20 -= que_20.front().close;
			que_20.pop_front();
		}
		
		if(que_60.size() > 60 ) 
		{
			sum60 -= que_60.front().close;
			que_60.pop_front();
		}
	}

	void ComputeHighLow()
	{
		int nCnt = 0;
		deque<KLine>::reverse_iterator rit;

		for(rit = que_60.rbegin(); rit != que_60.rend(); rit++)
		{
			if(nCnt == 0)
			{
				high5 = high10 = high20 = high60 = rit->high;
				low5 = low10 = low20 = low60 = rit->low;
			}
			else if(nCnt < 5)
			{
				if(rit->high > high5)
					high5 = rit->high;
				if(rit->low < low5)
					low5 = rit->low;
				
				high10 = high20 = high60 = high5;
				low10 = low20 = low60 = low5;
			}
			else if(nCnt < 10)
			{
				if(rit->high > high10)
					high10 = rit->high;
				if(rit->low < low10)
					low10 = rit->low;
				
				high20 = high60 = high10;
				low20 = low60 = low10;
			}
			else if(nCnt < 20)
			{
				if(rit->high > high20)
					high20 = rit->high;
				if(rit->low < low20)
					low20 = rit->low;
				
				high60 = high20;
				low60 = low20;
			}
			else if(nCnt < 60)
			{
				if(rit->high > high60)
					high60 = rit->high;
				if(rit->low < low60)
					low60 = rit->low;
			}

			nCnt++;
		}
	}


	double GetMA5() 
	{
		if(que_5.size() == 5) return sum5 / 5; 
		else return 0;
	}

	double GetMA10() 
	{ 
		if(que_10.size() == 10) return sum10 / 10; 
		else return 0;
	} 

	double GetMA20() 
	{ 
		if(que_20.size() == 20) return sum20 / 20; 
		else return 0;
	} 

	double GetMA60() 
	{ 
		if(que_60.size() == 60) return sum60 / 60; 
		else return 0;
	} 
};

class KLineReader
{
private:

	map<string, KLineCollection>	m_cachedData;

public:
	KLineReader(void);
	~KLineReader(void);

	void Read(string path, KLineCollection& klines, int nTillDate = -1);

	KLine GetKLineByTime(string path, int time);
};
