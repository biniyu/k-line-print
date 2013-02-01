// StkReceiver.cpp : implementation file
//
//		��Ʊ������մ��ڳ���
//	
//	
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stockdrv.h"		// ͨ�ӿ����ݽṹ����
#include "Tongshi.h"		// ͨ�ӿ�ʹ�ú���
#include "stkfmt.h"			// ���¿����ݽṹ����
#include "stkfilter.h"		// ���¿����ݽṹ����
#include "cppstkfilter.h"	// ���¿����ݽṹ����
#include "Yongxin.h"		// ���¿�ʹ�ú���
#include "STKDRVContainer.h"
#include "StkReceiver.h"
#include "SelectEngineDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	CNetSTKDRV			gNetTSDLL;
static	CSTKDRVContainer	gSTOCKDLL;
using	::thnht::stock::StkFilter;

CStkReceiver & AfxGetStkReceiver()
{
	static	CStkReceiver	gStkReceiver;
	return gStkReceiver;
}


/////////////////////////////////////////////////////////////////////////////
// CStkReceiver

#define	STKRECEIVER_TIMER_YONGXIN	1

HANDLE CStkReceiver::m_hEventKillProcessThread		=	NULL;
HANDLE CStkReceiver::m_hEventProcessThreadKilled	=	NULL;

CCriticalSection	g_mutexCommPacket;
CPtrArray			g_aptrCommPacket;

// ������յ���Report���ݵ��߳�
UINT StkProcessReceiveReport( LPVOID pParam )
{
	while( TRUE )
	{
		COMMPACKET * pCommPacket = NULL;
		{
			CSingleLock		lock(&g_mutexCommPacket,TRUE);
			if( g_aptrCommPacket.GetSize() > 0 )
			{
				pCommPacket = (COMMPACKET *)g_aptrCommPacket.ElementAt(0);
				g_aptrCommPacket.RemoveAt(0);
			}
		}

		if( NULL != pCommPacket && CStock::dataReport == pCommPacket->m_dwDataType )
		{
			// �޸�AfxGetStockContainer()
			for(DWORD i=0; i<pCommPacket->m_dwCount; i++)
				AfxGetStkReceiver().OnReceiveReport( &(pCommPacket->m_pReport[i]) );

			// �����������ļ�
			AfxGetDB().StoreReport( pCommPacket->m_pReport, pCommPacket->m_dwCount, FALSE );

			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			AfxGetStkReceiver().NotifyWndRcvData( CStock::dataReport, (LPARAM)pCommPacket );

			// �ͷű������ݴ洢�ռ�
			FreeCommPacket( pCommPacket );
		}

		Sleep( 1000 );

		// User wants to quit program
		if(WAIT_OBJECT_0 == WaitForSingleObject(CStkReceiver::m_hEventKillProcessThread,0) )
		{
			{
				CSingleLock		lock(&g_mutexCommPacket,TRUE);
				for( int i=0; i<g_aptrCommPacket.GetSize(); i++ )
				{
					pCommPacket = (COMMPACKET *)g_aptrCommPacket.ElementAt(i);
					FreeCommPacket( pCommPacket );
				}
				g_aptrCommPacket.RemoveAll();
			}
			SetEvent(CStkReceiver::m_hEventProcessThreadKilled);
			AfxEndThread( 0, TRUE );
			return 0;
		}
	}
}

CStkReceiver::CStkReceiver()
{
	m_bTongshiRunning	=	FALSE;
	m_bYongxinRunning	=	FALSE;
}

CStkReceiver::~CStkReceiver()
{
}

BOOL CStkReceiver::CreateReceiver( CWnd * pParentWnd )
{
	return CreateEx(0,NULL,"",WS_OVERLAPPEDWINDOW|WS_CHILDWINDOW,
		CRect(0,0,0,0),pParentWnd,0,NULL);
}

void CStkReceiver::OnReceiveReport( REPORT * pReport )
{
	CStockContainer & container = AfxGetStockContainer();
	REPORT reportLast;
	if( !UpdateStockContainerByREPORT( container, pReport, TRUE, &reportLast ) )
		return;

	CStockInfo infoCur;

	// ������
	if( container.GetStockInfo( pReport->m_szCode, &infoCur, NULL ) )
	{
		if( 0 != reportLast.m_time && reportLast.m_time < pReport->m_time
			&& pReport->m_fVolume - reportLast.m_fVolume >= 100000
			&& !infoCur.IsIndex() && !infoCur.IsBond() && !infoCur.IsFund() && !infoCur.IsBondIndex() )
		{
			REPORT	report;
			memcpy( &report, pReport, sizeof(report) );
			report.m_fVolume	=	pReport->m_fVolume - infoCur.m_reportLatest.m_fVolume;
			report.m_fAmount	=	pReport->m_fAmount - infoCur.m_reportLatest.m_fAmount;
			AfxGetDB().StoreReport( &report, 1, TRUE );
		}
	}

	// ָ�������仯
	if( 0 == strncmp(pReport->m_szCode, STKLIB_CODE_MAIN, sizeof(pReport->m_szCode) )
		|| 0 == strncmp(pReport->m_szCode, STKLIB_CODE_MAINSZN, sizeof(pReport->m_szCode) ) )
	{
		NotifyWndRcvAlarm( STKRCV_ALARM_REFRESH, 0 );
	}

	// ����Ԥ���ڴ˴�����
	if( AfxGetAlarmContainer().OnReceiveReport( &infoCur, pReport, &reportLast ) )
	{
		NotifyWndRcvAlarm( STKRCV_ALARM_WARNING, 0 );
	}
}

void CStkReceiver::AddRcvAlarmWnd( HWND hWnd )
{
	m_awndRcvAlarm.Add( hWnd );
}

