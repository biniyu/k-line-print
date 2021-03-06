
// KLinePrint.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CKLinePrintApp 构造

CKLinePrintApp::CKLinePrintApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CKLinePrintApp 对象

CKLinePrintApp theApp;


// CKLinePrintApp 初始化

BOOL CKLinePrintApp::InitInstance()
{
#if 0

	//	维持网络连接/会话
	Net net("192.168.1.1:8080");
	net.start();

	//	执行命令
	CommandExecutor ce(&net);
	string ret = ce.Execute("login");

	//	接收数据
	DataHandler dh(&net);
	

	//	初始化网络连接
	//	创建线程，发起并维持连接
	//	

	CLoginDialog loginDlg;

	if(loginDlg.DoModal()!=IDOK)
	{
		return FALSE;
	}

	CommandExecutor ce();

	string result = ce.Execute("login:oscar,12345,sr");



#endif

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Oscar"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKLinePrintDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CKLinePrintView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CalendarGenerator cg;
	cg.Generate("C:\\FutureData\\ZZ", m_cal);

	//	读入账户/品种信息
	EXCHANGE.SetBalance(Utility::ReadBalance());
	TP = Utility::ReadExchangeConfig();

	//	设置默认交易日志文件
	EXCHANGE.SetLogFile(Utility::GetProgramPath() + "log\\manual.log.txt");

	srand(time(0));

	//	加载回放配置并生成日历
	m_PlaybackConfig = Utility::ReadPlaybackConfig();
	LoadPlaybackCalendar(m_PlaybackConfig);

	//	自动打开上次的文件
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

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

void CKLinePrintApp::LoadPlaybackCalendar(PlaybackConfig pbConfig)
{
	m_PlaybackCalendar.clear();

	//	根据配置生成过滤后的日历

	int nCurDate = CALENDAR.GetFirst();

	while(nCurDate > 0)
	{
		//	满足所有条件才加入
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CKLinePrintApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



