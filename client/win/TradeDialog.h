#pragma once
#include "TickReader.h"
#include "TradeFacility.h"
#include "afxcmn.h"
#include "afxwin.h"

// CTradeDialog �Ի���

class CTradeDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeDialog)

public:
	CTradeDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeDialog();

// �Ի�������
	enum { IDD = IDD_TRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBuy();
	afx_msg void OnBnClickedButtonSell();
	virtual BOOL OnInitDialog();
	CListCtrl m_AccountInfo;
	CListCtrl m_PositionInfo;
	void EnableTrade(BOOL bEnable);
	void UpdateAccountInfo(void);

public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonReverse();

	UINT m_nFee;
	UINT m_nMargin;
	UINT m_nUnitsPerSlot;
	UINT m_nSlots;
	UINT m_nDefaultSlots;
	afx_msg void OnBnClickedButtonUpdateParam();

private:

	BOOL m_bEnableTrade;

public:
	CButton m_btnBuy;
	CButton m_btnSell;
	CButton m_btnClose;
	CButton m_btnReverse;
	int m_nMaxLoss;
	int m_nMaxProfit;
	UINT m_nTimeStop;
	int m_nMaxLossPerDay;
	int m_nMaxOpenTimes;
	CEdit m_ctlMaxSlot;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
