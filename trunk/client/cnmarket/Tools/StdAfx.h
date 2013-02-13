// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__63D37C59_6410_46E3_98B2_F67CEA6B91AA__INCLUDED_)
#define AFX_STDAFX_H__63D37C59_6410_46E3_98B2_F67CEA6B91AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#pragma warning( disable : 4244 )	// warning C4244: �������� : �ӡ�__time64_t��ת������int�������ܶ�ʧ����


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>

#include <afxinet.h>
#include <afxhtml.h>			// MFC HTML view support

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "htmlhelp/htmlhelp.h"

// GuiLib
#include <Guilib.h>

// XPFace
// #define	SPFACE_STATIC
#include "../XPFace/Include/XPFace.h"

// StkLib
// #define	STKLIB_STATIC
#include	"../StkLib/Include/StkLib.h"

// StkNet
// #define	STKNET_STATIC
#include	"../StkNet/Include/StkNet.h"

#include	"Resource.h"
#include	"RegisterDlg.h"
#include	"AfxCore.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__63D37C59_6410_46E3_98B2_F67CEA6B91AA__INCLUDED_)
