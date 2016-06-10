// SocketEx.cpp: implementation of the CNetTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CNetTools::CNetTools()
{
	
}

CNetTools::~CNetTools()
{
	
}

bool CNetTools::NT_InitEnv()  //初始化网络环境
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 0 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	
	return (err==0) ;	
}

void CNetTools::NT_UnInitEnv() //关闭网络环境
{
	WSACleanup();//释放资源的操作	
}


SOCKET CNetTools::NT_Connect(LPCSTR pstrHost, int nPort)
{
	
	LPHOSTENT lpHost;	
	struct sockaddr_in server;	
	SOCKET sSocket=NULL;
	
	lpHost = gethostbyname(pstrHost);
	if(lpHost == NULL) return NULL; //获得主机错误
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = *((u_long FAR *)(lpHost->h_addr));
	server.sin_port=htons(nPort);
	
	sSocket = socket(AF_INET, SOCK_STREAM, 0); //IPPROTO_TCP
    //--------
	//unsigned long cmd;
    //int nStatus = ioctlsocket(sSocket,FIONBIO,&cmd);
	
	if(sSocket <= 0) return NULL;//socket 创建错误

	int TimeOut=1000;//设置接收超时6秒
	if(::setsockopt(sSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR){
		return 0;
	}

	if(::setsockopt(sSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR){
		return 0;
	}

	if(connect(sSocket, (LPSOCKADDR)&server, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		closesocket(sSocket); //连接服务器错误
		sSocket = NULL;
		return NULL ;
	}

	return sSocket;
}
SOCKET CNetTools::NT_Connect2(LPCSTR pstrHost, int nPort,int m_nTimeOut)
{
	unsigned long non_blocking = 1;
	unsigned long blocking = 0;
	
	struct hostent* host = NULL;
	struct sockaddr_in saddr;
	unsigned int s = 0;
	BOOL  ret;
	time_t start;
	int error;
	host = gethostbyname (pstrHost);
	if (host==NULL)return  FALSE;
	
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(nPort);
	saddr.sin_addr = *((struct in_addr*)host->h_addr);
	
	
	if( (s=socket(AF_INET, SOCK_STREAM, 0))<0){
		return FALSE;
	}
	
	   ioctlsocket(s,FIONBIO,&non_blocking);//WIN32
	   //ioctl(s,FIONBIO,&non_blocking);
	   //fcntl(s,F_SETFL, O_NONBLOCK);
	   
	   if(connect(s,(struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
		   //if (errno == INPROGRESS){// it is in the connect process
		   struct timeval tv;
		   fd_set writefds;
		   tv.tv_sec = m_nTimeOut;
		   tv.tv_usec = 0;
		   FD_ZERO(&writefds);
		   FD_SET(s, &writefds);
		   
		   if(select(s+1,NULL,&writefds,NULL,&tv)>0){
			   int len=sizeof(int);
			   //下面的一句一定要，主要针对防火墙
			   getsockopt(s, SOL_SOCKET, SO_ERROR,(char*) &error, &len);
			   if(error==0) ret=TRUE;
			   else ret=FALSE;
		   }else   ret=FALSE;//timeout or error happen
		   //}else ret=FALSE;
	   }
	   else    ret=TRUE;
	   
	   if(ret)
	   {
		   ioctlsocket(s,FIONBIO,&blocking);//WIN32				
		   
	   }
	   else
		   
	   {
		   closesocket(s); //连接服务器错误
		   s = NULL;
	   }
	   
	   return s; 	
}


bool CNetTools::NT_DisConnect(SOCKET s)
{
	if(s > 0)
	{
		shutdown(s, SD_RECEIVE);
		shutdown(s, SD_SEND);
		
		//Sleep(10);
		
		closesocket(s);
	}
	
	return true;
}

int CNetTools::NT_GetStatus(SOCKET s)
{
	int error;
	int len=sizeof(int);
	getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
	
	return (error==0);
	
	/*
	if (s==NULL) 
	return FALSE;
	
	  int nRet = 0;
	  struct fd_set Fd_Recv;
	  struct timeval Time_Recv;
	  
		memset(&Fd_Recv, 0, sizeof(struct fd_set));
		FD_CLR(s, &Fd_Recv); 
		FD_SET(s, &Fd_Recv); 
		Time_Recv.tv_sec = 0;
		Time_Recv.tv_usec = 0;
		
		  nRet = select(s, &Fd_Recv, NULL, NULL, &Time_Recv);
		  
			return  (nRet == SOCKET_ERROR?-1:0) ;
    */
}

int CNetTools::NT_Write(SOCKET s, char* buf, int len)
{
	int nCount=0;
	
    if (NT_GetStatus(s)==-1) return 0;
    
	while (nCount<len)
	{
		nCount+=send(s,buf+nCount,len-nCount,0);
		
		if (NT_GetStatus(s)==-1)
			break;
		
	}
	
	return nCount;
}

int CNetTools::NT_Read(SOCKET s,char FAR* buf, int len)
{
	int nRet=0;
	//int nStatus=0;
	
	//	while((nStatus=GetRWStatus(s))==0)
	//		Sleep(10);
	
	//if (nStatus==SOCKET_ERROR)
	//	{
	//		return -1;
	//	}
	if (NT_GetStatus(s)==-1) return -1;
	
	nRet = recv(s, buf, len, 0);
	
	return nRet;
}

/*
int CNetTools:: GetRWStatus(SOCKET s,bool isWrite)
{
int ret=-1;
if (s==NULL) return -1;

  TIMEVAL tv01 = {0, 1};//1ms钟延迟,实际为9-10毫秒 
  
	int nSelectRet; 
	
	  FD_SET fd = {1, s}; 
	  
		if (isWrite)
		nSelectRet=::select(0, NULL,&fd, NULL,&tv01);//检查可写状态 
		else
		nSelectRet=::select(0, &fd, NULL, NULL, &tv01);//检查可读状态 
		
		  switch(nSelectRet)
		  {
		  case SOCKET_ERROR:
		  ret=-1;
		  break;
		  case 0:
		  ret=0;
		  break;
		  default:
		  ret=1;
		  }
		  
			return ret;
			}
			
			  
				int CNetTools::ReadEx2(SOCKET fd,char *bp,int len)
				{
				int cnt;//用来记录缓冲区中的剩余可用空间。
				int rc;
				cnt=len;
				while(cnt>0)
				{
				//Platform SDK: Windows Sockets  The recv function receives data from a connected or bound socket.
				//Return Values  If no error occurs, recv returns the number of bytes received. If the connection has been gracefully closed, the return value is zero. Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
				//cnt参数的值被设定为缓冲区的长度值，所以，在网络速度足够好的情况下，读取cnt这么
				//多的字节，没问题。这样的话，此while循环只需要执行一次就会正常退出了。从而，我也
				//明白了while循环的作用，就是：用来保证一定要读取cnt这么多的字节，读不够数就
				//反复地读，直至读够数了，或者是此连接被关闭掉了。
				//我有一个疑问：如果客户端只发过来50个字节的数据，会怎么样呢？其实，这个问题，只需要查到合适的资料，就可以轻松解决，并无任何的难度可言。
				//答：??????
				rc=recv(fd,bp,cnt,0);
				
				  if(rc==SOCKET_ERROR)
				  {
				  CString aa;
				  aa="接收错误！\n";
				  //			AfxGetMainWnd()->SendMessageToDescendants(WM_AGE1,(LPARAM)aa.GetBuffer(0),1);
				  aa.ReleaseBuffer();
				  return -1;
				  }
				  
					//If the connection has been gracefully closed, the return value is zero.
					if(rc==0) 
					return len-cnt;
					//读取了一些数据之后，缓冲区指针也应该相应地增加。
					bp+=rc;
					//读取了一些数据之后，缓冲区中的剩余可用空间也应该相应地减小。
					cnt-=rc;
					}
					return len;
					}
					
					  
						bool CNetTools::get_RemotePARA(SOCKET s, char* strIP,int* port )
						{
						if( strIP == NULL )
						return false;
						
						  struct sockaddr_in rsockaddr;
						  
							int namelen = sizeof( rsockaddr );
							
							  if( getpeername( s, (SOCKADDR*)&rsockaddr, &namelen ) == SOCKET_ERROR )
							  {
							  //set_LastError( "getpeername() failed", WSAGetLastError() );
							  return false;
							  }
							  
								longToDottedQuad( rsockaddr.sin_addr.s_addr, strIP );
								*port = ntohs( rsockaddr.sin_port );
								
								  return true;
								  }
								  */