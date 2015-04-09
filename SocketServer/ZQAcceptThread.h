#pragma once
#include "ZQThread.h"
#include "ZQCustomIOCPServer.h"
#include "ZQCustomClient.h"
class CZQAcceptThread :
	public CZQThread
{
	friend class CZQCustomIOCPServer;
public:
	CZQAcceptThread();
	CZQAcceptThread(CZQCustomIOCPServer * ParentSocket);
	virtual ~CZQAcceptThread();
protected:
	virtual void Execute();
	 CZQCustomIOCPServer * m_ParentSocket;


};

