
// UDPServerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MySocket.h"


// CUDPServerDlg 对话框
class CUDPServerDlg : public CDialogEx
{
// 构造
public:
	CUDPServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UDPSERVER_DIALOG };

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
	CString m_time;		//当前服务器的时间
	CString m_date;		//当前服务器的日期
	int m_port;			//当前服务器的端口
	CComboBox m_con_ip;	//当前服务器的IP地址
	CString m_ip;		//当前服务器的IP地址
	CListBox m_con_List;
	MySocket m_mysocket;
	void update_date_and_time(CString& date, CString& time);
	afx_msg void OnClose();
};
