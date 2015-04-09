#include "stdafx.h"
#include "ZQCustomIOCPServer.h"
#include "ZQAcceptThread.h"
#include "ZQWorkThread.h"
#include "ZQCustomClient.h"



CZQCustomIOCPServer::CZQCustomIOCPServer()
{
	WSADATA  wsadata;
	WSAStartup(0x0202, &wsadata);
	m_isRunning = false;
	InitializeCriticalSection(&m_ClientCS);
}


CZQCustomIOCPServer::~CZQCustomIOCPServer()
{
	WSACleanup();
	DeleteCriticalSection(&m_ClientCS);
}

void CZQCustomIOCPServer::Initialize()
{
		m_port = 7100;
   //���ȴ���һ����ɶ˿�
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	//��Ϊ��ɶ˿���Ҫ���ɵĹ����߳�
	SYSTEM_INFO  systeminfo;
	GetSystemInfo(&systeminfo);
	m_AcceptThread = new CZQAcceptThread(this);
	m_iWorkThreadCount = systeminfo.dwNumberOfProcessors;
	m_WorkThread = new CZQWorkThread*[m_iWorkThreadCount];
	//������Ҫ���������߳�
	for (int i = 0; i < m_iWorkThreadCount; i++)
	{
		//���������߳�
		m_WorkThread[i] = new CZQWorkThread(this);

	}
	//����Ǽ�����SOCKET
	
	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socket != INVALID_SOCKET)
	{
		SOCKADDR_IN addr;
		memset(&addr, 0, sizeof(SOCKADDR_IN));
		addr.sin_family = PF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;

		WSAHtons(m_socket, m_port, &addr.sin_port);
		if (bind(m_socket, (sockaddr*)&addr, sizeof(SOCKADDR_IN)) == 0)
		{
			//�������ļ�������ʵ���Ͽ�����ACCEPTTHREAD������
			if (listen(m_socket, SOMAXCONN))
			{
				OutputDebugString("Listen function is Error!");
			}
			m_isRunning = true;
			m_AcceptThread->Initialize();
			for (int i = 0; i < m_iWorkThreadCount; i++)
			{
				//���������߳�
				m_WorkThread[i]->Initialize();

			}
		}
	}
}

void CZQCustomIOCPServer::Finalize()
{
	//PostQueuedCompletionStatus(m_hCompletionPort, 0xFFFFFFFF, 0, NULL);
	FreeAllClient();
	closesocket(m_socket);
	m_AcceptThread->Terminate();
	m_AcceptThread->WaitFor();
	delete(m_AcceptThread);
	PostQueuedCompletionStatus(m_hCompletionPort, 0, 0, LPOVERLAPPED(SHUTDOWN_FLAG));

	for (int i = 0; i < m_iWorkThreadCount; i++)
	{
		//���������߳�
		m_WorkThread[i]->Terminate();
		m_WorkThread[i]->WaitFor();
		delete(m_WorkThread[i]);

	}
	delete (m_WorkThread);

	if (m_hCompletionPort != INVALID_HANDLE_VALUE)
		CloseHandle(m_hCompletionPort);
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);
}

void CZQCustomIOCPServer::SocketAccept(SOCKET  s, sockaddr_in addr)
{
	if (s != INVALID_SOCKET)
	{
		//������Ҫ�����Զ���ķ�ʽ��д
		CZQCustomClient  * Client;
		if (m_CreateSocketClient)
			Client = m_CreateSocketClient();
		else
			Client = new CZQCustomClient();
		Client->m_socket = s;
		Client->m_IOCP = m_hCompletionPort;
		strcpy_s(Client->m_address, 16, inet_ntoa(addr.sin_addr));
		Client->m_Port = ntohs(addr.sin_port);
		EnterCriticalSection(&m_ClientCS);
		m_ClientList.push_back(Client);
		LeaveCriticalSection(&m_ClientCS);
		if (CreateIoCompletionPort((HANDLE)s, m_hCompletionPort, (ULONG_PTR)Client, 0) == 0)
		{
			OutputDebugString("function SocketAccept(SOCKET  s, sockaddr_in addr) at CreateIOColpletionPort is failed!");
		}
		else
		{
			Client->ReadyReviceNextData(&Client->m_ReviceData);
		}
	}
	
}

void CZQCustomIOCPServer::FreeClient(CZQCustomClient * client)
{
	std::list<CZQCustomClient*>::iterator it;
	for (it = m_ClientList.begin(); it != m_ClientList.end(); it++)
	{
		if (*it == client)
		{
			client->ForceClose();
			m_ClientList.erase(it);
			break;
		}

	}
}

void CZQCustomIOCPServer::FreeAllClient()
{
	std::list<CZQCustomClient*>::iterator it;
	
	for (it = m_ClientList.begin(); it != m_ClientList.end();)
	{
		EnterCriticalSection(&m_ClientCS);
		CZQCustomClient * client = *it;
		m_ClientList.erase(it);
		it = m_ClientList.begin();
		delete(client);
		LeaveCriticalSection(&m_ClientCS);

	}
	//m_ClientList.clear();
}

