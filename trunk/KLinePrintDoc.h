
// KLinePrintDoc.h : CKLinePrintDoc ��Ľӿ�
//
#include <string>
#include "KLineCollection.h"
#include "TickReader.h"
#include "KLineReader.h"
#include "PlaybackConfig.h"
#include "CalendarGenerator.h"

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

	TickCollection	m_TickData;
	KLineCollection m_15SecData;
	KLineCollection m_1MinData;
	KLineCollection m_DayData;

	KLineReader		m_KLineReader;
	TickReader		m_TickReader;

	Calendar		m_FilteredCalendar;		// ���ݻط�ѡ����˳����ķ�������������

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

	void DisplayTill(int nTillMilliTime = -1, int nTillDate = -1);

	/* ������ĳ��ʱ�� */
	void PlayTillTime(int nTillMilliTime);

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
};

