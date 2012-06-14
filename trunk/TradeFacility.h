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
	bool	bBuy;				//	��������
	bool	bOpen;				//	��ƽ��
	int		nPrice;				//	�ɽ��۸�
	int		nSlot;				//	����
	int		nFee;				//	������
	int		nProfit;			//	ƽ��ӯ��
};

class TradeRecordCollection : public vector<TradeRecord>
{

};

class Position
{
public:
	int nSlot;		//	������ʾ����
	int nPrice;		//	���ּ۸�
	int nProfit;	//	����ӯ��
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

	//	�����˻����	
	void SetBalance(int nMoney) { m_nBalance = nMoney; }

	//	���ú�Լ����
	void SetParam(int nFee, int nMargin, int nUnitsPerSlot)
	{
		m_nFee = nFee;
		m_nMargin = nMargin;
		m_nUnitsPerSlot = nUnitsPerSlot;
	}

	//	���õ�ǰ�ֱ����ݣ����Ҹ��¸���ӯ����
	void SetTick(Tick tick);

	//	����
	void Buy(int nSlot);
	void Sell(int nSlot);

	//	ƽ��
	void Close();

	//	����
	void Reverse();

	//	���׼�¼
	void Log(int nTime, bool bBuy, bool bOpen, 
			int nPrice, int nSlot, int nFee, int nProfit);

};
