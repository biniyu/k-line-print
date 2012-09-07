
// KLinePrintDoc.h : CKLinePrintDoc 类的接口
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
protected: // 仅从序列化创建
	CKLinePrintDoc();
	DECLARE_DYNCREATE(CKLinePrintDoc)

private:

	int	m_nCurrentTickIdx;		//	当前tick的索引
	int m_nCurrentTickTime;		//	当前时间

	TickCollection			m_TickData;
	KLineCollection			m_15SecData;
	KLineCollection			m_1MinData;
	KLineCollection			m_DayData;

	Strategy				m_Strategy;

	//	当前交易记录
	TradeRecordCollection	m_TradeRecords;

	KLineReader				m_KLineReader;
	TickReader				m_TickReader;

	Calendar				m_FilteredCalendar;		// 根据回放选项过滤出来的符合条件的日期

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CKLinePrintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	string m_CurCsvFile;
	string m_CurDayFile;

	//	选中的交易记录
	void SetSelTradeIdx(int nSelIdx) { m_TradeRecords.SetSelIdx(nSelIdx); }

	//	添加交易记录
	void AppendTradeRecord(TradeRecord tr) 
	{
		switch(tr.nCode)
		{
		case TRADE_CODE_SUCCESS:
			m_TradeRecords.push_back(tr); 
			break;
		case TRADE_CODE_NO_MONEY:
			AfxMessageBox(_T("保证金不足"));
			break;
		case TRADE_CODE_MAX_OPEN:
			AfxMessageBox(_T("达到日内最大开仓次数"));
			break;
		case TRADE_CODE_MAX_LOSS:
			AfxMessageBox(_T("达到日内最大亏损额度"));
			break;
		case TRADE_CODE_OPPOSITE:
			AfxMessageBox(_T("反向开仓请先平仓"));
			break;
		default:
			break;
		}
	}

	//	清除交易记录
	void ClearTradeRecord() { m_TradeRecords.clear(); }

	//	设置交易记录
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

	//  加载下一日数据准备播放
	BOOL LoadNextDay();

	/* 显示至某个时点 */

	/* nTillTime		:	-1是播放所有，0是播放首tick，其他是播放至目标时间 */
	/* nTillDate(日线)	:	-1是播放所有日期，其他是播放至目标日期 */
	/* bTest : 策略测试 */
	void DisplayTill(int nTillMilliTime = -1, int nTillDate = -1, bool bTest = false);

	/* 播放至某个时点 */
	void PlayTillTime(int nTillMilliTime, bool bTest = false);

	//	获取当前价格(0为当前tick)
	Tick GetTick(int nOffset = 0);

	//  获取当前时间
	int GetCurTickTime()
	{
		return m_nCurrentTickTime;
	}

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnGenDayline();
	afx_msg void OnStrategy();
};


