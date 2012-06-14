#pragma once

#include "TickReader.h"
#include <string>
#include <vector>

using namespace std;

class TradeRecord
{
public:
	int		nRealDate;
	int		nRealTime;
	string	sFileName;
	int		nSimuTime;
	bool	bBuy;				//	买入卖出
	bool	bOpen;				//	开平仓
	int		nPrice;				//	成交价格
	int		nSlot;				//	手数
	int		nFee;				//	手续费
	int		nProfit;			//	平仓盈亏
};

class TradeRecordCollection : public vector<TradeRecord>
{

};

class Position
{
public:
	int nSlot;		//	负数表示卖空
	int nPrice;		//	开仓价格
	int nProfit;	//	浮动盈亏
};

class TradeFacility
{
public:

	int			m_nBalance;
	int			m_nFee;
	int			m_nMargin;
	int			m_nUnitsPerSlot;
	Tick		m_nTick;
	Position	m_nPosition;
	string		m_nFilePath;

public:

	TradeFacility(void);
	~TradeFacility(void);

	//	设置账户余额	
	void SetBalance(int nMoney) { m_nBalance = nMoney; }

	//	设置合约参数
	void SetParam(int nFee, int nMargin, int nUnitsPerSlot)
	{
		m_nFee = nFee;
		m_nMargin = nMargin;
		m_nUnitsPerSlot = nUnitsPerSlot;
	}

	//	设置当前分笔数据，并且更新浮动盈亏等
	void SetTick(Tick tick);

	//	开仓
	void Buy(int nSlot);
	void Sell(int nSlot);

	//	平仓
	void Close();

	//	反手
	void Reverse();

	//	交易记录
	void Log(int nTime, bool bBuy, bool bOpen, 
			int nPrice, int nSlot, int nFee, int nProfit);

};
