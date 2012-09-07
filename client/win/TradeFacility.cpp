#include "StdAfx.h"
#include "TradeFacility.h"
#include "Utility.h"

TradeParam	gTradeParam;					//	交易参数

TradeFacility::TradeFacility(void)
{
	m_nPosition.nPrice = 0;
	m_nPosition.nSlot = 0;
	m_nPosition.nProfit = 0;
	m_nBalance = 0;
	m_nPosition.nProfitStop = m_nPosition.nLossStop = m_nPosition.nTrigger = 0;
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
		m_nPosition.nProfit = (m_nTick.price - m_nPosition.nPrice) * m_nPosition.nSlot * TP.nUnitsPerSlot;
	}
}

TradeRecord TradeFacility::Log(int nTime, bool bBuy, bool bOpen, 
						int nPrice, int nSlot, int nFee, int nProfit)
{
	TradeRecord tr(TRADE_CODE_SUCCESS);

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

	Utility::WriteLog(m_strLogPath, tr);

	return tr;
}

//	开仓
TradeRecord TradeFacility::Buy(int nSlot, int nLossStop)
{
	//	必须先平仓后再开
	if(m_nPosition.nSlot != 0) 
		return TradeRecord(TRADE_CODE_OPPOSITE);

	//	检查保证金是否足够
	float margin = nSlot * m_nTick.priceS1 * TP.nUnitsPerSlot * TP.nMarginRate / 100.0;

	if(m_nBalance < margin)
	{
		return TradeRecord(TRADE_CODE_NO_MONEY);
	}

	if(m_nOpenTimes >= TP.nMaxOpenTimes)
	{
		return TradeRecord(TRADE_CODE_MAX_OPEN);
	}

	if(m_nTotalFee - m_nTotalProfit > TP.nMaxLossPerDay)
	{
		return TradeRecord(TRADE_CODE_MAX_LOSS);
	}

	m_nPosition.nPrice = m_nTick.priceS1;
	m_nPosition.nTime = m_nTick.time_ms;
	m_nPosition.nSlot = nSlot;
	m_nPosition.nProfit = 0;
	m_nPosition.nLossStop = nLossStop;
	m_nPosition.nTrigger = 0;

	m_nMargin = margin;

	m_nBalance -= (nSlot * TP.nFee);
	m_nTotalFee += nSlot * TP.nFee;

	m_nOpenTimes++;

	return Log(m_nTick.time_ms, true, true, 
			   m_nTick.priceS1, nSlot, nSlot * TP.nFee, 0);
}

TradeRecord TradeFacility::Sell(int nSlot, int nLossStop)
{
	if(m_nPosition.nSlot != 0) 
		return TradeRecord(TRADE_CODE_OPPOSITE);

	//	检查保证金是否足够
	float margin = nSlot * m_nTick.priceS1 * TP.nUnitsPerSlot * TP.nMarginRate / 100.0;

	if(m_nBalance < margin)
	{
		return TradeRecord(TRADE_CODE_NO_MONEY);
	}

	if(m_nOpenTimes >= TP.nMaxOpenTimes)
	{
		return TradeRecord(TRADE_CODE_MAX_OPEN);
	}

	if(m_nTotalFee - m_nTotalProfit > TP.nMaxLossPerDay)
	{
		return TradeRecord(TRADE_CODE_MAX_LOSS);
	}

	m_nPosition.nPrice = m_nTick.priceB1;
	m_nPosition.nTime = m_nTick.time_ms;
	m_nPosition.nSlot = -nSlot;
	m_nPosition.nProfit = 0;
	m_nPosition.nLossStop = nLossStop;
	m_nPosition.nTrigger = 0;

	m_nMargin = margin;

	m_nBalance -= (nSlot * TP.nFee);
	m_nTotalFee += nSlot * TP.nFee;

	m_nOpenTimes++;

	return Log(m_nTick.time_ms, false, true, 
			   m_nTick.priceB1, nSlot, nSlot * TP.nFee, 0);
}

//	平仓
TradeRecord TradeFacility::Close()
{
	int nProfit = 0, nFee = 0;
	TradeRecord rec(TRADE_CODE_SUCCESS);

	if(m_nPosition.nSlot == 0) return TradeRecord(TRADE_CODE_SUCCESS);

	nFee = (abs(m_nPosition.nSlot) * TP.nFee);

	if(m_nPosition.nSlot > 0)
	{
		nProfit = (m_nTick.priceB1 - m_nPosition.nPrice) * m_nPosition.nSlot * TP.nUnitsPerSlot;
		m_nBalance += nProfit;

		//	卖出平仓
		rec = Log(m_nTick.time_ms, false, false, 
				  m_nTick.priceB1, abs(m_nPosition.nSlot), nFee, nProfit);
	}
	else
	{
		nProfit = (m_nTick.priceS1 - m_nPosition.nPrice) * m_nPosition.nSlot * TP.nUnitsPerSlot;
		m_nBalance += nProfit;

		//	买入平仓
		rec = Log(m_nTick.time_ms, true, false, 
				  m_nTick.priceS1, abs(m_nPosition.nSlot), nFee, nProfit);
	}

	m_nBalance -= nFee;
	m_nTotalFee += nFee;
	m_nTotalProfit += nProfit; 

	m_nPosition.nTime = 0;
	m_nPosition.nSlot = 0;
	m_nPosition.nPrice = 0;
	m_nPosition.nProfit = 0;
	m_nPosition.nLossStop = m_nPosition.nProfitStop = m_nPosition.nTrigger = 0;

	m_nMargin = 0;

	return rec;
}

//	反手
TradeRecordCollection TradeFacility::Reverse()
{
	int nSlot = m_nPosition.nSlot;
	TradeRecordCollection trs;

	if(nSlot == 0) return trs;

	trs.push_back(Close());

	if(nSlot > 0)
	{
		trs.push_back(Sell(abs(nSlot)));
	}
	else
	{
		trs.push_back(Buy(abs(nSlot)));
	}

	return trs;
}
