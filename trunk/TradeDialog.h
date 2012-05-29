#pragma once
#include "TickReader.h"
#include "TradeFacility.h"
#include "afxcmn.h"

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
	void UpdateAccountInfo(void);

public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonReverse();

	UINT m_nFee;
	UINT m_nMargin;
	UINT m_nUnitsPerSlot;
	UINT m_nSlots;
	afx_msg void OnBnClickedButtonUpdateParam();
};
