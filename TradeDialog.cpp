// TradeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeDialog.h"


// CTradeDialog �Ի���

IMPLEMENT_DYNAMIC(CTradeDialog, CDialog)

CTradeDialog::CTradeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeDialog::IDD, pParent)
{

}

CTradeDialog::~CTradeDialog()
{
}

void CTradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTradeDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BUY, &CTradeDialog::OnBnClickedButtonBuy)
	ON_BN_CLICKED(IDC_BUTTON_SELL, &CTradeDialog::OnBnClickedButtonSell)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CTradeDialog::OnHdnItemdblclickListPosition)
END_MESSAGE_MAP()


// CTradeDialog ��Ϣ�������

void CTradeDialog::OnBnClickedButtonBuy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CTradeDialog::OnBnClickedButtonSell()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CTradeDialog::OnHdnItemdblclickListPosition(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