BOOL CStkReceiver::RemoveRcvAlarmWnd( HWND hWnd )
{
	for( int i=0; i<m_awndRcvAlarm.GetSize(); i++ )
	{
		if( m_awndRcvAlarm.ElementAt(i) == hWnd )
		{
			m_awndRcvAlarm.RemoveAt(i,1);
			return TRUE;
		}
	}
	return FALSE;
}

void CStkReceiver::NotifyWndRcvAlarm( WPARAM wParam, LPARAM lParam )
{
	for( int i=0; i<m_awndRcvAlarm.GetSize(); i++ )
		::PostMessage( m_awndRcvAlarm[i], WM_APP_STKRECEIVER_ALARM, wParam, lParam );
}

void CStkReceiver::AddRcvDataWnd( HWND hWnd )
{
	m_awndRcvData.Add( hWnd );
}

BOOL CStkReceiver::RemoveRcvDataWnd( HWND hWnd )
{
	for( int i=0; i<m_awndRcvData.GetSize(); i++ )
	{
		if( m_awndRcvData.ElementAt(i) == hWnd )
		{
			m_awndRcvData.RemoveAt(i,1);
			return TRUE;
		}
	}
	return FALSE;
}

void CStkReceiver::NotifyWndRcvData( WPARAM wParam, LPARAM lParam )
{
	for( int i=0; i<m_awndRcvData.GetSize(); i++ )
		::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA , wParam, lParam );
}



BOOL CStkReceiver::NetEngineBeginWorking( )
{
	return SendMessage( WM_APP_STKRECEIVER_CONNECTSERVER );
}

BOOL CStkReceiver::NetEngineBeginWorking( LPCTSTR lpszAddress, UINT nPort, LPCTSTR lpszUser, LPCTSTR lpszPasswd )
{
	if( gNetTSDLL.LoadDriver( "NetTS.dll" )
		&& gNetTSDLL.Stock_Init_Nodialog(m_hWnd,WM_APP_STKRECEIVER_TSDATA,RCV_WORK_SENDMSG,
					lpszAddress,nPort,lpszUser,lpszPasswd) > 0 )
		return TRUE;
	return FALSE;
}

BOOL CStkReceiver::NetEngineEndWorking( )
{
	return SendMessage( WM_APP_STKRECEIVER_DISCONNECTSERVER );
}

BOOL CStkReceiver::NetEngineIsWorking( )
{
	return gNetTSDLL.IsEngineWorking();
}

BOOL CStkReceiver::NetEngineSetAutoReport( int bAutoReport )
{
	return gNetTSDLL.SetAutoReport( bAutoReport );
}

int CStkReceiver::RequestStockData( int nDataType/*CStock::DataType*/, CStockInfo * pStockInfo, int nSize, int nKType, int nDataCount )
{
	int nDataTypeTS = DataType_to_TSDataType( nDataType );

	if( nSize < 0 )
		return 0;

	STOCK_STRUCTEx * pStocks = new STOCK_STRUCTEx[nSize+1];
	memset( pStocks, 0, sizeof(STOCK_STRUCTEx)*(nSize+1) );
	
	for( int i=0; i<nSize; i++ )
	{
		pStocks[i].m_type = (BYTE)pStockInfo[i].GetType();
		strncpy( pStocks[i].m_code, pStockInfo[i].GetStockCode(), sizeof(pStocks[i].m_code) );
	}
	int nRet = gNetTSDLL.RequestStockData( nDataTypeTS, pStocks, nSize, nKType, nDataCount );
	delete [] pStocks;
	return nRet;
}

int	CStkReceiver::RequestKData( CStock *pstock, int period )
{
	int nDataCount = 480;

	CStockInfo & info = pstock->GetStockInfo();
	CKData & kdata = pstock->GetKData(period);

	DWORD dateLatest = info.m_datebegin;
	if( kdata.GetSize() > 0 )
		dateLatest = kdata.ElementAt(kdata.GetSize()-1).m_date;
	CSPTime	tmLatest;
	tmLatest.FromStockTime( dateLatest, CKData::IsDayOrMin(period) );
	CSPTime tmInitial = CStDatabase::GetTimeInitial( );
	if( tmLatest < tmInitial )
		tmLatest	=	tmInitial;
	CSPTime tmNow = CSPTime::GetCurrentTime();
	CSPTimeSpan span = tmNow - tmLatest;
	switch( period )
	{
	case CKData::ktypeDay:		nDataCount = span.GetDays()*5/7+1;	break;
	case CKData::ktypeWeek:		nDataCount = span.GetDays()/7+1;	break;
	case CKData::ktypeMonth:	nDataCount = span.GetDays()/30+1;	break;
	default: ;
	}
	if( nDataCount < 100 ) nDataCount = 100;
	return RequestStockData( CStock::dataK, &info, 1, period, nDataCount );
}

BOOL CStkReceiver::EngineBeginWorking( BOOL bShowSelectEngineDialog )
{
	return SendMessage( WM_APP_STKRECEIVER_INIT, !bShowSelectEngineDialog );	// ����ͨ�ӽ���ϵͳ
}

BOOL CStkReceiver::EngineSetup( )
{
	return SendMessage( WM_APP_STKRECEIVER_SETUP );
}

BOOL CStkReceiver::EngineEndWorking( )
{
	return SendMessage( WM_APP_STKRECEIVER_QUIT );
}

BOOL CStkReceiver::EngineIsWorking( )
{
	return m_bTongshiRunning || m_bYongxinRunning;
}

