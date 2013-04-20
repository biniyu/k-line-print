// strings.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include	"StdAfx.h"
#include	"Qianlong.h"
#include	"Shenglong.h"
#include	"SelfDB.h"
#include	"Strings.h"

char CQianlong::m_szDataType[]	=	"Ǯ��";
char CShenglong::m_szDataType[]	=	"ʤ��";
char CSelfDB::m_szDataType[]	=	"����Ϊ��";

//////////////////////////////////////////////////////////////////////////////////
// �ַ���
char	sz_stockname_fund[]		=	"����";
char	sz_stockname_bond[]		=	"ծȯ";
char	sz_stockname_index[]	=	"ָ��";
char	sz_stockname_bondindex[]		=	"��ծָ��";
char	sz_stockname_bondindexcorp[]	=	"��ծָ��";

//////////////////////////////////////////////////////////////////////////////////
// ָ�������ź��ַ���
char	its_sellintense[]		=	"ǿ������";
char	its_sell[]				=	"����";
char	its_sellfeeble[]		=	"΢������";
char	its_nothing[]			=	"��";
char	its_buyfeeble[]			=	"΢�����";
char	its_buy[]				=	"���";
char	its_buyintense[]		=	"ǿ�����";

char	itsc_nothing[]			=	"��";
char	itsc_deviateonbottom[]	=	"�ױ���";
char	itsc_deviateontop[]		=	"������";
char	itsc_goldenfork[]		=	"���";
char	itsc_deadfork[]			=	"����";
char	itsc_long[]				=	"��ͷǿ��";
char	itsc_short[]			=	"��ͷǿ��";
char	itsc_overbought[]		=	"����";
char	itsc_oversold[]			=	"����";
char	itsc_support[]			=	"֧��";
char	itsc_resistance[]		=	"ѹ��";

//////////////////////////////////////////////////////////////////////////////////
// ָ���ַ���

// ��������ָ��Ψһ��ţ����ؼ���ָ����������
CSPString	AfxGetSTTName( UINT nTech )
{
	static	CSPString	stt_names[STT_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );
	
	if( stt_names[0].GetLength() <= 0 )
	{
		// stt_names
		stt_names[0]			=	"STTNames";

		//	K��������
		stt_names[STT_MA]		=	("�ƶ�ƽ����");
		stt_names[STT_BBI]		=	("���ָ��");
		stt_names[STT_BOLL]		=	("���ִ�");
		stt_names[STT_PV]		=	("��֤������");
		stt_names[STT_SAR]		=	("ͣ���ת��ָ��");
		stt_names[STT_DJ]		=	("����ͼ");
		stt_names[STT_CW]		=	("����ֲ�ͼ");
		//	������
		stt_names[STT_MACD]		=	("ָ��ƽ����ͬ�ƶ�ƽ����");
		stt_names[STT_MIKE]		=	("���ָ��");
		stt_names[STT_PSY]		=	("������");
		//	������
		stt_names[STT_VOLUME]	=	("�ɽ���");
		stt_names[STT_NVI]		=	("���ɽ���ָ��");
		stt_names[STT_PVI]		=	("���ɽ���ָ��");
		stt_names[STT_VR]		=	("��������");
		stt_names[STT_VROC]		=	("�ɽ����䶯��ָ��");
		stt_names[STT_OBV]		=	("������");
		stt_names[STT_MOBV]		=	("��������");
		stt_names[STT_MFI]		=	("�ʽ�����ָ��");
		stt_names[STT_VMACD]	=	("��ָ��ƽ����ͬ�ƶ�ƽ����");
		stt_names[STT_WVAD]		=	("����������ɢ��");
		stt_names[STT_EMV]		=	("���ײ���ָ��");
		stt_names[STT_VRSI]		=	("�����ǿ��ָ��");
		stt_names[STT_NVRSI]	=	("�������ǿ��ָ��");
		stt_names[STT_AD]		=	("��/ɢָ��");
		stt_names[STT_CI]		=	("�ۻ�ָ��");
		//	�ڶ���
		stt_names[STT_KDJ]		=	("���ָ��");
		stt_names[STT_R]		=	("����ָ��");
		stt_names[STT_RSI]		=	("���ǿ��ָ��");
		stt_names[STT_BIAS]		=	("������");
		stt_names[STT_MTM]		=	("����ָ��");
		stt_names[STT_DMI]		=	("����ָ��");
		stt_names[STT_ROC]		=	("�䶯��ָ��");
		stt_names[STT_CCI]		=	("˳��ͨ��ָ��");
		stt_names[STT_CV]		=	("����ָ��");
		stt_names[STT_ARBR]		=	("������Ըָ��");
		stt_names[STT_CR]		=	("�м���Ըָ��");
		stt_names[STT_OSC]		=	("�ڶ���");
		stt_names[STT_UOS]		=	("���ڶ�ָ��");
		stt_names[STT_MAOSC]	=	("�ƶ�ƽ���ڶ���");
		stt_names[STT_36BIAS]	=	("3��6�չ���");
		stt_names[STT_DPO]		=	("ȥ�Ƽ۸�ڶ�ָ��");
		stt_names[STT_KST]		=	("ȷȻָ��");
		//	������
		stt_names[STT_REI]		=	("��Χ����ָ��");
		stt_names[STT_DMKI]		=	("����˶�ָ��");
		stt_names[STT_PCNT]		=	("���ȱ�");
		stt_names[STT_HLC]		=	("���ȷ���");
		stt_names[STT_CDP]		=	("���Ʋ���ָ��");
		stt_names[STT_ASI]		=	("�ۻ��ڶ�ָ��");
		stt_names[STT_ATR]		=	("����ָ��");
		//	����Ϊ��ָ����
		stt_names[STT_CYO]		=	("���ڰڶ���");
		stt_names[STT_DCYO]		=	("ȥ�����ڰڶ���");
		stt_names[STT_HSL]		=	("��Ի�����");
		stt_names[STT_DPER]		=	("ȥ�ưٷֱ�ָ��");
	}
	
	if( nTech >= STT_MIN && nTech <= STT_MAX )
	{
		return	stt_names[nTech];
	}
	if( nTech >= STT_USER_MIN )
	{
		return CTechUser::GetTechUserName( nTech );
	}
	return "";
}

