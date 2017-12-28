
// MyAdminView.h : CMyAdminView 类的接口
//

#pragma once


class CMyAdminView : public CView
{
protected: // 仅从序列化创建
	CMyAdminView();
	DECLARE_DYNCREATE(CMyAdminView)

// 特性
public:
	CMyAdminDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMyAdminView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MyAdminView.cpp 中的调试版本
inline CMyAdminDoc* CMyAdminView::GetDocument() const
   { return reinterpret_cast<CMyAdminDoc*>(m_pDocument); }
#endif

