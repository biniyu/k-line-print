/****************************************************************************************
*									WIN95 ��Ʊ��������									*
*								 �����������ݹ㲥�о�����								*
*									     1998.3.5										*
*****************************************************************************************/

/****************************************************************************************
*					��ע���� HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv ��					* 
*					Driver  = "����DLLȫ·��"											*
*					Provider= "TongShi"													*
****************************************************************************************/


#ifndef __STOCKDRV_H__
#define __STOCKDRV_H__

/////////////////////////////////////////////////////////////////////////////////////////
//  �����ǽӿڷ�ʽһ,���� DOS ��ʽ�ӿ�,ֻ�ṩ����ӿ�,
//	�������ݼ���Ϣ�ӿڰ��ӿڷ�ʽ��.

// ������ʽ���Ͷ���
#define RCV_WORK_SENDMSG			4					// �汾 2 ����ʹ�õķ�ʽ	
// ����������ʽ����

//	��Ϣ����
#define RCV_REPORT			0x3f001234
#define RCV_FILEDATA		0x3f001235
#define RCV_PANKOUDATA		0x3f001258

#define	TS_StockDrv_IdTxt	"TongShi_StockDrv_2.00"		// ͨ����Ϣ��������

//==================================== ���ݽṹ =================================================
// ֤ȯ�г�
#define		SH_MARKET_EX			'HS'		// �Ϻ�
#define		SZ_MARKET_EX			'ZS'		// ����
#define		HK_MARKET_EX			'KH'		// ���

// �ļ���������
// �ṹ������ʽ���ļ�����
#define		FILE_HISTORY_EX			2			// ����������
#define		FILE_MINUTE_EX			4			// ������������
#define		FILE_POWER_EX			6			// �����Ȩ����

#define		FILE_BASE_EX			0x1000		// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
#define		FILE_NEWS_EX			0x1002		// ������,��������m_szFileName����Ŀ¼������
#define		FILE_HTML_EX			0x1004		// HTML�ļ�,m_szFileNameΪURL

#define		FILE_SOFTWARE_EX		0x2000		// �������

#define		FILE_SHAZQDATA_EX		0x3000		// �Ϻ�ծȯ���۽���

#define		FILE_TYPE_RES			-1			// ����

// ��Ϣ������
#define		News_Sha_Ex				 2			// ��֤��Ϣ
#define		News_Szn_Ex				 4			// ��֤��Ϣ
#define		News_Fin_Ex				 6			// �ƾ�����
#define		News_TVSta_Ex			 8			// ����̨֪ͨ
#define		News_Unknown_Ex			 -1			// δ֪�ṩ��

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define		RI_IDSTRING				1			// ��������,����(LPCSTR)������
#define		RI_IDCODE				2			// ����
#define		RI_VERSION				3			// ��������汾
#define		RI_ERRRATE				4			// ȡ�ŵ�����
#define		RI_STKNUM				5			// ȡ���й�Ʊ�ܼ���
#define		RI_SUPPORTEXTHQ			6			// �Ƿ�֧��JSJ��ʽ
#define		RI_ENABLEDZHTRANS		7			// ֧�ִ�����ǻ��ļ�ϵͳ
#define		RI_ENABLETS3FILE		8			// ֧���ļ�����
#define		RI_PanKouSUPPORT		0x100


#define		STKLABEL_LEN			10			// �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define		STKNAME_LEN				32			// ��������

//#define		TIME_TYPE				time_t		// UCT ����VC6�汾 �°汾VS2005�����ϰ汾 �ѱ��8�ֽ�ʱ�� �ɸ�Ϊ __time32_t ���� ʹ����һ�д���
#define		TIME_TYPE				__time32_t	// �°汾VS2005�����ϰ汾 �ѱ��8�ֽ�ʱ�� ���ʹ���°汾VC��ע����һ�� ������һ��

#pragma pack(push)
#pragma pack(1)                                 // ��Ϊ���ֽڶ���

