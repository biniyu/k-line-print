#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTradeLogDialog �Ի���

class CTradeLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeLogDialog)

public:
	CTradeLogDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeLogDialog();

// �Ի�������
	enum { IDD = IDD_TRADE_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CListBox m_ctlListDate;
	CListBox m_ctlListFile;
	CListCtrl m_ctlListLog;

	vector<TradeRecord> m_trs;
};
