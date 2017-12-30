// DlgNetInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgNetInfo.h"
#include "SECMNGSECNODE.h"


// CDlgNetInfo

IMPLEMENT_DYNCREATE(CDlgNetInfo, CFormView)

CDlgNetInfo::CDlgNetInfo()
	: CFormView(CDlgNetInfo::IDD)
	, m_dateBegin(COleDateTime::GetCurrentTime())
	, m_dateEnd(COleDateTime::GetCurrentTime())
{

}

CDlgNetInfo::~CDlgNetInfo()
{
}

void CDlgNetInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECNODE, m_listSecNode);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dateBegin);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_dateEnd);
}

BEGIN_MESSAGE_MAP(CDlgNetInfo, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_NETSAVE, &CDlgNetInfo::OnBnClickedButtonNetsave)
	ON_BN_CLICKED(IDC_BUTTON_NETSEARCH, &CDlgNetInfo::OnBnClickedButtonNetsearch)
END_MESSAGE_MAP()


// CDlgNetInfo 诊断

#ifdef _DEBUG
void CDlgNetInfo::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgNetInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgNetInfo 消息处理程序

/* 
typedef struct tagLVITEMA
{
	UINT mask; //显示方式
	int iItem;  //第几行数据
	int iSubItem; //列的数组的下表
	UINT state;
	UINT stateMask;
	LPSTR pszText;  //节点新
	int cchTextMax;
	int iImage; //图标的数组下表
	LPARAM lParam;
	int iIndent;
#if (NTDDI_VERSION >= NTDDI_WINXP)
	int iGroupId;
	UINT cColumns; // tile view columns
	PUINT puColumns;
#endif
#if (NTDDI_VERSION >= NTDDI_VISTA) // Will be unused downlevel, but sizeof(LVITEMA) must be equal to sizeof(LVITEMW)
	int* piColFmt;
	int iGroup; // readonly. only valid for owner data.
#endif
} LVITEMA, *LPLVITEMA;
*/
int CDlgNetInfo::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode)
{
	CString  strTime;

	LVITEM   lvi;
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0;	//在第几行上插入数据 始终头插法
	lvi.iImage = 4;

	//插入第0列数据
	lvi.iSubItem = 0;		// Set subitem 0
	lvi.pszText = (LPTSTR)(LPCTSTR)ID;
	m_listSecNode.InsertItem(&lvi);

	////插入第1列数据
	lvi.iSubItem = 1;		// Set subitem 1
	lvi.pszText = (LPTSTR)(LPCTSTR)Name;
	m_listSecNode.SetItem(&lvi);

	//插入第2列数据
	strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.iSubItem = 2;		// Set subitem 2
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	//插入第3列数据
	lvi.iSubItem = 3;		// Set subitem 3
	if (state == 1)
	{
		lvi.pszText = "禁用";

	}
	else {
		lvi.pszText = "正常";
	}
	m_listSecNode.SetItem(&lvi);


	//插入第4列数据
	lvi.iSubItem = 4;		// Set subitem 4
	//CString strAuthcode(authcode) ;
	char buf[100];
	sprintf(buf, "%d", authcode);
	lvi.pszText = buf;
	m_listSecNode.SetItem(&lvi);


	return 0;
}

void CDlgNetInfo::OnBnClickedButtonNetsave()
{
	// TODO:  在此添加控件通知处理程序代码
	CString ID = "1111";
	CString Name = "myname";

				//COleDateTime::GetCurrentTime()
	CTime time = COleDateTime::GetCurrentTime();
	int state = 0;
	int authcode = 1111;

	DbInitListSecNode(ID, Name, time, state, authcode);
}


void CDlgNetInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//m_listSecNode;

	HIMAGELIST hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_imageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP_SECNODE);
	m_imageList.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	m_listSecNode.SetImageList(&m_imageList, LVSIL_SMALL);

	DWORD dwExStyle = ListView_GetExtendedListViewStyle(m_listSecNode.m_hWnd);
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_listSecNode.m_hWnd, dwExStyle);

	CRect rect; //msdn
	m_listSecNode.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_listSecNode.SetRedraw(FALSE);
	m_listSecNode.InsertColumn(0, "网点编号", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "网点名称", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "网点创建时间", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "网点状态", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "网点授权码", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
	m_listSecNode.SetRedraw(TRUE);


	// TODO:  在此添加专用代码和/或调用基类
}

extern CDatabase	*g_pDB;

void CDlgNetInfo::OnBnClickedButtonNetsearch()
{
	int			dbtag = 0;
	CWnd		*myWnd = NULL;
	CButton		*But = NULL;
	int			rv = 0, tag = 0;//表示没有检索到记录
	int			dbTag = 0; //数据库操作是否失败0成功
	CString		strFilter;

	UpdateData(TRUE); //把界面的数据 传递给 C++变量

	myWnd = CWnd::GetDlgItem(IDC_CHECK_TIME);
	But = (CButton *)myWnd;
	if (But->GetCheck() == BST_UNCHECKED)
	{
		AfxMessageBox("没有选中时间 复选框");
		return;
	}

	//定义了一个记录类的 变量
	CSECMNGSECNODE	secNodeRSet(g_pDB);
	TRY
	{
		//secNodeRSet.m_strFilter.Format("key='%s'", "secmng_server_ip");
		CTime myTime1(m_dateBegin.GetYear(), m_dateBegin.GetMonth(), m_dateBegin.GetDay(), 0, 0, 0);
		CTime myTime2(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(), 23, 59, 59);
		if (myTime1 >= myTime2)
		{
			MessageBox("开始时间不能大于结束时间！", "时间查询", MB_MODEMASK);
			return;
		}


		//相当于 向服务器发送了一个sql语句 并且结果已经返回
		if (!secNodeRSet.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("打开表SECMNG.SECNODE 网点信息表失败");
			return;
		}
		
		//向空间 挂数据之前 情况控件
		m_listSecNode.DeleteAllItems();

		while (!secNodeRSet.IsEOF())
		{
			//如果你获取的时间 比大的大,比小的小 ; 说明这个记录不是我们想要的记录
			if (secNodeRSet.m_CREATETIME > myTime2 ||
				secNodeRSet.m_CREATETIME < myTime1)
			{
				secNodeRSet.MoveNext();
				continue;
			}
			
			secNodeRSet.m_ID.TrimLeft();		secNodeRSet.m_ID.TrimRight();
			secNodeRSet.m_NAME.TrimLeft();		secNodeRSet.m_NAME.TrimRight();
			secNodeRSet.m_NODEDESC.TrimLeft();	secNodeRSet.m_NODEDESC.TrimRight();
			//secNodeRSet.m_CREATETIME;
			//secNodeRSet.m_AUTHCODE;  //编译器给我自动生成的 我们可以修改 微调
			//secNodeRSet.m_STATE;
			DbInitListSecNode(secNodeRSet.m_ID, secNodeRSet.m_NAME, secNodeRSet.m_CREATETIME,
				secNodeRSet.m_STATE, secNodeRSet.m_AUTHCODE);	
			secNodeRSet.MoveNext();
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		if (secNodeRSet.IsOpen())
		{
			secNodeRSet.Close();
		}
	}
	END_CATCH_ALL
}
