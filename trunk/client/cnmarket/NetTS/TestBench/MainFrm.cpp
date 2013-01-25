// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TestBench.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CONNECT, OnViewConnect)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONNECT, OnUpdateViewConnect)
	ON_COMMAND(ID_VIEW_DISCONNECT, OnViewDisconnect)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISCONNECT, OnUpdateViewDisconnect)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_STKRECEIVER_TSDATA,OnStkReceiverTsdata)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	Stock_Init( GetSafeHwnd(), WM_APP_STKRECEIVER_TSDATA, 0 );

	GetTotalNumber();
	GetStockByNoEx( 0, NULL );
	GetStockByCodeEx( NULL, 0, NULL );
	SetupReceiver( FALSE );
	GetStockDrvInfo( 0, NULL );

	SetTimer( 1, 2000, NULL );
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if( 1 == nIDEvent )
	{
		BOOL bWork = IsEngineWorking( );
		if( bWork )
		{
			STOCK_STRUCTEx stock;
			stock.m_type = 0x11;
			strncpy( stock.m_code, "600028", sizeof(stock.m_code) );

			RequestStockData( RCV_REPORT, &stock, 1, 0, 0 );
			RequestStockData( FILE_MINUTE_EX, &stock, 1, 0, 0 );
			RequestStockData( FILE_HISTORY_EX, &stock, 1, ktypeDay, 0x150 );
			RequestStockData( FILE_MULTISORT_EX, &stock, 1, 0, 0 );
			RequestStockData( FILE_DETAIL_EX, &stock, 1, 0, 0 );
			RequestStockData( FILE_BASE_EX, &stock, 1, 0, 0 );

			KillTimer(1);
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnClose() 
{
	Stock_Quit( GetSafeHwnd() );

	CFrameWnd::OnClose();
}

void CMainFrame::OnViewConnect() 
{
	Stock_Init( GetSafeHwnd(), WM_APP_STKRECEIVER_TSDATA, 0 );
}

void CMainFrame::OnUpdateViewConnect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !IsEngineWorking() );
}

void CMainFrame::OnViewDisconnect() 
{
	Stock_Quit( GetSafeHwnd() );
}

void CMainFrame::OnUpdateViewDisconnect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( IsEngineWorking() );
}

// ͨ�Ӵ�������
LRESULT CMainFrame::OnStkReceiverTsdata(
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
			
			// �޸�AfxGetStockContainer()
			int nBufSize = pHeader->m_pReport[0].m_cbSize;
			PBYTE pBaseBuf = (PBYTE)&pHeader->m_pReport[0];
			for(i=0; i<pHeader->m_nPacketNum; i++)
			{
				PRCV_REPORT_STRUCTEx pReport = (PRCV_REPORT_STRUCTEx)(pBaseBuf + nBufSize*i );
			}
		}
		break;
	
	case RCV_FILEDATA:
		switch(pHeader->m_wDataType)
		{
		case FILE_MULTISORT_EX:		// �ۺ�����
			break;
		case FILE_DETAIL_EX:		// �ɽ���ϸ
			break;
		case FILE_HISTORY_EX:						// ����������
			{
				RCV_HISTORY_STRUCTEx * pDay = pHeader->m_pDay;
				int	nPacketNum	=	pHeader->m_nPacketNum;
				CString sCode;

				// ��������
				DWORD	dwCount	=	0;
				for(i=0; i<nPacketNum && pDay; i++ )
				{
					if( pDay[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
					{
						sCode		=	pDay[i].m_head.m_szLabel;
						continue;
					}
					dwCount	++;
				}
			}
			break;

		case FILE_MINUTE_EX:						// ������������
		case 83:	// ���ֱ�����
			{
				RCV_MINUTE_STRUCTEx * pMin = pHeader->m_pMinute;
				ASSERT(pMin->m_head.m_dwHeadTag == EKE_HEAD_TAG);
				CString	sCode;
				
				DWORD	dwCount	=	0;
				for(i=0; i<pHeader->m_nPacketNum; i++)
				{
					if( pMin[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
					{
						sCode		=	pMin[i].m_head.m_szLabel;
						continue;
					}
					dwCount	++;
				}
			}
			break;

		case FILE_POWER_EX:							// �����Ȩ����
			{
				RCV_POWER_STRUCTEx * pPower = pHeader->m_pPower;
				int	nPacketNum	=	pHeader->m_nPacketNum;
				CString	sCode;

				DWORD	dwCount	=	0;
				for(i=0; i<nPacketNum && pPower; i++ )
				{
					if( pPower[i].m_head.m_dwHeadTag == EKE_HEAD_TAG )
					{
						sCode		=	pPower[i].m_head.m_szLabel;
						continue;
					}
					dwCount	++;
				}
			}
			break;

		case FILE_BASE_EX:						// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
			{
				if( pHeader->m_bDISK )
				{
					CString strFileName = pHeader->m_File.m_szFileName;
				}
				else
				{
					CString strBaseText = (char*)pHeader->m_pData;
				}
			}
			break;

		case FILE_NEWS_EX:						// ������,��������m_szFileName����Ŀ¼������
			{
				if( pHeader->m_bDISK )
				{
					CString strFileName = pHeader->m_File.m_szFileName;
				}
				else
				{
					CString strBaseText = (char *)pHeader->m_pData;
				}
			}
			break;

		case FILE_HTML_EX:						// HTML�ļ�,m_szFileNameΪURL
			{
				CString strHTML = pHeader->m_File.m_szFileName;
			}
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

