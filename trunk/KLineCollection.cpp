#include "StdAfx.h"
#include "KLineCollection.h"
#include "TickReader.h"

#define ZOOM_STEP 20

KLineCollection::KLineCollection(void)
{

}

KLineCollection::~KLineCollection(void)
{

}

void KLineCollection::Inflate()
{
	int s1 = curidx - startidx;
	int s2 = endidx - curidx;

	startidx += ZOOM_STEP * s1 / (float) (s1+s2);
	endidx -= ZOOM_STEP * s2 / (float) (s1+s2);

	if(startidx < 0) startidx = 0;
	if(endidx > size() - 1) endidx = size() - 1;

//	GetHighLowPrice();
}

void KLineCollection::Deflate()
{
	int s1 = curidx - startidx;
	int s2 = endidx - curidx;

	startidx -= ZOOM_STEP * s1 / (float) (s1+s2);
	endidx += ZOOM_STEP * s2 / (float) (s1+s2);

	if(startidx < 0) startidx = 0;
	if(endidx > size() - 1) endidx = size() - 1;

//	GetHighLowPrice();
}

void KLineCollection::MovePrev()
{
	curidx--;
}

void KLineCollection::MoveNext()
{
	curidx++;
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
	//	当日高低点
	maxvol = 0;

	high = ticks.high;
	low = ticks.low;

	int nLastSecond;

	/* 分钟K线的数值 */
	int kOpen, kClose, kHigh, kLow, kVol;
	float totalPrice = 0, totalVol = 0, kCount = 0;

	/* 昨天本合约的日K */
	push_back(prevDayLine);

	kOpen = kClose = kHigh = kLow = ticks[0].price;
	nLastSecond = ticks[0].time;
	kVol = 0;

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

			kline.time = nCurSecond;
			kline.high = kHigh;
			kline.low = kLow;
			kline.open = kOpen;
			kline.close = kClose;
			kline.vol = kVol;

			if(kVol > maxvol)
				maxvol = kVol;

			//	计算均价线
			kline.avg = totalPrice / totalVol;

			push_back(kline);

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

	avgvol = totalVol/(float)kCount;

	// 初始化显示区域
	startidx = 0;
	endidx = size() - 1;
	curidx = (startidx + endidx) / 2;
}
