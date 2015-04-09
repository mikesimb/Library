#pragma once

/*
这是一个线程类CZQThread.
作者：张在伟
该线程中目前不具备线程同步能力
*/

#include <windows.h>
#include <string>

#define Max_Wait_for_Timeout    6000



class CZQThread
{
public:
	CZQThread();
	virtual ~CZQThread();

	

public:
	//启动线程
	BOOL Initialize(BOOL bSuspended = FALSE, unsigned int uStackSize = 0);

	//线程挂起
	BOOL Suspend(void);

	//恢复线程
	BOOL Resume(void);

	//终止线程
	void Terminate(void);

	//等待线程
	void WaitFor(const DWORD dwTimeOut = Max_Wait_for_Timeout);

	const char * getThreadID() { return (char *)m_dwThreadID; }
	const char * getThreadHandle(){ return (char *)m_hThread; }
	//线程是否终止
	BOOL Terminated(void);

	//设置线程名称
	void SetThreadName(char * name);



protected:


	//执行过程
	virtual void Execute(void) = 0;
	//终止事件
	virtual void OnTerminate(void) { OutputDebugString(ThreadName.c_str()); };

	volatile BOOL m_bTerminated;

private:
	//线程ID
	DWORD m_dwThreadID;
	//线程句柄
	HANDLE m_hThread;
	//当前是否挂起
	BOOL m_bIsSuspended;
	//线程同步事件

	//线程的名字
	std::string ThreadName;

	//线程过程
	static unsigned WINAPI ThreadProc(LPVOID pParam);
};

