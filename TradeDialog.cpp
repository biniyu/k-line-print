// TradeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "TradeDialog.h"


// CTradeDialog 对话框

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


// CTradeDialog 消息处理程序

void CTradeDialog::OnBnClickedButtonBuy()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTradeDialog::OnBnClickedButtonSell()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTradeDialog::OnHdnItemdblclickListPosition(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
