#pragma once
#include "afxcmn.h"
#include "TabSheet.h"

#include "DlgTab1.h"
#include "DlgTab2.h"
#include "DlgTab3.h"



// CSysAll 窗体视图

class CSysAll : public CFormView
{
	DECLARE_DYNCREATE(CSysAll)

protected:
	CSysAll();           // 动态创建所使用的受保护的构造函数
	virtual ~CSysAll();

public:
	enum { IDD = IDD_DIALOG_SYSALL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CTabCtrl myTab;  //vc向导 只能 提供默认的类
	CTabSheet myTab;  //手工修改成 用户自定义类
	CDlgTab1		m_dlgTab1;
	CDlgTab2		m_dlgTab2;
	CDlgTab3		m_dlgTab3;

	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
};


