#include "StdAfx.h"
#include "KLineCollection.h"
#include "TickReader.h"


KLineCollection::KLineCollection(void)
{

}

KLineCollection::~KLineCollection(void)
{

}

void KLineCollection::GetHighLowPrice()
{
	for(int i = startidx; i <= endidx; i++)
	{
		KLine kline = (*this)[startidx];
		
		if(i == startidx)
		{
			high = kline.high;
			low = kline.low;
		}
		else
		{
			if(kline.high > high) high = kline.high;
			if(kline.low < low) low = kline.low;
		}
	}
}

void KLineCollection::Generate(TickCollection& ticks, int seconds, KLine prevDayLine)
{
	//	���ոߵ͵�
	maxvol = 0;

	high = ticks.high;
	low = ticks.low;

	int nLastSecond;

	/* ����K�ߵ���ֵ */
	int kOpen, kClose, kHigh, kLow, kVol;
	float totalPrice = 0, totalVol = 0, kCount = 0;

	/* ���챾��Լ����K */
	push_back(prevDayLine);

	kOpen = kClose = kHigh = kLow = ticks[0].price;
	nLastSecond = ticks[0].time;
	kVol = 0;

	// ��K����ǰ�յ���K�����ܼ�����ܳɽ���

	for(int i = 1; i < ticks.size(); i++)
	{
		int nCurSecond = ticks[i].time;
		int price = ticks[i].price;
		int vol = ticks[i].vol;

		totalPrice += (price*vol);
		totalVol += vol;

		if((nCurSecond / seconds) != (nLastSecond / seconds))
		{
			/* K ����ɣ�д���ļ� */ 
			KLine kline;

			kline.time = nCurSecond;
			kline.high = kHigh;
			kline.low = kLow;
			kline.open = kOpen;
			kline.close = kClose;
			kline.vol = kVol;

			if(kVol > maxvol)
				maxvol = kVol;

			//	���������
			kline.avg = totalPrice / totalVol;

			push_back(kline);

			kCount++;

			/* ����K�� */
			kOpen = kClose = kHigh = kLow = price;
			kVol = vol;
		}
		else
		{
			if(price > kHigh) kHigh = price;
			if(price < kLow) kLow = price;
			kClose = price;
			kVol += vol;
		}

		nLastSecond = nCurSecond;		
	}

	avgvol = totalVol/(float)kCount;
}
