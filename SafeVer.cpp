// SafeVer.cpp: implementation of the CSafeVer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SafeVer.h"

#include <string>
#include <sstream>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char CSafeVer::m_alphabet[] = 
TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

CSafeVer::CSafeVer()
{
    m_OverdueTime=0;
    m_ModuleFilePath="";
}

CSafeVer::~CSafeVer()
{

}

void CSafeVer::CreateVer()
{
//	string dd=GetSiteID();
	//	if(IsOverdue())
    {
		CString str=GetLicense();
		
		stringstream strstr((LPCTSTR)str);	
		string cam="";
		list<CString> snlist;
		while( getline( strstr , cam ,',' ) )
		{
			if( cam.empty() )
				continue;
			
			snlist.push_back(cam.c_str());
			cam = "";
		}

		WriteKeynum(snlist.size());

		int i=0;
		for(list<CString>::iterator i1=snlist.begin();i1!=snlist.end();i1++)
		{
            CString sn=*i1;
			int num=0,size=sn.GetLength();
			int destSize=BASE64ENCODE_MAXSIZE(size)+2;
			
			if(destSize<=0)
                continue ;

			char* destBuf = new char[destSize];// destBuf[1024]
			
			::ZeroMemory(destBuf,destSize);
			
			Base64_Encode((BYTE*)sn.GetBuffer(0),size,destBuf,destSize,&num);

			string sid2="",sid=destBuf;

            RSA_GenKey(sid,sid2);

            WriteKey(i,sid2.c_str());

			if(destBuf)
			   delete destBuf;

			i++;
		}
	}
}

void CSafeVer::WriteKey(int i,CString str)
{ 
    HKEY hSubKey;
	DWORD lpdw;
	CString key;
	key.Format("Verification%d",i);
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,"Software\\Reg\\Admin",0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hSubKey,&lpdw);//REG_OPTION_NON_VOLATILE:保存，重启还在；REG_OPTION_VOLATILE:不保存，重启不在。
	RegSetValueEx(hSubKey,(LPCTSTR)key.GetBuffer(0),0,REG_SZ,(LPBYTE)str.GetBuffer(0),str.GetLength());
	RegCloseKey(hSubKey);
}

CString CSafeVer::GetCPUid()
{
	
	// BOOL bException = FALSE;
	BYTE szCpu[16]  = { 0 };
	UINT uCpuID     = 0U;
	
	_asm 
	{
		mov eax, 0
			cpuid
			mov dword ptr szCpu[0], ebx
			mov dword ptr szCpu[4], edx
			mov dword ptr szCpu[8], ecx
			mov eax, 1
			cpuid
			mov uCpuID, edx
	}
	
	
	CString id="";
	id.Format("%s%d",szCpu,uCpuID);
	return id;
}

void CSafeVer::GetKey()
{
   	HKEY hKEY;//键
   
	//打开注册表中的该键
	long ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Reg\\Admin",0, KEY_READ,&hKEY);
	if(ret!=ERROR_SUCCESS) 
		return ;

	//读取该键的值
	unsigned char tmp[256]="\0";
	DWORD type=REG_SZ;
	DWORD size=256;//必须大于你的键值字符长度
	ret=::RegQueryValueEx(hKEY,"KeyNum",NULL,&type,tmp,&size);
	if(ret!=ERROR_SUCCESS)
		return ;

    CString tmpstring;
	tmpstring.Format("%s",tmp);

	int num=atoi(tmpstring);

	for(int i=0;i<num;i++)
	{
        CString key;
		key.Format("Verification%d",i);
		
		//读取该键的值
		unsigned char tmp[256]="\0";
		DWORD type=REG_SZ;
		DWORD size=256;//必须大于你的键值字符长度
		ret=::RegQueryValueEx(hKEY,(LPCTSTR)key,NULL,&type,tmp,&size);
		if(ret!=ERROR_SUCCESS)
			continue ;

        tmpstring="";
        tmpstring.Format("%s",tmp);
		m_KeyList.push_back(tmpstring);
	}
   
	return ;
}

