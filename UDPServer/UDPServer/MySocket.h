#pragma once

// MySocket ����Ŀ��

class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();
	virtual void OnReceive(int nErrorCode);
};


