#pragma once

#include "TickReader.h"

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
};
