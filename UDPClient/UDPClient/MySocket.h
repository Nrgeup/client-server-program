#pragma once

// CMySocket ����Ŀ��
#include "MySocket.h"

class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();
	virtual void OnReceive(int nErrorCode);

public:
	CString  GetBuff;	//��Ŵ��������ܵ����ݱ�
	CString  GetIp;		//��Ŵ��������ܵ�IP��ַ
	UINT	 GetPort;	//��Ŵ��������ܵĶ˿ں�
};


