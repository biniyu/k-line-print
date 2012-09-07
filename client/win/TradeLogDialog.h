#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TradeFacility.h"

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
	CComboBox m_comboLogFiles;

private:
	map<int, map<string, TradeRecordCollection>> m_trs;
	map<int, int>	m_capital;
	string			m_curfile;

public:
	afx_msg void OnLbnSelchangeListDate();
	afx_msg void OnLbnSelchangeListFile();
	afx_msg void OnNMClickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboLogfile();
	afx_msg void OnPaint();
};
