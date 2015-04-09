#include "stdafx.h"
#include "ZQWorkThread.h"
#include "ZQCustomClient.h"


CZQWorkThread::CZQWorkThread()
{

}


CZQWorkThread::CZQWorkThread(CZQCustomIOCPServer * parentSocket)
{
	m_parentsocket = parentSocket;
}

CZQWorkThread::~CZQWorkThread()
{
}

void CZQWorkThread::Execute()
{
	DWORD dwBytestransferred;
	HANDLE  hIOCP = (HANDLE)m_parentsocket->m_hCompletionPort;
	SOCKET tsocket = INVALID_SOCKET;
	pBlock block = NULL;

	while (!Terminated())
	{
		DWORD NumberOfBytesRevced = 0;
		DWORD flag = 0;
		CZQCustomClient * client = NULL;

		BOOL ret = GetQueuedCompletionStatus(hIOCP, &dwBytestransferred, (PULONG_PTR)&client, (LPOVERLAPPED*)&block, INFINITE);

		if ((DWORD)block == SHUTDOWN_FLAG)
		{
			Terminate();
		}
		if (Terminated())
		{
			if (client != NULL)
				client->ForceClose();
			break;
		}
		if ((!ret) && dwBytestransferred == 0)
		{
			if (client != NULL)
				client->ForceClose();
			continue;
		}
// 		if ((DWORD)block == DISCONNECT_FLAG)
// 		{
// 			client->ForceClose();
// 			delete(client);
// 		}
		if (((DWORD)block == DISCONNECT_FLAG) && (client != NULL))
		{
			//释放这个客户端
			m_parentsocket->FreeClient(client);
			continue;
		}

		if ((dwBytestransferred != 0) && (ret))
		{
			switch (block->OperatorType)
			{
			case ioRead:
				client->DoRevice(block,dwBytestransferred);
				break;
			case ioWrite:
				client->DoSend(block, dwBytestransferred);
				break;
			default:
				break;
			}
		}
	}

	PostQueuedCompletionStatus(m_parentsocket->m_hCompletionPort, 0, 0, LPOVERLAPPED(SHUTDOWN_FLAG));
}