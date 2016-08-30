// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPServer.h"
#include "MySocket.h"
#include "UDPServerDlg.h"

// MySocket

MySocket::MySocket()
{
}

MySocket::~MySocket()
{
}


// MySocket 成员函数


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CUDPServerDlg* dig = (CUDPServerDlg*)theApp.m_pMainWnd; //获取对话框句柄
	TCHAR buff[4096];
	CString GetIP;
	UINT GetPort;
	ReceiveFrom(buff, 4096, GetIP, GetPort);
	CString data(buff);
	CString temp=data;

	dig->update_date_and_time(dig->m_date,dig->m_time);

	CString Answer=_T("错误请求");
	data.MakeLower();
	
	if(data=="time")
		Answer=dig->m_time;
	if(data=="date")
		Answer=dig->m_date;

	//响应客户端程序
	SendToEx(Answer.GetBuffer(), (Answer.GetLength() + 1) * sizeof(WCHAR), GetPort, GetIP);
	CString record;
	record.Format(L":收到IP=%s Port=%u请求【%s】,响应【%s】", GetIP, GetPort,temp,Answer);
	dig->m_con_List.AddString(dig->m_date+_T(" ")+dig->m_time+record);

	CAsyncSocket::OnReceive(nErrorCode);
}
