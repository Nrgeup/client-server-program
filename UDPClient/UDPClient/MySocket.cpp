// MySocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDPClient.h"
#include "UDPClientDlg.h"
#include "MySocket.h"


// CMySocket

MySocket::MySocket()
{
	//��ʼ��
	GetBuff.Empty();
	GetIp.Empty();
	GetPort=0;
}

MySocket::~MySocket()
{
}


// CMySocket ��Ա����


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	TCHAR buff[4096];  //���ڻ����������������ص����ݱ�
	ReceiveFrom(buff, sizeof(buff), GetIp, GetPort);	//��������������
	CString data(buff);
	GetBuff=data;

	CUDPClientDlg* dig = (CUDPClientDlg*)theApp.m_pMainWnd;
	dig->m_ServerReceive=GetBuff;
	dig->UpdateData(false);

	CAsyncSocket::OnReceive(nErrorCode);
}
