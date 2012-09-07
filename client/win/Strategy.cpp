#include "StdAfx.h"
#include "Strategy.h"
#include "KLinePrint.h"

Strategy::Strategy(void)
{
}

Strategy::~Strategy(void)
{
}

void Strategy::Quote(Tick tick)
{
	//	支持跨周期，支持各种指标
	KLine kline = (*m_p1MinData)[m_p1MinData->size() - 1];

	//	9-10区间突破

//	EXCHANGE.Buy(1);
//	EXCHANGE.Close();

	kline.avg;
}