// ��������ָ��Ψһ��ţ����ؼ���ָ��Ӣ�ļ��
CSPString	AfxGetSTTShortName( UINT nTech )
{
	static	CSPString	stt_shortnames[STT_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );

	if( stt_shortnames[0].GetLength() <= 0 )
	{
		// stt_shortnames
		stt_shortnames[0]			=	"STTShortNames";

		//	K��������
		stt_shortnames[STT_MA]		=	("MA");
		stt_shortnames[STT_BBI]		=	("BBI");
		stt_shortnames[STT_BOLL]	=	("BOLL");
		stt_shortnames[STT_PV]		=	("P/V");
		stt_shortnames[STT_SAR]		=	("SAR");
		stt_shortnames[STT_DJ]		=	("DJ");
		stt_shortnames[STT_CW]		=	("CW");
		//	������
		stt_shortnames[STT_MACD]	=	("MACD");
		stt_shortnames[STT_MIKE]	=	("MIKE");
		stt_shortnames[STT_PSY]		=	("PSY");
		//	������
		stt_shortnames[STT_VOLUME]	=	("VOLUME");
		stt_shortnames[STT_NVI]		=	("NVI");
		stt_shortnames[STT_PVI]		=	("PVI");
		stt_shortnames[STT_VR]		=	("VR");
		stt_shortnames[STT_VROC]	=	("VROC");
		stt_shortnames[STT_OBV]		=	("OBV");
		stt_shortnames[STT_MOBV]	=	("MOBV");
		stt_shortnames[STT_MFI]		=	("MFI");
		stt_shortnames[STT_VMACD]	=	("VMACD");
		stt_shortnames[STT_WVAD]	=	("WVAD");
		stt_shortnames[STT_EMV]		=	("EMV");
		stt_shortnames[STT_VRSI]	=	("VRSI");
		stt_shortnames[STT_NVRSI]	=	("NVRSI");
		stt_shortnames[STT_AD]		=	("A/D");
		stt_shortnames[STT_CI]		=	("CI");
		//	�ڶ���
		stt_shortnames[STT_KDJ]		=	("KDJ");
		stt_shortnames[STT_R]		=	("R");
		stt_shortnames[STT_RSI]		=	("RSI");
		stt_shortnames[STT_BIAS]	=	("BIAS");
		stt_shortnames[STT_MTM]		=	("MTM");
		stt_shortnames[STT_DMI]		=	("DMI");
		stt_shortnames[STT_ROC]		=	("ROC");
		stt_shortnames[STT_CCI]		=	("CCI");
		stt_shortnames[STT_CV]		=	("CV");
		stt_shortnames[STT_ARBR]	=	("ARBR");
		stt_shortnames[STT_CR]		=	("CR");
		stt_shortnames[STT_OSC]		=	("OSC");
		stt_shortnames[STT_UOS]		=	("UOS");
		stt_shortnames[STT_MAOSC]	=	("MAOSC");
		stt_shortnames[STT_36BIAS]	=	("3-6BIAS");
		stt_shortnames[STT_DPO]		=	("DPO");
		stt_shortnames[STT_KST]		=	("KST");
		//	������
		stt_shortnames[STT_REI]		=	("REI");
		stt_shortnames[STT_DMKI]	=	("DMKI");
		stt_shortnames[STT_PCNT]	=	("PCNT");
		stt_shortnames[STT_HLC]		=	("HLC");
		stt_shortnames[STT_CDP]		=	("CDP");
		stt_shortnames[STT_ASI]		=	("ASI");
		stt_shortnames[STT_ATR]		=	("ATR");
		//	����Ϊ��ָ����
		stt_shortnames[STT_CYO]		=	("CYO");
		stt_shortnames[STT_DCYO]	=	("DCYO");
		stt_shortnames[STT_HSL]		=	("HSL");
		stt_shortnames[STT_DPER]	=	("DPER");
	}

	if( nTech >= STT_MIN && nTech <= STT_MAX )
	{
		return	stt_shortnames[nTech];
	}
	if( nTech >= STT_USER_MIN )
	{
		return CTechUser::GetTechUserShortName( nTech );
	}
	return "";
}

