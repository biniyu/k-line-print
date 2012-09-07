#include "StdAfx.h"
#include "Strategy.h"
#include "KLinePrint.h"
#include "Utility.h"

Strategy::Strategy(void)
{
}

Strategy::~Strategy(void)
{
}

void Strategy::Quote(Tick tick)
{
	//	֧�ֿ����ڣ�֧�ָ���ָ��
	KLine kline = (*m_p1MinData)[m_p1MinData->size() - 1];

	//	9-10����ͻ��

	int curtime = Utility::ConvContTimeToDispTime(tick.time_ms/1000);

	if(EXCHANGE.m_nPosition.nSlot == 0 && curtime > 91000 && curtime < 91500)
	{
		EXCHANGE.Buy(1);
	}

	if(EXCHANGE.m_nPosition.nSlot && curtime > 92000)
	{
		EXCHANGE.Close();
	}

//	EXCHANGE.Buy(1);
//	EXCHANGE.Close();

	kline.avg;
}
