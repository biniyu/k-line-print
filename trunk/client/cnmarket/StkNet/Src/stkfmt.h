#ifndef	thnht_hibrocast_stkfmt_h_
#define	thnht_hibrocast_stkfmt_h_

#if ! defined(__cplusplus) || defined(_MSC_VER) && _MSC_VER <= 1200

#include <time.h>

#else

#include <ctime>

using std::time_t;
using std::time;
using std::localtime;
using std::tm;

#endif

#ifdef	__cplusplus
extern "C" {
#endif

#pragma pack( push, 8 )

const unsigned MAX_PACKET_SIZE = 4096;

const unsigned short STKCODE_LEN = 10;
// const unsigned short STKNAME_LEN = 16;	��ͨ�ӿ�������ͻ
const unsigned short YX_STKNAME_LEN = 16;

const unsigned short MARKET_SH = 0;
const unsigned short MARKET_SZ = 1;
const unsigned short MARKET_STARTUP = 2;

// constants used by STOCK_MINLINE.market:
const unsigned char ML_MARKET_UNKNOWN = 0;
const unsigned char ML_MARKET_SH = (unsigned char)( MARKET_SH + 1 );
const unsigned char ML_MARKET_SZ = (unsigned char)( MARKET_SZ + 1 );
const unsigned char ML_MARKET_STARTUP = (unsigned char)( MARKET_STARTUP + 1 );

// packet type constants:

enum PT_CONST {
	PT_REPORT = 1,	// ��������
	PT_MINLINE = 2,
	PT_OUTLINE = 3,

	PT_THIS_ENUM_IS_AT_LEAST_TWO_BYTES_IN_SIZE = 0xffff
};

enum FILE_TYPE {
    FILE_NORMAL = 1,	// ͨ������
    FILE_SHANEWS = 2,	// ��������
    FILE_SZNNEWS = 3,	// ��������
    FILE_QSNEWS = 4,	// ȯ������
	FILE_SHAGGMSG = 5,	// �Ϻ�������Ϣ
	FILE_SZNGGMSG = 6,	// ���ڸ�����Ϣ
	FILE_SHDAYLINE = 7,	// �Ϻ�������ʷ����
	FILE_SZDAYLINE = 8,	// ����������ʷ����

	FILE_THIS_ENUM_IS_AT_LEAST_TWO_BYTES_IN_SIZE = 0xffff
};

typedef struct STOCK_REPORT {
	WORD	m_wMarket;					// ֤ȯ�г�����
	char	m_szCode[STKCODE_LEN];		// ֤ȯ����,��'\0'��β
	char	m_szName[YX_STKNAME_LEN];	// ֤ȯ����,��'\0'��β

	BYTE	m_btFlag;					// ͣ�Ʊ�־
	char	fill1[3];

	float	m_fLast;					// ����
	float	m_fOpen;					// ��
	float	m_fHigh;					// ���
	float	m_fLow;						// ���
	float	m_fNew;						// ����
	DWORD	m_dwVolume;					// �ɽ���
	float	m_fAmount;					// �ɽ���
	DWORD	m_dwDeltaVolume;			// �ɽ����仯
	float	m_fDeltaAmount;				// �ɽ���仯

	float	m_fBuyPrice[3];				// �����1,2,3
	DWORD	m_dwBuyVolume[3];			// ������1,2,3
	float	m_fSellPrice[3];			// ������1,2,3
	DWORD	m_dwSellVolume[3];			// ������1,2,3
} STOCK_REPORT;

typedef struct STOCK_REPORTSET {
	time_t	m_time;
	WORD	m_wReportCount;
	WORD	m_wDataSize;
	STOCK_REPORT	data[1];
} STOCK_REPORTSET;

typedef struct STOCK_MINLINE {
	BYTE	m_btType;	// 1 min, 5 min, 15 min, 30 min line

	char	m_szCode[STKCODE_LEN];
	unsigned char	market;		// see ML_MARKET_*.

	time_t	m_time;
	float	m_fHigh;
	float	m_fLow;
	float	m_fNew;
	DWORD	m_dwDeltaVolume;
	float	m_fDeltaAmount;
	DWORD	m_dwVolume;
	float	m_fAmount;
} STOCK_MINLINE;

typedef	struct STOCK_MINLINESET {
	WORD	m_wLineCount;
	WORD	m_wDataSize;
	STOCK_MINLINE	data[1];
} STOCK_MINLINESET;

typedef struct STOCK_OUTLINE {
	time_t	m_time;

	DWORD	m_dwShHq;
	DWORD	m_dwSzHq;
	DWORD	m_dwShPriceUp;
	DWORD	m_dwShPriceDown;
	DWORD	m_dwSzPriceUp;
	DWORD	m_dwSzPriceDown;
	DWORD	m_dwShWbUp;
	DWORD	m_dwShWbDown;
	DWORD	m_dwSzWbUp;
	DWORD	m_dwSzWbDown;
	DWORD	m_dwShStockCount;
	DWORD	m_dwSzStockCount;

	DWORD	reserved;
} STOCK_OUTLINE;

typedef struct COMM_PACKET {
	WORD	m_wType;		// enum PT_CONST.
	WORD	m_wReserved;
	union
	{
		STOCK_REPORTSET		m_ReportSet;
		STOCK_MINLINESET	m_MinLineSet;
		STOCK_OUTLINE		m_Outline;
	};
} COMM_PACKET;

// ��ʷ�����ļ��ṹ:

typedef struct HisItemDesc
{
	char	szCode[STKCODE_LEN];
	char	fill1[2];
	DWORD	dwItemStartPos;				// ��ֻ��ƱSTOCK_DAYLINE�ṹ��ʼ��
	DWORD	dwItemCount;				// ��ֻ��ƱSTOCK_DAYLINE�ṹ��Ŀ
} HisItemDesc;

const DWORD HISC_MAGIC = DWORD('C') + (DWORD('S') << 8) + (DWORD('I') << 16) + (DWORD('H') << 24);

typedef struct HisCollectHeader
{
	DWORD	dwMagic;			// == HISC_MAGIC
	DWORD   dwComplement;		// == ~ dwMagic

	DWORD	dwItemDescCount;
	DWORD	dwItemDescSize;
	DWORD   dwItemDescOffset;           // HisItemDesc�б�ʼ��
	DWORD   dwItemCollectOffset;        // STOCK_DAYLINE�ṹ��ʼ��
	//HisItemDesc	itemDesc[];			// ��ƫ��dwItemDescOffset��
	//STOCK_DAYLINE	item[];				// ��ƫ��dwItemCollectOffset��
} HisCollectHeader;

typedef struct STOCK_DAYLINE {
	WORD	m_wMarket;

	char	m_szCode[STKCODE_LEN];
	time_t	m_time;

	float	m_fOpen;
	float	m_fHigh;
	float	m_fLow;
	float	m_fClose;
	DWORD	m_dwVolume;
	float	m_fAmount;

	WORD	m_wAdvance;
	WORD	m_wDecline;
} STOCK_DAYLINE;


#pragma pack(pop)

#ifdef	__cplusplus
}
#endif

#endif
