#pragma once


// CPlaybackConfDialog �Ի���

class CPlaybackConfDialog : public CDialog
{
	DECLARE_DYNAMIC(CPlaybackConfDialog)

public:
	CPlaybackConfDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlaybackConfDialog();

// �Ի�������
	enum { IDD = IDD_PLAYBACK_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	int m_nPlaybackOrder;
	int m_nDateRangeOption;
	BOOL m_bAllWeekDays;
	BOOL m_bMonday;
	BOOL m_bTuesday;
	BOOL m_bWednesday;
	BOOL m_bThursday;
	BOOL m_bFriday;
	CTime m_StartDate;
	CTime m_EndDate;
	BOOL m_bGap;
	BOOL m_bFluncAbove;
	BOOL m_bFluncBelow;
	int m_nGap;
	int m_nFluncAbove;
	int m_nFluncBelow;
	
	afx_msg void OnBnClickedOk();
};