int CStkReceiver::RefreshStockContainer( CStockContainer & container, BOOL bAddIfNotExist )
{
	if( gNetTSDLL.IsEngineWorking() || gNetTSDLL.GetTotalNumber() > 0 )
	{
		int	nMaxNumber	=	gNetTSDLL.GetTotalNumber();
		ASSERT( nMaxNumber >= 0 );
		if( nMaxNumber <= 0 )
			return 0;

		int	nNO	=	0;
		while( TRUE )
		{
			RCV_REPORT_STRUCTEx	tsreport;
			memset(&tsreport,0,sizeof(tsreport));
			int	nRet	=	gNetTSDLL.GetStockByNoEx( nNO, &tsreport );
			if( nRet <= 0 )
				break;
			REPORT	report;
			if( convert_TSREPORT_to_REPORT( &tsreport, &report ) )
				UpdateStockContainerByREPORT( container, &report, bAddIfNotExist );

			nNO	++;
		}
		return nNO;
	}

	if( m_bTongshiRunning )
	{
		int	nMaxNumber	=	gSTOCKDLL.GetTotalNumber();
		ASSERT( nMaxNumber >= 0 );
		if( nMaxNumber <= 0 )
			return 0;

		int	nNO	=	0;
		while( TRUE )
		{
			RCV_REPORT_STRUCTEx	tsreport;
			memset(&tsreport,0,sizeof(tsreport));
			int	nRet	=	gSTOCKDLL.GetStockByNoEx( nNO, &tsreport );
			if( nRet <= 0 )
				break;
			REPORT	report;
			if( convert_TSREPORT_to_REPORT( &tsreport, &report ) )
				UpdateStockContainerByREPORT( container, &report, bAddIfNotExist );

			nNO	++;
		}
		return nNO;
	}

	return 0;
}

int CStkReceiver::GetReport( CStockInfo &info, CReport &aReport )
{
	if( m_bTongshiRunning )
	{
		int	nMarket	=	SH_MARKET_EX;
		if( info.IsShangHai() )
			nMarket	=	SH_MARKET_EX;
		else if( info.IsShenZhen() )
			nMarket	=	SZ_MARKET_EX;
		RCV_REPORT_STRUCTEx tsreport;
		REPORT	 report;
		int	nRet	=	gSTOCKDLL.GetStockByCodeEx( (char *)info.GetStockCode(), nMarket, &tsreport );
		if( nRet > 0 && 0 == strncmp(info.GetStockCode(),tsreport.m_szLabel,min(strlen(info.GetStockCode()),sizeof(tsreport.m_szLabel)))
			&& convert_TSREPORT_to_REPORT( &tsreport, &report ) )
		{
			UpdateStockInfoByREPORT( info, &report );
			return 1;
		}
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CStkReceiver, CWnd)
	//{{AFX_MSG_MAP(CStkReceiver)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_STKRECEIVER_CONNECTSERVER,OnStkReceiverConnectserver)
	ON_MESSAGE(WM_APP_STKRECEIVER_DISCONNECTSERVER,OnStkReceiverDisconnectserver)
	ON_MESSAGE(WM_APP_STKRECEIVER_INIT,OnStkReceiverInit)
	ON_MESSAGE(WM_APP_STKRECEIVER_SETUP,OnStkReceiverSetup)
	ON_MESSAGE(WM_APP_STKRECEIVER_QUIT,OnStkReceiverQuit)
	ON_MESSAGE(WM_APP_STKRECEIVER_TSDATA,OnStkReceiverTsdata)
	ON_MESSAGE(WM_APP_STKRECEIVER_YXDATA,OnStkReceiverYxdata)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStkReceiver message handlers

int CStkReceiver::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CStkReceiver::m_hEventKillProcessThread = CreateEvent(NULL,FALSE,FALSE,NULL);
	CStkReceiver::m_hEventProcessThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

	AfxBeginThread( StkProcessReceiveReport, NULL, THREAD_PRIORITY_NORMAL );

	return 0;
}

void CStkReceiver::OnDestroy()
{
	OnStkReceiverQuit( 0, 0 );
	OnStkReceiverDisconnectserver( 0, 0 );

/*
	// These codes do not work well. system will terminate the thread auto, but cause some memory leaks
	if( CStkReceiver::m_hEventKillProcessThread )
		SetEvent(CStkReceiver::m_hEventKillProcessThread);

	if(WAIT_OBJECT_0 == WaitForSingleObject(CStkReceiver::m_hEventKillProcessThread,2000) )
	{
		if( CStkReceiver::m_hEventKillProcessThread )
			CloseHandle(CStkReceiver::m_hEventKillProcessThread);
		if( CStkReceiver::m_hEventProcessThreadKilled )
			CloseHandle(CStkReceiver::m_hEventProcessThreadKilled);
		CStkReceiver::m_hEventKillProcessThread	=	NULL;
		CStkReceiver::m_hEventProcessThreadKilled	=	NULL;
	}
*/
	CWnd::OnDestroy();
}

// ���´�����Ϣ�ص�����
void __stdcall YXErrorHandler( DWORD ecode, char const* msg )
{
	AfxMessageBox( msg, MB_OK );
}

// ���°����ջص�����
BOOL __stdcall YXStkPacketHandler( COMM_PACKET const*, DWORD )
{
	AfxGetStkReceiver().PostMessage( WM_APP_STKRECEIVER_YXDATA );
	return FALSE;	// please queue the packet, we'll claim it later.
}

