#pragma once
#include "TickReader.h"
#include "TradeFacility.h"

// CTradeDialog 对话框

class CTradeDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeDialog)

public:
	CTradeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeDialog();

	void SetTick(Tick tick) { m_tf.SetTick(tick); }

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
};
