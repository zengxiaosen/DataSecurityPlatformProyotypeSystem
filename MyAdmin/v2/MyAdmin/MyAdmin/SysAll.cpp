// SysAll.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "SysAll.h"


// CSysAll

IMPLEMENT_DYNCREATE(CSysAll, CFormView)

CSysAll::CSysAll()
	: CFormView(CSysAll::IDD)
{

}

CSysAll::~CSysAll()
{
}

void CSysAll::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MYTAB, myTab);
}

BEGIN_MESSAGE_MAP(CSysAll, CFormView)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSysAll 诊断

#ifdef _DEBUG
void CSysAll::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSysAll::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSysAll 消息处理程序

// 子类的OnInitialUpdate 什么时候被MFC框架调用呐?
void CSysAll::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	myTab.AddPage(_T("创建共享内存"), &m_dlgTab1, IDD_DIALOG_TAB1);
	myTab.AddPage(_T("创建共享内存"), &m_dlgTab2, IDD_DIALOG_TAB2);
	myTab.AddPage(_T("创建共享内存"), &m_dlgTab3, IDD_DIALOG_TAB3);
	myTab.Show();


	// TODO:  在此添加专用代码和/或调用基类
}

class MyBase
{
public:
	virtual int myPrintf()
	{
		printf("我是爹\n");
		return 0;

	}
};


class MyChild : public MyBase
{
public:
	virtual int myPrintf()
	{
		printf("我是儿子\n");
		return 0;
	}
};

void howToPrint(MyBase *base)
{
	base->myPrintf();
}


void myMain()
{
	MyChild c1;
	howToPrint(&c1);

	MyBase  b1;
	howToPrint(&b1);
}


void CSysAll::OnPaint()
{
	

	CPaintDC dc(this); // device context for painting

	CRect rect, rect1;
	this->GetClientRect(&rect);
	myTab.GetClientRect(&rect1);

	if ((rect1.Width() < rect.Width()))
		myTab.MoveWindow(rect, FALSE);

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()


}
