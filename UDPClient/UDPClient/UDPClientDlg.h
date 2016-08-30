
// UDPClientDlg.h : 头文件
//

#pragma once
#include "MySocket.h"

// CUDPClientDlg 对话框
class CUDPClientDlg : public CDialogEx
{
// 构造
public:
	CUDPClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UDPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_ServerIp;	//服务器的IP地址
	int m_ServerPort;	//服务器的端口值
	CString m_ServerReceive;	//服务器响应的数据报
	CString m_command;			//向服务器请求的命令
	MySocket m_mysocket;
	afx_msg void OnClickedIdsend();
	afx_msg void OnClose();
	
};
