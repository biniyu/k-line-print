// PlaybackConfDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KLinePrint.h"
#include "PlaybackConfDialog.h"


// CPlaybackConfDialog 对话框

IMPLEMENT_DYNAMIC(CPlaybackConfDialog, CDialog)

CPlaybackConfDialog::CPlaybackConfDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaybackConfDialog::IDD, pParent)
{

}

CPlaybackConfDialog::~CPlaybackConfDialog()
{
}

void CPlaybackConfDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlaybackConfDialog, CDialog)
END_MESSAGE_MAP()


// CPlaybackConfDialog 消息处理程序
