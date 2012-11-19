
// KLinePrint.h : KLinePrint Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "PlaybackConfig.h"
#include "CalendarGenerator.h"
#include "TradeFacility.h"

// CKLinePrintApp:
// �йش����ʵ�֣������ KLinePrint.cpp
//

class CKLinePrintApp : public CWinAppEx
{
public:
	CKLinePrintApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	UINT			m_nAppLook;
	BOOL			m_bHiColorIcons;

	Calendar		m_cal;
	PlaybackConfig	m_PlaybackConfig;		//	�ط�ѡ��
	TradeFacility	m_ex;					//	������

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKLinePrintApp theApp;

#define CALENDAR ((CKLinePrintApp*)AfxGetApp())->m_cal
#define PBCONFIG ((CKLinePrintApp*)AfxGetApp())->m_PlaybackConfig
#define EXCHANGE ((CKLinePrintApp*)AfxGetApp())->m_ex

