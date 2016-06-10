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
#include "time.h" //��û���ã�
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
	
	static bool NT_InitEnv();//��ʼ�����绷��
	void NT_UnInitEnv();//�ر����绷��
	
	static SOCKET NT_Connect(LPCSTR pstrHost, int nPort);//ͬ������
	static SOCKET NT_Connect2(LPCSTR pstrHost, int nPort,int m_nTimeOut);//�첽����
	static bool NT_DisConnect(SOCKET s);//�Ͽ�����
	static int NT_GetStatus(SOCKET s);//�õ����ӵ�״̬���쳣����״̬�޷����
	
	static int NT_Write(SOCKET s,char* buf, int len); //���ͺ���
	static int NT_Read(SOCKET s,char FAR* buf, int len);//���պ���
	
};

#endif // !defined(AFX_SOCKETEX_H__84D1B237_B1C9_4ECF_8B92_16D75A3EC336__INCLUDED_)
//static int ReadEx2(SOCKET fd,char *bp,int len);



//static BOOL IsSockConnected(SOCKET s);

//static bool get_RemotePARA(SOCKET s, char* strIP,int* port );