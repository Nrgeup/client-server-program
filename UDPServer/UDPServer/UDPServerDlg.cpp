
// UDPServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPServer.h"
#include "UDPServerDlg.h"
#include "afxdialogex.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CUDPServerDlg 对话框



CUDPServerDlg::CUDPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDPServerDlg::IDD, pParent)
	, m_time(_T(""))
	, m_date(_T(""))
	, m_port(2000)
	, m_ip(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME, m_time);
	DDX_Text(pDX, IDC_DATE, m_date);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Control(pDX, IDC_IP, m_con_ip);
	DDX_CBString(pDX, IDC_IP, m_ip);
	DDX_Control(pDX, IDC_LIST, m_con_List);
}

BEGIN_MESSAGE_MAP(CUDPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUDPServerDlg 消息处理程序

BOOL CUDPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	update_date_and_time(m_date,m_time);//更新日期和时间

	if (!AfxSocketInit()) {//初始化
		MessageBox(L"初始化失败", L"提示", MB_OK|MB_ICONSTOP);
	}
	update_date_and_time(m_date,m_time);//更新日期和时间
	m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+_T("Socket初始化成功！"));


	//获取主机名和可用IP地址
	
	char hostname[20] = "";
	int errorcode;
	if ((errorcode = gethostname(hostname, sizeof(hostname))) != 0) {
		char char_error[100];
		_itoa_s(errorcode, char_error, 10);
		MessageBoxA(this->GetSafeHwnd(), char_error, "errorcode", MB_OK);
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	};

	CString hostname_cstring(hostname);
	CString record;
	record.Format(L"当前获取的主机名是： 【 %s 】", hostname_cstring);
	update_date_and_time(m_date,m_time);//更新日期和时间
	m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+record);

	hostent *hn;
	hn = gethostbyname(hostname);
	int i = 0;
	while (hn->h_addr_list[i] != 0) {
		char *p = inet_ntoa(*(in_addr *)hn->h_addr_list[i++]);
		wchar_t pw[20];
		SHAnsiToUnicode(p, pw, 20);
		CString str;
		str.Format(L"%s", pw);
		m_con_ip.AddString(str);
		record.Format(L"当前主机第 %d 个可用IP地址是： 【 %s 】", i, pw);
		update_date_and_time(m_date,m_time);//更新日期和时间
		m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+record);
	}
	if (i == 0) {
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	UpdateData(false);
	m_con_ip.SetCurSel(0);
	UpdateData(true);

	//启动服务器操作

	if(m_ip.IsEmpty())
	{MessageBox(_T("无服务器Ip地址！"),_T("错误！"),MB_OK|MB_ICONEXCLAMATION); return TRUE;}
	if(m_port <=0 || m_port> 65535)
	{MessageBox(_T("端口值设置错误！"),_T("错误！"),MB_OK|MB_ICONEXCLAMATION); return TRUE;}

	UpdateData(true);
	if (!m_mysocket.Create(m_port, SOCK_DGRAM, FD_READ,m_ip)) {//以数据报方式创建socket
		MessageBox(L"Socket套接字创建失败", L"错误", MB_OK|MB_ICONSTOP);return TRUE;
	};
	record.Format(L"启动服务器成功！当前主机IP是：【 %s 】，端口号是：【 %u 】！", m_ip,m_port);
	update_date_and_time(m_date,m_time);//更新日期和时间
	m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+record);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDPServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUDPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//更新当前时刻的日期和时刻值
void CUDPServerDlg::update_date_and_time(CString& date, CString& time)
{
	date=CTime::GetCurrentTime().Format(L"%Y/%m/%d");
	time=CTime::GetCurrentTime().Format("%H:%M:%S");
	UpdateData(false);
}


void CUDPServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_mysocket.Close();
	CDialogEx::OnClose();
}
