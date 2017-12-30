
// MyAdminView.cpp : CMyAdminView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MyAdmin.h"
#endif

#include "MyAdminDoc.h"
#include "MyAdminView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyAdminView

IMPLEMENT_DYNCREATE(CMyAdminView, CView)

BEGIN_MESSAGE_MAP(CMyAdminView, CView)
END_MESSAGE_MAP()

// CMyAdminView 构造/析构

CMyAdminView::CMyAdminView()
{
	// TODO:  在此处添加构造代码

}

CMyAdminView::~CMyAdminView()
{
}

BOOL CMyAdminView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMyAdminView 绘制

void CMyAdminView::OnDraw(CDC* /*pDC*/)
{
	CMyAdminDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
}


// CMyAdminView 诊断

#ifdef _DEBUG
void CMyAdminView::AssertValid() const
{
	CView::AssertValid();
}

void CMyAdminView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyAdminDoc* CMyAdminView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyAdminDoc)));
	return (CMyAdminDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyAdminView 消息处理程序
