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
	bool	bBuy;				//	��������
	bool	bOpen;				//	��ƽ��
	int		nPrice;				//	�ɽ��۸�
	int		nSlot;				//	����
	int		nFee;				//	������
	int		nProfit;			//	ƽ��ӯ��
	int		nCode;				//	�ɽ����
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
	int nSlot;		//	������ʾ����
	int nTime;		//	����ʱ�䣬����ʱ��ֹ��
	int nPrice;		//	���ּ۸�
	int nProfit;	//	����ӯ��
	int nLossStop;	//	ֹ���
	int nProfitStop;//	ֹӯ��
	int nTimeStop;	//	ʱ��ֹ��
	int nTrigger;	//	������
};

class TradeParam
{
public:

	int			nFee;
	int			nMarginRate;				//	��֤����
	int			nDefaultSlots;
	int			nUnitsPerSlot;

	int			nMaxLossStop;				//	���ֹ��
	int			nMaxProfitStop;			//	���ֹӯ
	int			nTimeStop;				//	ʱ��ֹ��
	int			nMaxOpenTimes;			//	��󿪲ִ���
	int			nMaxLossPerDay;			//	ÿ�������������
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

	//	�����˻����	
	void SetBalance(int nMoney) { m_nBalance = nMoney; }

	//	���õ�ǰ�ֱ����ݣ����Ҹ��¸���ӯ����
	void SetTick(Tick tick);

	//	����
	TradeRecord Buy(int nSlot = 0, int nLossStop = 0);
	TradeRecord Sell(int nSlot = 0, int nLossStop = 0);

	//	ƽ��
	TradeRecord Close();

	//	����
	TradeRecordCollection Reverse();

	//	���׼�¼
	TradeRecord Log(int nTime, bool bBuy, bool bOpen, 
					int nPrice, int nSlot, int nFee, int nProfit);

	void SetLogFile(string path) { m_strLogPath = path; }

public:

	Tick		m_nTick;
	Position	m_nPosition;
	string		m_nFilePath;

	string		m_strLogPath;				//	��־·��

	int			m_nIntialBalance;			//	���ճ�ʼ���
	int			m_nOpenTimes;				//	���տ��ִ���
	float		m_nMargin;					//	��ǰ��֤��
	int			m_nBalance;					//	��ǰ�˻����
	int			m_nTotalFee;				//	�����������ܶ�
	int			m_nTotalProfit;				//	����ë�����ܶ�

};

extern TradeParam gTradeParam;
#define TP gTradeParam	