// PlaybackConfDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "PlaybackConfDialog.h"


// CPlaybackConfDialog 对话框

IMPLEMENT_DYNAMIC(CPlaybackConfDialog, CDialog)

CPlaybackConfDialog::CPlaybackConfDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaybackConfDialog::IDD, pParent)
	, m_nPlaybackOrder(0)
	, m_nDateRangeOption(0)
	, m_bAllWeekDays(FALSE)
	, m_bMonday(FALSE)
	, m_bTuesday(FALSE)
	, m_bWednesday(FALSE)
	, m_bThursday(FALSE)
	, m_bFriday(FALSE)
	, m_StartDate(0)
	, m_EndDate(0)
	, m_bGap(FALSE)
	, m_bFluncAbove(FALSE)
	, m_bFluncBelow(FALSE)
	, m_nGap(0)
	, m_nFluncAbove(0)
	, m_nFluncBelow(0)
{

}

CPlaybackConfDialog::~CPlaybackConfDialog()
{
}

void CPlaybackConfDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SEQUENCE, m_nPlaybackOrder);
	DDX_Radio(pDX, IDC_RADIO_1YEAR, m_nDateRangeOption);
	DDX_Check(pDX, IDC_CHECK_ALLWEEK, m_bAllWeekDays);
	DDX_Check(pDX, IDC_CHECK_MONDAY, m_bMonday);
	DDX_Check(pDX, IDC_CHECK_TUESDAY, m_bTuesday);
	DDX_Check(pDX, IDC_CHECK_WEDNESDAY, m_bWednesday);
	DDX_Check(pDX, IDC_CHECK_THURSDAY, m_bThursday);
	DDX_Check(pDX, IDC_CHECK_FRIDAY, m_bFriday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_FROM, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TO, m_EndDate);
	DDX_Check(pDX, IDC_CHECK_GAP, m_bGap);
	DDX_Check(pDX, IDC_CHECK_FLUNC_ABOVE, m_bFluncAbove);
	DDX_Check(pDX, IDC_CHECK_FLUNC_BELOW, m_bFluncBelow);
	DDX_Text(pDX, IDC_EDIT_GAP, m_nGap);
	DDX_Text(pDX, IDC_EDIT_FLUNC_ABOVE, m_nFluncAbove);
	DDX_Text(pDX, IDC_EDIT_FLUNC_BELOW, m_nFluncBelow);
}


BEGIN_MESSAGE_MAP(CPlaybackConfDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CPlaybackConfDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CPlaybackConfDialog 消息处理程序

void CPlaybackConfDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	OnOK();
}