// �����ļ����ջص�����
BOOL __stdcall YXBeforeReceiveHandler( StkFileInfo* fileinfo )
{
	ASSERT( fileinfo && 1 == fileinfo->m_wVersion );

	CSPString	strPath;
	
	switch( fileinfo->m_wFileType )
	{
	case FILE_NORMAL:		// ͨ������
		strPath	=	AfxGetProfile().GetSelfDBNewsPath(CStock::marketUnknown);
		break;
	case FILE_SHANEWS:		// ��������
		strPath	=	AfxGetProfile().GetSelfDBNewsPath(CStock::marketSHSE);
		break;
	case FILE_SZNNEWS:		// ��������
		strPath	=	AfxGetProfile().GetSelfDBNewsPath(CStock::marketSZSE);
		break;
	case FILE_QSNEWS:		// ȯ������
		strPath	=	AfxGetProfile().GetSelfDBNewsPath(CStock::marketUnknown);
		break;
	case FILE_SHAGGMSG:		// �Ϻ�������Ϣ
		strPath	=	AfxGetProfile().GetSelfDBBasePath(CStock::marketSHSE);
		break;
	case FILE_SZNGGMSG:		// ���ڸ�����Ϣ
		strPath	=	AfxGetProfile().GetSelfDBBasePath(CStock::marketSZSE);
		break;
	case FILE_SHDAYLINE:	// �Ϻ�������ʷ����
	case FILE_SZDAYLINE:	// ����������ʷ����
		strPath	=	(LPCTSTR)AfxGetProfile().GetYongxinPath();
		break;
	default:
		ASSERT( FALSE );
	}
	
	char	*buf = static_cast<char*>( 
		StkFilter::Allocate( strPath.GetLength() + strlen(fileinfo->m_szServerPath) + 1 )
		);
	if( NULL == buf )
		return FALSE;
	strcpy( buf, strPath );
	strcat( buf, fileinfo->m_szServerPath );
	fileinfo->m_szLocalPath = buf;
	return TRUE;
}

CCriticalSection	gYXFileArrayMutex;
CStringArray		gYXFileArray;


void __stdcall YXAfterReceiveHandler( StkFileInfo const* fileinfo )
{
	CString	strPath;
	const size_t MAX_QUEUED_FILES = 1024;

	switch( fileinfo->m_wFileType )
	{
	case FILE_NORMAL:		// ͨ������
	case FILE_SHANEWS:		// ��������
	case FILE_SZNNEWS:		// ��������
	case FILE_QSNEWS:		// ȯ������
	case FILE_SHAGGMSG:		// �Ϻ�������Ϣ
	case FILE_SZNGGMSG:		// ���ڸ�����Ϣ
		break;
	case FILE_SHDAYLINE:	// �Ϻ�������ʷ����
	case FILE_SZDAYLINE:	// ����������ʷ����
		{
			char const* path = 0;
			if( fileinfo->m_szLocalPath != 0 )
				path = fileinfo->m_szLocalPath;
			else
				path = fileinfo->m_szServerPath;

			CSingleLock	lock(&gYXFileArrayMutex,TRUE);
			if( gYXFileArray.GetSize() < MAX_QUEUED_FILES )
				gYXFileArray.Add( path );
		}
		break;
	default:
		ASSERT( FALSE );
	}
}

LRESULT CStkReceiver::OnStkReceiverConnectserver( WPARAM wParam, LPARAM lParam )
{
	if( gNetTSDLL.LoadDriver( "NetTS.dll" )
		&& gNetTSDLL.Stock_Init(m_hWnd,WM_APP_STKRECEIVER_TSDATA,RCV_WORK_SENDMSG) > 0 )
		return TRUE;
	return FALSE;
}

LRESULT CStkReceiver::OnStkReceiverDisconnectserver( WPARAM wParam, LPARAM lParam )
{
	gNetTSDLL.Stock_Quit(m_hWnd);
	// gNetTSDLL.UnloadDriver( ); // ���UnloadDriver, NetTS.dll�е��̻߳����.
	return TRUE;
}

LRESULT CStkReceiver::OnStkReceiverInit( WPARAM wParam, LPARAM lParam )
{
	// ͨ�Ӽ������豸
	{
		CStringArray astrProvider, astrDriver;
		if( !wParam )
		{
			CSelectEngineDlg	dlg;
			if( IDOK != dlg.DoModal() )
				return -1L;
			dlg.GetSelectedDriver( astrProvider, astrDriver );
		}
		
		if( astrDriver.GetSize() <= 0 )
		{
			CSPStringArray	aProDefault, aDrvDefault;
			AfxGetProfile().GetRcvDriverDefault( aProDefault, aDrvDefault );
			int i;
			for( i=0; i<aProDefault.GetSize(); i++ )	astrProvider.Add(aProDefault[i]);
			for( i=0; i<aDrvDefault.GetSize(); i++ )		astrDriver.Add(aDrvDefault[i]);
		}

		if( astrDriver.GetSize() <= 0 )
		{
			// AfxRegQueryString( HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), _T("�����豸"), astrProvider, astrDriver );
			astrProvider.InsertAt(0, AfxRegQueryString( HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), "", "", _T("Provider"), "Tongshi" ) );
			astrDriver.InsertAt(0, AfxRegQueryString( HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), "", "", _T("Driver"), "Stock.dll" ) );
		}
		
		// ���ݹ�������
		gSTOCKDLL.Stock_Quit(m_hWnd);
		gSTOCKDLL.UnloadDriver();
		if( gSTOCKDLL.LoadDriver( astrDriver )
			&& gSTOCKDLL.Stock_Init(m_hWnd,WM_APP_STKRECEIVER_TSDATA,RCV_WORK_SENDMSG) > 0 )
			m_bTongshiRunning	=	TRUE;
		else if( !wParam )
			AfxMessageBox( AfxModuleLoadString(IDS_SELECTENGINE_ERRLOAD), MB_OK | MB_ICONINFORMATION );
	}
	
	// ����
	{
		if( StkFilter::PacketRegisterHandler( YXStkPacketHandler )
			&& StkFilter::FileRegisterHandler( YXBeforeReceiveHandler, YXAfterReceiveHandler ) )
		{
			StkFilter::RegisterErrorHandler( YXErrorHandler );
			m_bYongxinRunning	=	TRUE;
			SetTimer( STKRECEIVER_TIMER_YONGXIN, 10000, NULL );
		}
	}

	return TRUE;
}

