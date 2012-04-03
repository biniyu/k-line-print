#pragma once
#include "afxwin.h"


// CSearchContractDialog 对话框

class CSearchContractDialog : public CDialog
{
	DECLARE_DYNAMIC(CSearchContractDialog)

public:
	CSearchContractDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchContractDialog();

// 对话框数据
	enum { IDD = IDD_SEARCH_CONTRACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
	CComboBox m_comboResult;
	afx_msg void OnCbnSelchangeCombo1();
};
