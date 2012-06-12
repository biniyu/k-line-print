#include "StdAfx.h"
#include "TradeFacility.h"
#include "Utility.h"

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
	m_nTick = tick;

	if(m_nPosition.nSlot)
	{
		m_nPosition.nProfit = (m_nTick.price - m_nPosition.nPrice) * m_nPosition.nSlot * m_nUnitsPerSlot;
	}
}

void TradeFacility::Log(int nTime, bool bBuy, bool bOpen, 
						int nPrice, int nSlot, int nFee, int nProfit)
{
	TradeRecord tr;

	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	tr.nRealDate = (1900 + timeinfo->tm_year) * 10000 + (timeinfo->tm_mon + 1) * 100 + timeinfo->tm_mday;
	tr.nRealTime = timeinfo->tm_hour * 10000 + timeinfo->tm_min * 100 + timeinfo->tm_sec;

	tr.sFileName = m_nFilePath;
	tr.nSimuTime = nTime;
	tr.bBuy = bBuy;
	tr.bOpen = bOpen;
	tr.nPrice = nPrice;
	tr.nSlot = nSlot;
	tr.nFee = nFee;
	tr.nProfit = nProfit;

	Utility::WriteLog(tr);
}

//	开仓
void TradeFacility::Buy(int nSlot)
{
	//	必须先平仓后再开
	if(m_nPosition.nSlot != 0) return;

	m_nPosition.nPrice = m_nTick.priceS1;
	m_nPosition.nSlot = nSlot;
	m_nPosition.nProfit = 0;

	m_nBalance -= (nSlot * m_nFee);

	Log(m_nTick.time_ms, true, true, 
		m_nTick.priceS1, nSlot, nSlot * m_nFee, 0);
}

void TradeFacility::Sell(int nSlot)
{
	if(m_nPosition.nSlot != 0) return;

	m_nPosition.nPrice = m_nTick.priceB1;
	m_nPosition.nSlot = -nSlot;
	m_nPosition.nProfit = 0;

	m_nBalance -= (nSlot * m_nFee);

	Log(m_nTick.time_ms, false, true, 
		m_nTick.priceB1, nSlot, nSlot * m_nFee, 0);
}

//	平仓
void TradeFacility::Close()
{
	int nProfit = 0, nFee = 0;

	if(m_nPosition.nSlot == 0) return;

	nFee = (abs(m_nPosition.nSlot) * m_nFee);

	if(m_nPosition.nSlot > 0)
	{
		nProfit = (m_nTick.priceB1 - m_nPosition.nPrice) * m_nPosition.nSlot * m_nUnitsPerSlot;
		m_nBalance += nProfit;

		//	卖出平仓
		Log(m_nTick.time_ms, false, false, 
			m_nTick.priceB1, m_nPosition.nSlot, nFee, nProfit);
	}
	else
	{
		nProfit = (m_nTick.priceS1 - m_nPosition.nPrice) * m_nPosition.nSlot * m_nUnitsPerSlot;
		m_nBalance += nProfit;

		//	买入平仓
		Log(m_nTick.time_ms, true, false, 
			m_nTick.priceS1, m_nPosition.nSlot, nFee, nProfit);
	}

	m_nBalance -= nFee;

	m_nPosition.nSlot = 0;
	m_nPosition.nPrice = 0;
	m_nPosition.nProfit = 0;
}

//	反手
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