LRESULT CStkReceiver::OnStkReceiverSetup( WPARAM wParam, LPARAM lParam )
{
	return gSTOCKDLL.SetupReceiver( TRUE );
}

LRESULT CStkReceiver::OnStkReceiverQuit( WPARAM wParam, LPARAM lParam )
{
	// ͨ��
	{
		gSTOCKDLL.Stock_Quit(m_hWnd);
		m_bTongshiRunning	=	FALSE;
		gSTOCKDLL.UnloadDriver( );
	}

	// ����
	{
		StkFilter::PacketRegisterHandler(0);
		StkFilter::FileRegisterHandler(0,0);
		StkFilter::RegisterErrorHandler(0);
		m_bYongxinRunning	=	FALSE;
		KillTimer( STKRECEIVER_TIMER_YONGXIN );
	}

	return TRUE;
}

// ͨ�Ӵ�������
LRESULT CStkReceiver::OnStkReceiverTsdata(
		UINT wFileType,				// �ļ�����, �μ� StockDrv.H
		LONG lPara)
{
	int	i = 0;
	PBYTE pFileBuf = NULL;
	PBYTE pBuffx = NULL;
	PGZLXBINDATA pGZLX = NULL;
	RCV_DATA *	pHeader = (RCV_DATA *) lPara;

	// ������
	ASSERT( pHeader && pHeader->m_pData );
	if( !pHeader || !pHeader->m_pData )
		return -1L;
	// ASSERT( pHeader->m_nPacketNum > 0 || pHeader->m_bDISK );
	if( pHeader->m_nPacketNum <= 0 && !pHeader->m_bDISK )
		return -1L;

	//	���ڴ����������������,���齫���ݱ���,����һ�̴߳���
	switch( wFileType )
	{
	case RCV_REPORT:
		{
			// ������
			ASSERT( pHeader->m_nPacketNum > 0 && !pHeader->m_bDISK );
			if( pHeader->m_nPacketNum <= 0 || pHeader->m_bDISK )
				return -1L;
			
			// ���䱸�����ݴ洢�ռ�
			COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataReport, pHeader->m_nPacketNum );
			if( NULL == pCommPacket )
				return -1L;

			// �޸�AfxGetStockContainer()
			int nBufSize = pHeader->m_pReport[0].m_cbSize;
			PBYTE pBaseBuf = (PBYTE)&pHeader->m_pReport[0];
			for(i=0; i<pHeader->m_nPacketNum; i++)
			{
				PRCV_REPORT_STRUCTEx pReport = (PRCV_REPORT_STRUCTEx)(pBaseBuf + nBufSize*i );

				//	��ʱ��Ϊ�Ϻ�֤ȯ������
//				pReport->m_wMarket = SH_MARKET_EX;

				if( convert_TSREPORT_to_REPORT( pReport, &(pCommPacket->m_pReport[i]) ) )
				{
					/* ���´���ת�Ƶ��½��߳���ִ�� */
					// AfxGetStkReceiver().OnReceiveReport( &(pCommPacket->m_pReport[i]) );
				}
			}
			
			{
				CSingleLock		lock(&g_mutexCommPacket,TRUE);
				g_aptrCommPacket.Add( pCommPacket );
			}

			/* ���´���ת�Ƶ��½��߳���ִ��
			// �����������ļ�
			AfxGetDB().StoreReport( pCommPacket->m_pReport, pCommPacket->m_dwCount, FALSE );

			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			for( i=0; i<m_awndRcvData.GetSize(); i++ )
				::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataReport, (LPARAM)pCommPacket );

			// �ͷű������ݴ洢�ռ�
			FreeCommPacket( pCommPacket );
			*/ 
		}
		break;
	
	case RCV_FILEDATA:
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:						// ����������
			{
				RCV_HISTORY_STRUCTEx * pDay = pHeader->m_pDay;
				int	nPacketNum	=	pHeader->m_nPacketNum;

//				CKData	kdata( CKData::ktypeDay );
				int nKtype =CKData::ktypeDay;
				if ( pHeader->m_wDataType == FILE_MIN1_EX)   //��Ҫ�ӿ�֧��
				{
					 nKtype = CKData::ktypeMin1;
				}
				else if ( pHeader->m_wDataType == FILE_MIN5_EX)   //��Ҫ�ӿ�֧��
				{
					 nKtype = CKData::ktypeMin5;
				}
                else if ( pHeader->m_wDataType == FILE_MIN15_EX)
				{
					 nKtype = CKData::ktypeMin15;
				}
                else if ( pHeader->m_wDataType == FILE_MIN30_EX)
				{
					 nKtype = CKData::ktypeMin30;
				}
                else if ( pHeader->m_wDataType == FILE_MIN60_EX)
				{
					  nKtype = CKData::ktypeMin60;
				}
                else if ( pHeader->m_wDataType ==  FILE_WEEK_EX)
				{
					  nKtype = CKData::ktypeWeek;
				}
                else if ( pHeader->m_wDataType == FILE_MONTH_EX)
				{
					  nKtype = CKData::ktypeMonth;
				}
				else
				{
                      nKtype = CKData::ktypeDay;
				}
				CKData	kdata( nKtype);

				CString	sCode;
				DWORD	dwMarket = CStock::marketUnknown;

				// ��ȡ�ļ�
				if( pHeader->m_bDISK )
				{
					pDay	=	NULL;
					CFile	file;
					if( file.Open(pHeader->m_File.m_szFileName,CFile::modeRead) )
					{
						ASSERT( file.GetLength() == pHeader->m_File.m_dwLen );
						nPacketNum	=	(int)(file.GetLength()/sizeof(RCV_HISTORY_STRUCTEx));
						if( nPacketNum > 0 )
							pDay = new RCV_HISTORY_STRUCTEx[nPacketNum];
						if( pDay )
							file.Read( pDay, sizeof(RCV_HISTORY_STRUCTEx)*nPacketNum );
						file.Close();
					}
				}

				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataK, nPacketNum );
				if( NULL == pCommPacket )
					return -1L;

				// ��������
				DWORD	dwCount	=	0;
				for(i=0; i<nPacketNum && pDay; i++ )
				{
					if( pDay[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
					{
						if( kdata.GetSize() > 0 )
						{
							kdata.AutoSetKType();
							UpdateStockContainerByKData( AfxGetStockContainer(), sCode, kdata );
							AfxGetDB().InstallKData( kdata );
							kdata.RemoveAll();
						}
						sCode		=	pDay[i].m_head.m_szLabel;
						dwMarket	=	TSMarket_to_Market( pDay[i].m_head.m_wMarket );
						continue;
					}
					convert_TSHISTORY_to_KDATA( dwMarket, sCode, &(pDay[i]), &(pCommPacket->m_pKdata[dwCount]) );
					kdata.Add( pCommPacket->m_pKdata[dwCount] );
					dwCount	++;
				}
				pCommPacket->m_dwCount	=	dwCount;
				if( pHeader->m_bDISK && pDay )
					delete [] pDay;
				if( kdata.GetSize() > 0 )
				{
					kdata.AutoSetKType();
					UpdateStockContainerByKData( AfxGetStockContainer(), sCode, kdata );
					AfxGetDB().InstallKData( kdata );
				}

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for( i=0; i<m_awndRcvData.GetSize(); i++ )
					::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataK, (LPARAM)pCommPacket );
	
				// �ͷű������ݴ洢�ռ�
				FreeCommPacket( pCommPacket );
			}
			break;

		case FILE_MINUTE_EX:						// ������������
		case 83:	// ���ֱ�����
			{
				RCV_MINUTE_STRUCTEx * pMin = pHeader->m_pMinute;
				ASSERT(pMin->m_head.m_dwHeadTag == EKE_HEAD_TAG);
				CMinute	minute;
				BOOL bAutoSaveKDataMin = AfxGetProfile().GetAutoSaveKDataMin(); // ��Ϊ����K�߱���
				CString	sCode;
				DWORD	dwMarket = CStock::marketUnknown;
				
				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataMinute, pHeader->m_nPacketNum );
				if( NULL == pCommPacket )
					return -1L;

				DWORD	dwCount	=	0;
				for(i=0; i<pHeader->m_nPacketNum; i++)
				{
					if( pMin[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
					{
						if( minute.GetSize() > 0 )
						{
							CKData	kdMin(CKData::ktypeMin5);
							if( minute.ToKData(kdMin) )
								AfxGetDB().InstallKData( kdMin );
							minute.RemoveAll();
						}
						sCode		=	pMin[i].m_head.m_szLabel;
						dwMarket	=	TSMarket_to_Market( pMin[i].m_head.m_wMarket );
						continue;
					}
					convert_TSMINUTE_to_MINUTE( dwMarket, sCode, &(pMin[i]), &(pCommPacket->m_pMinute[dwCount]) );
					if( bAutoSaveKDataMin )
						minute.Add( pCommPacket->m_pMinute[dwCount] );
					dwCount	++;
				}
				pCommPacket->m_dwCount	=	dwCount;
				if( minute.GetSize() > 0 )
				{
					CKData	kdMin(CKData::ktypeMin5);
					if( minute.ToKData(kdMin) )
						AfxGetDB().InstallKData( kdMin );
				}

				// �����������ļ�
				AfxGetDB().StoreMinute( pCommPacket->m_pMinute, pCommPacket->m_dwCount );

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for( i=0; i<m_awndRcvData.GetSize(); i++ )
					::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataMinute, (LPARAM)pCommPacket );

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket( pCommPacket );
			}
			break;

		case FILE_POWER_EX:							// �����Ȩ����
			{
				RCV_POWER_STRUCTEx * pPower = pHeader->m_pPower;
				int	nPacketNum	=	pHeader->m_nPacketNum;
				CDRData	drdata;
				CString	sCode;
				DWORD	dwMarket = CStock::marketUnknown;

				if( pHeader->m_bDISK )
				{
					pPower	=	NULL;
					CFile	file;
					if( file.Open(pHeader->m_File.m_szFileName,CFile::modeRead) )
					{
						ASSERT( file.GetLength() == pHeader->m_File.m_dwLen );
						nPacketNum	=	(int)(file.GetLength()/sizeof(RCV_POWER_STRUCTEx));
						if( nPacketNum > 0 )
							pPower = new RCV_POWER_STRUCTEx[nPacketNum];
						if( pPower )
							file.Read( pPower, sizeof(RCV_POWER_STRUCTEx)*nPacketNum );
						file.Close();
					}
				}

				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataDR, nPacketNum );
				if( NULL == pCommPacket )
					return -1L;

				DWORD	dwCount	=	0;
				for(i=0; i<nPacketNum && pPower; i++ )
				{
					if( pPower[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
					{
						if( drdata.GetSize() > 0 )
							AfxGetDB().InstallDRData( drdata );
						drdata.RemoveAll();
						sCode		=	pPower[i].m_head.m_szLabel;
						dwMarket	=	TSMarket_to_Market( pPower[i].m_head.m_wMarket );
						continue;
					}
					convert_TSPOWER_to_DRDATA( dwMarket, sCode, &(pPower[i]), &(pCommPacket->m_pDrdata[dwCount]) );
					drdata.Add( pCommPacket->m_pDrdata[dwCount] );
					dwCount	++;
				}
				if( pHeader->m_bDISK && pPower )
					delete [] pPower;
				if( drdata.GetSize() > 0 )
					AfxGetDB().InstallDRData( drdata );
				pCommPacket->m_dwCount	=	dwCount;

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for( i=0; i<m_awndRcvData.GetSize(); i++ )
					::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataDR, (LPARAM)pCommPacket );

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket( pCommPacket );
			}
			break;

		case FILE_MULTISORT_EX:					// �ۺ�����
			{
				RCV_MULTISORT_STRUCTEx * pMultisort = pHeader->m_pMultisort;
				
				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataMultisort, pHeader->m_nPacketNum );
				if( NULL == pCommPacket )
					return -1L;

				for(i=0; i<pHeader->m_nPacketNum; i++)
				{
					convert_TSMULTISORT_to_MULTISORT( &(pMultisort[i]), &(pCommPacket->m_pMultisort[i]) );
				}
				pCommPacket->m_dwCount	=	pHeader->m_nPacketNum;

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for( i=0; i<m_awndRcvData.GetSize(); i++ )
					::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataMultisort, (LPARAM)pCommPacket );

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket( pCommPacket );
			}
			break;

		case FILE_BASE_EX:						// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
			{
				if( pHeader->m_bDISK )
				{
					AfxGetDB().InstallBaseText( pHeader->m_File.m_szFileName, AfxGetFileTitle(pHeader->m_File.m_szFileName) );
				}
				else
				{
					AfxGetDB().InstallBaseText( LPCTSTR(pHeader->m_pData), pHeader->m_File.m_dwLen, AfxGetFileTitle(pHeader->m_File.m_szFileName) );
				}

				// ���䱸�����ݴ洢�ռ�
				COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataCode, 1 );
				if( NULL == pCommPacket )
					return -1L;
				CString	sTemp	=	AfxGetFileTitleNoExt( pHeader->m_File.m_szFileName);
				strncpy( pCommPacket->m_pStockcode[0].m_szCode, sTemp, min(sizeof(pCommPacket->m_pStockcode[0].m_szCode)-1,sTemp.GetLength()) );
				pCommPacket->m_pStockcode[0].m_dwMarket	=	CStock::marketUnknown;

				// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
				for( i=0; i<m_awndRcvData.GetSize(); i++ )
					::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataBasetext, (LPARAM)pCommPacket );

				// �ͷű������ݴ洢�ռ�
				FreeCommPacket( pCommPacket );
			}
			break;

		case FILE_NEWS_EX:						// ������,��������m_szFileName����Ŀ¼������
			if( pHeader->m_bDISK )
			{
				AfxGetDB().InstallNewsText( pHeader->m_File.m_szFileName, AfxGetFileTitle(pHeader->m_File.m_szFileName) );
			}
			else
			{
				AfxGetDB().InstallNewsText( LPCTSTR(pHeader->m_pData), pHeader->m_File.m_dwLen, AfxGetFileTitle(pHeader->m_File.m_szFileName) );
			}
			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			for( i=0; i<m_awndRcvData.GetSize(); i++ )
				::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataNews, NULL );
			break;

		case FILE_HTML_EX:						// HTML�ļ�,m_szFileNameΪURL
			AfxGetProfile().AddHTMLURLToExplorer( pHeader->m_File.m_szFileName );
			break;

		case FILE_SOFTWARE_EX:
			TRACE("%s\n",pHeader->m_File.m_szFileName);
			TRACE("%d\n",pHeader->m_File.m_dwLen);
			pBuffx = (PBYTE)pHeader->m_pData;
			break;

		case FILE_SHAZQDATA_EX:		//h�Ϻ���ծ���۽���
			break;
		case FILE_TYPE_RES:
			TRACE("MSG: CStkReceiver::OnStkDataOK, Replaced data \n");
			break;
		default:
			TRACE("Msg: CStkReceiver::OnStkReceiverData,Unkonw data type\n");
		}
		break;
	}

	return 0L;
}

