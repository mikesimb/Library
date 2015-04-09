#include "stdafx.h"
#include "ZQAcceptThread.h"
#include "ZQCustomIOCPServer.h"


CZQAcceptThread::CZQAcceptThread(CZQCustomIOCPServer * ParentSocket)
{
	m_ParentSocket = ParentSocket;
}


CZQAcceptThread::CZQAcceptThread()
{

}

CZQAcceptThread::~CZQAcceptThread()
{
}

void CZQAcceptThread::Execute()
{
	
	while (!Terminated())
	{
		if (m_ParentSocket->m_socket == INVALID_SOCKET)
		{
			OutputDebugString("m_ParentSocket->m_Socket  is INVALID_SOCKET");
			break;
		}
		SOCKADDR_IN   ToAddr;
		memset(&ToAddr, 0, sizeof(SOCKADDR_IN));
	//	ToAddr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
		int AddrLen = sizeof(SOCKADDR_IN);
		SOCKET  tSocket = WSAAccept(m_ParentSocket->m_socket, (sockaddr*)&ToAddr, &AddrLen,NULL,NULL);
		if (tSocket == 0xFFFFFFFF)
			continue;

	//	m_ParentSocket->SocketAccept(tSocket,ToAddr);
		if (tSocket != INVALID_SOCKET)
		{
			m_ParentSocket->SocketAccept(tSocket, ToAddr);
		}

		//这里的DATA一定是需要分配在堆上的
		//如果一定要用指针可以这样用
		/*
		lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(PER_IO_OPERATION_DATA));
		lpPerIOData->Buffer.len = MSGSIZE;
		lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
		lpPerIOData->OperationType = RECV_POSTED;
		那么在释放的时候也可以这样
		HeapFree(GetProcessHeap(), 0, lpPerIOData);
		*/
		//data = (pBlock)malloc(sizeof(Block));



		
	}
	PostQueuedCompletionStatus(m_ParentSocket->m_hCompletionPort, 0, 0, LPOVERLAPPED(SHUTDOWN_FLAG));

}
