// SearchContractDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "SearchContractDialog.h"
#include "MainFrm.h"

// CSearchContractDialog 对话框

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


// CSearchContractDialog 消息处理程序


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
	// 根据条件搜索合约，更新到Combo中



	OnOK();
}
