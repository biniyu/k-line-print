
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

	int	m_nCurrentTickIdx;		//	��һ��������tick������

	TickCollection	m_TickData;
	KLineCollection m_15SecData;
	KLineCollection m_1MinData;
	KLineCollection m_DayData;

	KLineReader		m_KLineReader;
	TickReader		m_TickReader;

	Calendar		m_FilteredCalendar;		// ���ݻط�ѡ����˳����ķ�������������
	PlaybackConfig	m_PlaybackConfig;		// �ط�ѡ��

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

	void SetPlaybackConfig(PlaybackConfig pc);
	PlaybackConfig GetPlaybackConfig() { return m_PlaybackConfig; }

	BOOL ValidatePlaybackConfig(int nDate);

	void ViewNeighborDate(BOOL bPrev);
	void ReloadByDate(int nDate);
	void LoadKLineGroup(string targetCsvFile);
	void ReloadDetailData(int second);

	//  ������һ������׼������
	BOOL LoadNextDay();

	/* ��ʾ��ĳ��ʱ�� */

	/* nTillTime		:	-1�ǲ������У�0�ǲ�����tick�������ǲ�����Ŀ��ʱ�� */
	/* nTillDate(����)	:	-1�ǲ����������ڣ������ǲ�����Ŀ������ */

	void DisplayTill(int nTillTime = -1, int nTillDate = -1);

	/* ������ĳ��ʱ�� */
	BOOL PlayTillTime(int nTillTime);

	// ��ȡ��ǰʱ��	
	int GetCurrentTickTime();

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};


