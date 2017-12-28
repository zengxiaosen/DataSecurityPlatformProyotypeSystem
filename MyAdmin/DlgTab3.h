#pragma once


// CDlgTab3 对话框

class CDlgTab3 : public CDialog
{
	DECLARE_DYNAMIC(CDlgTab3)

public:
	CDlgTab3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTab3();

// 对话框数据
	enum { IDD = IDD_DIALOG_TAB3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
