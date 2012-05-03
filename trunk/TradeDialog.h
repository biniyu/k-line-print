#pragma once
#include "TickReader.h"
#include "TradeFacility.h"
#include "afxcmn.h"

// CTradeDialog 对话框

class CTradeDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeDialog)

public:
	CTradeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeDialog();

	void SetTick(Tick tick);

// 对话框数据
	enum { IDD = IDD_TRADE };

private:

	TradeFacility m_tf;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBuy();
	afx_msg void OnBnClickedButtonSell();
	afx_msg void OnHdnItemdblclickListPosition(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_nSlots;
	virtual BOOL OnInitDialog();
	CListCtrl m_AccountInfo;
	CListCtrl m_PositionInfo;
private:
	void UpdateAccountInfo(void);
public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonReverse();
};
