
// KLinePrint.h : KLinePrint Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "CalendarGenerator.h"

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
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	Calendar cal;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKLinePrintApp theApp;
