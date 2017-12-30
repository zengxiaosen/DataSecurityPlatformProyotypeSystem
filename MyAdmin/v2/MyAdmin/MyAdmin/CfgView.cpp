// CfgView.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "CfgView.h"
#include "SECMNGSRVCFG.h"


// CCfgView

IMPLEMENT_DYNCREATE(CCfgView, CFormView)

CCfgView::CCfgView()
	: CFormView(CCfgView::IDD)
	, m_strsrvip(_T(""))
	, m_strsrvnodenum(_T(""))
	, m_strsrvport(_T(""))
{

}

CCfgView::~CCfgView()
{
}

void CCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRVIP, m_strsrvip);
	DDX_Text(pDX, IDC_EDIT_SRVNODENUM, m_strsrvnodenum);
	DDX_Text(pDX, IDC_EDIT_SRVPORT, m_strsrvport);
}

BEGIN_MESSAGE_MAP(CCfgView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SAVESERVER, &CCfgView::OnBnClickedButtonSaveserver)
END_MESSAGE_MAP()


// CCfgView 诊断

#ifdef _DEBUG
void CCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCfgView 消息处理程序

extern CDatabase	*g_pDB;

//查询一次
void CCfgView::OnInitialUpdate2()
{
	CFormView::OnInitialUpdate();
	// 
	//定义了一个记录类的 变量
	CSECMNGSRVCFG	srvCfgRecordSet(g_pDB);
	TRY 
	{
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_ip");

		//相当于 向服务器发送了一个sql语句 并且结果已经返回
		if (!srvCfgRecordSet.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("打开表SECMNG.SRVCFG表失败");
			return;
		}

		if (!srvCfgRecordSet.IsEOF())
		{
			srvCfgRecordSet.m_KEY.TrimLeft(); srvCfgRecordSet.m_KEY.TrimRight();
			srvCfgRecordSet.m_VALUDE.TrimLeft(); srvCfgRecordSet.m_VALUDE.TrimRight();
			m_strsrvip = srvCfgRecordSet.m_VALUDE;
		}
		else
		{
			m_strsrvip = srvCfgRecordSet.m_VALUDE;
		}
	}
	CATCH_ALL (e)
	{
		e->ReportError();
		if (srvCfgRecordSet.IsOpen())
		{
			srvCfgRecordSet.Close();
		}
	}
	END_CATCH_ALL

	UpdateData(FALSE); //把C++的变量传递给界面

}


//反复查询  
void CCfgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// 
	//定义了一个记录类的 变量
	CSECMNGSRVCFG	srvCfgRecordSet(g_pDB);
	TRY
	{
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_ip");

		//相当于 向服务器发送了一个sql语句 并且结果已经返回
		if (!srvCfgRecordSet.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("打开表SECMNG.SRVCFG表失败");
			return;
		}

		if (!srvCfgRecordSet.IsEOF())
		{
			srvCfgRecordSet.m_KEY.TrimLeft(); srvCfgRecordSet.m_KEY.TrimRight();
			srvCfgRecordSet.m_VALUDE.TrimLeft(); srvCfgRecordSet.m_VALUDE.TrimRight();
			m_strsrvip = srvCfgRecordSet.m_VALUDE;
		}
		else
		{
			m_strsrvip = "";
		}


		//反复查询
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_port");
		srvCfgRecordSet.Requery();
		if (!srvCfgRecordSet.IsEOF())
		{
			srvCfgRecordSet.m_KEY.TrimLeft(); srvCfgRecordSet.m_KEY.TrimRight();
			srvCfgRecordSet.m_VALUDE.TrimLeft(); srvCfgRecordSet.m_VALUDE.TrimRight();
			m_strsrvport = srvCfgRecordSet.m_VALUDE;
		}
		else
		{
			m_strsrvport = "";
		}

		//反复查询 查询 secmng_server_maxnetnum
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_maxnetnum");
		srvCfgRecordSet.Requery();
		if (!srvCfgRecordSet.IsEOF())
		{
			srvCfgRecordSet.m_KEY.TrimLeft(); srvCfgRecordSet.m_KEY.TrimRight();
			srvCfgRecordSet.m_VALUDE.TrimLeft(); srvCfgRecordSet.m_VALUDE.TrimRight();
			m_strsrvnodenum = srvCfgRecordSet.m_VALUDE;
		}
		else
		{
			m_strsrvnodenum = "";
		}


	}
	CATCH_ALL(e)
	{
		e->ReportError();
		if (srvCfgRecordSet.IsOpen())
		{
			srvCfgRecordSet.Close();
		}
	}
	END_CATCH_ALL

	UpdateData(FALSE); //把C++的变量传递给界面

}


