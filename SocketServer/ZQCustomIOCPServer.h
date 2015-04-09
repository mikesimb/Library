#pragma once
#include <WinSock2.h>
#include <list>
#include <functional>

class CZQCustomClient;

typedef std::function<void()>  OnSocketNotifyEvent;
typedef std::function<void(CZQCustomClient * client)>  OnSocketConnectNotifyEvent;
typedef std::function<void(char * buf, int buflen)>  OnSocketReadNotifyEvent;
typedef std::function<void(char * buf, int buflen)>  OnSocketWriteNotifyEvent;
typedef std::function<CZQCustomClient * ()> OnCreateSocketClientNotifyEvent;


#pragma comment(lib,"ws2_32.lib")

#define MAX_BUFFER_LEN     4096
#define DISCONNECT_FLAG   0xFFFFFFFE
#define SHUTDOWN_FLAG    0xFFFFFFFF

typedef enum {ioNone,ioRead,ioWrite}  IoOperator;

typedef struct  __Blocktag
{
	WSAOVERLAPPED  overloapped;
	WSABUF     buf;
	char     MsgBuf[MAX_BUFFER_LEN];
	IoOperator OperatorType;
}Block,*pBlock;

class CZQCustomIOCPServer
{

	friend class CZQAcceptThread;
	friend class CZQWorkThread;
	friend class CZQCustomClient;
public:
	CZQCustomIOCPServer();
	virtual ~CZQCustomIOCPServer();
    
	virtual void Initialize();
	virtual void Finalize();
protected:
	void SocketAccept(SOCKET  s, sockaddr_in addr);
	void FreeClient(CZQCustomClient * client);
	void FreeAllClient();

//	virtual  CZQCustomClient * OnCreateClient();
	OnCreateSocketClientNotifyEvent  m_CreateSocketClient;
private:
	 //·þÎñÆ÷µÄ¼àÌýSOCKET;
	SOCKET  m_socket;
	char      m_address[16];
	int         m_port;
	HANDLE   m_hCompletionPort;
	int   m_iWorkThreadCount;
	pBlock   m_pBlock;
	bool m_isRunning;

	CRITICAL_SECTION  m_ClientCS;

	CZQAcceptThread * m_AcceptThread;
	CZQWorkThread ** m_WorkThread;

	std::list<CZQCustomClient * > m_ClientList;

	
};

