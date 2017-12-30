// DlgInitCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgInitCfg.h"
#include "afxdialogex.h"
#include "cfg_op.h"


// CDlgInitCfg 对话框

IMPLEMENT_DYNAMIC(CDlgInitCfg, CDialog)

CDlgInitCfg::CDlgInitCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInitCfg::IDD, pParent)
	, m_dbdsn(_T(""))
	, m_dbpw(_T(""))
	, m_dbuid(_T(""))
{

}

CDlgInitCfg::~CDlgInitCfg()
{
}

void CDlgInitCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBDSN, m_dbdsn);
	DDX_Text(pDX, IDC_EDIT_DBPW, m_dbpw);
	DDX_Text(pDX, IDC_EDIT_DBUID, m_dbuid);
}


BEGIN_MESSAGE_MAP(CDlgInitCfg, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInitCfg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK_TEST, &CDlgInitCfg::OnBnClickedTest)
	ON_BN_CLICKED(IDCANCEL, &CDlgInitCfg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgInitCfg 消息处理程序
extern CString g_SecMngIniName;
extern CString	g_dbSource, g_dbUse, g_dbpw;
//读配置文件 
void CDlgInitCfg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);


	m_dbdsn.TrimRight(); m_dbdsn.TrimLeft();
	m_dbpw.TrimRight(); m_dbpw.TrimLeft();
	m_dbuid.TrimRight(); m_dbuid.TrimLeft();

	if (m_dbdsn.IsEmpty() || m_dbpw.IsEmpty() || m_dbuid.IsEmpty())
	{
		AfxMessageBox("用户输入空 请检查输入");
	}

	//读配置文件
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


	//写配置项 DSN
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)g_strINIPath, "DSN" /*in*/, (LPTSTR)(LPCTSTR)m_dbdsn/*in*/, m_dbdsn.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("写配置项 DSN 失败");
		return;
	}
	g_dbSource = m_dbdsn;

	//写配置项 UID
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)g_strINIPath, "UID" /*in*/, (LPTSTR)(LPCTSTR)m_dbuid/*in*/, m_dbuid.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("写配置项 UID 失败");
		return;
	}
	g_dbUse = m_dbuid;

	//写配置项 PWD
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)g_strINIPath, "PWD" /*in*/, (LPTSTR)(LPCTSTR)m_dbpw/*in*/, m_dbpw.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("写配置项 PWD 失败");
		return;
	}
	g_dbpw = m_dbpw;
	CDialog::OnOK();
}


void CDlgInitCfg::OnBnClickedTest()
{
	// TODO:  在此添加控件通知处理程序代码
	CDatabase db;

	UpdateData(TRUE);
	m_dbdsn.TrimLeft(); m_dbdsn.TrimRight();
	m_dbuid.TrimLeft(); m_dbuid.TrimRight();
	m_dbpw.TrimLeft(); m_dbpw.TrimRight();

	CString		strCon;
	strCon.Format("DSN=%s;UID=%s;PWD=%s", m_dbdsn, m_dbuid, m_dbpw);

	TRY 
	{
		if (!db.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("测试 配置数据库连接信息失败 ");
			return;

		}
	}
	CATCH_ALL (e)
	{
		e->ReportError();
	}
	END_CATCH_ALL

	AfxMessageBox("测试 配置数据库连接信息 ok");
}


void CDlgInitCfg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
