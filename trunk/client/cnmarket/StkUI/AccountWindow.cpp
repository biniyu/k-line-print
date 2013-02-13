#include "StdAfx.h"
#include "StkUI.h"
#include "AccountWindow.h"
#include "Resource.h"

CAccountWindow::CAccountWindow(void)
{
}

CAccountWindow::~CAccountWindow(void)
{
}

BEGIN_MESSAGE_MAP(CAccountWindow, CGuiControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CAccountWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_TabWnd.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,0x9999))
		return -1;

	const DWORD dwStyle =LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_AccountInfo.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 1))
		return -1;

	SetDefaultListCtrlStyle(m_AccountInfo);

	m_AccountInfo.InsertColumn( 0, "�˻�", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 1, "���ս��", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 2, "����ӯ��", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 3, "ƽ��ӯ��", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 4, "��̬Ȩ��", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 5, "�����ʽ�", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 6, "�ֱֲ�֤��", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 7, "�����ʽ�", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 8, "������", LVCFMT_CENTER, 80 );
	m_AccountInfo.InsertColumn( 9, "����", LVCFMT_CENTER, 80 );

	int nRow = m_AccountInfo.InsertItem(0, "10800110");
    m_AccountInfo.SetItemText(nRow, 1, "jacky");
    m_AccountInfo.SetItemText(nRow, 2, "jacky");
    m_AccountInfo.SetItemText(nRow, 3, "jacky");
    m_AccountInfo.SetItemText(nRow, 4, "jacky");
    m_AccountInfo.SetItemText(nRow, 5, "jacky");

	if (!m_Positions.Create(dwStyle,CRect(0,0,0,0), &m_TabWnd, 2))
		return -1;

	SetDefaultListCtrlStyle(m_Positions);

	m_Positions.InsertColumn( 0, "�˻�", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 1, "��Ʒ", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 2, "�ֲܳ�", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 3, "����", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 4, "�ּ�", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 5, "����ӯ��", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 6, "��ֲ�", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 7, "�����", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 8, "��ӯ��", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 9, "������", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 10, "���ֲ�", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 11, "������", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 12, "��ӯ��", LVCFMT_CENTER, 80 );
	m_Positions.InsertColumn( 13, "������", LVCFMT_CENTER, 80 );

	if (!m_Trades.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 3))
		return -1;

	SetDefaultListCtrlStyle(m_Trades);

	m_Trades.InsertColumn( 0, "�˻�", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 1, "��ͬ��", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 2, "��Ʒ", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 3, "����", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 4, "״̬", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 5, "��ƽ��־", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 6, "����", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 7, "ί�м۸�", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 8, "ί��ʱ��", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 9, "�ɽ�����", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 10, "�ɽ��۸�", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 11, "δ������", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 12, "����", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 13, "ֹ���", LVCFMT_CENTER, 80 );
	m_Trades.InsertColumn( 14, "������", LVCFMT_CENTER, 80 );

	if (!m_Triggers.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 4))
		return -1;

	SetDefaultListCtrlStyle(m_Triggers);

	m_Triggers.InsertColumn( 0, "�˻�", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 1, "����ʱ��", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 2, "��Ʒ", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 3, "����", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 4, "״̬", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 5, "��ƽ��־", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 6, "��������", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 7, "�����۸�", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 8, "ִ�м۸�", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 9, "ֹ���", LVCFMT_CENTER, 80 );
	m_Triggers.InsertColumn( 10, "������", LVCFMT_CENTER, 80 );

	if (!m_Stops.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 5))
		return -1;

	SetDefaultListCtrlStyle(m_Stops);

	m_Stops.InsertColumn( 0, "�˻�", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 1, "��Ʒ", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 2, "����", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 3, "����", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 4, "״̬", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 5, "ֹ���", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 6, "������", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 7, "�ɽ��۸�", LVCFMT_CENTER, 80 );
	m_Stops.InsertColumn( 8, "�ɽ�ʱ��", LVCFMT_CENTER, 80 );

	if (!m_History.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabWnd, 6))
		return -1;

	SetDefaultListCtrlStyle(m_History);

	m_History.InsertColumn( 0, "ʱ��", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 1, "���ս��", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 2, "ǰ�ս��", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 3, "����ӯ��", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 4, "�����ʽ�", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 5, "������", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 6, "���ճ���", LVCFMT_CENTER, 80 );
	m_History.InsertColumn( 7, "�������", LVCFMT_CENTER, 80 );

	m_TabWnd.SetImageList(IDB_DBPROJECTS, 16,8, RGB (255, 0, 0));
	m_TabWnd.Addtab(&m_AccountInfo,"�˻�����",1);
	m_TabWnd.Addtab(&m_Positions,"�ֲ�ͳ��",2);
	m_TabWnd.Addtab(&m_Trades,"���ս���",3);
	m_TabWnd.Addtab(&m_Triggers,"������",4);
	m_TabWnd.Addtab(&m_Stops,"ֹ�������",5);
	m_TabWnd.Addtab(&m_History,"��ʷ��ϸ",6);
	
//	SetIcon(IDB_BITMAPHELP,16,5,RGB(255,0,0),3);

	return 0;
}

void CAccountWindow::SetDefaultListCtrlStyle(CListCtrl& ctlList)
{
	LONG lStyle;
	lStyle = GetWindowLong(ctlList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(ctlList.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle2 = ctlList.GetExtendedStyle();
	dwStyle2 |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle2 |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	dwStyle2 |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	ctlList.SetExtendedStyle(dwStyle2); //������չ���
}
