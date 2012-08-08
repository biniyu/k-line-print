#pragma once

#include "TickReader.h"
#include <vector>
#include <map>
using namespace std;

class KLine
{
public:
	int time;				//	K�ߵ�ǰʱ��
	int start_time;			//	K����ʼʱ��
	int high;
	int low;
	int open;
	int close;
	int vol;				//	��ǰK�ߵĳɽ���
	int interest;			//  �ֲ���
	
	//	����
	int		avg;			//	������
	int		avg_devi;		//	����ƫ��ֵ
	float	vol_acc;		//	�ۻ��ɽ���
	double	price_acc;		//	�ۻ��ɽ��۸�
	
	//	����
	double ma5;
	double ma10;
	double ma20;
	double ma60;

	//	�����ͼ�
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

	//	����ʱ�佨�����������ڿ��ٲ���
	map<int, int>		m_mapTime2Idx;

	//	�����ڷ���ͼ����ʾ����MA
	map<int, string>	m_mapKeyPrice;

	//	K������(����Ϊ��λ, -1Ϊ����)
	int					m_nKLinePeriod;

public:

	int					m_nMaxPrice;			//	������߼�
	int					m_nMinPrice;			//	������ͼ�

	
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
	void GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol, int& nMaxInterest);

	//	��ʼ���շֱ�����
	void StartQuote(Tick tick);

	//	���շֱ�����
	void Quote(Tick tick);

	//	�������ƫ��ֵ
	int GetAvgDevi(KLine kline);

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds);

	//	�ӷֱ���������ָ������(����Ϊ��λ)��K������
	void Generate(TickCollection& ticks, int seconds, KLine prevDayLine);

	//	��ӹؼ��۸�
	void AddKeyPrice(int price, string desc) { m_mapKeyPrice[price] = desc; }

	map<int, string>& GetKeyPrices() { return m_mapKeyPrice; }

	void Clear();
};
