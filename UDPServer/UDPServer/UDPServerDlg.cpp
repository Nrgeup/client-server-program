
// UDPServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDPServer.h"
#include "UDPServerDlg.h"
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


// CUDPServerDlg �Ի���



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


// CUDPServerDlg ��Ϣ�������

BOOL CUDPServerDlg::OnInitDialog()
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
	
	update_date_and_time(m_date,m_time);//�������ں�ʱ��

	if (!AfxSocketInit()) {//��ʼ��
		MessageBox(L"��ʼ��ʧ��", L"��ʾ", MB_OK|MB_ICONSTOP);
	}
	update_date_and_time(m_date,m_time);//�������ں�ʱ��
	m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+_T("Socket��ʼ���ɹ���"));


	//��ȡ�������Ϳ���IP��ַ
	
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
	record.Format(L"��ǰ��ȡ���������ǣ� �� %s ��", hostname_cstring);
	update_date_and_time(m_date,m_time);//�������ں�ʱ��
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
		record.Format(L"��ǰ������ %d ������IP��ַ�ǣ� �� %s ��", i, pw);
		update_date_and_time(m_date,m_time);//�������ں�ʱ��
		m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+record);
	}
	if (i == 0) {
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	UpdateData(false);
	m_con_ip.SetCurSel(0);
	UpdateData(true);

	//��������������

	if(m_ip.IsEmpty())
	{MessageBox(_T("�޷�����Ip��ַ��"),_T("����"),MB_OK|MB_ICONEXCLAMATION); return TRUE;}
	if(m_port <=0 || m_port> 65535)
	{MessageBox(_T("�˿�ֵ���ô���"),_T("����"),MB_OK|MB_ICONEXCLAMATION); return TRUE;}

	UpdateData(true);
	if (!m_mysocket.Create(m_port, SOCK_DGRAM, FD_READ,m_ip)) {//�����ݱ���ʽ����socket
		MessageBox(L"Socket�׽��ִ���ʧ��", L"����", MB_OK|MB_ICONSTOP);return TRUE;
	};
	record.Format(L"�����������ɹ�����ǰ����IP�ǣ��� %s �����˿ں��ǣ��� %u ����", m_ip,m_port);
	update_date_and_time(m_date,m_time);//�������ں�ʱ��
	m_con_List.AddString(m_date+_T(" ")+m_time+_T(":")+record);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUDPServerDlg::OnPaint()
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
HCURSOR CUDPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���µ�ǰʱ�̵����ں�ʱ��ֵ
void CUDPServerDlg::update_date_and_time(CString& date, CString& time)
{
	date=CTime::GetCurrentTime().Format(L"%Y/%m/%d");
	time=CTime::GetCurrentTime().Format("%H:%M:%S");
	UpdateData(false);
}


void CUDPServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_mysocket.Close();
	CDialogEx::OnClose();
}