BOOL CSafeVer::IsOverdue()
{
   CString path=m_ModuleFilePath;
   path+="\\CacheMap.dll";

   WIN32_FIND_DATA ffd ;
   HANDLE hFind = FindFirstFile(path,&ffd);


   SYSTEMTIME Filetime={0};
   FileTimeToSystemTime(&ffd.ftLastWriteTime,&Filetime);
   COleDateTime ftime;

   if((Filetime.wYear==0)||(Filetime.wMonth==0)||(Filetime.wDay==0))
   {
	   return FALSE;
   }

 //  CString msg;
 //  msg.Format("%d %d %d %d %d %d",Filetime.wYear,Filetime.wMonth,Filetime.wDay,Filetime.wHour,Filetime.wMinute,Filetime.wSecond);

   ftime.SetDateTime(Filetime.wYear,Filetime.wMonth,Filetime.wDay,Filetime.wHour,Filetime.wMinute,Filetime.wSecond);
   COleDateTime currttime=COleDateTime::GetCurrentTime();

   if(m_OverdueTime>0)
   {
	   if(abs(currttime-ftime)>m_OverdueTime)
	     return FALSE;
   }

   return TRUE;
}

CString CSafeVer::GetModuleFilePath()
{
    CString path ;
	char* pStr = path.GetBuffer( 255 );
	GetModuleFileName( NULL , pStr , 255 );
	path.ReleaseBuffer();
	int nRet = path.ReverseFind('\\');
	
	string filepath=FSO_Tools::GetPathFromFileName((LPCTSTR)path);

    m_ModuleFilePath=filepath.c_str();

	return filepath.c_str();
}

void CSafeVer::SetMode(int overdue)
{
    m_OverdueTime=overdue;
}

BOOL CSafeVer::CheckModuleFile()
{
    if(!FSO_Tools::isFsoExists((LPCTSTR)m_ModuleFilePath))
	  return FALSE;

	CString path=m_ModuleFilePath;
	path+="\\Explain.exe";

	if(FSO_Tools::isFsoExists((LPCTSTR)path))
		return TRUE;

	path=m_ModuleFilePath;
	path+="\\HighSpeedGrab.exe";

	if(!FSO_Tools::isFsoExists((LPCTSTR)path))
		return FALSE;

	path=m_ModuleFilePath;
	path+="\\grab.xml";

	if(!FSO_Tools::isFsoExists((LPCTSTR)path))
		return FALSE;

	path=m_ModuleFilePath;
	path+="\\CamraDll.dll";

	if(!FSO_Tools::isFsoExists((LPCTSTR)path))
		return FALSE;

	path=m_ModuleFilePath;
	path+="\\VA.dll";

	if(!FSO_Tools::isFsoExists((LPCTSTR)path))
		return FALSE;

	return TRUE;
}



CString CSafeVer::GetLicense()
{
     CString path=m_ModuleFilePath;
	path+="\\Key.txt";

	path.Replace("\\\\","\\");

	string sn="";
	FSO_Tools::ReadString((LPCTSTR)path,sn);

	DeleteFile(path);
	return sn.c_str();
}

void CSafeVer::WriteKeynum(int num)
{
	HKEY hSubKey;
	DWORD lpdw;
	CString item;
	item.Format("%d",num);
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,"Software\\Reg\\Admin",0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hSubKey,&lpdw);//REG_OPTION_NON_VOLATILE:保存，重启还在；REG_OPTION_VOLATILE:不保存，重启不在。
	RegSetValueEx(hSubKey,"KeyNum",0,REG_SZ,(LPBYTE)item.GetBuffer(0),item.GetLength());
	RegCloseKey(hSubKey);
}

BOOL CSafeVer::CheckKey(CString sn)
{
    GetKey();
	
	int num=0,size=sn.GetLength();
	int destSize=BASE64ENCODE_MAXSIZE(size)+2;
	
	if(destSize<=0)
		return FALSE;
	
	char* destBuf = new char[destSize];// destBuf[1024]
	
	::ZeroMemory(destBuf,destSize);
	
	Base64_Encode((BYTE*)sn.GetBuffer(0),size,destBuf,destSize,&num);
	
	string sid2="",sid=destBuf;
	
	RSA_GenKey(sid,sid2);
	
	if(destBuf)
		delete destBuf;

	
    for(list<CString>::iterator i1=m_KeyList.begin();i1!=m_KeyList.end();i1++)
	{
		CString osn=*i1;
		
		if(osn==sid2.c_str())
			return TRUE;
	}

	return FALSE;
}

