// SafeVer.h: interface for the CSafeVer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAFEVER_H__49567928_89F0_424B_AB93_BE7B05321AAC__INCLUDED_)
#define AFX_SAFEVER_H__49567928_89F0_424B_AB93_BE7B05321AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
extern "C"
{
#include "MIRACL.H"
#include "MIRDEF.H"
#pragma comment(lib,"MS32.LIB")
}
/*
#if _DEBUG
  #pragma comment(linker,"/NODEFAULTLIB:LIBC")
#endif
  */
#include <stdio.h>
#include <windows.h>
#include <ole2.h>
#include <locale.h>

#include "FSO_Tools.h"
//预知bas64编码后大小
#define BASE64ENCODE_MAXSIZE(size) ((size/3)*4+((size%3)>0?4:0)) //最大值
#define BASE64DECODE_MAXSIZE(size) ((size/4)*3+((size%4)>0?3:0)) //最大值
   //  Max number of drives assuming primary/secondary, master/slave topology
#define  MAX_IDE_DRIVES  4
#define  IDENTIFY_BUFFER_SIZE  512
   //  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
//#define  DFP_SEND_DRIVE_COMMAND   0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088


typedef struct _GETVERSIONOUTPARAMS
{
   BYTE bVersion;      // Binary driver version.
   BYTE bRevision;     // Binary driver revision.
   BYTE bReserved;     // Not used.
   BYTE bIDEDeviceMap; // Bit map of IDE devices.
   DWORD fCapabilities; // Bit mask of driver capabilities.
   DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

typedef struct _IDEREGS
{
   BYTE bFeaturesReg;       // Used for specifying SMART "commands".
   BYTE bSectorCountReg;    // IDE sector count register
   BYTE bSectorNumberReg;   // IDE sector number register
   BYTE bCylLowReg;         // IDE low order cylinder value
   BYTE bCylHighReg;        // IDE high order cylinder value
   BYTE bDriveHeadReg;      // IDE drive/head register
   BYTE bCommandReg;        // Actual IDE command.
   BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;


typedef struct _SENDCMDINPARAMS
{
   DWORD     cBufferSize;   //  Buffer size in bytes
   IDEREGS   irDriveRegs;   //  Structure with drive register values.
   BYTE bDriveNumber;       //  Physical drive number to send 
                            //  command to (0,1,2,3).
   BYTE bReserved[3];       //  Reserved for future expansion.
   DWORD     dwReserved[4]; //  For future use.
   BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
 
   //  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.


   // Status returned from driver
typedef struct _DRIVERSTATUS
{
   BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
   BYTE  bIDEStatus;    //  Contents of IDE Error register.
                        //  Only valid when bDriverError is SMART_IDE_ERROR.
   BYTE  bReserved[2];  //  Reserved for future expansion.
   DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;


   // Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
   DWORD         cBufferSize;   //  Size of bBuffer in bytes
   DRIVERSTATUS  DriverStatus;  //  Driver status structure.
   BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

using namespace std;

class CSafeVer  
{
	int m_OverdueTime;
	CString m_ModuleFilePath;
	list<CString> m_KeyList;
public:
	void GetCardSerial(CString& number);
	BOOL CheckKey(CString sn);
	void WriteKeynum(int num);
	CString GetLicense();
	BOOL CheckModuleFile();
	void SetMode(int overdue);
	CString GetModuleFilePath();
	CSafeVer();
	virtual ~CSafeVer();

	BOOL IsOverdue();
	void GetKey();
	CString GetCPUid();
	void WriteKey(int i,CString str);
	void CreateVer();

	//获得本机的安全SITE_ID
	static string GetSiteID();

	//字符串 base64编码解码
	static bool Base64_Encode(const BYTE *pSrc, int srclen, char *szOut,int szOutLen,int *nCount=NULL);
	static bool Base64_Decode(const char *szCoded, int srclen, BYTE *pOut,int pOutLen,int *nCount =NULL);

	//字符串 RSA加密 校验
	static bool  RSA_GenKey(string strRegName,string& strKey);
	static bool  RSA_Verify(string strRegName,string strKey);

	static int ReadPhysicalDriveInNT (string &strSiteID);

	//for get HDD info
	static char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex);

	static BOOL DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned);
	//for base64
	static const char m_alphabet[];

	static void string_replace(string & strBig, const string & strsrc, const string &strdst);

};

#endif // !defined(AFX_SAFEVER_H__49567928_89F0_424B_AB93_BE7B05321AAC__INCLUDED_)
