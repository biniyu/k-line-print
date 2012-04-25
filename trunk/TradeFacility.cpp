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
	m_nTick = tick;

	if(m_nPosition.nSlot)
	{
		m_nPosition.nProfit = (m_nTick.price - m_nPosition.nPrice) * m_nPosition.nSlot * m_nUnitsPerSlot;
	}
}

//	����
void TradeFacility::Buy(int nSlot)
{
	//	������ƽ�ֺ��ٿ�
	if(m_nPosition.nSlot != 0) return;

	m_nPosition.nPrice = m_nTick.priceS1;
	m_nPosition.nSlot = nSlot;
	m_nPosition.nProfit = 0;

	m_nBalance -= (nSlot * m_nFee);
}

void TradeFacility::Sell(int nSlot)
{
	if(m_nPosition.nSlot != 0) return;

	m_nPosition.nPrice = m_nTick.priceB1;
	m_nPosition.nSlot = -nSlot;
	m_nPosition.nProfit = 0;

	m_nBalance -= (nSlot * m_nFee);
}

//	ƽ��
void TradeFacility::Close()
{
	if(m_nPosition.nSlot == 0) return;

	if(m_nPosition.nSlot > 0)
	{
		m_nBalance += (m_nTick.priceB1 - m_nPosition.nPrice) * m_nPosition.nSlot * m_nUnitsPerSlot;
	}
	else
	{
		m_nBalance += (m_nTick.priceS1 - m_nPosition.nPrice) * m_nPosition.nSlot * m_nUnitsPerSlot;		
	}

	m_nBalance -= (abs(m_nPosition.nSlot) * m_nFee);

	m_nPosition.nSlot = 0;
	m_nPosition.nPrice = 0;
	m_nPosition.nProfit = 0;
}

//	����
void TradeFacility::Reverse()
{
	int nSlot = m_nPosition.nSlot;

	if(nSlot == 0) return;

	Close();

	if(nSlot > 0)
	{
		Sell(abs(nSlot));
	}
	else
	{
		Buy(abs(nSlot));
	}
}
