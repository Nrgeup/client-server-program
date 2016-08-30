
// UDPClientDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"

// CUDPClientDlg �Ի���
class CUDPClientDlg : public CDialogEx
{
// ����
public:
	CUDPClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UDPCLIENT_DIALOG };

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
	CString m_ServerIp;	//��������IP��ַ
	int m_ServerPort;	//�������Ķ˿�ֵ
	CString m_ServerReceive;	//��������Ӧ�����ݱ�
	CString m_command;			//����������������
	MySocket m_mysocket;
	afx_msg void OnClickedIdsend();
	afx_msg void OnClose();
	
};
