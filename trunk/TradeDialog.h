#pragma once


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
};