// 查询有没有数据
//1 当没有记录	新增记录
//2 当有记录时	 修改记录
void CCfgView::OnBnClickedButtonSaveserver()
{
	// TODO:  在此添加控件通知处理程序代码
	int			idbTag = 0; //成功
	//把界面输入 传送 到 C++变量中
	UpdateData(TRUE);
	m_strsrvip.TrimLeft(); m_strsrvip.TrimRight();
	m_strsrvport.TrimLeft(); m_strsrvport.TrimRight();
	m_strsrvnodenum.TrimLeft(); m_strsrvnodenum.TrimRight();

	if (m_strsrvip.IsEmpty() || m_strsrvport.IsEmpty() || m_strsrvnodenum.IsEmpty())
	{
		AfxMessageBox("用户输入为空");
	}

	g_pDB->BeginTrans();
	CSECMNGSRVCFG	srvCfgRecordSet(g_pDB);
	TRY
	{
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_ip");

		//相当于 向服务器发送了一个sql语句 并且结果已经返回
		if (!srvCfgRecordSet.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			idbTag = 1;
			g_pDB->Rollback();
			AfxMessageBox("打开表SECMNG.SRVCFG表失败");
			return;
		}

		if (!srvCfgRecordSet.IsEOF()) //有记录 则修改
		{
			srvCfgRecordSet.Edit();
			srvCfgRecordSet.m_VALUDE = m_strsrvip;
			srvCfgRecordSet.m_KEY.Format("%s", "secmng_server_ip");
			srvCfgRecordSet.Update();	
		}
		else
		{
			srvCfgRecordSet.AddNew();
			srvCfgRecordSet.m_VALUDE = m_strsrvip;
			srvCfgRecordSet.m_KEY.Format("%s", "secmng_server_ip");
			srvCfgRecordSet.Update();
		}

		//保存port
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_port");
		srvCfgRecordSet.Requery();
		if (!srvCfgRecordSet.IsEOF()) //有记录 则修改
		{
			srvCfgRecordSet.Edit();
			srvCfgRecordSet.m_VALUDE = m_strsrvport;
			srvCfgRecordSet.m_KEY.Format("%s", "secmng_server_port");
			srvCfgRecordSet.Update();
		}
		else
		{
			srvCfgRecordSet.AddNew();
			srvCfgRecordSet.m_VALUDE = m_strsrvport;
			srvCfgRecordSet.m_KEY.Format("%s", "secmng_server_port");
			srvCfgRecordSet.Update();
		}

		//网点配置 secmng_server_maxnetnum
		srvCfgRecordSet.m_strFilter.Format("key='%s'", "secmng_server_maxnetnum");
		srvCfgRecordSet.Requery();
		if (!srvCfgRecordSet.IsEOF()) //有记录 则修改
		{
			srvCfgRecordSet.Edit();
			srvCfgRecordSet.m_VALUDE = m_strsrvnodenum;
			srvCfgRecordSet.m_KEY.Format("%s", "secmng_server_maxnetnum");
			srvCfgRecordSet.Update();
		}
		else
		{
			srvCfgRecordSet.AddNew();
			srvCfgRecordSet.m_VALUDE = m_strsrvnodenum;
			srvCfgRecordSet.m_KEY.Format("%s", "secmng_server_maxnetnum");
			srvCfgRecordSet.Update();
		}
	}
	CATCH_ALL(e)
	{
		idbTag = 1;
		e->ReportError();
	}
	END_CATCH_ALL

	if (idbTag == 0)
	{
		g_pDB->CommitTrans();
	}
	else
	{
		g_pDB->Rollback();
	}
	if (srvCfgRecordSet.IsOpen())
	{
		srvCfgRecordSet.Close();
	}

}