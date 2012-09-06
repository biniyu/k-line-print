#include "StdAfx.h"
#include "Strategy.h"

Strategy::Strategy(void)
{
}

Strategy::~Strategy(void)
{
}

void Strategy::Quote(Tick tick)
{
	//	支持跨周期，支持各种指标
	(*m_p1MinData)[m_p1MinData->size() - 1];
	
}