// ��������ָ��Ψһ��ţ����ؼ���ָ����������+Ӣ�ļ��
CSPString	AfxGetSTTFullName( UINT nTech )
{
	return AfxGetSTTName( nTech ) + "(" + AfxGetSTTShortName( nTech ) + ")";
}

// ��������ָ��Ψһ��ţ����ؼ���ָ���������
CSPString	AfxGetSTTClassName( UINT nTech )
{
	static	CSPString	stt_classnames[STT_CLASS_MAX+1];
	SP_ASSERT( STT_MAX >= 0 );

	if( stt_classnames[0].GetLength() <= 0 )
	{
		// stt_classnames
		stt_classnames[0]					=	"STTClassNames";

		stt_classnames[STT_CLASS_KLINE]		=	"K��������";
		stt_classnames[STT_CLASS_TREND]		=	"������";
		stt_classnames[STT_CLASS_ENERGY]	=	"������";
		stt_classnames[STT_CLASS_SWING]		=	"�ڶ���";
		stt_classnames[STT_CLASS_OTHER]		=	"������";
		stt_classnames[STT_CLASS_CLK]		=	"����Ϊ��ָ����";
		stt_classnames[STT_CLASS_USER]		=	"���ָ����";
	}

	if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
		return stt_classnames[STT_CLASS_KLINE];
	else if( nTech >= STT_TREND_MIN && nTech <= STT_TREND_MAX )
		return stt_classnames[STT_CLASS_TREND];
	else if( nTech >= STT_ENERGY_MIN && nTech <= STT_ENERGY_MAX )
		return stt_classnames[STT_CLASS_ENERGY];
	else if( nTech >= STT_SWING_MIN && nTech <= STT_SWING_MAX )
		return stt_classnames[STT_CLASS_SWING];
	else if( nTech >= STT_OTHER_MIN && nTech <= STT_OTHER_MAX )
		return stt_classnames[STT_CLASS_OTHER];
	else if( nTech >= STT_CLK_MIN && nTech <= STT_CLK_MAX )
		return stt_classnames[STT_CLASS_CLK];
	else if( nTech >= STT_USER_MIN )
		return stt_classnames[STT_CLASS_USER];
	return "";
}

