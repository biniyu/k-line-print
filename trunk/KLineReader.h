#pragma once

#include <string>
#include <deque>

using namespace std;

class KLineCollection;


class FastMovingAverageComputer
{
private:

	double sum5;
	double sum10;
	double sum20;
	double sum60;

	deque<double> que_ma5;
	deque<double> que_ma10;
	deque<double> que_ma20;
	deque<double> que_ma60;

public:

	FastMovingAverageComputer()
	{
		sum5 = sum10 = sum20 = sum60 = 0;
	}


	void PushNewData(double data)
	{
		que_ma5.push_back(data);
		que_ma10.push_back(data);
		que_ma20.push_back(data);
		que_ma60.push_back(data);

		sum5 += data;
		sum10 += data;
		sum20 += data;
		sum60 += data;

		if(que_ma5.size() > 5 ) 
		{
			sum5 -= que_ma5.front();
			que_ma5.pop_front();
		}
		
		if(que_ma10.size() > 10 ) 
		{
			sum10 -= que_ma10.front();
			que_ma10.pop_front();
		}

		if(que_ma20.size() > 20 ) 
		{
			sum20 -= que_ma20.front();
			que_ma20.pop_front();
		}
		
		if(que_ma60.size() > 60 ) 
		{
			sum60 -= que_ma60.front();
			que_ma60.pop_front();
		}
	}

	double GetMA5() 
	{
		if(que_ma5.size() == 5) return sum5 / 5; 
		else return 0;
	}

	double GetMA10() 
	{ 
		if(que_ma10.size() == 10) return sum10 / 10; 
		else return 0;
	} 

	double GetMA20() 
	{ 
		if(que_ma20.size() == 20) return sum20 / 20; 
		else return 0;
	} 

	double GetMA60() 
	{ 
		if(que_ma60.size() == 60) return sum60 / 60; 
		else return 0;
	} 
};

class KLineReader
{
public:
	KLineReader(void);
	~KLineReader(void);

	void Read(string path, KLineCollection& klines, int nTillDate = -1);
};
