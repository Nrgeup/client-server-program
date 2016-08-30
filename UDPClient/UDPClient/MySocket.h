#pragma once

// CMySocket 命令目标
#include "MySocket.h"

class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();
	virtual void OnReceive(int nErrorCode);

public:
	CString  GetBuff;	//存放从主机接受的数据报
	CString  GetIp;		//存放从主机接受的IP地址
	UINT	 GetPort;	//存放从主机接受的端口号
};


