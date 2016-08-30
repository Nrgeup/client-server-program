// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPClient.h"
#include "UDPClientDlg.h"
#include "MySocket.h"


// CMySocket

MySocket::MySocket()
{
	//初始化
	GetBuff.Empty();
	GetIp.Empty();
	GetPort=0;
}

MySocket::~MySocket()
{
}


// CMySocket 成员函数


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	TCHAR buff[4096];  //用于缓冲区接收主机返回的数据报
	ReceiveFrom(buff, sizeof(buff), GetIp, GetPort);	//从主机接收数据
	CString data(buff);
	GetBuff=data;

	CUDPClientDlg* dig = (CUDPClientDlg*)theApp.m_pMainWnd;
	dig->m_ServerReceive=GetBuff;
	dig->UpdateData(false);

	CAsyncSocket::OnReceive(nErrorCode);
}