// ���������ź�ֵ���õ�����˵��
CSPString	AfxGetIntensityString( int nIntensity )
{
	if( nIntensity >= ITS_MIN && nIntensity <= ITSG_SELLINTENSE )
		return its_sellintense;
	else if( nIntensity > ITSG_SELLINTENSE && nIntensity <= ITSG_SELL )
		return its_sell;
	else if( nIntensity > ITSG_SELL && nIntensity <= ITSG_SELLFEEBLE )
		return its_sellfeeble;
	else if( nIntensity > ITSG_SELLFEEBLE && nIntensity < ITSG_BUYFEEBLE  )
		return its_nothing;
	else if( nIntensity >= ITSG_BUYFEEBLE && nIntensity < ITSG_BUY )
		return its_buyfeeble;
	else if( nIntensity >= ITSG_BUY && nIntensity < ITSG_BUYINTENSE )
		return its_buy;
	else if( nIntensity >= ITSG_BUYINTENSE && nIntensity <= ITS_MAX )
		return its_buyintense;
	
	SP_ASSERT( FALSE );
	return "";
}

// ���������ź�����˵��ֵ���õ�����˵��
CSPString	AfxGetIntensityCodeString( UINT nCode )
{
	switch( nCode )
	{
	case	ITSC_NOTHING:			return	itsc_nothing;
	case	ITSC_DEVIATEONBOTTOM:	return	itsc_deviateonbottom;
	case	ITSC_DEVIATEONTOP:		return	itsc_deviateontop;
	case	ITSC_GOLDENFORK:		return	itsc_goldenfork;
	case	ITSC_DEADFORK:			return	itsc_deadfork;
	case	ITSC_LONG:				return	itsc_long;
	case	ITSC_SHORT:				return	itsc_short;
	case	ITSC_OVERBOUGHT:		return	itsc_overbought;
	case	ITSC_OVERSOLD:			return	itsc_oversold;
	case	ITSC_SUPPORT:			return	itsc_support;
	case	ITSC_RESISTANCE:		return	itsc_resistance;
	default:
		SP_ASSERT( FALSE );
	}
	return "";
}

CSPString AfxGetKTypeString( int ktype )
{
	if( CKData::ktypeMonth == ktype )
		return "����";
	else if( CKData::ktypeWeek == ktype )
		return "����";
	else if( CKData::ktypeDay == ktype )
		return "����";
	else if( CKData::ktypeMin1 == ktype )
		return "һ������";
	else if( CKData::ktypeMin5 == ktype )
		return "�������";
	else if( CKData::ktypeMin15 == ktype )
		return "ʮ�������";
	else if( CKData::ktypeMin30 == ktype )
		return "��ʮ������";
	else if( CKData::ktypeMin60 == ktype )
		return "��ʮ������";
	else
		return "δ֪";
	return "";
}

