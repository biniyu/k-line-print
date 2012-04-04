// SearchContractDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "CalendarGenerator.h"
#include "SearchContractDialog.h"
#include "MainFrm.h"
#include "KLinePrintDoc.h"
#include "VolatilityRanker.h"
#include <vector>
#include <string>

using namespace std;

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
	DDX_Control(pDX, IDC_COMBO1, m_comboResult);
	DDX_Control(pDX, IDC_CHECK_HIGH_LOW_RANGE, m_chkHighLowMode);
	DDX_Control(pDX, IDC_CHECK_OPEN_CLOSE_RANGE, m_chkOpenCloseMode);
	DDX_Control(pDX, IDC_CHECK_GAP_RANGE, m_chkGapMode);
}


BEGIN_MESSAGE_MAP(CSearchContractDialog, CDialog)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CSearchContractDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSearchContractDialog::OnCbnSelchangeCombo1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CSearchContractDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSearchContractDialog 消息处理程序


void CSearchContractDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CSearchContractDialog::OnBnClickedOk()
{
	vector<string> result;

	CKLinePrintDoc* pDoc = (CKLinePrintDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	VolatilityRanker vr(pDoc->m_CurCsvFile);

	if(m_chkHighLowMode.GetCheck())
	{
		vr.SetRankMode(VolatilityRanker::en_RankModeHighLow);
	}
	else if(m_chkOpenCloseMode.GetCheck())
	{
		vr.SetRankMode(VolatilityRanker::en_RankModeOpenClose);
	}
	else if(m_chkGapMode.GetCheck())
	{
		vr.SetRankMode(VolatilityRanker::en_RankModeGap);
	}
	else
	{
//		AfxMessageBox("请选择排序模式.", MB_OK);
		return;
	}

	result = vr.Rank();

	m_comboResult.Clear();

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
	  
	// 转换后的数据存放在InfoString数组中   
	if (!WideCharToMultiByte(CP_ACP,0,LPCTSTR(strCBText),-1,InfoString,100,NULL,NULL))    
	{    
		return;    
	} 

	pDoc->LoadKLineGroup(InfoString);
}

void CSearchContractDialog::OnDestroy()
{
	CDialog::OnDestroy();
	delete this;
}

void CSearchContractDialog::OnBnClickedCancel()
{
	OnCancel();
	DestroyWindow(); 
}
