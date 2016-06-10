#pragma once

#include "..\..\include\cini.hpp"

#define FILENUM      6
#define CARDPORTMAX      2
#define DEVICEMAX      8


struct CARD_INI
{
	CString m_AcqCardID;
	CString m_AcqCardName;
	int m_AcqCardIndex;
	CString m_AcqCardApp;
	CARD_INI()
	{
		m_AcqCardID="";
		m_AcqCardName="";
		m_AcqCardIndex=0;
		m_AcqCardApp="";
	}
};

struct PORTDMA_INI
{
	int m_FatherCardIndex;
	int m_DmaToCardIndex;
	int m_DmaIndex;
	CString m_DmaName;
	int m_AcqCashNum;
	CString m_CamDeviceID;
	CString m_CamDeviceName;
	int m_TriMode;
	int m_TrigInput;
	int m_TriDelay;
	int m_LinkMode;
	int m_BitCount;
	int m_BitAlign;
	int m_LeftX;
	int m_TopY;
	int m_Width;
	int m_Height;
	int m_MaxW;
	int m_MinW;
	int m_MaxH;
	int m_MinH;
	int m_Step;
	int m_Exposure;
	int m_FPS;
	int m_MaxFps;
	float m_Gain;
	float m_MinGain;
	float m_MaxGain;
	int m_BackLevel;
	int m_MaxBackLevel;
	int m_ExposureMode;
	int m_Linlog;
	int m_PixelFormat;
	BOOL m_DataTurn;
	int  m_BayerFilter;
	float m_RedGain;
	float m_GreenGain;
	float m_BlueGain;
	int m_RedGamma;
	int m_GreenGamma;
	int m_BlueGamma;
	CString m_StorageID;
	CString m_Filepath[6];
	int m_Filenum;
	int m_StorageCashNum;
	BOOL m_IsFact;
	CString m_FactPath;
	int m_MinFreeSize;
	BOOL m_RevEnable;
	CString m_RevID;
	int m_RevPort;
	int m_RevRate;
	int m_RevLen;
	PORTDMA_INI()
	{
			m_FatherCardIndex=0;
			m_DmaToCardIndex=0;
			m_DmaIndex=0;
			m_DmaName="";
			m_AcqCashNum=0;
			m_CamDeviceID="";
	        m_CamDeviceName="";
			m_TriMode=0;
			m_TrigInput=0;
			m_TriDelay=0;
			m_LinkMode=0;
			m_BitCount=0;
			m_BitAlign=0;
			m_LeftX=0;
			m_TopY=0;
			m_Width=0;
			m_Height=0;
			m_Exposure=0;
			m_FPS=0;
			m_Gain=0;
			m_BackLevel=0;
			m_ExposureMode=0;
			m_Linlog=0;
			m_PixelFormat=0;
			m_DataTurn=0;
			m_BayerFilter=0;
			m_RedGain=0;
			m_GreenGain=0;
			m_BlueGain=0;
			m_RedGamma=0;
			m_GreenGamma=0;
			m_BlueGamma=0;
			m_StorageID="";
			for(int i=0;i<6;i++)
	           m_Filepath[i]="";
			m_Filenum=0;
	        m_StorageCashNum=0;
	        m_IsFact=0;
	        m_FactPath="";
	        m_MinFreeSize=0;
			m_RevEnable=0;
			m_RevID="";
			m_RevPort=0;
			m_RevRate=0;
			m_RevLen=0;
			m_MaxW=0;
		    m_MaxH=0;
		    m_MinW=0;
		    m_MinH=0;
		    m_Step=0;
		    m_MinGain=0;
		    m_MaxGain=0;
		    m_MaxBackLevel=0;
		    m_MaxFps=0;
		}
};


class CIniCtrl
{
public:
	CIniCtrl(void);
	~CIniCtrl(void);
public:
	CString m_MasteriniPath;
public:
	BOOL m_IsCh;    //是否参数改变

	int m_DeviceID;
	CString m_DeviceName;
	CString m_LogPath;

	BOOL m_ClientUDPEnable; //               UDP客服端使能
    BOOL m_SvrUDPEnable; //                    UDP服务器使能
    BOOL m_ClientTCPEnable; //                TCP客服端使能
    BOOL m_GPSEnable; //                      GPS对时使能
    BOOL m_SyncEnable; //                     同步控制器使能
	long m_DoubleRate;
    int m_AcqCardNum; //                      采集设备个数
    int m_ImageDmaNum; // 

	CString m_UdpID;
	CString m_SvrIP;
	int m_SvrPort;
	int m_SendTime;
	CString m_LocalIP;
	int m_LocalPort;

	CString m_TCPSvrIP;
	int m_TCPSvrPort;

	CString m_GPSID;
	int m_GpsPort;
	int m_GpsType;
	int m_GpsRate;

	CString m_SyncID;
	int m_SyncPort;
	int m_SyncRate;
	int m_SyncTigType;
	int m_SyncEdge;
	int m_SyncChennlEnable[12];
	int m_SyncDelay[12];
	int m_SyncFramRate[12];

	CARD_INI m_CardIni[DEVICEMAX/2];

	PORTDMA_INI m_PortDmaIni[DEVICEMAX];
public:
	BOOL GetMasterIni(CString inipath);
	int GetLinkMode(int pos);
	//保存配置文件
	BOOL SaveMasterIni(CString inipath="");
	int CopyFile(char* src,char* dst);
};

