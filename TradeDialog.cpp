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
END_MESSAGE_MAP()


// CTradeDialog ��Ϣ�������
