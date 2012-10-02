#pragma once

#include "TickReader.h"
#include <string>
#include <vector>

using namespace std;

#define TRADE_CODE_SUCCESS		0
#define TRADE_CODE_NO_MONEY		1
#define TRADE_CODE_MAX_OPEN		2
#define TRADE_CODE_MAX_LOSS		3
#define TRADE_CODE_OPPOSITE		4

class TradeRecord
{
public:

	TradeRecord(int code) { nCode = code; }

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
	int		nCode;				//	成交结果
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
	int nLossStop;	//	止损价
	int nProfitStop;//	止盈价
	int nTimeStop;	//	时间止损
	int nTrigger;	//	触发价
};

class TradeParam
{
public:

	int			nFee;
	int			nMarginRate;				//	保证金率
	int			nDefaultSlots;
	int			nUnitsPerSlot;

	int			nMaxLossStop;				//	最大止损
	int			nMaxProfitStop;			//	最大止盈
	int			nTimeStop;				//	时间止损
	int			nMaxOpenTimes;			//	最大开仓次数
	int			nMaxLossPerDay;			//	每日最大允许亏损额
};	

class TradeFacility
{
public:

	TradeFacility(void);
	~TradeFacility(void);

	void ResetStats()
	{
		m_nIntialBalance = m_nBalance;
		m_nTotalFee = m_nTotalProfit = 0;
		m_nOpenTimes = 0;
	}

	//	设置账户余额	
	void SetBalance(int nMoney) { m_nBalance = nMoney; }

	//	设置当前分笔数据，并且更新浮动盈亏等
	void SetTick(Tick tick);

	//	开仓
	TradeRecord Buy(int nSlot = 0, int nLossStop = 0);
	TradeRecord Sell(int nSlot = 0, int nLossStop = 0);

	//	平仓
	TradeRecord Close();

	//	反手
	TradeRecordCollection Reverse();

	//	交易记录
	TradeRecord Log(int nTime, bool bBuy, bool bOpen, 
					int nPrice, int nSlot, int nFee, int nProfit);

	void SetLogFile(string path) { m_strLogPath = path; }

public:

	Tick		m_nTick;
	Position	m_nPosition;
	string		m_nFilePath;

	string		m_strLogPath;				//	日志路径

	int			m_nIntialBalance;			//	当日初始余额
	int			m_nOpenTimes;				//	当日开仓次数
	float		m_nMargin;					//	当前保证金
	int			m_nBalance;					//	当前账户余额
	int			m_nTotalFee;				//	当日手续费总额
	int			m_nTotalProfit;				//	当日毛利润总额

};

extern TradeParam gTradeParam;
#define TP gTradeParam	