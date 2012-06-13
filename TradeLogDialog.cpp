// TradeLogDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeLogDialog.h"
#include "Utility.h"

// CTradeLogDialog �Ի���

IMPLEMENT_DYNAMIC(CTradeLogDialog, CDialog)

CTradeLogDialog::CTradeLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeLogDialog::IDD, pParent)
{

}

CTradeLogDialog::~CTradeLogDialog()
{
}

void CTradeLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATE, m_ctlListDate);
	DDX_Control(pDX, IDC_LIST_FILE, m_ctlListFile);
	DDX_Control(pDX, IDC_LIST_LOG, m_ctlListLog);
}


BEGIN_MESSAGE_MAP(CTradeLogDialog, CDialog)
END_MESSAGE_MAP()


// CTradeLogDialog ��Ϣ�������

BOOL CTradeLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	Utility::ReadLog(m_trs);

	//	��������ϰ����

	m_ctlListLog.InsertColumn(0, _T("ʱ��"), 0, 60);
	m_ctlListLog.InsertColumn(1, _T("��/��"), 0, 60);
	m_ctlListLog.InsertColumn(2, _T("��/ƽ"), 0, 60);
	m_ctlListLog.InsertColumn(3, _T("�۸�"), 0, 60);
	m_ctlListLog.InsertColumn(4, _T("����"), 0, 60);
	m_ctlListLog.InsertColumn(5, _T("������"), 0, 70);
	m_ctlListLog.InsertColumn(6, _T("ӯ��"), 0, 60);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
