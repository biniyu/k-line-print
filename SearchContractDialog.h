#pragma once
#include "afxwin.h"


// CSearchContractDialog �Ի���

class CSearchContractDialog : public CDialog
{
	DECLARE_DYNAMIC(CSearchContractDialog)

public:
	CSearchContractDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSearchContractDialog();

// �Ի�������
	enum { IDD = IDD_SEARCH_CONTRACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
	CComboBox m_comboResult;
	afx_msg void OnCbnSelchangeCombo1();
};
