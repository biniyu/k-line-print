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
	int avg;	// ������
};

class KLineCollection : public vector<KLine>
{
public:
	KLineCollection(void);
	~KLineCollection(void);

	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol);

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);
};
