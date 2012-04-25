#pragma once
#include "TickReader.h"
#include "TradeFacility.h"

// CTradeDialog �Ի���

class CTradeDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeDialog)

public:
	CTradeDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeDialog();

	void SetTick(Tick tick) { m_tf.SetTick(tick); }

// �Ի�������
	enum { IDD = IDD_TRADE };

private:

	TradeFacility m_tf;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBuy();
	afx_msg void OnBnClickedButtonSell();
	afx_msg void OnHdnItemdblclickListPosition(NMHDR *pNMHDR, LRESULT *pResult);
};