//////////////////////////////////////////////////////////////////////////////////
//��ծ���۽�������
typedef struct GzLxBINData {
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	DWORD	m_LiXiDate;									//��ʼ������Ϣ������
	double	m_fMoney100;								//ÿ100Ԫ����Ϣ
	DWORD	m_DayNum;;									//��Ϣ��������
	float	m_fShowPrice;								//Ʊ��۸�
}GZLXBINDATA,*PGZLXBINDATA;


//////////////////////////////////////////////////////////////////////////////////
//��������
typedef struct tagRCV_REPORT_STRUCTEx
{
	WORD	m_cbSize;									// �ṹ��С
	TIME_TYPE	m_time;									// ����ʱ��
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;								// ����
	float	m_fOpen;									// ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���
	float	m_fAmount;									// �ɽ���

	float	m_fBuyPrice[3];								// �����1,2,3
	float	m_fBuyVolume[3];							// ������1,2,3
	float	m_fSellPrice[3];							// ������1,2,3
	float	m_fSellVolume[3];							// ������1,2,3

	float	m_fBuyPrice4;								// �����4
	float	m_fBuyVolume4;								// ������4
	float	m_fSellPrice4;								// ������4
	float	m_fSellVolume4;								// ������4

	float	m_fBuyPrice5;								// �����5
	float	m_fBuyVolume5;								// ������5
	float	m_fSellPrice5;								// ������5
	float	m_fSellVolume5;								// ������5

} RCV_REPORT_STRUCTEx, *PRCV_REPORT_STRUCTEx;


typedef struct tagRCV_PANKOU_STRUCTEx
{
	TIME_TYPE	m_time;									// ����ʱ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���
	float	m_fAmount;									// �ɽ���
	int		m_lStroke;									// ����   4�ֽ�

	float	m_fBuyPrice[5];								// �����1,2,3
	float	m_fBuyVolume[5];							// ������1,2,3
	float	m_fSellPrice[5];							// ������1,2,3
	float	m_fSellVolume[5];							// ������1,2,3

	float   m_fAvgPrice;								// �ڻ������ �Թ����ڻ��г���Ч 4�ֽ�
	DWORD   m_fTickAll;									// �ɽ��ܱ���(����������) ����ֽ�Ϊ�������� 0.01 ��Ϊ��λ

} RCV_PANKOU_STRUCTEx, *PRCV_PANKOU_STRUCTEx;

typedef struct tagMy_PankouType
{
	WORD	m_wMarket;									// ��Ʊ�г�����
	WORD	m_wStkIdx;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	long	m_lDate;									// FORMAT: 20010305,0��ʾ�������������յ���ϸ���� 4�ֽ�

	float	m_fLastClose;								// ����
	float	m_fOpen;									// ��
	DWORD	m_nAllCount;								// ���̿�������                                     4�ֽ�
	DWORD	m_nCount;									// ��ǰ�����        V3.62 �汾��˫�ֽ����䵽       4�ֽ�
	WORD	R0;											// ������¼��ǰ�ֱ��ǵڼ���     V3.68 �汾
	RCV_PANKOU_STRUCTEx m_Data[0];

} My_PankouType, *PMy_PankouType;
/*
  My_PankouType=record          // ���طֱ���������  V3.62 �����޸� ��ʹ��3.62�Ժ�İ汾ʹ���µķֱʶ���
    m_wMarket:WORD;             // ��Ʊ�г����͡���������������ͬͨ�ӹ淶����       2�ֽ�
    m_wStkIdx:WORD;             // ��Ʊ�ڸ��г����������      V3.62 �汾�޸��ֶ�   2�ֽ�
    m_szLabel:StockNumType;     // ��Ʊ����,��'\0'��β,�� "600100"  ͬͨ�ӹ淶����  10�ֽ�
    m_lDate:longint;            // FORMAT: 20010305,0��ʾ�������������յ���ϸ���� 4�ֽ�
    m_fLastClose:single;        // ����                                             4�ֽ� 
    m_fOpen:single;             // ��                                             4�ֽ� 
    m_nAllCount:DWORD;          // ���̿�������                                     4�ֽ�
    m_nCount:DWORD;             // ��ǰ�����        V3.62 �汾��˫�ֽ����䵽       4�ֽ� m_nCount<=m_nAllCount һ��Ʒ�ֵķֱʿ���ͨ����������� ����ֱ��������ܴ�Ļ� ��ҪӦ�ò��Լ��ϲ���Ŀǰÿ���ֱʰ�������6000�ʣ���ǰ�ǵڼ���������һ���ֶα�ʶ��
    R0:word;                    // ������¼��ǰ�ֱ��ǵڼ���     V3.68 �汾                     2�ֽ� ����ͷ��һ�� 36 �ֽ� ���������峤��Ϊ 116*m_nCount
    m_Data:array[0..0] of RCV_PANKOU_STRUCTEx; //    V3.62 �汾ÿ������8�ֽ� �û������ĳ���Ϊ m_nCount*sizeof(RCV_PANKOU_STRUCTEx)=m_nCount*116 �û�������ÿ�ʶ�������� RCV_PANKOU_STRUCTEx
  end;

*/

