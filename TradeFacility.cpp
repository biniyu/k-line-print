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

//	���õ�ǰ�ֱ����ݣ����Ҹ��¸���ӯ����
void TradeFacility::SetTick(Tick tick)
{

}

//	����
void TradeFacility::Buy(int nSlot)
{

}

void TradeFacility::Sell(int nSlot)
{

}

//	ƽ��
void TradeFacility::Close()
{

}

//	����
void TradeFacility::Reverse()
{

}
