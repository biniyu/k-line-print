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
	//	֧�ֿ����ڣ�֧�ָ���ָ��
	KLine kline = (*m_p1MinData)[m_p1MinData->size() - 1];

	//	9-10����ͻ��

//	EXCHANGE.Buy(1);
//	EXCHANGE.Close();

	kline.avg;
}
