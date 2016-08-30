#pragma once

// MySocket ÃüÁîÄ¿±ê

class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();
	virtual void OnReceive(int nErrorCode);
};