CSPString AfxGetMaindataTypeString( int type )
{
	if( CKData::mdtypeClose == type )
		return "���̼�";
	else if( CKData::mdtypeOpen == type )
		return "���̼�";
	else if( CKData::mdtypeAverage == type )
		return "ƽ����";
	else
		return "δ֪";
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
// ��Ʊ�б��������ַ���
char	slh_hdrday[]	=	"��";
char	slh_avrcode[]	=	"AVR";
char	slh_avrname[]	=	"ƽ��ֵ";
char	slh_wavrcode[]	=	"WAVR";
char	slh_wavrname[]	=	"��Ȩƽ��";

CSPString	AfxGetSLHTitle( UINT nSLH )
{
	static	CSPString	slh_titles[SLH_MAX+1];
	SP_ASSERT( SLH_MAX >= 0 );

	if( slh_titles[0].GetLength() <= 0 )
	{
		// slh_titles
		slh_titles[SLH_NONE]				=	"SLHTitles";
		
		slh_titles[SLH_CODE]				=	"����";
		slh_titles[SLH_NAME]				=	"����";
		
		//	��������
		slh_titles[SLH_DATE]				=	"����";
		slh_titles[SLH_LASTCLOSE]			=	"����";
		slh_titles[SLH_OPEN]				=	"���̼�";
		slh_titles[SLH_CLOSE]				=	"�ּ�";
		slh_titles[SLH_HIGH]				=	"��߼�";
		slh_titles[SLH_LOW]					=	"��ͼ�";
		slh_titles[SLH_AVERAGE]				=	"����";
		slh_titles[SLH_DIFF]				=	"�ǵ�";
		slh_titles[SLH_DIFFPERCENT]			=	"�Ƿ�%";
		slh_titles[SLH_SCOPE]				=	"���%";
		slh_titles[SLH_VOLUME]				=	"�ɽ���(��)";
		slh_titles[SLH_AMOUNT]				=	"�ɽ���(ǧԪ)";
		slh_titles[SLH_VOLUP]				=	"����";
		slh_titles[SLH_VOLDOWN]				=	"����";
		slh_titles[SLH_DIFFPERCENT_MIN5]	=	"������Ƿ�%";
		slh_titles[SLH_SELLBUYRATIO]		=	"ί��%";
		slh_titles[SLH_SELLBUYDIFF]			=	"ί����";
		slh_titles[SLH_BUYPRICE3]			=	"�����";
		slh_titles[SLH_BUYPRICE2]			=	"��۶�";
		slh_titles[SLH_BUYPRICE1]			=	"���һ";
		slh_titles[SLH_SELLPRICE1]			=	"����һ";
		slh_titles[SLH_SELLPRICE2]			=	"���۶�";
		slh_titles[SLH_SELLPRICE3]			=	"������";
		slh_titles[SLH_BUYVOLUME3]			=	"������";
		slh_titles[SLH_BUYVOLUME2]			=	"������";
		slh_titles[SLH_BUYVOLUME1]			=	"����һ";
		slh_titles[SLH_SELLVOLUME1]			=	"����һ";
		slh_titles[SLH_SELLVOLUME2]			=	"������";
		slh_titles[SLH_SELLVOLUME3]			=	"������";

		//	��Ҫ����ָ��
		slh_titles[SLH_REPORTTYPE]			=	"��������";
		slh_titles[SLH_PE]					=	"��ӯ��";
		slh_titles[SLH_PNETASSET]			=	"�о���";
		slh_titles[SLH_PMAININCOME]			=	"������";
		slh_titles[SLH_RATIO_PCASH]			=	"�۸��ֽ��";
		slh_titles[SLH_RATIO_CURRENCY]		=	"��ͨ��%";
		slh_titles[SLH_RATIO_CHANGEHAND]	=	"������%";
		slh_titles[SLH_RATIO_VOLUME]		=	"����";
		slh_titles[SLH_RS]					=	"���ǿ��";
		slh_titles[SLH_MARKETVALUE]			=	"����ֵ-��";
		slh_titles[SLH_MARKETVALUEA]		=	"A����ֵ-��";
		slh_titles[SLH_MARKETVALUEB]		=	"B����ֵ-��";
		
		// �ﳥծ����
		slh_titles[SLH_RATIO_LIQUIDITY]		=	"��������";
		slh_titles[SLH_RATIO_QUICK]			=	"�ٶ�����";
		slh_titles[SLH_VELOCITY_RECEIVABLES]=	"Ӧ���˿�����";
		
		// �ﾭӪ����
		slh_titles[SLH_VELOCITY_MERCHANDISE]=	"�����ת��";
		slh_titles[SLH_MAIN_INCOME]			=	"��Ӫ����-��";
		slh_titles[SLH_CASH_PS]				=	"ÿ�ɾ��ֽ�";
		
		// ��ӯ������
		slh_titles[SLH_PROFIT_MARGIN]		=	"��Ӫ������%";
		slh_titles[SLH_NETASSET_YIELD]		=	"���ʲ�������%";
		
		// ���ʱ��ṹ
		slh_titles[SLH_DATE_BEGIN]			=	"��������";
		slh_titles[SLH_SHARE_COUNT_TOTAL]	=	"�ܹɱ�-��";
		slh_titles[SLH_SHARE_COUNT_A]		=	"A��-��";
		slh_titles[SLH_SHARE_COUNT_B]		=	"B��-��";
		slh_titles[SLH_SHARE_COUNT_H]		=	"H��-��";
		slh_titles[SLH_SHARE_COUNT_NATIONAL]=	"���й�-��";
		slh_titles[SLH_SHARE_COUNT_CORP]	=	"���˹�-��";
		slh_titles[SLH_PROFIT_PSUD]			=	"ÿ��δ������";
		slh_titles[SLH_ASSET]				=	"���ʲ�-��";
		slh_titles[SLH_RATIO_HOLDERRIGHT]	=	"�ɶ�Ȩ���%";
		slh_titles[SLH_RATIO_LONGDEBT]		=	"���ڸ�ծ��%";
		slh_titles[SLH_RATIO_DEBT]			=	"�ʲ���ծ��%";

		// ��Ͷ����������
		slh_titles[SLH_NETASSET_PS]			=	"ÿ�ɾ��ʲ�";
		slh_titles[SLH_NETASSET_PS_REGULATE]=	"����ÿ�ɾ��ʲ�";
		slh_titles[SLH_EPS]					=	"ÿ������";
		slh_titles[SLH_EPS_DEDUCT]			=	"�۳�ÿ������";
		slh_titles[SLH_NET_PROFIT]			=	"������-��";
		slh_titles[SLH_MAIN_PROFIT]			=	"��Ӫҵ������-��";
		slh_titles[SLH_TOTAL_PROFIT]		=	"�����ܶ�-��";
		slh_titles[SLH_PROFIT_INC]			=	"��Ӫ����������%";
		slh_titles[SLH_INCOME_INC]			=	"����������%";
		slh_titles[SLH_ASSET_INC]			=	"���ʲ�������%";
		slh_titles[SLH_ANNGAINS_AVERAGE]	=	"��ƽ��������%";
		slh_titles[SLH_ANNGAINS_STDDEV]		=	"�����׼��%";
		slh_titles[SLH_BETA]				=	"��ֵ";
		slh_titles[SLH_SHARP]				=	"���շ���ָ��";
		slh_titles[SLH_TRADE]				=	"��ҵ";
		slh_titles[SLH_PROVINCE]			=	"ʡ��";
	}

	if( nSLH >= SLH_MIN && nSLH <= SLH_MAX )
	{
		return	slh_titles[nSLH];
	}
	return "";
}

CSPString	AfxGetSLHDescript( UINT nSLH )
{
	static	CSPString	slh_descripts[SLH_MAX+1];
	SP_ASSERT( SLH_MAX >= 0 );

	if( slh_descripts[0].GetLength() <= 0 )
	{
		// slh_descripts
		slh_descripts[SLH_PE]			=	"ÿ�ɼ۸��ÿ������";

		slh_descripts[SLH_PNETASSET]	=	"ÿ�ɼ۸��ÿ�ɾ��ʲ�";
		slh_descripts[SLH_PMAININCOME]	=	"����ֵ����Ӫ����";
		slh_descripts[SLH_RATIO_PCASH]	=	"ÿ�ɼ۸��ÿ�ɾ��ֽ�";
		slh_descripts[SLH_RS]			=	"n�����Ƿ������֮��";
		slh_descripts[SLH_ANNGAINS_AVERAGE]="Ͷ�ʸù�Ʊ��������";
		slh_descripts[SLH_ANNGAINS_STDDEV]=	"�������ʵı�׼��";
		slh_descripts[SLH_BETA]			=	"��ʾ����������";
		slh_descripts[SLH_SHARP]		=	"�������ʱȱ�׼��";
	}
	
	if( nSLH >= SLH_MIN && nSLH <= SLH_MAX )
	{
		return	slh_descripts[nSLH];
	}
	return "";
}


//////////////////////////////////////////////////////////////////////////////////
// ���ʽ������ʾ�ַ���
char	express_errparserstack[]	=	"���ʽ�����ʽ̫����ջ�����";
char	express_errbadrange[]		=	"���ʽ����ֵ������Χ��";
char	express_errexpression[]		=	"���ʽ���ַ������ִ���";
char	express_erroperator[]		=	"���ʽ�����������ִ���";
char	express_erropenparen[]		=	"���ʽ�������ų��ִ���";
char	express_errcloseparen[]		=	"���ʽ�������ų��ִ���";
char	express_errinvalidnum[]		=	"���ʽ���Ƿ����֡�";
char	express_errmath[]			=	"���ʽ����ѧ������ִ���";
char	express_errunknown[]		=	"���ʽ��δ֪����";


//////////////////////////////////////////////////////////////////////////////////
// ��Ʊ����ַ���
char	domain_self[]	=	"��ѡ��";


//////////////////////////////////////////////////////////////////////////////////
// ������ɫ�����ַ���
char	colorclass_standard[]		=	"��ͳʽ��ɫ����";
char	colorclass_byzantinesque[]	=	"��ռͥʽ��ɫ����";
char	colorclass_gothic[]			=	"����ʽ��ɫ����";
char	colorclass_baroque[]		=	"�����ʽ��ɫ����";


//////////////////////////////////////////////////////////////////////////////////
// ������˵��������֧��
char	sz_mainserverinfo[]			=	"��������";
char	sz_mailsubjectsupport[]		=	"����֧��";


//////////////////////////////////////////////////////////////////////////////////
// ��
char	sz_sunday[]		=	"������";
char	sz_monday[]		=	"����һ";
char	sz_tuesday[]	=	"���ڶ�";
char	sz_wednesday[]	=	"������";
char	sz_thursday[]	=	"������";
char	sz_friday[]		=	"������";
char	sz_saturday[]	=	"������";


//////////////////////////////////////////////////////////////////////////////////
// ��������
char	sz_reportQuarter[]	=	"��һ���ȼ���";
char	sz_reportMid[]		=	"�б�";
char	sz_reportQuarter3[]	=	"�������ȼ���";
char	sz_reportAnnals[]	=	"�걨";

//////////////////////////////////////////////////////////////////////////////////
// �����ַ���
char	db_errcreateselfdb[]	=	"���������ļ�ʧ�ܣ������°�װ������������������Ӧ����ϵ";
char	db_errrootpath[]		=	"��ʼ������ʧ�ܣ������°�װ������������������Ӧ����ϵ";
char	db_errqianlongpath[]	=	"��ȡָ����Ǯ������Դʧ�ܡ�";


//////////////////////////////////////////////////////////////////////////////////
// �����ַ���

char	strategy_logicand[]		=	"ȫ��";
char	strategy_logicor[]		=	"����һ��";

char	strategy_sdonce[]		=	"ȫ��";
char	strategy_sdtwice[]		=	"���";
char	strategy_sdthird[]		=	"����֮һ��";
char	strategy_sdforth[]		=	"�ķ�֮һ��";
char	strategy_sdfifth[]		=	"���֮һ��";
char	strategy_sdsixth[]		=	"����֮һ��";
char	strategy_sdseventh[]	=	"�߷�֮һ��";
char	strategy_sdeighth[]		=	"�˷�֮һ��";
char	strategy_sdninth[]		=	"�ŷ�֮һ��";
char	strategy_sdtenth[]		=	"ʮ��֮һ��";

char	strategy_noselectedstock[]	=	"û��ѡ�й�Ʊ";
char	strategy_noselectedtech[]	=	"û��ѡ��ָ��";

char	strategy_optype_buy[]		=	"����";
char	strategy_optype_sell[]		=	"����";
char	strategy_optype_addstock[]	=	"���ӹ�Ʊ";
char	strategy_optype_removestock[]	=	"���ٹ�Ʊ";
char	strategy_optype_addcash[]		=	"�����ֽ�";
char	strategy_optype_removecash[]	=	"�����ֽ�";

char	strategy_errfile[]			=	"�ļ����Ͳ�����ָ���ļ����ǲ����ļ���";
char	strategy_errfilever[]		=	"������ļ��汾��������������Ա���Զ�ȡ�߰汾�Ĳ����ļ���";


//////////////////////////////////////////////////////////////////////////////////
// ��ݼ�
// ������ݼ�Ψһ��ţ����ؿ�ݼ�������������
CSPString	AfxGetAccelerator( UINT nAcce, int nCharLeft )
{
	static	CSPString	acce_names[ACCE_MAX+1];
	SP_ASSERT( ACCE_MAX >= 0 );
	
	if( acce_names[0].GetLength() <= 0 )
	{
		// acce_names
		acce_names[0]			=	"ACCENames";

		// ��ݼ�
		acce_names[ACCE_01]		=	"01      �ȼ�F1";
		acce_names[ACCE_02]		=	"02      �ȼ�F2";
		acce_names[ACCE_03]		=	"03      �ȼ�F3";
		acce_names[ACCE_04]		=	"04      �ȼ�F4";
		acce_names[ACCE_05]		=	"05      �ȼ�F5";
		acce_names[ACCE_06]		=	"06      �ȼ�F6";
		acce_names[ACCE_07]		=	"07      �ȼ�F7";
		acce_names[ACCE_08]		=	"08      �ȼ�F8";
		acce_names[ACCE_09]		=	"09      �ȼ�F9";
		acce_names[ACCE_10]		=	"10      �ȼ�F10";
		acce_names[ACCE_1]		=	"1       ��A����";
		acce_names[ACCE_2]		=	"2       ��B����";
		acce_names[ACCE_3]		=	"3       ��A����";
		acce_names[ACCE_4]		=	"4       ��B����";
		acce_names[ACCE_5]		=	"5       ��ծ����";
		acce_names[ACCE_6]		=	"6       ��ծ����";
		acce_names[ACCE_61]		=	"61      ��A�ǵ�";
		acce_names[ACCE_62]		=	"62      ��B�ǵ�";
		acce_names[ACCE_63]		=	"63      ��A�ǵ�";
		acce_names[ACCE_64]		=	"64      ��B�ǵ�";
		acce_names[ACCE_65]		=	"65      ��ծ�ǵ�";
		acce_names[ACCE_66]		=	"66      ��ծ�ǵ�";
		acce_names[ACCE_71]		=	"71      ��֤��Ϣ";
		acce_names[ACCE_72]		=	"72      ��֤��Ϣ";
		acce_names[ACCE_73]		=	"73      ȯ����Ϣ";
		acce_names[ACCE_81]		=	"81      ��A�ۺ�";
		acce_names[ACCE_82]		=	"82      ��B�ۺ�";
		acce_names[ACCE_83]		=	"83      ��A�ۺ�";
		acce_names[ACCE_84]		=	"84      ��B�ۺ�";
		acce_names[ACCE_85]		=	"85      ��ծ�ۺ�";
		acce_names[ACCE_86]		=	"86      ��ծ�ۺ�";
		acce_names[ACCE_LBDK]	=	"LBDK    ���ȣ���գ�";
		acce_names[ACCE_MMLD]	=	"MMLD    �����������ǵ��ʣ�";
	}
	
	if( nAcce >= ACCE_MIN && nAcce <= ACCE_MAX )
	{
		if( 8 == nCharLeft )
			return	acce_names[nAcce];
		
		CSPString	strResult	=	acce_names[nAcce];
		int nIndex = strResult.Find( ' ' );
		if( -1 != nIndex )
			strResult	=	strResult.Left(nIndex) + strResult.Mid(8);
		return strResult;
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
// ���һ����

void AfxGetAllDistrict( CSPStringArray & astr )
{
	astr.SetSize( 0, 30 );
	astr.Add( "�Ĵ�����" );
	astr.Add( "����͢" );
	astr.Add( "����" );
	astr.Add( "���ô�" );
	astr.Add( "�й�" );
	astr.Add( "����" );
	astr.Add( "����" );
	astr.Add( "�¹�" );
	astr.Add( "�й����" );
	astr.Add( "ӡ��" );
	astr.Add( "�����" );
	astr.Add( "�ձ�" );
	astr.Add( "����" );
	astr.Add( "ī����" );
	astr.Add( "Ų��" );
	astr.Add( "�¼���" );
	astr.Add( "������" );
	astr.Add( "���" );
	astr.Add( "�й�̨��" );
	astr.Add( "Ӣ��" );
	astr.Add( "����" );
	astr.Add( "����" );
}


//////////////////////////////////////////////////////////////////////////////////
// �򵥷���
#define	CHS_ENU_MAX_WORDS	32
typedef	struct	chs_enu_pair_t	{
	char	chs[CHS_ENU_MAX_WORDS];
	char	enu[CHS_ENU_MAX_WORDS];
}CHS_ENU_PAIR;

CSPString	AfxFaintlyChsToEnu( const char * szCh )
{
	static	CHS_ENU_PAIR	chs_enu_pairs[]	=	{
		{ "�й�",	"China" },
		{ "ָ��",	"Index" },
		{ "��֤",	"Shanghai" },
		{ "��֤",	"Shenzhen" },
		{ "����",	"Fund" },
		{ "��ծ",	"Bond" },
		{ "ծȯ",	"Bond" },
		{ "תծ",	"Convertible Bond" },
		{ "����",	"Bank" },
		{ "����",	"Other" },
		{ "����ҵ",	"Bank" },
		{ "",	"" },
	};
	
	CSPString	sResult = szCh;
	int i = 0;
	while( * chs_enu_pairs[i].chs )
	{
		sResult.Replace( chs_enu_pairs[i].chs, chs_enu_pairs[i].enu );
		i ++;
	}

	return sResult;
}

