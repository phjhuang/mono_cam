// SocketEx.h: interface for the CSocketEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETEX_H__84D1B237_B1C9_4ECF_8B92_16D75A3EC336__INCLUDED_)
#define AFX_SOCKETEX_H__84D1B237_B1C9_4ECF_8B92_16D75A3EC336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"
#include <stdio.h>
#include "time.h" //有没有用？
#pragma comment(lib,"ws2_32.lib")


#define ERR_SUCCESS  0x00000000	// Successful
#define ERR_BADPARAM 0x80000001	// Bad argument passed
#define ERR_WSAERROR 0x80000002	// Need to get_LastError()
#define ERR_MAXLENGTH 512

class CNetTools  
{
public:
	CNetTools();
	virtual ~CNetTools();
	
	static bool NT_InitEnv();//初始化网络环境
	void NT_UnInitEnv();//关闭网络环境
	
	static SOCKET NT_Connect(LPCSTR pstrHost, int nPort);//同步连接
	static SOCKET NT_Connect2(LPCSTR pstrHost, int nPort,int m_nTimeOut);//异步连接
	static bool NT_DisConnect(SOCKET s);//断开连接
	static int NT_GetStatus(SOCKET s);//得到连接的状态，异常断线状态无法获得
	
	static int NT_Write(SOCKET s,char* buf, int len); //发送函数
	static int NT_Read(SOCKET s,char FAR* buf, int len);//接收函数
	
};

#endif // !defined(AFX_SOCKETEX_H__84D1B237_B1C9_4ECF_8B92_16D75A3EC336__INCLUDED_)
//static int ReadEx2(SOCKET fd,char *bp,int len);



//static BOOL IsSockConnected(SOCKET s);

//static bool get_RemotePARA(SOCKET s, char* strIP,int* port );