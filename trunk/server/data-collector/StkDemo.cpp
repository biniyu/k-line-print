// StkDemo.cpp : implementation file
//
//		通视股票接收示范程序
//	
//	
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StkDemo.h"

#include "StockDrv.H"
#include "STKDRV.H"

#include <string>
#include <map>

using namespace std;

CSTKDRV		gSTOCKDLL;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStkDemo

CStkDemo::CStkDemo()
{
int i;
	for(i=0; i<StkBufNum; i++){
		m_Stock[i] = _T("股票: ");
	}
	m_StkPtr = 0;
	m_Min = _T("分时走势:");
	m_File = _T("文件:");
	m_bRunFlag = FALSE;
}

CStkDemo::~CStkDemo()
{
}


BEGIN_MESSAGE_MAP(CStkDemo, CWnd)
	//{{AFX_MSG_MAP(CStkDemo)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(My_Msg_StkData,OnStkDataOK)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStkDemo message handlers

void CStkDemo::OnPaint() 
{
int i;
int k;
int High;
int y;
TEXTMETRIC tm;
char * pTitle = "  股号     名  称      最新     今开     昨收     最高     最低     成交量   成交额";
	CPaintDC dc(this); // device context for painting
	dc.GetTextMetrics(&tm);
	High = tm.tmHeight + 3;
	k = m_StkPtr;
	y = 1;
	dc.TextOut(1,y,pTitle,strlen(pTitle));
	y += High;
	for(i=0; i<StkBufNum; i++)
	{
		k = k % StkBufNum;
		dc.TextOut(1,y,m_Stock[k],m_Stock[k].GetLength());
		y += High;
		k ++;
	}
	y += High;
	dc.TextOut(1,y,m_Min,m_Min.GetLength());
	y += High;
	dc.TextOut(1,y,m_File,m_File.GetLength());
}

map<string, string> codemap;