string CSafeVer::GetSiteID()
{
	string strSiteID="";
	
	OSVERSIONINFO version;
	memset (&version, 0, sizeof (version));
	version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	
	::GetVersionEx (&version);
	
	if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)//nt
	{
		ReadPhysicalDriveInNT(strSiteID);
	}
	
	if (strSiteID.size()<=0)
	{
		char buf[255];
		
		DWORD Serial;
		DWORD Length;

		GetVolumeInformation("C:",NULL,MAX_PATH,&Serial,&Length,NULL,NULL,MAX_PATH);
		
		memset(buf,0,sizeof(buf));
		
		ltoa((long)Serial,buf,10);
		
		strSiteID=(char*)buf;
	}
	
	string_replace(strSiteID," ","");
	//AfxMessageBox((CString)strSiteID.c_str());
	
	if (strSiteID.size()>16) 
		strSiteID=strSiteID.substr(0,16);
	
	//AfxMessageBox((CString)strSiteID.c_str());
	
	string strSiteID2="";
	
	if (SUCCEEDED(RSA_GenKey(strSiteID,strSiteID2)))
		return strSiteID2;
	else
		return "";
	
}

bool CSafeVer::Base64_Encode(const BYTE *pSrc, int srclen, char *szOut,int szOutLen,int *nCount)
{
	
	long destLen=BASE64ENCODE_MAXSIZE(srclen);
	
	if (destLen>szOutLen) return false;
	
	UINT buf;
	int i, nOffset;
	BYTE *p = (BYTE *)pSrc;      
	char *r = szOut;
	
	int iSize=0;
	
	while(p - pSrc < srclen)
	{
		//3 * 8 ==> 4 * 6
		if(p - pSrc +3 < srclen)
		{
			buf = (*p << 16) | (*(p + 1) << 8) | *(p + 2);
			p += 3;
			
			iSize+=4;
		}
		else
		{
			buf = 0;
			nOffset = p - (BYTE*)pSrc;
			while(p - pSrc < srclen)
			{
				buf <<= 8;
				buf |= *p++;
			}
			buf <<= ((srclen - nOffset + 1) * 6) % 8;
			
			for(i = srclen - nOffset; i >= 0; i--)
			{
				*r++ = m_alphabet[(buf >> (i * 6)) & 0x3F];
				iSize++;
			}
			
			break;
		}
		*r++ = m_alphabet[(buf >> 18) & 0x3F];
		*r++ = m_alphabet[(buf >> 12) & 0x3F];
		*r++ = m_alphabet[(buf >> 6) & 0x3F];
		*r++ = m_alphabet[buf & 0x3F];
	}      
	*r = 0;
	
	if (nCount)
		*nCount=iSize;
	
	return true;
	
}

bool CSafeVer::Base64_Decode(const char *szCoded, int srclen, BYTE *pOut,int pOutLen,int*nCount )
{
	long destLen=BASE64DECODE_MAXSIZE(srclen);
	
	if (destLen>pOutLen) return false;
	
	
	int iSize=0;
	
	short nDecTab[256];
	short i;
	UINT buf;
	int nOffset, len = srclen;//strlen(szCoded);
	char *p = (char *)szCoded;      
	BYTE *r = pOut;
	memset(nDecTab, -1, sizeof(short) * 256);
	
	for(i = 0; i < 64; i++)
	{
		nDecTab[m_alphabet[i]] = i;
	}
	nDecTab['='] = -1;
	
	while(*p)
	{      
		//4 * 6 ==> 3 * 8
		if(p + 4 - szCoded <= len)
		{
			buf = ((nDecTab[*p] & 0x3F) << 18) | ((nDecTab[*(p + 1)] & 0x3F) << 12) |
				((nDecTab[*(p + 2)] & 0x3F) << 6) | (nDecTab[*(p + 3)] & 0x3F);
			p += 4;
			
			iSize+=3;
		}
		else
		{
			nOffset = p - szCoded;
			
			if(nOffset != len)
			{
				buf = 0;
				while(*p)
				{
					
					
					buf <<= 6;
					buf |= (nDecTab[*p++] & 0x3F);   
					
					
					
				}
				
				buf >>= ((len - nOffset) * 6) % 8;
				
				for(i = ((len - nOffset) * 6) >> 3; i > 0; i--)
				{
					*r++ = (buf >> ((i - 1) << 3)) & 0xFF;
					
					iSize++;	
				}
				
				break;
			}                  
		}
		
		//if (iSize>=pOutLen)
		//   break;
		
		*r++ = (buf >> 16) & 0xFF;
		*r++ = (buf >> 8) & 0xFF;
		*r++ = buf & 0xFF;            
	}    
	
	*r = 0;
	
	if (nCount)
		*nCount=iSize;
	
	return  true;
}

