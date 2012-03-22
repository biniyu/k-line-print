// SearchContractDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "SearchContractDialog.h"
#include "MainFrm.h"

// CSearchContractDialog �Ի���

IMPLEMENT_DYNAMIC(CSearchContractDialog, CDialog)

CSearchContractDialog::CSearchContractDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchContractDialog::IDD, pParent)
{

}

CSearchContractDialog::~CSearchContractDialog()
{
}

void CSearchContractDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VAR_LIST, m_ComboVarList);
}


BEGIN_MESSAGE_MAP(CSearchContractDialog, CDialog)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CSearchContractDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSearchContractDialog ��Ϣ�������


void CSearchContractDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	m_ComboVarList.AddString(CString("SR"));
	m_ComboVarList.AddString(CString("TA"));
	m_ComboVarList.AddString(CString("RU"));
	m_ComboVarList.AddString(CString("CU"));
	m_ComboVarList.AddString(CString("CF"));
}

void CSearchContractDialog::OnBnClickedOk()
{
	// ��������������Լ�����µ�Combo��



	OnOK();
}
