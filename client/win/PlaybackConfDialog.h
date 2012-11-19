#pragma once

#include "PlaybackConfig.h"

// CPlaybackConfDialog �Ի���

class CPlaybackConfDialog : public CDialog
{
	DECLARE_DYNAMIC(CPlaybackConfDialog)

public:
	CPlaybackConfDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlaybackConfDialog();

	void SetPlaybackConfig(PlaybackConfig pc) { m_pc = pc; } 
	PlaybackConfig GetPlaybackConfig() { return m_pc; }

// �Ի�������
	enum { IDD = IDD_PLAYBACK_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	PlaybackConfig m_pc;

	int m_nDateRangeOption;
	BOOL m_bMonday;
	BOOL m_bTuesday;
	BOOL m_bWednesday;
	BOOL m_bThursday;
	BOOL m_bFriday;
	CTime m_StartDate;
	CTime m_EndDate;

public:
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CTime m_TimeFrom;
	CTime m_TimeTo;
};
