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
	int weekday = Utility::GetWeekDayByDate(tick.date);

	static int high = 0,low = 0;

	//	只在星期一、五交易
	if(weekday != 1 && weekday != 5) return;

	if(curtime < m_lasttime || m_lasttime == 0)
	{
		high = low = tick.price;
		m_lastprice = tick.price;
	}

	if(curtime > 90000 && curtime < 100000)
	{
		if(tick.price > high) high = tick.price;
		if(tick.price < low) low = tick.price;
	}

	if(curtime > 100000 && curtime < 145500)
	{
		if(m_lastprice <= high + 6 && tick.price > high + 6)
		{
			if(position == 0)
			{
				if(EXCHANGE.m_nOpenTimes == 0 && kline.interest > 20000)
					EXCHANGE.Buy(1);
			}
			else if(position < 0)
			{
				//EXCHANGE.Reverse();
			}
		}
		else if(m_lastprice >= high && tick.price < high)
		{
			if(position > 0) EXCHANGE.Close();
				//EXCHANGE.Reverse();
		}
		else if(m_lastprice >= low - 6 && tick.price < low - 6)
		{
			if(position == 0)
			{
				if(EXCHANGE.m_nOpenTimes == 0 && kline.interest > 20000)
					EXCHANGE.Sell(1);
			}
			else if(position > 0)
			{
				//EXCHANGE.Reverse();
			}
		}
		else if(m_lastprice <= low && tick.price > low)
		{
			if(position < 0) EXCHANGE.Close();
				//EXCHANGE.Reverse();
		}
	}

	if(curtime > 145500 && position)
	{
		EXCHANGE.Close();
	}

	m_lasttime = curtime;
	m_lastprice = tick.price;
}
