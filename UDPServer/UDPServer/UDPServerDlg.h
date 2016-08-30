
// UDPServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MySocket.h"


// CUDPServerDlg �Ի���
class CUDPServerDlg : public CDialogEx
{
// ����
public:
	CUDPServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UDPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_time;		//��ǰ��������ʱ��
	CString m_date;		//��ǰ������������
	int m_port;			//��ǰ�������Ķ˿�
	CComboBox m_con_ip;	//��ǰ��������IP��ַ
	CString m_ip;		//��ǰ��������IP��ַ
	CListBox m_con_List;
	MySocket m_mysocket;
	void update_date_and_time(CString& date, CString& time);
	afx_msg void OnClose();
};
