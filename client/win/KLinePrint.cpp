
// KLinePrint.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "KLinePrint.h"
#include "MainFrm.h"
#include "Utility.h"

#include "LoginDialog.h"

#include "KLinePrintDoc.h"
#include "KLinePrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKLinePrintApp

BEGIN_MESSAGE_MAP(CKLinePrintApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CKLinePrintApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CKLinePrintApp ����

CKLinePrintApp::CKLinePrintApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CKLinePrintApp ����

CKLinePrintApp theApp;


// CKLinePrintApp ��ʼ��

BOOL CKLinePrintApp::InitInstance()
{
#if 0

	//	ά����������/�Ự
	Net net("192.168.1.1:8080");
	net.start();

	//	ִ������
	CommandExecutor ce(&net);
	string ret = ce.Execute("login");

	//	��������
	DataHandler dh(&net);
	

	//	��ʼ����������
	//	�����̣߳�����ά������
	//	

	CLoginDialog loginDlg;

	if(loginDlg.DoModal()!=IDOK)
	{
		return FALSE;
	}

	CommandExecutor ce();

	string result = ce.Execute("login:oscar,12345,sr");



#endif

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Oscar"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKLinePrintDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CKLinePrintView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CalendarGenerator cg;
	cg.Generate("C:\\FutureData\\ZZ", m_cal);

	//	�����˻�/Ʒ����Ϣ
	EXCHANGE.SetBalance(Utility::ReadBalance());
	TP = Utility::ReadExchangeConfig();

	//	����Ĭ�Ͻ�����־�ļ�
	EXCHANGE.SetLogFile(Utility::GetProgramPath() + "log\\manual.log.txt");

	srand(time(0));

	//	���ػط����ò���������
	m_PlaybackConfig = Utility::ReadPlaybackConfig();
	LoadPlaybackCalendar(m_PlaybackConfig);

	//	�Զ����ϴε��ļ�
	char path[MAX_PATH];

	::GetPrivateProfileStringA("Files","Current","", 
							path, sizeof(path), 
							(Utility::GetProgramPath() + "klinep.ini").c_str());

	CKLinePrintDoc* pDoc = NULL;

	if(strlen(path))
	{
		pDoc = (CKLinePrintDoc*)OpenDocumentFile(CString(path));
		pDoc->LoadNextDay();
	}

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

void CKLinePrintApp::LoadPlaybackCalendar(PlaybackConfig pbConfig)
{
	m_PlaybackCalendar.clear();

	//	�����������ɹ��˺������

	int nCurDate = CALENDAR.GetFirst();

	while(nCurDate > 0)
	{
		//	�������������ż���
		if(ValidatePlaybackConfig(nCurDate, pbConfig))
			m_PlaybackCalendar.Add(nCurDate);

		nCurDate = CALENDAR.GetNext(nCurDate);
	}
}

BOOL CKLinePrintApp::ValidatePlaybackConfig(int nDate, PlaybackConfig pbConfig)
{
	if(pbConfig.nStartDate)
	{
		if(nDate < pbConfig.nStartDate) return FALSE;
	}

	if(pbConfig.nEndDate)
	{
		if(nDate > pbConfig.nEndDate) return FALSE;
	}

	int nWeekDay = Utility::GetWeekDayByDate(nDate);

	if(pbConfig.bDayOfWeek[nWeekDay] == FALSE)
		return FALSE;

	return TRUE;
}

int CKLinePrintApp::GetPlaybackDate()
{
	int nDayCnt = m_PlaybackCalendar.size();
	return m_PlaybackCalendar.GetBySeq(rand() % nDayCnt);
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CKLinePrintApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



