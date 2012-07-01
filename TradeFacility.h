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
private:
	int m_nSelIdx;

public:

	int GetSelIdx() { return m_nSelIdx; }

	void SetSelIdx(int nSelIdx) 
	{
		if(nSelIdx < 0 || nSelIdx > size() - 1)
			m_nSelIdx = size() - 1;
		else
			m_nSelIdx = nSelIdx;
	}
};

class Position
{
public:
	int nSlot;		//	负数表示卖空
	int nTime;		//	交易时间，用于时间止损
	int nPrice;		//	开仓价格
	int nProfit;	//	浮动盈亏
};

class TradeFacility
{
public:

	int			m_nIntialBalance;			//	当日初始余额
	int			m_nBalance;
	int			m_nFee;
	int			m_nMargin;
	int			m_nDefaultSlots;
	int			m_nUnitsPerSlot;
	int			m_nMaxLoss;					//	最大止损
	int			m_nMaxProfit;				//	最大止盈
	int			m_nTimeStop;				//	时间止损

	Tick		m_nTick;
	Position	m_nPosition;
	string		m_nFilePath;

	int			m_nTotalFee;				//	当日手续费总额
	int			m_nTotalProfit;				//	当日毛利润总额

public:

	TradeFacility(void);
	~TradeFacility(void);

	void ResetStats()
	{
		m_nIntialBalance = m_nBalance;
		m_nTotalFee = m_nTotalProfit = 0;
	}

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
	TradeRecord Buy(int nSlot);
	TradeRecord Sell(int nSlot);

	//	平仓
	TradeRecord Close();

	//	反手
	TradeRecordCollection Reverse();

	//	交易记录
	TradeRecord Log(int nTime, bool bBuy, bool bOpen, 
					int nPrice, int nSlot, int nFee, int nProfit);

};
