// RegCheck.cpp: implementation of the CRegCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegCheck.h"
#include "Str_Tools.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegCheck::CRegCheck()
{

}

CRegCheck::~CRegCheck()
{

}

string CRegCheck::GetCheckID()
{
   /*		DWORD   dwId1, dwId2, dwId3, dwId4;     
    char   szCompany[13];     
    PCHAR   pCompany = szCompany;  
      
    //DWORD dwCPU;  
  
    szCompany[12]=0;     
    _asm     
    {     
        pushfd     
            pushad     
            //取得CPU的ID号     
            mov   eax,1  //功能号     
            _emit   0x0f     
            _emit   0xa2     
            mov   dwId1,eax     
            mov   dwId2,ebx     
            mov   dwId3,ecx     
            mov   dwId4,edx     
              
            //取得CPU的制造公司名称  
            mov   edi,pCompany   //功能号     
            mov   eax,0     
            _emit   0x0f     
            _emit   0xa2     
            mov   eax,ebx     
            stosd     
            mov   eax,edx     
            stosd     
            mov   eax,ecx     
            stosd   
            popad     
            popfd     
    }     
  

	string CPUID;

	char dd[64];
	memset(dd,0,64);

	sprintf(dd, "%08X-%08X-%08X-%08X", dwId1,dwId2,dwId3,dwId4);

	CPUID=dd;

	return CPUID;*/


	return "";




}


   
int CRegCheck::GetKeyList()
{
	HKEY hKEY;//键
   
	//打开注册表中的该键
	long ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Reg\\Admin",0, KEY_READ|KEY_WOW64_64KEY,&hKEY);
	if(ret!=ERROR_SUCCESS) 
		return 0;

	//读取该键的值
	unsigned char tmp[256]="\0";

	DWORD type=REG_SZ|KEY_WOW64_64KEY;
	DWORD size=256;//必须大于你的键值字符长度
	ret=::RegQueryValueEx(hKEY,"KeyNum",NULL,&type,tmp,&size);
	if(ret!=ERROR_SUCCESS)
		return 0;

    string tmpstring=(char*)tmp;

	int num=atoi(tmpstring.c_str());


	for(int i=0;i<num;i++)
	{
        char tmp2[256]="\0";

		sprintf(tmp2, "Verification%d\0", i);

		//读取该键的值
		unsigned char tmp1[256]="\0";
		DWORD type=REG_SZ|KEY_WOW64_64KEY;
		DWORD size=256;//必须大于你的键值字符长度
		ret=::RegQueryValueEx(hKEY,(LPCTSTR)tmp2,NULL,&type,tmp1,&size);
		if(ret!=ERROR_SUCCESS)
			continue ;

        tmpstring="";
		tmpstring=(char*)tmp1;
	//	sprintf(tmpstring, "%s", tmp1);
		m_KeyList.push_back(tmpstring);

		//DEBUG_INFO1("id:%s",tmpstring.c_str());
	}
   
	return num;
 }
	
BOOL CRegCheck::CheckKey(string sn)
{
    int keynum=GetKeyList();
	
	//sn.r
	Str_Tools::Remove(sn,' ');
	
	//DEBUG_INFO1("id1:%s",sn.c_str());
	if(keynum<=0)
		return FALSE;

	int num=0,size=sn.size();

    for(list<string>::iterator i1=m_KeyList.begin();i1!=m_KeyList.end();i1++)
	{
		string osn=*i1;
		Str_Tools::Remove(osn,' ');
		if(osn==sn)
			return TRUE;
	}

	return FALSE;
}

int CRegCheck::ReadPhysicalDriveInNT(string &strSiteID)
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

BOOL CRegCheck::DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
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

char * CRegCheck::ConvertToString(DWORD diskdata [256], int firstIndex, int lastIndex)
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