//////////////////////////////////////////////////////////////////////////////////
//��������ͷ
//����ͷ m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG	0xffffffff

typedef struct	tagRCV_EKE_HEADEx
{
	DWORD	m_dwHeadTag;								// = EKE_HEAD_TAG
	WORD	m_wMarket;									// �г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����
}RCV_EKE_HEADEx,*PRCV_EKE_HEADEx;

//������������
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		TIME_TYPE	m_time;				//UCT
		float	m_fOpen;			//����
		float	m_fHigh;			//���
		float	m_fLow;				//���
		float	m_fClose;			//����
		float	m_fVolume;			//��
		float	m_fAmount;			//��
		WORD	m_wAdvance;			//����,��������Ч
		WORD	m_wDecline;			//����,��������Ч
	};
	RCV_EKE_HEADEx	m_head;
}RCV_HISTORY_STRUCTEx,*PRCV_HISTORY_STRUCTEx;

//�����ʱ������
typedef union tagRCV_MINUTE_STRUCTEx
{
	struct{
		TIME_TYPE	m_time;				// UCT
		float	m_fPrice;
		float	m_fVolume;
		float	m_fAmount;
	};
	RCV_EKE_HEADEx	m_head; 
}RCV_MINUTE_STRUCTEx,*PRCV_MINUTE_STRUCTEx;

//�����Ȩ����
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		TIME_TYPE	m_time;				// UCT
		float	m_fGive;			// ÿ����
		float	m_fPei;				// ÿ����
		float	m_fPeiPrice;		// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
		float	m_fProfit;			// ÿ�ɺ���
	};
	RCV_EKE_HEADEx	m_head;
}RCV_POWER_STRUCTEx,*PRCV_POWER_STRUCTEx;