bool  CSafeVer::RSA_GenKey(string strRegName,string& strKey)
{
	int namelen=strRegName.size ();	//获取Name的长度
	
	//务必满足：Name的长度<=(Keysize/8)，使M<N成立。
	if (namelen!=0&&namelen<=16)	//如果不为空且<=128/8则开始计算SN
	{
		miracl *mip=mirsys(100,0);
		mip->IOBASE=16;	//16进制模式
		
		//定义并初始化变量
		big m=mirvar(0);	//m 放明文：注册码SN
		big c=mirvar(0);	//c 放密文：用户名Name
		big n=mirvar(0);	//n 模数
		big d=mirvar(0);	//d 私钥
		TCHAR Name[256]={0};
		TCHAR SN[256]={0};
		
		char* ptr= (char*)strRegName.c_str();
        memcpy(Name,ptr,namelen);
		//strRegName.ReleaseBuffer();
		
		//m_name.GetWindowText (Name,namelen+1);
		
		//获取Name
		bytes_to_big(namelen,Name,c);					//转换成16进制
		
		cinstr(n,"963251DC5A9C90D9F203A03C363BA411");	//初始化模数n
		
		
		cinstr(d,"56157D29A89D77BF2F669A8F0B123CC9"); //初始化私钥d
		
		
		
		powmod(c,d,n,m);								//计算m=c^d mod n
		cotstr(m,SN);									//将m以16进制串写入SN
		
		strKey=(char*)SN;						//输出16进制SN
		
		//释放内存
		mirkill(m);
		mirkill(c);
		mirkill(n);
		mirkill(d);
		mirexit();
	}
	else
		return false;
	
	return true;
}

bool  CSafeVer::RSA_Verify(string strRegName,string strKey)
{
	bool ret=false;
	
	miracl *mip=mirsys(100,0);
	mip->IOBASE=16;	//16进制模式
	
	//定义并初始化变量
	big m=mirvar(0);	//m 放明文：注册码SN
	big c=mirvar(0);	//c 放密文：用户名Name
	big n=mirvar(0);	//n 模数
	big e=mirvar(0);	//e 公钥
	TCHAR Name[256]={0};
	TCHAR SN[256]={0};
	TCHAR temp[256]={0};
	int len=0;
	int i,j;
	
	//获取Name
	len=strRegName.size();
	
	//m_name.GetWindowText (Name,len+1);
	
	char* ptr= (char*)strRegName.c_str();
    memcpy(Name,ptr,len);
	//strRegName.ReleaseBuffer();
	
	//获取SN
	len=strKey.size();
	ptr= (char*)strKey.c_str();
    memcpy(SN,ptr,len);
	//strKey.ReleaseBuffer();
	
	//m_sn.GetWindowText (SN,len+1);
	
	//检查SN是否为16进制
	for (i=0,j=0;i<len;i++)
	{
		if(isxdigit(SN[i])==0)
		{
			j=1;
			break;
		}
	}
	
	//如果输入的SN为16进制且长度不为0
	if (j!=1&&len!=0)
	{
		
		cinstr(m,SN);//初始化明文m
		cinstr(n,"963251DC5A9C90D9F203A03C363BA411");	//初始化模数n
		
		
		//cinstr(n,"963251DC5A9C90D9F203A03C363BA411");	//初始化模数n
		
		cinstr(e,"10001");								//初始化公钥e
		
		//当m<n时
		if(compare(m,n)==-1)
		{
			powmod(m,e,n,c);//计算c=m^e mod n
			big_to_bytes(256,c,temp,FALSE);//将c转换成数组写入temp
			
			//释放内存
			mirkill(m);
			mirkill(c);
			mirkill(n);
			mirkill(e);
			mirexit();
		}
		else
			j=1;
		
	}
	
	//对Name、temp， m、n， SN的长度进行检查
	if(!(lstrcmp(Name,temp)!=0||j==1||len==0))
		ret=true;
	
	return ret;
}

