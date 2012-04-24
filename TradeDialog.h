#pragma once


// CTradeDialog 对话框

class CTradeDialog : public CDialog
{
	DECLARE_DYNAMIC(CTradeDialog)

public:
	CTradeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradeDialog();

// 对话框数据
	enum { IDD = IDD_TRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
