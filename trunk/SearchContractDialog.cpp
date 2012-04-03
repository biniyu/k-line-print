// SearchContractDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "SearchContractDialog.h"
#include "MainFrm.h"
#include "KLinePrintDoc.h"
#include "VolatilityRanker.h"
#include <vector>
#include <string>

using namespace std;

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
	DDX_Control(pDX, IDC_COMBO1, m_comboResult);
}


BEGIN_MESSAGE_MAP(CSearchContractDialog, CDialog)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CSearchContractDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSearchContractDialog::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CSearchContractDialog ��Ϣ�������


void CSearchContractDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CSearchContractDialog::OnBnClickedOk()
{
	vector<string> result;

	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	VolatilityRanker vr(pDoc->m_CurCsvFile);

	result = vr.Rank();

	for(int i = 0; i < result.size(); i++)
	{
		m_comboResult.AddString(CString(result[i].c_str()));
	}
}

void CSearchContractDialog::OnCbnSelchangeCombo1()
{
	CString strCBText;
	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	int nIndex = m_comboResult.GetCurSel();	
	m_comboResult.GetLBText(nIndex, strCBText);

	char InfoString[256];    
	  
	// ת��������ݴ����InfoString������   
	if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(strCBText),-1,InfoString,100,NULL,NULL))    
	{    
		return;    
	} 

	pDoc->LoadKLineGroup(InfoString);
}
