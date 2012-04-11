#pragma once

#include "TickReader.h"
#include <vector>
#include <map>
using namespace std;

class KLine
{
public:
	int time;
	int high;
	int low;
	int open;
	int close;
	int vol;		//	��ǰK�ߵĳɽ���
	int vol_acc;	//	�ۻ��ɽ���
	int avg;		//	������
	double ma5;
	double ma10;
	double ma20;
	double ma60;
};

class KLineCollection : public vector<KLine>
{
private:

	//	����ʱ�佨�����������ڿ��ٲ���
	map<int, int>		m_mapTime2Idx;

	//	�����ڷ���ͼ����ʾ����MA
	map<int, string>	m_mapKeyPrice;

	//	K������(����Ϊ��λ, -1Ϊ����)
	int					m_nKLinePeriod;
	
public:
	KLineCollection(void);
	~KLineCollection(void);

	//	��ȡָ��ʱ��/���ڵ�K������
	KLine GetKLineByTime(int nTime);

	//	����K������(����Ϊ��λ, -1Ϊ����)
	void SetPeriod(int nPeriod) { m_nKLinePeriod = nPeriod; }

	//	ͳһ�ӿ����ڴ�������
	void AddToTail(KLine kline);

	//	��ȡ�۸�ͳɽ����������Сֵ
	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol);

	//	��ʼ���շֱ�����
	void StartQuote(Tick tick);

	//	���շֱ�����
	void Quote(Tick tick);

	//	�������շֱ�����
	void EndQuote(Tick tick);

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds);

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);

	//	��ӹؼ��۸�
	void AddKeyPrice(int price, string desc) { m_mapKeyPrice.insert(pair<int,string>(price, desc)); }

	map<int, string>& GetKeyPrices() { return m_mapKeyPrice; }

	void Clear();
};
