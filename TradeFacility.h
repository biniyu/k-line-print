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
};

class TradeFacility
{
public:

	int			m_nIntialBalance;			//	���ճ�ʼ���
	int			m_nBalance;
	int			m_nFee;
	int			m_nMargin;
	int			m_nDefaultSlots;
	int			m_nUnitsPerSlot;
	int			m_nMaxLoss;					//	���ֹ��
	int			m_nMaxProfit;				//	���ֹӯ
	int			m_nTimeStop;				//	ʱ��ֹ��

	Tick		m_nTick;
	Position	m_nPosition;
	string		m_nFilePath;

	int			m_nTotalFee;				//	�����������ܶ�
	int			m_nTotalProfit;				//	����ë�����ܶ�

public:

	TradeFacility(void);
	~TradeFacility(void);

	void ResetStats()
	{
		m_nIntialBalance = m_nBalance;
		m_nTotalFee = m_nTotalProfit = 0;
	}

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
	TradeRecord Buy(int nSlot);
	TradeRecord Sell(int nSlot);

	//	ƽ��
	TradeRecord Close();

	//	����
	TradeRecordCollection Reverse();

	//	���׼�¼
	TradeRecord Log(int nTime, bool bBuy, bool bOpen, 
					int nPrice, int nSlot, int nFee, int nProfit);

};
