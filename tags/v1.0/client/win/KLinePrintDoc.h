
// KLinePrintDoc.h : CKLinePrintDoc ��Ľӿ�
//
#include <string>
#include "KLineCollection.h"
#include "TickReader.h"
#include "KLineReader.h"
#include "PlaybackConfig.h"
#include "CalendarGenerator.h"
#include "TradeFacility.h"
#include "Strategy.h"

using namespace std;

#pragma once

class CKLinePrintDoc : public CDocument
{
protected: // �������л�����
	CKLinePrintDoc();
	DECLARE_DYNCREATE(CKLinePrintDoc)

private:

	int	m_nCurrentTickIdx;		//	��ǰtick������
	int m_nCurrentTickTime;		//	��ǰʱ��

	TickCollection			m_TickData;
	KLineCollection			m_15SecData;
	KLineCollection			m_1MinData;
	KLineCollection			m_DayData;

	Strategy				m_Strategy;

	//	��ǰ���׼�¼
	TradeRecordCollection	m_TradeRecords;

	KLineReader				m_KLineReader;
	TickReader				m_TickReader;

	Calendar				m_FilteredCalendar;		// ���ݻط�ѡ����˳����ķ�������������

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CKLinePrintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	string m_CurCsvFile;
	string m_CurDayFile;

	//	ѡ�еĽ��׼�¼
	void SetSelTradeIdx(int nSelIdx) { m_TradeRecords.SetSelIdx(nSelIdx); }

	//	��ӽ��׼�¼
	void AppendTradeRecord(TradeRecord tr) 
	{
		switch(tr.nCode)
		{
		case TRADE_CODE_SUCCESS:
			m_TradeRecords.push_back(tr); 
			break;
		case TRADE_CODE_NO_MONEY:
			AfxMessageBox(_T("��֤����"));
			break;
		case TRADE_CODE_MAX_OPEN:
			AfxMessageBox(_T("�ﵽ������󿪲ִ���"));
			break;
		case TRADE_CODE_MAX_LOSS:
			AfxMessageBox(_T("�ﵽ������������"));
			break;
		case TRADE_CODE_OPPOSITE:
			AfxMessageBox(_T("���򿪲�����ƽ��"));
			break;
		default:
			break;
		}
	}

	//	������׼�¼
	void ClearTradeRecord() { m_TradeRecords.clear(); }

	//	���ý��׼�¼
	void SetTradeRecord(TradeRecordCollection trs) { m_TradeRecords = trs; }

	TradeRecordCollection* GetTradeRecord() { return &m_TradeRecords; }

	void LoadPlaybackCalendar(PlaybackConfig pbConfig);
	BOOL ValidatePlaybackConfig(int nDate, PlaybackConfig pbConfig);

	BOOL HasPlaybackCalendar() { return (m_FilteredCalendar.size()!=0); }

	void ViewNeighborDate(BOOL bPrev);
	void ReloadByDate(int nDate);
	void LoadKLineGroup(string targetCsvFile);
	void ReloadDetailData(int second);

	KLine GenerateDayLineFromQuoteData(string path, int nDate);

	//  ������һ������׼������
	BOOL LoadNextDay();

	/* ��ʾ��ĳ��ʱ�� */

	/* nTillTime		:	-1�ǲ������У�0�ǲ�����tick�������ǲ�����Ŀ��ʱ�� */
	/* nTillDate(����)	:	-1�ǲ����������ڣ������ǲ�����Ŀ������ */
	/* bTest : ���Բ��� */
	void DisplayTill(int nTillMilliTime = -1, int nTillDate = -1, bool bTest = false);

	/* ������ĳ��ʱ�� */
	void PlayTillTime(int nTillMilliTime, bool bTest = false);

	//	��ȡ��ǰ�۸�(0Ϊ��ǰtick)
	Tick GetTick(int nOffset = 0);

	//  ��ȡ��ǰʱ��
	int GetCurTickTime()
	{
		return m_nCurrentTickTime;
	}

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnGenDayline();
	afx_msg void OnStrategy();
};


