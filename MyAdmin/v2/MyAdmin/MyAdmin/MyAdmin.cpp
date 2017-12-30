
// MyAdmin.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyAdmin.h"
#include "MainFrm.h"

#include "MyAdminDoc.h"
#include "MyAdminView.h"
#include "cfg_op.h"
#include "secmng_globvar.h"
#include "DlgInitCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyAdminApp

BEGIN_MESSAGE_MAP(CMyAdminApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMyAdminApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMyAdminApp 构造

CMyAdminApp::CMyAdminApp()
{

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyAdmin.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMyAdminApp 对象

CMyAdminApp theApp;


//CString g_SecMngIniName = "secmngadmin.ini";

//需要动态的获取进程所在的路径 
int CMyAdminApp::ReadSecMngCfg()
{
	int				ret = 0;
		 
	char strFileName[2048] = { 0 };
	CString  g_strINIPath = "";

	GetModuleFileName(AfxGetInstanceHandle(), strFileName, sizeof(strFileName)); ////获得运行程序名字
	//AfxMessageBox(strFileName);
	//获取程序运行目录，组合配置文件路径
	g_strINIPath.Format("%s", strFileName);
	int i = g_strINIPath.ReverseFind('\\');
	g_strINIPath = g_strINIPath.Left(i);
	g_strINIPath = g_strINIPath + "\\" + g_SecMngIniName;


	//获取配置项
	char	tmpBuf[1024] = { 0 };
	int		tmpBufLen = 0;
	//读文件 配置项 DSN 
	
	ret = GetCfgItem((LPTSTR)(LPCTSTR)g_strINIPath /*in*/, "DSN"/*in*/, tmpBuf, &tmpBufLen);
	if (ret != 0)
	{
		AfxMessageBox("读配置 DSN 失败");
		return ret;
	}
	g_dbSource = tmpBuf;
	

	//读文件 配置项 UID 
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)g_strINIPath /*in*/, "UID"/*in*/, tmpBuf, &tmpBufLen);
	if (ret != 0)
	{
		AfxMessageBox("读配置 UID 失败");
		return ret;
	}
	g_dbUse = tmpBuf;
	
	//读文件 配置项 PWD 
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)g_strINIPath /*in*/, "PWD"/*in*/, tmpBuf, &tmpBufLen);
	if (ret != 0)
	{
		AfxMessageBox("读配置 PWD 失败");
		return ret;
	}
	g_dbpw = tmpBuf;

	return 0;
}
// CMyAdminApp 初始化

int CMyAdminApp::OdbcConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	
	CString		strCon;
	strCon.Format("DSN=%s;UID=%s;PWD=%s", g_dbSource, g_dbUse, g_dbpw);

	TRY
	{
		if (!myDB.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("测试 配置数据库连接信息失败 ");
			return -1;
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL

	g_pDB = &myDB;
	//AfxMessageBox("测试 配置数据库连接信息 ok");
	return 0;
}


BOOL CMyAdminApp::InitInstance()
{
	//AfxMessageBox("");
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);
	
	int				ret = 0;
	//信息系统的初始化
	ret = ReadSecMngCfg();
	if (ret != 0)
	{
		CDlgInitCfg   dlgInitCfg;

		if (dlgInitCfg.DoModal() == IDCANCEL)
		{
			return FALSE;
		}
		else
		{
			//g_dbSource = dlgInitCfg.m_dbsource; //给全局变量赋值
			//g_dbpw = dlgInitCfg.m_dbpw;
			//g_dbUse = dlgInitCfg.m_dbuser;
		}
	}

	//和数据库建立一条连接
	ret = CMyAdminApp::OdbcConnect();
	if (ret != 0)
	{
		AfxMessageBox("和数据库建立一条连接 失败");
		return FALSE;
	}

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyAdminDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CMyAdminView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CMyAdminApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CMyAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyAdminApp 消息处理程序



