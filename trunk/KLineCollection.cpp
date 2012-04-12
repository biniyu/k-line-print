#include "StdAfx.h"
#include "KLineCollection.h"

KLineCollection::KLineCollection(void)
{

}

KLineCollection::~KLineCollection(void)
{

}

KLine KLineCollection::GetKLineByTime(int nTime)
{
	if(m_mapTime2Idx.find(nTime) == m_mapTime2Idx.end())
	{
		KLine tmp;
		memset(&tmp, 0, sizeof(tmp));
		return tmp;
	}
	else
	{
		return (*this)[m_mapTime2Idx[nTime]];
	}
}

//	ͳһ�ӿ����ڴ�������
void KLineCollection::AddToTail(KLine kline)
{
	m_mapTime2Idx[kline.time] = this->size();
	push_back(kline);
}

void KLineCollection::Clear()
{
	m_mapTime2Idx.clear();
	m_mapKeyPrice.clear();
	clear();
}

void KLineCollection::GetPriceVolRange(int nStartIdx, int nEndIdx, int& nHighPr, int& nLowPr, int& nMaxVol)
{
	int high = 0, low = 0, maxvol = 0;

	for(int i = nStartIdx; i <= nEndIdx; i++)
	{
		if(i >= this->size()) continue;

		KLine kline = (*this)[i];
		
		if(i == nStartIdx)
		{
			high = kline.high;
			low = kline.low;
			maxvol = kline.vol;
		}
		else
		{
			if(kline.high > high) high = kline.high;
			if(kline.low < low) low = kline.low;
			if(kline.vol > maxvol) maxvol = kline.vol;
		}
	}

	nHighPr = high;
	nLowPr = low;
	nMaxVol = maxvol;
}

//	��ʼ���շֱ�����
void KLineCollection::StartQuote(Tick tick)
{
	KLine tmp;

	tmp.high = tmp.low = tmp.open = tmp.close = tick.price;
	tmp.vol = tick.vol;
	tmp.time = tick.time;

	AddToTail(tmp);
}

//	���շֱ�����
void KLineCollection::Quote(Tick tick)
{
	KLine& curKLine = (*this)[this->size() - 1];

	if((tick.time / m_nKLinePeriod) != (curKLine.time / m_nKLinePeriod))
	{
		int tmphour, tmpmin, tmpsec;

		//	��ԭʱ��
		tmphour = tick.time / 3600;
		tmpmin = tick.time % 3600 / 60;
		tmpsec = tick.time % 3600 % 60;

		curKLine.time = tmphour * 10000 + tmpmin * 100 + tmpsec;

		/* ����K�� */
		KLine tmp;
		tmp.high = tmp.low = tmp.open = tmp.close = tick.price;
		tmp.vol = tick.vol;
		tmp.time = tick.time;
		AddToTail(tmp);
	}
	else
	{
		if(tick.price > curKLine.high) 
			curKLine.high = tick.price;

		if(tick.price < curKLine.low) 
			curKLine.low = tick.price;

		curKLine.close = tick.price;
		curKLine.vol += tick.vol;
	}
}

//	�������շֱ�����
void KLineCollection::EndQuote(Tick tick)
{

}

void KLineCollection::Generate(TickCollection& ticks, int seconds)
{
	int nLastSecond;

	/* ����K�ߵ���ֵ */
	int kOpen, kClose, kHigh, kLow, kVol;
	float totalPrice = 0, totalVol = 0, kCount = 0;

	kOpen = kClose = kHigh = kLow = ticks[0].price;
	nLastSecond = ticks[0].time;
	kVol = ticks[0].vol;
	totalVol = kVol;
	totalPrice = kClose * kVol;

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

			int tmphour, tmpmin, tmpsec;

			//	��ԭʱ��
			tmphour = nCurSecond / 3600;
			tmpmin = nCurSecond % 3600 / 60;
			tmpsec = nCurSecond % 3600 % 60;

			kline.time = tmphour * 10000 + tmpmin * 100 + tmpsec;
			kline.high = kHigh;
			kline.low = kLow;
			kline.open = kOpen;
			kline.close = kClose;
			kline.vol = kVol;
			kline.vol_acc = totalVol;

			//	���������
			kline.avg = totalPrice / totalVol;

			AddToTail(kline);

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
}

void KLineCollection::Generate(TickCollection& ticks, int seconds, KLine prevDayLine)
{
	/* ���챾��Լ����K */
	AddToTail(prevDayLine);
	Generate(ticks, seconds);
}
