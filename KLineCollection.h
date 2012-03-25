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
	int avg;	// ������
};

class KLineCollection : public vector<KLine>
{
private:

	//	����ʱ�佨�����������ڿ��ٲ���
	map<int, int> m_mapTime2Idx;
	
public:
	KLineCollection(void);
	~KLineCollection(void);

	//	��ȡָ��ʱ��/���ڵ�K������
	KLine GetKLineByTime(int nTime);

	//	ͳһ�ӿ����ڴ�������
	void AddToTail(KLine kline);

	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol);

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);
};