typedef struct tagRCV_SHAGZ_STRUCTEx
{
	WORD	m_wMarket;									// �г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����
	DWORD	m_StartDate;								//��ʼ������Ϣ������ YYMMDD format
	double	m_fLiXi100RMB;								//Ʊ��100Ա��
	DWORD	m_DayNum;
	float	m_fPrice;
}RCV_SHAGZ,*PRCV_SHAGZ;
//////////////////////////////////////////////////////////////////////////////////
// �ļ��������ݰ�ͷ
//  עһ:
//	m_wDataType == FILE_BASE_EX
//				m_dwAttrib = ��Ʊ֤ȯ�г�,m_szFileName�������ļ���
//				m_FileTime = �����������ļ���������
//  m_wDataType == FILE_NEWS_EX
//				m_dwAttrib = ��Ϣ��Դ,m_szFileName ����Ŀ¼���ļ���,Ŀ¼��Ϊ��Ϣ��Դ
//				m_dwSerialNo = ���к�
//				��:  "�Ͻ�����Ϣ\\0501Z012.TXT","������\\XLD0001.TXT"
//  m_wDataType == FILE_HTML_EX
//				m_dwAttrib ����, m_szFileNameΪURL
//	m_wDataType	== FILE_SOFTWARE_EX
//				m_dwAttrib �����������, ���ڳ����ж�
//				m_szFileName ������� ID �����ַ��� + '\\' + �ļ���
//				�� "TongShi\\TS\\RECEIVE\\RECEIVE.EXE",
//				ID �����ַ���Ϊ "TongShi", "TS\\RECEIVE\\RECEIVE.EXE" Ϊ�ļ���
//				�����ַ��� �� �ļ������京���ɷ�������̶���
//  ע��:
//		�����ļ�ѭ������,ÿ���ļ���Ψһ�����к�,�Ա����ظ�����
typedef struct tagRCV_FILE_HEADEx
{
	DWORD	m_dwAttrib;							// �ļ�������
	DWORD	m_dwLen;							// �ļ�����
	union 
	{
		DWORD	m_dwSerialNo;					// ���к�,�Թ���
		TIME_TYPE	m_FileTime;						// �ļ�����ʱ��
	};
	char	m_szFileName[MAX_PATH];				// �ļ��� or URL
} RCV_FILE_HEADEx,*PRCV_FILE_HEADEx;

//////////////////////////////////////////////////////////////////////////////////
//  ����֪ͨ��Ϣ
//��ѯ��ʽ֪ͨ��Ϣ
//	wParam = MsgPara_StkData			�й�Ʊ����
//	wParam = MsgPara_File				���ļ������ļ�
//ֱ����������֪ͨ��Ϣ
//	wParam = RCV_WPARAM;
//  lParam ָ�� RCV_DATA�ṹ;
//	���� 1 �Ѿ�����, 0 δ������ܴ���,Ŀǰ�÷���ֵ������

// עһ:
//	  ��¼����ʾ�������ݺͲ�������(���� Header)�����ݰ���,���ļ���������, = 1
// ע��:
//	  �� m_bDISK = FALSE, m_pData Ϊ���ݻ�����ָ��
//		 ******** ���ݹ���,�����޸����� **********
//		 m_bDISK = TRUE,  m_pData Ϊ���ļ��Ĵ����ļ���,һ�����ֻ��
//		 ��������ȴ��ļ��ô��̷�ʽ
typedef struct tagRCV_DATA
{
	int					m_wDataType;			// �ļ�����
	int					m_nPacketNum;			// ��¼��,�μ�עһ
	RCV_FILE_HEADEx		m_File;					// �ļ��ӿ�
	BOOL				m_bDISK;				// �ļ��Ƿ��Ѵ��̵��ļ�
	union
	{
		RCV_REPORT_STRUCTEx  *	m_pReport;
		RCV_HISTORY_STRUCTEx *	m_pDay;
		RCV_MINUTE_STRUCTEx  *	m_pMinute;
		RCV_POWER_STRUCTEx	 *	m_pPower;
		void				 *	m_pData;		// �μ�ע��
	};
} RCV_DATA,*PRCV_DATA;

#pragma pack(pop)

