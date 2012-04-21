#include "StdAfx.h"
#include "Utility.h"
#include "KLineCollection.h"

KLineCollection::KLineCollection(void)
{
	m_nMaxPrice = m_nMinPrice = 0;
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

//	统一接口用于创建索引
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

//	开始接收分笔数据
void KLineCollection::StartQuote(Tick tick)
{
	KLine tmp;

	tmp.high = tmp.low = tmp.open = tmp.close = tick.price;
	tmp.vol = tick.vol;

	tmp.time = tick.time;
	tmp.start_time = tick.time;

	tmp.vol_acc = tick.vol;
	tmp.price_acc = tick.vol * tick.price;

	m_nMaxPrice = m_nMinPrice = tick.price;

	AddToTail(tmp);
}

//	接收分笔数据
void KLineCollection::Quote(Tick tick)
{
	KLine& curKLine = (*this)[this->size() - 1];

	if(tick.price > m_nMaxPrice) m_nMaxPrice = tick.price;

	if(tick.price < m_nMinPrice) m_nMinPrice = tick.price;

	if((tick.time / m_nKLinePeriod) != (curKLine.start_time / m_nKLinePeriod))
	{
		curKLine.time = tick.time;
		curKLine.avg = curKLine.price_acc / (float) curKLine.vol_acc;

		/* 新起K线 */
		KLine tmp;
		
		tmp.high = tmp.low = tmp.open = tmp.close = tick.price;
		tmp.vol = tick.vol;
		
		tmp.time = tick.time;
		tmp.start_time = tick.time;

		tmp.vol_acc = curKLine.vol_acc + tick.vol;
		tmp.price_acc = curKLine.price_acc + tick.vol * tick.price;
		tmp.avg = tmp.price_acc / (float) tmp.vol_acc;

		AddToTail(tmp);
	}
	else
	{
		if(tick.price > curKLine.high) 
			curKLine.high = tick.price;

		if(tick.price < curKLine.low) 
			curKLine.low = tick.price;

		curKLine.time = tick.time;
		curKLine.close = tick.price;
		curKLine.vol += tick.vol;
		curKLine.vol_acc += tick.vol;
		curKLine.price_acc += tick.vol * tick.price;
		curKLine.avg = curKLine.price_acc / (float) curKLine.vol_acc;
	}
}

void KLineCollection::Generate(TickCollection& ticks, int seconds)
{
	int nLastSecond;

	/* 分钟K线的数值 */
	int kOpen, kClose, kHigh, kLow, kVol;
	float totalPrice = 0, totalVol = 0, kCount = 0;

	kOpen = kClose = kHigh = kLow = ticks[0].price;
	nLastSecond = ticks[0].time;
	kVol = ticks[0].vol;
	totalVol = kVol;
	totalPrice = kClose * kVol;

	// 首K线是前日的日K，不能计算进总成交量

	for(int i = 1; i < ticks.size(); i++)
	{
		int nCurSecond = ticks[i].time;
		int price = ticks[i].price;
		int vol = ticks[i].vol;

		totalPrice += (price*vol);
		totalVol += vol;

		if((nCurSecond / seconds) != (nLastSecond / seconds))
		{
			/* K 已完成，写入文件 */ 
			KLine kline;

			kline.time = Utility::ConvContTimeToDispTime(nCurSecond);
			kline.high = kHigh;
			kline.low = kLow;
			kline.open = kOpen;
			kline.close = kClose;
			kline.vol = kVol;
			kline.vol_acc = totalVol;

			//	计算均价线
			kline.avg = totalPrice / totalVol;

			AddToTail(kline);

			kCount++;

			/* 新起K线 */
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
	/* 昨天本合约的日K */
	AddToTail(prevDayLine);
	Generate(ticks, seconds);
}