// 处理数据
LONG CStkDemo::OnStkDataOK(
		UINT wFileType,				// 文件类型, 参见 StockDrv.H
		LONG lPara)					
{
	int i, j;
	FILE* fp;

	char buf[2048];
	CString filename;
	string contractname;

	const char* header = "日期,时间,成交价,成交量,总量,属性(持仓增减),B1价,B1量,B2价,B2量,B3价,B3量,S1价,S1量,S2价,S2量,S3价,S3量,BS\n";

	PBYTE pFileBuf = NULL;
	RCV_DATA *	pHeader;
	PGZLXBINDATA pGZLX;

	pHeader = (RCV_DATA *) lPara;
	My_PankouType* pData;
	PBYTE pBuffx;

//	对于处理较慢的数据类型,建议将数据备份,另创建一线程处理

	switch( wFileType )
	{
	case RCV_REPORT:
		{
			int nBufSize = pHeader->m_pReport[0].m_cbSize;
			PBYTE pBaseBuf = (PBYTE)&pHeader->m_pReport[0];
			for(i=0; i<pHeader->m_nPacketNum; i++)
			{
				RCV_REPORT_STRUCTEx & Buf = *(PRCV_REPORT_STRUCTEx)(pBaseBuf + nBufSize*i );
				m_Stock[m_StkPtr].Format("%6s %8s N=%8.3f O=%8.3f C=%8.3f H=%8.3f L=%8.3f V=%10.0f A=%10.f",
					Buf.m_szLabel,Buf.m_szName,Buf.m_fNewPrice,Buf.m_fOpen,Buf.m_fLastClose,\
					Buf.m_fHigh,Buf.m_fLow,Buf.m_fVolume,Buf.m_fAmount);
//				m_Stock[m_StkPtr].Format("%6s %8s %8.3f %8.3f %8.3f %8.3f %8.3f %10.0f %10.f",
//					Buf.m_szLabel,Buf.m_szName,Buf.m_fNewPrice,Buf.m_fOpen,Buf.m_fLastClose,\
//					Buf.m_fHigh,Buf.m_fLow,Buf.m_fVolume,Buf.m_fAmount);


// 增加的数据获得如下
//				TRACE("PB5=%8.3f VB5=%8.2f PS5=%8.3f VS5=%8.2f\n",Buf.m_fBuyPrice[0],Buf.m_fBuyVolume[0],Buf.m_fSellPrice[0],Buf.m_fSellVolume[0]);

				TRACE("%s <-> %s\n", Buf.m_szLabel, Buf.m_szName);

				string label = Buf.m_szLabel;
				string name = Buf.m_szName;

				codemap[label] = name;

				m_StkPtr ++;
				m_StkPtr = m_StkPtr % StkBufNum;
			}
		}
		break;

	case RCV_PANKOUDATA:

		pData = (My_PankouType*)lPara;

		contractname = codemap[pData->m_szLabel];

		//	过滤掉无关的合约
		if(!isdigit((unsigned char)contractname[contractname.length() - 1])) return 0L;
		if(!isdigit((unsigned char)contractname[contractname.length() - 2])) return 0L;

		//	去掉合约的年份数字
		contractname.erase(contractname.length() - 4, 2);
		
		filename.Format("%s_%d.csv", contractname.c_str(), pData->m_lDate);

		TRACE("pankou data : market:%d index:%d label:%s date:%d   %d ticks of %d (block %d), lastclose %f, open %f , file: %s\n",
			pData->m_wMarket, pData->m_wStkIdx, 
			contractname.c_str(), pData->m_lDate, pData->m_nCount, pData->m_nAllCount, 
			pData->R0, pData->m_fLastClose, pData->m_fOpen, filename);

		if(0 == pData->R0)	//	重建文件
		{
			fp = fopen(filename, "w+");
			fputs(header, fp);
		}
		else				//	追加记录
		{
			fp = fopen(filename, "a+");
		}

		if(!fp) return 0L;

#if 1
		for(i = 0; i < pData->m_nCount; i++)
		{
			RCV_PANKOU_STRUCTEx& tick = pData->m_Data[i];

			struct tm * tt;
			tt = localtime(&tick.m_time);

			int year = pData->m_lDate / 10000;
			int month = pData->m_lDate % 10000 / 100;
			int day = pData->m_lDate % 10000 % 100;

			sprintf(buf, "%d-%02d-%02d,%02d:%02d:%02d,"
						"%.2f,%d,%d,%d, %.2f,%d,%.2f,%d,%.2f,%d, %.2f,%d,%.2f,%d,%.2f,%d,%c\n",
						year, month, day, tt->tm_hour, tt->tm_min, tt->tm_sec,
						tick.m_fNewPrice, 0, (int)tick.m_fVolume, 0,
						tick.m_fBuyPrice[0], (int)tick.m_fBuyVolume[0], 
						tick.m_fBuyPrice[1], (int)tick.m_fBuyVolume[1], 
						tick.m_fBuyPrice[2], (int)tick.m_fBuyVolume[2], 
						tick.m_fSellPrice[0], (int)tick.m_fSellVolume[0], 
						tick.m_fSellPrice[1], (int)tick.m_fSellVolume[1], 
						tick.m_fSellPrice[2], (int)tick.m_fSellVolume[2], 
						'-');
/*
			TRACE(" %02d:%02d:%02d %.2f B:%.2f(%.2f) S:%.2f(%.2f) AVG %.2f h:%.2f l:%.2f vol:%.2f\n", 
				tt->tm_hour, tt->tm_min, tt->tm_sec,
				tick.m_fNewPrice, 
				tick.m_fBuyPrice[0], tick.m_fBuyVolume[0],
				tick.m_fSellPrice[0], tick.m_fSellVolume[0],
				tick.m_fAvgPrice, tick.m_fHigh, tick.m_fLow, tick.m_fVolume);
*/
			fputs(buf, fp);
		}
#endif
		fclose(fp);
		break;
	
	case RCV_FILEDATA:
	    if( !pHeader->m_pData || pHeader->m_wDataType == FILE_TYPE_RES)
		{
			TRACE("MSG: CStkDemo::OnStkDataOK, Replaced data \n");
			break;
		}
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:						// 补日线数据
			RCV_HISTORY_STRUCTEx * pDay;
			pDay = pHeader->m_pDay;
			ASSERT(pHeader->m_pDay[0].m_head.m_dwHeadTag == EKE_HEAD_TAG);
			m_File = "日线";
			j = 0;
			for(i=0; i<pHeader->m_nPacketNum ; i++)
			{
				if( pDay[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
				{
					m_File = m_File + (const char *) pDay[i].m_head.m_szLabel + "  ";
					j ++;
				}
				if( j > 10) break;
			}
			break;

		case FILE_MINUTE_EX:						// 补分钟线数据
			RCV_MINUTE_STRUCTEx * pMin;
			pMin = pHeader->m_pMinute;
			ASSERT(pMin->m_head.m_dwHeadTag == EKE_HEAD_TAG);
			m_Min = "分时走势: ";
			for(i=0; i<pHeader->m_nPacketNum; i++)
			{
				if( pMin[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
				{
					m_Min = m_Min + (LPCSTR)pMin[i].m_head.m_szLabel + "  ";
				}
			}
			break;

		case FILE_BASE_EX:						// 钱龙兼容基本资料文件,m_szFileName仅包含文件名
			m_File.Format("基本资料: %s",pHeader->m_File.m_szFileName);
			break;

		case FILE_NEWS_EX:						// 新闻类,其类型由m_szFileName中子目录名来定
			m_File.Format("公告消息: %s",pHeader->m_File.m_szFileName);
			break;

		case FILE_SOFTWARE_EX:
			TRACE("%s\n",pHeader->m_File.m_szFileName);
			TRACE("%d\n",pHeader->m_File.m_dwLen);
			pBuffx = (PBYTE)pHeader->m_pData;
			break;

		case FILE_SHAZQDATA_EX:		//h上海国债净价交易
			pGZLX = (PGZLXBINDATA)pHeader->m_pData;
			for (i=0;i<(int)(pHeader->m_File.m_dwLen/sizeof(GZLXBINDATA));i++)
			{
				TRACE("%s\n",pGZLX[i].m_szLabel);
			}
			break;
		default:
			TRACE("Msg: CStkDemo::OnStkDataOK,Unkonw data type\n");
		}
		break;
	}
	RedrawWindow();
	return 0L;
}

int CStkDemo::MyCreate(CWnd* pWnd)
{
CRect rect(0, 0, 550,280);
HBRUSH hBrush;
	if( m_bRunFlag ) return -1;
	hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
   	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,0,hBrush);
	CreateEx(0,lpszClassName,"股票接收测试",WS_OVERLAPPEDWINDOW,
		rect,pWnd, 0, NULL);

	ShowWindow(SW_SHOWNORMAL);
	UpdateWindow();

	gSTOCKDLL.GetStockDrvInfo(RI_SUPPORTEXTHQ,NULL);	//设置 Szn Label6 Receive
	gSTOCKDLL.GetStockDrvInfo(RI_PanKouSUPPORT,NULL);

	int ok = gSTOCKDLL.Stock_Init(m_hWnd,My_Msg_StkData,RCV_WORK_SENDMSG);	// 数据共享引用

	if( ok > 0 )
	{
	//
		DWORD m_TV_Code = 201;
//		gSTOCKDLL.GetStockDrvInfo(RI_ENABLETS3FILE,&m_TV_Code);


//		char szTitle[10]; 
		CString Title;
//		Title.Format("股票接收测试-%08lX-行情字段大小:%d",gSTOCKDLL.GetStockDrvInfo(RI_IDCODE,szTitle),sizeof(RCV_REPORT_STRUCTEx));
		Title.Format("股票接收测试-行情字段大小:%d",sizeof(RCV_REPORT_STRUCTEx));
		SetWindowText(Title);
		m_bRunFlag = TRUE;
		return 1;
	}
	PostQuitMessage(-1);
	return -1;
}

void CStkDemo::OnDestroy() 
{
	CWnd::OnDestroy();
	m_bRunFlag = FALSE;
	gSTOCKDLL.Stock_Quit(m_hWnd);
}