void CStkReceiver::OnTimer(UINT nIDEvent) 
{
	if( STKRECEIVER_TIMER_YONGXIN == nIDEvent )
	{
		while( TRUE )
		{
			CString	sFileName;
			{
				CSingleLock		lock(&gYXFileArrayMutex,TRUE);
				if( gYXFileArray.GetSize() <= 0 )
					break;
				sFileName = gYXFileArray.ElementAt(gYXFileArray.GetSize()-1);
				gYXFileArray.RemoveAt(gYXFileArray.GetSize()-1);
			}
			ProcessReceivedYXFile( sFileName );
		}
	}

	CWnd::OnTimer(nIDEvent);
}

// ���´�������
LRESULT CStkReceiver::OnStkReceiverYxdata( WPARAM wParam, LPARAM lParam )
{
	char	buf[MAX_PACKET_SIZE];
	COMM_PACKET *	pPacket = reinterpret_cast<COMM_PACKET*>(buf);
	DWORD	dwSize = sizeof(buf);
	if( StkFilter::PacketGet( pPacket, &dwSize ) )
	{
		ProcessYXPacket( *pPacket, dwSize );
	}
	else if( dwSize > 0 )
	{
		// the packet is unreasonably large, throw it away:
		StkFilter::PacketGet(0,0);
	}
	return 0L;
}

