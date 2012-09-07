#include "StdAfx.h"
#include "Strategy.h"
#include "KLinePrint.h"
#include "Utility.h"

Strategy::Strategy(void)
{
	m_lasttime = 0;
}

Strategy::~Strategy(void)
{
}

void Strategy::Quote(Tick tick)
{
	//	支持跨周期，支持各种指标
	KLine kline = (*m_p1MinData)[m_p1MinData->size() - 1];

	//	9-10区间突破

	int curtime = Utility::ConvContTimeToDispTime(tick.time_ms/1000);
	int position = EXCHANGE.m_nPosition.nSlot;

	static int high = 0,low = 0;

	if(curtime < m_lasttime || m_lasttime == 0)
	{
		high = low = tick.price;
	}

	if(curtime > 90000 && curtime < 100000)
	{
		if(tick.price > high) high = tick.price;
		if(tick.price < low) low = tick.price;
	}

	if(curtime > 100001 && curtime < 145500)
	{
		if(tick.price > high && position == 0)
		{
			EXCHANGE.Buy(1);
		}

		if(tick.price < low && position == 0)
		{
			EXCHANGE.Sell(1);
		}
	}

	if(curtime > 145500 && position)
	{
		EXCHANGE.Close();
	}

	m_lasttime = curtime;
}
