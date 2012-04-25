#include "StdAfx.h"
#include "TradeFacility.h"

TradeFacility::TradeFacility(void)
{
	m_nPosition.nPrice = 0;
	m_nPosition.nSlot = 0;
	m_nPosition.nProfit = 0;
	m_nBalance = 0;
}

TradeFacility::~TradeFacility(void)
{
}

//	设置当前分笔数据，并且更新浮动盈亏等
void TradeFacility::SetTick(Tick tick)
{

}

//	开仓
void TradeFacility::Buy(int nSlot)
{

}

void TradeFacility::Sell(int nSlot)
{

}

//	平仓
void TradeFacility::Close()
{

}

//	反手
void TradeFacility::Reverse()
{

}
