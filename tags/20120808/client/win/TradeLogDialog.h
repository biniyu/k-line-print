#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TradeFacility.h"

// CTradeLogDialog 对话框

class CTradeLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeLogDialog)

public:
	CTradeLogDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeLogDialog();

// 对话框数据
	enum { IDD = IDD_TRADE_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CListBox m_ctlListDate;
	CListBox m_ctlListFile;
	CListCtrl m_ctlListLog;

private:
	map<int, map<string, TradeRecordCollection>> m_trs;

public:
	afx_msg void OnLbnSelchangeListDate();
	afx_msg void OnLbnSelchangeListFile();
	afx_msg void OnNMClickListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListLog(NMHDR *pNMHDR, LRESULT *pResult);
};
