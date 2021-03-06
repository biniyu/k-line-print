// PlaybackConfDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Utility.h"
#include "KLinePrint.h"
#include "PlaybackConfDialog.h"


// CPlaybackConfDialog 对话框

IMPLEMENT_DYNAMIC(CPlaybackConfDialog, CDialog)

CPlaybackConfDialog::CPlaybackConfDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaybackConfDialog::IDD, pParent)
	, m_nDateRangeOption(0)
	, m_bMonday(FALSE)
	, m_bTuesday(FALSE)
	, m_bWednesday(FALSE)
	, m_bThursday(FALSE)
	, m_bFriday(FALSE)
	, m_StartDate(0)
	, m_EndDate(0)
	, m_TimeFrom(0)
	, m_TimeTo(0)
{

}

CPlaybackConfDialog::~CPlaybackConfDialog()
{
}

void CPlaybackConfDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ALLDATE, m_nDateRangeOption);
	DDX_Check(pDX, IDC_CHECK_MONDAY, m_bMonday);
	DDX_Check(pDX, IDC_CHECK_TUESDAY, m_bTuesday);
	DDX_Check(pDX, IDC_CHECK_WEDNESDAY, m_bWednesday);
	DDX_Check(pDX, IDC_CHECK_THURSDAY, m_bThursday);
	DDX_Check(pDX, IDC_CHECK_FRIDAY, m_bFriday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_FROM, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TO, m_EndDate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_FROM, m_TimeFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_TO, m_TimeTo);
}


BEGIN_MESSAGE_MAP(CPlaybackConfDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CPlaybackConfDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CPlaybackConfDialog 消息处理程序

void CPlaybackConfDialog::OnBnClickedOk()
{
	UpdateData();

	int nLastDate = CALENDAR.GetLast();

	int year = nLastDate / 10000;
	int mon = nLastDate % 10000 / 100;
	int day = nLastDate % 10000 % 100;

	switch(m_nDateRangeOption)
	{
		case 0:
			m_pc.nStartDate = CALENDAR.GetFirst();
			m_pc.nEndDate = nLastDate;
			break;

		case 1: /* 最近一年 */
			m_pc.nStartDate = (year - 1) * 10000 + mon * 100 + day;
			m_pc.nEndDate = nLastDate;
			break;

		case 2:	/* 最近半年 */
			mon -= 6;
			if(mon <= 0) 
			{
				mon += 12;
				year -= 1;
			}
			m_pc.nStartDate = year * 10000 + mon * 100 + day;
			m_pc.nEndDate = nLastDate;
			break;

		case 3:	/* 最近三个月 */
			mon -= 3;
			if(mon <= 0) 
			{
				mon += 12;
				year -= 1;
			}
			m_pc.nStartDate = year * 10000 + mon * 100 + day;
			m_pc.nEndDate = nLastDate;
			break;

		case 4:
			m_pc.nStartDate = m_StartDate.GetYear() * 10000 + m_StartDate.GetMonth() * 100 + m_StartDate.GetDay();
			m_pc.nEndDate = m_EndDate.GetYear() * 10000 + m_EndDate.GetMonth() * 100 + m_EndDate.GetDay();
			break;
	}

	m_pc.nStartTime = m_TimeFrom.GetHour() * 3600 + m_TimeFrom.GetMinute() * 60 + m_TimeFrom.GetSecond();
	m_pc.nEndTime = m_TimeTo.GetHour() * 3600 + m_TimeTo.GetMinute() * 60 + m_TimeTo.GetSecond();
	
	m_pc.bDayOfWeek[1] = m_bMonday;
	m_pc.bDayOfWeek[2] = m_bTuesday;
	m_pc.bDayOfWeek[3] = m_bWednesday;
	m_pc.bDayOfWeek[4] = m_bThursday;
	m_pc.bDayOfWeek[5] = m_bFriday;

	//	保存到配置文件
	PBCONFIG = m_pc;
	Utility::SavePlaybackConfig(m_pc);

	OnOK();
}

BOOL CPlaybackConfDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(m_pc.bDayOfWeek[1]) 
		m_bMonday = TRUE;
	else 
		m_bMonday = FALSE;

	if(m_pc.bDayOfWeek[2]) 
		m_bTuesday = TRUE;
	else 
		m_bTuesday = FALSE;

	if(m_pc.bDayOfWeek[3]) 
		m_bWednesday = TRUE;
	else 
		m_bWednesday = FALSE;

	if(m_pc.bDayOfWeek[4]) 
		m_bThursday = TRUE;
	else 
		m_bThursday = FALSE;

	if(m_pc.bDayOfWeek[5]) 
		m_bFriday = TRUE;
	else 
		m_bFriday = FALSE;

	m_nDateRangeOption = 4;

	if(m_pc.nStartDate)
		m_StartDate = CTime(m_pc.nStartDate /10000, m_pc.nStartDate % 10000 / 100, m_pc.nStartDate % 10000 % 100, 0,0,0);
	if(m_pc.nEndDate)
		m_EndDate = CTime(m_pc.nEndDate /10000, m_pc.nEndDate % 10000 / 100, m_pc.nEndDate % 10000 % 100, 0,0,0);

	if(!m_pc.nStartTime)
		m_TimeFrom = CTime(2000,1,1,8,59,0);
	else
		m_TimeFrom = CTime(2000,1,1, 
						m_pc.nStartTime / 3600, m_pc.nStartTime % 3600 / 60, m_pc.nStartTime % 3600 % 60);

	if(!m_pc.nEndTime)
		m_TimeTo = CTime(2000,1,1,15,1,0);

	else
		m_TimeTo = CTime(2000,1,1, 
						m_pc.nEndTime / 3600, m_pc.nEndTime % 3600 / 60, m_pc.nEndTime % 3600 % 60);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