/* ��Ϣ������� DEMO
LONG OnStkDataOK(UINT wParam,LONG lParam)
{
union tagSrcStock	DOS_StkBuf;
RCV_REPORT_STRUCTEx NEW_StkBuf;
PBYTE				pDataBuf;
RCV_DATA			Header;
RCV_DATA	*		pHeader;
DWORD				dwFileLen;
int					ok;
	pHeader = (RCV_DATA *)lParam;
	switch( wParam )
	{
	case RCV_REPORT:						// �����������÷�ʽ,��Ʊ����
		for(i=0; i<pHeader->m_nPacketNum; i++)
		{
			pHeader->m_pReport[i] ...
			// ���ݴ���
		}
		break;

	case RCV_FILEDATA:						// �����������÷�ʽ,�ļ�
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:				// ����������
			break;
		case FILE_MINUTE_EX:				// ������������
			break;
		case FILE_POWER_EX:					// �����Ȩ����
			break;
		case FILE_BASE_EX:					// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
			break;
		case FILE_NEWS_EX:					// ������,��������m_szFileName����Ŀ¼������
			break;
		case FILE_HTML_EX:					// HTML�ļ�,m_szFileNameΪURL
			break;
		case FILE_SOFTWARE_EX:				// �������
			break;
		}
		break;
	default:
		return 0;							// unknown data
	}
	return 1;
}
*/


//////////////////////////////////////////////////////////////////////////////////
//APIs
#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////////////
// ע�ắ��

// ��Ʊ��ʼ��
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��
//		Msg				�û��Զ�����Ϣ
//		nWorkMode		�ӿڹ�����ʽ, Ӧ���� RCV_WORK_SENDMSG
// ���ز���:
//		 1				�ɹ�	
//		-1				ʧ��
int WINAPI	Stock_Init(HWND hWnd,UINT Msg,int nWorkMode);

// �˳�,ֹͣ������Ϣ
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��,ͬ Stock_Init �ĵ�����ڲ���
//	���ز���:
//		 1				�ɹ�	
//		-1				ʧ��
int WINAPI 	Stock_Quit(HWND hWnd);

//////////////////////////////////////////////////////////////////////////////////
// ����ӿ�       
      
// ȡ�ѽ��յ��Ĺ�Ʊ����
int WINAPI 	GetTotalNumber();													

// �����ȡ��Ʊ����(��չ)
// ��ڲ���:	
//			nNo			���
//			pBuf		��Ź�Ʊ���ݵĻ�����
// ���ز���:
//		    NoStockData	�޹�Ʊ����	
// ע:
//			�ú����ṩ��Ʊ���ݵ���Ҫ����;�������������ʱ,���Կ��ٽ������ݿ��
int WINAPI 	GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf);			

// �ɹɺ�ȡ��Ʊ����(��չ)
// ��ڲ���:	
//			pszStockCode��Ʊ����	
//			nMarket		֤ȯ�г�
//			pBuf		��Ź�Ʊ���ݵĻ�����
// ���ز���:
//		    NoStockData	�޹�Ʊ����	
// ע:
//			�ú����ṩ��Ʊ���ݵ���Ҫ����;�������������ʱ,���Կ��ٽ������ݿ��
int WINAPI 	GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf);

// ������ճ���,��������
// ��ڲ���:
//			bShowWindow		TRUE		��ʾ����,��������
//							FALSE		��������
// ���ز���:
//			 1				�ɹ�
//			-1				ʧ��
int	WINAPI  SetupReceiver(BOOL bShowWindow);

//	ȡ�ù�Ʊ������Ϣ
//	��ڲ���:
//			nInfo		����
//			pBuf		������
//	���ڲ���:
//			nInfo == RI_IDSTRING,	���������ַ�������, pBuf Ϊ�����ַ���
//									��:	"TongShi_StockDrv_1.00"
//			nInfo == RI_IDCODE,		������Ϣ�� ID ��, pBuf Ϊ�ַ�����ʽ�� ID ��
//									��:	0x78001234	  "78001234"
//			nInfo == RI_VERSION,	������Ϣ���汾��, pBuf Ϊ�ַ����汾
//									��:  1.00		  "1.00"		
//			nInfo == RI_ERRRATE,	ȡ�ŵ�����,
//			nInfo == RI_STKNUM,		ȡ���й�Ʊ�ܼ���
DWORD WINAPI  GetStockDrvInfo(int nInfo,void * pBuf);

#ifdef __cplusplus
}
#endif

#endif // __STOCKDRV_H__