char * CSafeVer::ConvertToString(DWORD diskdata [256], int firstIndex, int lastIndex)
{
	static char string [1024];
	int index = 0;
	int position = 0;
	
	//  each integer has two characters stored in it backwards
	for (index = firstIndex; index <= lastIndex; index++)
	{
		//  get high byte for 1st character
		string [position] = (char) (diskdata [index] / 256);
		position++;
		
		//  get low byte for 2nd character
		string [position] = (char) (diskdata [index] % 256);
		position++;
	}
	//  end the string 
	string [position] = '\0';
	
	//  cut off the trailing blanks
	for (index = position - 1; index > 0 && ' ' == string [index]; index--)
		string [index] = '\0';
	return string;
}

BOOL CSafeVer::DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
					PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
					PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP -> irDriveRegs.bFeaturesReg = 0;
	pSCIP -> irDriveRegs.bSectorCountReg = 1;
	pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP -> irDriveRegs.bCylLowReg = 0;
	pSCIP -> irDriveRegs.bCylHighReg = 0;
	
	// Compute the drive number.
	pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);
	
	// The command can either be IDE identify or ATAPI identify.
	pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
	pSCIP -> bDriveNumber = bDriveNum;
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	
	return( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID) pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID) pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL) );
}

int CSafeVer::ReadPhysicalDriveInNT(string &strSiteID)
{
	//CString strSiteID;
	int done = FALSE;
	int drive = 0;
	
	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;
		
		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char driveName [256];
		
		sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);
		
		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFile (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
		//            drive, GetLastError ());
		
		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               cbBytesReturned = 0;
			
            // Get the version, etc of PhysicalDrive IOCTL
			memset ((void*) &VersionParams, 0, sizeof(VersionParams));
			
			if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof(VersionParams),
				&cbBytesReturned, NULL) )
			{         
				// printf ("DFP_GET_VERSION failed for drive %d\n", i);
				// continue;
			}
			
            // If there is a IDE device at number "i" issue commands
            // to the device
			if (VersionParams.bIDEDeviceMap > 0)
			{
				BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
				SENDCMDINPARAMS  scip;
				//SENDCMDOUTPARAMS OutCmd;
				BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
				
				memset (&scip, 0, sizeof(scip));
				memset (IdOutCmd, 0, sizeof(IdOutCmd));
				
				if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
					&scip, 
					(PSENDCMDOUTPARAMS)&IdOutCmd, 
					(BYTE) bIDCmd,
					(BYTE) drive,
					&cbBytesReturned))
				{
					DWORD diskdata [256];
					int ijk = 0;
					USHORT *pIdSector = (USHORT *)
						((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;
					
					for (ijk = 0; ijk < 256; ijk++)
						diskdata [ijk] = pIdSector [ijk];
					
					strSiteID=(string)(ConvertToString (diskdata, 10, 19));
					
					//ReturnInfo (drive, diskdata,strSiteID);
					
					done = TRUE;
				}
			}
			
			CloseHandle (hPhysicalDriveIOCTL);
		}
	}
	
	return done;
}

void CSafeVer::string_replace(string & strBig, const string & strsrc, const string &strdst) {
	string::size_type pos=0;
	string::size_type srclen=strsrc.size();
	string::size_type dstlen=strdst.size();
	
	while( (pos=strBig.find(strsrc, pos)) != string::npos){
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
/*

void CSafeVer::GetCardSerial(CString& number)
{
	Fg_Struct *fg;
	
	if((fg = Fg_Init("DualAreaGray16.dll",0)) == NULL)
	{
	    number="";
		return ;	
	}

	CString ss="";

	ss.Format("%x",Fg_getSerialNumber(fg));

    number=ss;

	Fg_FreeGrabber(fg);
}
*/