BOOL CStkReceiver::ProcessYXPacket( COMM_PACKET &packet, DWORD dwSize )
{
	int	i;

	switch( packet.m_wType )
	{
	case PT_REPORT:
		{
			// ������
			ASSERT( packet.m_ReportSet.m_wReportCount > 0 );
			if( packet.m_ReportSet.m_wReportCount <= 0 )
				return -1L;
			
			// ���䱸�����ݴ洢�ռ�
			COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataReport, packet.m_ReportSet.m_wReportCount );
			if( NULL == pCommPacket )
				return -1L;

			// �޸�AfxGetStockContainer()
			for(i=0; i<packet.m_ReportSet.m_wReportCount; i++)
			{
				if( convert_YXREPORT_to_REPORT( packet.m_ReportSet.m_time, &(packet.m_ReportSet.data[i]), &(pCommPacket->m_pReport[i]) ) )
				{
					/* ���´���ת�Ƶ��½��߳���ִ�� */
					// AfxGetStkReceiver().OnReceiveReport( &(pCommPacket->m_pReport[i]) );
				}
			}

			{
				CSingleLock		lock(&g_mutexCommPacket,TRUE);
				g_aptrCommPacket.Add( pCommPacket );
			}
	
			/* ���´���ת�Ƶ��½��߳���ִ��
			// �����������ļ�
			AfxGetDB().StoreReport( pCommPacket->m_pReport, pCommPacket->m_dwCount, FALSE );

			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			for( i=0; i<m_awndRcvData.GetSize(); i++ )
				::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataReport, (LPARAM)pCommPacket );

			// �ͷű������ݴ洢�ռ�
			FreeCommPacket( pCommPacket );
			*/
		}
		break;

	case PT_MINLINE:
		{
			// ������
			ASSERT( packet.m_MinLineSet.m_wLineCount > 0 );
			if( packet.m_MinLineSet.m_wLineCount <= 0 )
				return -1L;

			CMinute	minute;
			BOOL bAutoSaveKDataMin = AfxGetProfile().GetAutoSaveKDataMin(); // ��Ϊ����K�߱���
			CString	sCode;
			
			// ���䱸�����ݴ洢�ռ�
			COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataMinute, packet.m_MinLineSet.m_wLineCount );
			if( NULL == pCommPacket )
				return -1L;

			for( i=0; i<packet.m_MinLineSet.m_wLineCount; i++)
			{
				if( minute.GetSize() > 0 && sCode != pCommPacket->m_pMinute[i].m_szCode )
				{
					CKData	kdMin(CKData::ktypeMin5);
					if( minute.ToKData(kdMin) )
						AfxGetDB().InstallKData( kdMin );
					minute.RemoveAll();
				}
				convert_YXMINLINE_to_MINUTE( &(packet.m_MinLineSet.data[i]), &(pCommPacket->m_pMinute[i]) );
				if( bAutoSaveKDataMin )
				{
					minute.Add(pCommPacket->m_pMinute[i]);
					sCode = pCommPacket->m_pMinute[i].m_szCode;
				}
			}
			if( minute.GetSize() > 0 )
			{
				CKData	kdMin(CKData::ktypeMin5);
				if( minute.ToKData(kdMin) )
					AfxGetDB().InstallKData( kdMin );
			}

			// �����������ļ�
			AfxGetDB().StoreMinute( pCommPacket->m_pMinute, pCommPacket->m_dwCount );

			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			for( i=0; i<m_awndRcvData.GetSize(); i++ )
				::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataMinute, (LPARAM)pCommPacket );

			// �ͷű������ݴ洢�ռ�
			FreeCommPacket( pCommPacket );
		}
		break;
	case PT_OUTLINE:
		{
			// ���䱸�����ݴ洢�ռ�
			COMMPACKET * pCommPacket = AllocCommPacket( CStock::dataOutline, 1 );
			if( NULL == pCommPacket )
				return -1L;

			convert_YXOUTLINE_to_OUTLINE( &(packet.m_Outline), &(pCommPacket->m_pOutline[0]) );

			// �����������ļ�
			AfxGetDB().StoreOutline( pCommPacket->m_pOutline, 1 );

			// ֪ͨ����Ʊ��Ϣ��ʾ���ڣ�ˢ����������
			for( i=0; i<m_awndRcvData.GetSize(); i++ )
				::SendMessage( m_awndRcvData[i], WM_APP_STKRECEIVER_DATA, CStock::dataOutline, (LPARAM)pCommPacket );

			// �ͷű������ݴ洢�ռ�
			FreeCommPacket( pCommPacket );
		}
		break;
	}
	return TRUE;
}

