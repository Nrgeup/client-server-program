
// UDPClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPClient.h"
#include "UDPClientDlg.h"
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


// CUDPClientDlg 对话框



CUDPClientDlg::CUDPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDPClientDlg::IDD, pParent)
	, m_ServerIp(_T(""))
	, m_ServerPort(2000)
	, m_ServerReceive(_T(""))
	, m_command(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, m_ServerIp);
	DDX_Text(pDX, IDC_HOST, m_ServerPort);
	DDX_Text(pDX, IDC_RECEIVE, m_ServerReceive);
	DDX_Text(pDX, IDC_COMMAND, m_command);
}

BEGIN_MESSAGE_MAP(CUDPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSEND, &CUDPClientDlg::OnClickedIdsend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUDPClientDlg 消息处理程序

BOOL CUDPClientDlg::OnInitDialog()
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

	if (!AfxSocketInit()) {//初始化
		MessageBox(L"初始化失败", L"提示", MB_OK|MB_ICONSTOP);
	}

	//随机产生一个客户端口号
	srand((unsigned)time(NULL));
	int max=RAND_MAX;
	int client_port=(int)(rand()*(5000)/max+2002);  


	if (!m_mysocket.Create(client_port, SOCK_DGRAM, FD_READ)) {//以数据报方式创建socket
		MessageBox(L"Socket套接字创建失败", L"错误", MB_OK|MB_ICONSTOP);
	};

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDPClientDlg::OnPaint()
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
HCURSOR CUDPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPClientDlg::OnClickedIdsend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);	//更新控件对应的变量值
	if(m_ServerIp.IsEmpty())
	{MessageBox(_T("无服务器Ip地址！"),_T("错误！"),MB_OK|MB_ICONEXCLAMATION); return ;}
	if(m_ServerPort <=0 || m_ServerPort> 65535)
	{MessageBox(_T("无请求命令！"),_T("错误！"),MB_OK|MB_ICONEXCLAMATION); return ;}
	if(m_ServerPort <=0 || m_ServerPort> 65535)
	{MessageBox(_T("端口值不正确！"),_T("错误！"),MB_OK|MB_ICONEXCLAMATION);return;}

	//向服务器发送数据报  UDP方式
	int flag=m_mysocket.SendToEx(m_command.GetBuffer(),(m_command.GetLength() + 1) * sizeof(WCHAR),m_ServerPort,m_ServerIp);
	if (flag == SOCKET_ERROR) {
		MessageBox(_T("向服务器发送请求失败！"),_T("错误！"),MB_OK|MB_ICONSTOP); return;
	}
}


void CUDPClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_mysocket.Close();		//关闭套接口
	CDialogEx::OnClose();
}
