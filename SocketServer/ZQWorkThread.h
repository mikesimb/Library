#pragma once
#include "ZQThread.h"
#include "ZQCustomIOCPServer.h"
class CZQWorkThread :
	public CZQThread
{
	friend class CZQCustomIOCPServer;
public:
	CZQWorkThread();
	CZQWorkThread(CZQCustomIOCPServer * parentSocket);
	virtual ~CZQWorkThread();
protected:
	virtual void Execute();

private:
	CZQCustomIOCPServer * m_parentsocket;
};