BOOL CStkReceiver::ProcessReceivedYXFile( LPCTSTR lpszFileName )
{
	CFile	file;
	if( file.Open( lpszFileName, CFile::modeRead ) )
	{
		HisCollectHeader	header;
			
		// read header
		if( sizeof(header) != file.Read(&header,sizeof(header)) )
			return FALSE;
		ASSERT( header.dwMagic == HISC_MAGIC && header.dwComplement == ~HISC_MAGIC );
		if( header.dwMagic != HISC_MAGIC && header.dwComplement != ~HISC_MAGIC )
			return FALSE;

		// read stock item
		for( DWORD i=0; i<header.dwItemDescCount; i++ )
		{
			HisItemDesc			item;
			file.Seek( header.dwItemDescOffset + i*header.dwItemDescSize, CFile::begin );
			if( sizeof(item) != file.Read(&item,sizeof(item)) )
			{
				ASSERT( FALSE );
				return FALSE;
			}

			ASSERT( item.dwItemStartPos >= header.dwItemCollectOffset );

			CKData	kdata(CKData::ktypeDay);
			for( DWORD j=0; j<item.dwItemCount; j++ )
			{
				STOCK_DAYLINE		day;
				file.Seek( item.dwItemStartPos + i*sizeof(STOCK_DAYLINE), CFile::begin );
				if( sizeof(day) != file.Read(&day,sizeof(day)) )
				{
					ASSERT( FALSE );
					return FALSE;
				}

				KDATA	kd;
				convert_DAYLINE_to_KDATA( &day, &kd );
				kdata.Add( kd );
			}
			AfxGetDB().InstallKData( kdata );
		}
		
		file.Close();
		return TRUE;
	}

	return FALSE;
}

