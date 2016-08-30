
// UDPClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDPClient.h"
#include "UDPClientDlg.h"
#include "afxdialogex.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CUDPClientDlg �Ի���



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


// CUDPClientDlg ��Ϣ�������

BOOL CUDPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	if (!AfxSocketInit()) {//��ʼ��
		MessageBox(L"��ʼ��ʧ��", L"��ʾ", MB_OK|MB_ICONSTOP);
	}

	//�������һ���ͻ��˿ں�
	srand((unsigned)time(NULL));
	int max=RAND_MAX;
	int client_port=(int)(rand()*(5000)/max+2002);  


	if (!m_mysocket.Create(client_port, SOCK_DGRAM, FD_READ)) {//�����ݱ���ʽ����socket
		MessageBox(L"Socket�׽��ִ���ʧ��", L"����", MB_OK|MB_ICONSTOP);
	};

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUDPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUDPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPClientDlg::OnClickedIdsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);	//���¿ؼ���Ӧ�ı���ֵ
	if(m_ServerIp.IsEmpty())
	{MessageBox(_T("�޷�����Ip��ַ��"),_T("����"),MB_OK|MB_ICONEXCLAMATION); return ;}
	if(m_ServerPort <=0 || m_ServerPort> 65535)
	{MessageBox(_T("���������"),_T("����"),MB_OK|MB_ICONEXCLAMATION); return ;}
	if(m_ServerPort <=0 || m_ServerPort> 65535)
	{MessageBox(_T("�˿�ֵ����ȷ��"),_T("����"),MB_OK|MB_ICONEXCLAMATION);return;}

	//��������������ݱ�  UDP��ʽ
	int flag=m_mysocket.SendToEx(m_command.GetBuffer(),(m_command.GetLength() + 1) * sizeof(WCHAR),m_ServerPort,m_ServerIp);
	if (flag == SOCKET_ERROR) {
		MessageBox(_T("���������������ʧ�ܣ�"),_T("����"),MB_OK|MB_ICONSTOP); return;
	}
}


void CUDPClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_mysocket.Close();		//�ر��׽ӿ�
	CDialogEx::OnClose();
}
