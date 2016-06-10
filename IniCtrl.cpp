#include "StdAfx.h"
#include "IniCtrl.h"

#include <string>
#include <sstream>
using namespace std;

CIniCtrl::CIniCtrl(void)
{
	m_MasteriniPath="";
	m_GpsType=0;
	m_IsCh=FALSE;
	m_DoubleRate=0;
}


CIniCtrl::~CIniCtrl(void)
{

}


BOOL CIniCtrl::GetMasterIni(CString inipath)
{
	if(inipath=="")
		m_MasteriniPath=".\\Master.ini";
	else m_MasteriniPath=inipath;

	cini   Master((LPCTSTR)m_MasteriniPath);

	string tempstr="";
	Master["BaseInfo"];//

	Master.get("DeviceID",m_DeviceID);
	Master.get("DeviceName",tempstr);
	m_DeviceName=tempstr.c_str();
	m_DeviceName.Replace(";","");

	Master.get("LogPath",tempstr);
	m_LogPath=tempstr.c_str();
	m_LogPath.Replace(";","");

	tempstr="";
	Master["Ctrl"];//

	Master.get("ClientUDPEnable",m_ClientUDPEnable);
	Master.get("SvrUDPEnable",m_SvrUDPEnable);
	Master.get("ClientTCPEnable",m_ClientTCPEnable);
	Master.get("GPSEnable",m_GPSEnable);
	Master.get("SyncEnable",m_SyncEnable);
	Master.get("AcqCardNum",m_AcqCardNum);
	Master.get("ImageDmaNum",m_ImageDmaNum);

	{
		tempstr="";
		Master["UDP"];//
		Master.get("UdpID",tempstr);
		m_UdpID=tempstr.c_str();
		m_UdpID.Replace(";","");
		tempstr="";
		Master.get("SvrIP",tempstr);
		m_SvrIP=tempstr.c_str();
		m_SvrIP.Replace(";","");

		Master.get("SvrPort",m_SvrPort);
		Master.get("SendTime",m_SendTime);

		tempstr="";
		Master.get("LocalIP",tempstr);
		m_LocalIP=tempstr.c_str();
		m_LocalIP.Replace(";","");

		Master.get("LocalPort",m_LocalPort);
	}


	//if(m_ClientTCPEnable)
	{
		tempstr="";
		Master["ClientTCP"];
		Master.get("SvrIP",tempstr);
		m_TCPSvrIP=tempstr.c_str();
		m_TCPSvrIP.Replace(";","");
		Master.get("SvrPort",m_TCPSvrPort);
	}


	//if(m_GPSEnable)
	{
		tempstr="";
		Master["GPSCom"];//
		Master.get("GPSID",tempstr);
		m_GPSID=tempstr.c_str();
		m_GPSID.Replace(";","");
		Master.get("GPSport",m_GpsPort);
		Master.get("GPSType",m_GpsType);
		Master.get("GPSRate",m_GpsRate);
	}

	//if(m_SyncEnable)
	{
		tempstr="";
		Master["SyncCom"];//
		Master.get("SyncID",tempstr);
		m_SyncID=tempstr.c_str();
		m_SyncID.Replace(";","");
		Master.get("Syncport",m_SyncPort);
		Master.get("SyncRate",m_SyncRate);

		Master.get("SyncTigType",m_SyncTigType);
		Master.get("SyncEdge",m_SyncEdge);

		tempstr="";
		Master.get("SyncChennlEnable",tempstr);
		CString ff=tempstr.c_str();
		ff.Replace(";","");
		stringstream strstr1( (LPCTSTR)ff );
		string pos="";

		getline(strstr1,pos,',');

		int k=0;
		while(pos!="")
		{
			if(pos!="")
			{
				m_SyncChennlEnable[k]=atoi(pos.c_str());
				k++;
				if(k>=12)
					break;
			}

			pos="";
			getline(strstr1,pos,',');
		}

		tempstr="";
		Master.get("SyncDelay",tempstr);
		ff=tempstr.c_str();
		ff.Replace(";","");
		stringstream strstr2( (LPCTSTR)ff );
		pos="";

		getline(strstr2,pos,',');

		k=0;
		while(pos!="")
		{
			if(pos!="")
			{
				m_SyncDelay[k]=atoi(pos.c_str());
				k++;
				if(k>=12)
					break;
			}

			pos="";
			getline(strstr2,pos,',');
		}

		tempstr="";
		Master.get("SyncFramRate",tempstr);
		ff=tempstr.c_str();
		ff.Replace(";","");
		stringstream strstr3( (LPCTSTR)ff );
		pos="";

		getline(strstr3,pos,',');

		k=0;
		while(pos!="")
		{
			if(pos!="")
			{
				m_SyncFramRate[k]=atoi(pos.c_str());
				k++;
				if(k>=12)
					break;
			}

			pos="";
			getline(strstr3,pos,',');
		}

	}

	if(m_AcqCardNum>0)
	{
		for(int i=0;i<m_AcqCardNum;i++)
		{
			CString key="";
			key.Format("AcqCardInfo%d",i);
			Master[key];

			tempstr="";
			Master.get("AcqCardID",tempstr);
			m_CardIni[i].m_AcqCardID=tempstr.c_str();
			m_CardIni[i].m_AcqCardID.Replace(";","");

			tempstr="";
			Master.get("AcqCardName",tempstr);
			m_CardIni[i].m_AcqCardName=tempstr.c_str();
			m_CardIni[i].m_AcqCardName.Replace(";","");

			Master.get("AcqCardIndex",m_CardIni[i].m_AcqCardIndex);

			tempstr="";
			Master.get("AcqCardApp",tempstr);
			m_CardIni[i].m_AcqCardApp=tempstr.c_str();
			m_CardIni[i].m_AcqCardApp.Replace(";","");
		}

		if(m_ImageDmaNum>0)
		{
			for(int i=0;i<m_ImageDmaNum;i++)
			{
				CString key="";
				key.Format("PortDma%d",i);
				Master[key];

				Master.get("FatherCardIndex",m_PortDmaIni[i].m_FatherCardIndex);

				Master.get("DmaToCardIndex",m_PortDmaIni[i].m_DmaToCardIndex);

				Master.get("DmaIndex",m_PortDmaIni[i].m_DmaIndex);

				tempstr="";
				Master.get("DmaName",tempstr);
				m_PortDmaIni[i].m_DmaName=tempstr.c_str();
				m_PortDmaIni[i].m_DmaName.Replace(";","");

				Master.get("AcqCashNum",m_PortDmaIni[i].m_AcqCashNum);

				tempstr="";
				Master.get("CamDeviceID",tempstr);
				m_PortDmaIni[i].m_CamDeviceID=tempstr.c_str();
				m_PortDmaIni[i].m_CamDeviceID.Replace(";","");

				tempstr="";
				Master.get("CamDeviceName",tempstr);
				m_PortDmaIni[i].m_CamDeviceName=tempstr.c_str();
				m_PortDmaIni[i].m_CamDeviceName.Replace(";","");

				if(m_PortDmaIni[i].m_CamDeviceName.Find("DR1")>=0)
				{
					CString app=m_CardIni[m_PortDmaIni[i].m_DmaToCardIndex].m_AcqCardApp;

					m_DoubleRate=atoi(app.Left(app.Find("-")));
				}

				Master.get("TriMode",m_PortDmaIni[i].m_TriMode);

				Master.get("TrigInput",m_PortDmaIni[i].m_TrigInput);
				Master.get("TriDelay",m_PortDmaIni[i].m_TriDelay);

				Master.get("LinkMode",m_PortDmaIni[i].m_LinkMode);

				Master.get("BitCount",m_PortDmaIni[i].m_BitCount);
				Master.get("BitAlign",m_PortDmaIni[i].m_BitAlign);

				Master.get("LeftX",m_PortDmaIni[i].m_LeftX);
				Master.get("TopY",m_PortDmaIni[i].m_TopY);
				Master.get("Width",m_PortDmaIni[i].m_Width);

				tempstr="";
				Master.get("WidthRange",tempstr);
				CString tmp=tempstr.c_str();
				tmp.Replace(";","");

				int pos=tmp.Find("-");
				if(pos>0)
				{
					m_PortDmaIni[i].m_MinW=atoi(tmp.Left(pos));
					m_PortDmaIni[i].m_MaxW=atoi(tmp.Mid(pos+1));
				}
				else 
				{
					m_PortDmaIni[i].m_MinW=m_PortDmaIni[i].m_Width;
					m_PortDmaIni[i].m_MaxW=m_PortDmaIni[i].m_Width;
				}

				Master.get("Height",m_PortDmaIni[i].m_Height);

				tempstr="";
				Master.get("HeightRange",tempstr);
				tmp=tempstr.c_str();
				tmp.Replace(";","");
				pos=tmp.Find("-");
				if(pos>0)
				{
					m_PortDmaIni[i].m_MinH=atoi(tmp.Left(pos));
					m_PortDmaIni[i].m_MaxH=atoi(tmp.Mid(pos+1));
				}
				else 
				{
					m_PortDmaIni[i].m_MinH=m_PortDmaIni[i].m_Height;
					m_PortDmaIni[i].m_MaxH=m_PortDmaIni[i].m_Height;
				}

				Master.get("Exposure",m_PortDmaIni[i].m_Exposure);
				Master.get("FPS",m_PortDmaIni[i].m_FPS);
				Master.get("FPSMax",m_PortDmaIni[i].m_MaxFps);

				Master.get("Gain",m_PortDmaIni[i].m_Gain);

				tempstr="";
				Master.get("GainRange",tempstr);
				tmp=tempstr.c_str();
				tmp.Replace(";","");
				pos=tmp.Find("-");
				if(pos>0)
				{
					m_PortDmaIni[i].m_MinGain=atoi(tmp.Left(pos));
					m_PortDmaIni[i].m_MaxGain=atoi(tmp.Mid(pos+1));
				}
				else 
				{
					m_PortDmaIni[i].m_MinGain=m_PortDmaIni[i].m_Gain;
					m_PortDmaIni[i].m_MaxGain=m_PortDmaIni[i].m_Gain;
				}

				Master.get("BackLevel",m_PortDmaIni[i].m_BackLevel);//
				Master.get("BackLevelMax",m_PortDmaIni[i].m_MaxBackLevel);//
				Master.get("ExposureMode",m_PortDmaIni[i].m_ExposureMode);

				Master.get("Linlog",m_PortDmaIni[i].m_Linlog);

				Master.get("PixelFormat",m_PortDmaIni[i].m_PixelFormat);

				if(m_DoubleRate==1)
					m_PortDmaIni[i].m_PixelFormat=2;
				else if(m_DoubleRate==2)
					m_PortDmaIni[i].m_PixelFormat=3;

				Master.get("DataTurn",m_PortDmaIni[i].m_DataTurn);
				Master.get("BayerFilter",m_PortDmaIni[i].m_BayerFilter);
				Master.get("RedGain",m_PortDmaIni[i].m_RedGain);

				Master.get("GreenGain",m_PortDmaIni[i].m_GreenGain);
				Master.get("BlueGain",m_PortDmaIni[i].m_BlueGain);
				Master.get("RedGamma",m_PortDmaIni[i].m_RedGamma);

				Master.get("GreenGamma",m_PortDmaIni[i].m_GreenGamma);
				Master.get("BlueGamma",m_PortDmaIni[i].m_BlueGamma);

				tempstr="";
				Master.get("StorageID",tempstr);
				m_PortDmaIni[i].m_StorageID=tempstr.c_str();
				m_PortDmaIni[i].m_StorageID.Replace(";","");

				tempstr="";
				Master.get("Filepath",tempstr);
				tmp=tempstr.c_str();
				tmp.Replace(";","");

				stringstream strstr3( (LPCTSTR)tmp );
			    string spos="";

				getline(strstr3,spos,',');

				int k=0;
				while(spos!="")
				{
					if(spos!="")
					{
						m_PortDmaIni[i].m_Filepath[k]=spos.c_str();
						k++;
						if(k>=6)
							break;
					}

					spos="";
					getline(strstr3,spos,',');
				}

				m_PortDmaIni[i].m_Filenum=k;

				Master.get("StorageCashNum",m_PortDmaIni[i].m_StorageCashNum);
				Master.get("IsFact",m_PortDmaIni[i].m_IsFact);
				
				tempstr="";
				Master.get("FactPath",tempstr);
				m_PortDmaIni[i].m_FactPath=tempstr.c_str();
				m_PortDmaIni[i].m_FactPath.Replace(";","");

				Master.get("MinFreeSize",m_PortDmaIni[i].m_MinFreeSize);

				tempstr="";
				Master.get("RevEnable",m_PortDmaIni[i].m_RevEnable);
				Master.get("RevID",tempstr);
				m_PortDmaIni[i].m_RevID=tempstr.c_str();
				Master.get("Revport",m_PortDmaIni[i].m_RevPort);
				Master.get("RevRate",m_PortDmaIni[i].m_RevRate);
				Master.get("RevLen",m_PortDmaIni[i].m_RevLen);
			}
		}
	}

	
	return 0;
}

int CIniCtrl::GetLinkMode(int pos)
{

	CString avdll=m_CardIni[m_PortDmaIni[pos].m_FatherCardIndex].m_AcqCardApp;

	if(avdll.Find("Dual")>=0)
	{
		if(avdll.Find("RGB")>=0)
			return 500;

		return 108;
	}
	else if(avdll.Find("Full")>=0)
	{
		return 308;
	}
	else if(avdll.Find("Medium")>=0)
	{
		if(avdll.Find("RGB")>=0)
			return 218;

		return 208;
	}
	
	return m_PortDmaIni[pos].m_LinkMode;
}

/*2014 04 24 保存配置文件*/
BOOL CIniCtrl::SaveMasterIni(CString inipath)
{
	CString savepath=inipath;
	if(inipath=="")
		savepath=m_MasteriniPath;

	if(savepath=="")
		return FALSE;

	cini   Master((LPCTSTR)savepath);

	string tempstr="";
	Master["BaseInfo"];//

	tempstr="";
	//tempstr=(LPCTSTR)m_DeviceID;
	Master.set("DeviceID",m_DeviceID);

	tempstr="";
	tempstr=(LPCTSTR)m_DeviceName;
	Master.set("DeviceName",tempstr);

	
	tempstr="";
	tempstr=(LPCTSTR)m_LogPath;
	Master.set("LogPath",tempstr);

	tempstr="";
	Master["Ctrl"];//

	Master.set("ClientUDPEnable",m_ClientUDPEnable);
	Master.set("SvrUDPEnable",m_SvrUDPEnable);
	Master.set("ClientTCPEnable",m_ClientTCPEnable);
	Master.set("GPSEnable",m_GPSEnable);
	Master.set("SyncEnable",m_SyncEnable);
	Master.set("AcqCardNum",m_AcqCardNum);
	Master.set("ImageDmaNum",m_ImageDmaNum);

	tempstr="";
	Master["UDP"];//

	tempstr="";
	tempstr=(LPCTSTR)m_UdpID;
	Master.set("UdpID",tempstr);

	tempstr="";
	tempstr=(LPCTSTR)m_SvrIP;
	Master.set("SvrIP",tempstr);

	Master.set("SvrPort",m_SvrPort);
	Master.set("SendTime",m_SendTime);

	tempstr="";
	tempstr=(LPCTSTR)m_LocalIP;
	Master.set("LocalIP",tempstr);

	Master.set("LocalPort",m_LocalPort);

	tempstr="";
	Master["ClientTCP"];
	tempstr="";
	tempstr=(LPCTSTR)m_TCPSvrIP;
	Master.set("SvrIP",tempstr);

	Master.set("SvrPort",m_TCPSvrPort);
	
	tempstr="";
	Master["GPSCom"];//
	tempstr="";
	tempstr=(LPCTSTR)m_GPSID;
	Master.set("GPSID",tempstr);
	Master.set("GPSport",m_GpsPort);
	Master.set("GPSType",m_GpsType);
	Master.set("GPSRate",m_GpsRate);

	tempstr="";
	Master["SyncCom"];//
	tempstr=(LPCTSTR)m_SyncID;
	Master.set("SyncID",tempstr);
	Master.set("Syncport",m_SyncPort);
	Master.set("SyncRate",m_SyncRate);
	Master.set("SyncTigType",m_SyncTigType);
	Master.set("SyncEdge",m_SyncEdge);

	tempstr="";
	CString str="";
	CString tmp="";

	for(int i=0;i<12;i++){
		if(i>0)
			str+=",";
		tmp.Format("%d",m_SyncChennlEnable[i]);
		str+=tmp;

	}
	str+=";";
	tempstr=(LPCTSTR)str;
	Master.set("SyncChennlEnable",tempstr);

	tempstr="";
	CString str2="";
	CString tmp2="";
	for(int i=0;i<12;i++){
		if(i>0)
			str2+=",";
		tmp2.Format("%d",m_SyncDelay[i]);
		str2+=tmp2;

	}

	str2+=";";
	//tempstr=(LPCTSTR)str;
	Master.set("SyncDelay",(LPCTSTR)str2);


	CString str3="";
	CString tmp3="";
	for(int i=0;i<12;i++){
		if(i>0)
			str3+=",";
		tmp3.Format("%d",m_SyncFramRate[i]);
		str3+=tmp3;

	}

	str3+=";";
	//tempstr=(LPCTSTR)str;
	Master.set("SyncFramRate",(LPCTSTR)str3);

	tempstr="";
	if(m_AcqCardNum>0)
	{
		for(int i=0;i<m_AcqCardNum;i++)
		{
			CString key="";
			key.Format("AcqCardInfo%d",i);
			Master[key];

			tempstr="";
			tempstr=(LPCTSTR)m_CardIni[i].m_AcqCardID;
			Master.set("AcqCardID",tempstr);
	

			tempstr="";
			tempstr=(LPCTSTR)m_CardIni[i].m_AcqCardName;
			Master.set("AcqCardName",tempstr);

			Master.set("AcqCardIndex",m_CardIni[i].m_AcqCardIndex);

			tempstr="";
			tempstr=(LPCTSTR)m_CardIni[i].m_AcqCardApp;
			Master.set("AcqCardApp",tempstr);
		}

		if(m_ImageDmaNum>0)
		{
			for(int i=0;i<m_ImageDmaNum;i++)
			{
				CString key="";
				key.Format("PortDma%d",i);
				Master[key];

				Master.set("FatherCardIndex",m_PortDmaIni[i].m_FatherCardIndex);

				Master.set("DmaToCardIndex",m_PortDmaIni[i].m_DmaToCardIndex);

				Master.set("DmaIndex",m_PortDmaIni[i].m_DmaIndex);

				tempstr="";
				tempstr=(LPCTSTR)m_PortDmaIni[i].m_DmaName;
				Master.set("DmaName",tempstr);

				Master.set("AcqCashNum",m_PortDmaIni[i].m_AcqCashNum);

				tempstr="";
				tempstr=(LPCTSTR)m_PortDmaIni[i].m_CamDeviceID;
				Master.set("CamDeviceID",tempstr);

				tempstr="";
				tempstr=(LPCTSTR)m_PortDmaIni[i].m_CamDeviceName;
				Master.set("CamDeviceName",tempstr);


				Master.set("TriMode",m_PortDmaIni[i].m_TriMode);

				Master.set("TrigInput",m_PortDmaIni[i].m_TrigInput);
				Master.set("TriDelay",m_PortDmaIni[i].m_TriDelay);

				Master.set("LinkMode",m_PortDmaIni[i].m_LinkMode);

				Master.set("BitCount",m_PortDmaIni[i].m_BitCount);
				Master.set("BitAlign",m_PortDmaIni[i].m_BitAlign);

				Master.set("LeftX",m_PortDmaIni[i].m_LeftX);
				Master.set("TopY",m_PortDmaIni[i].m_TopY);
				Master.set("Width",m_PortDmaIni[i].m_Width);

				tempstr="";
				CString str="";
				str.Format("%d-%d",m_PortDmaIni[i].m_MinW,m_PortDmaIni[i].m_MaxW);
				tempstr=(LPCTSTR)str;
				Master.set("WidthRange",tempstr);
			
				Master.set("Height",m_PortDmaIni[i].m_Height);

				tempstr="";
				str.Format("%d-%d",m_PortDmaIni[i].m_MinH,m_PortDmaIni[i].m_MaxH);
				tempstr=(LPCTSTR)str;
				Master.set("HeightRange",tempstr);

				Master.set("Exposure",m_PortDmaIni[i].m_Exposure);
				Master.set("FPS",m_PortDmaIni[i].m_FPS);
				Master.set("FPSMax",m_PortDmaIni[i].m_MaxFps);

				Master.set("Gain",m_PortDmaIni[i].m_Gain);

				tempstr="";
				str.Format("%d-%d",m_PortDmaIni[i].m_MinGain,m_PortDmaIni[i].m_MaxGain);
				tempstr=(LPCTSTR)str;
				Master.set("GainRange",tempstr);

				Master.set("BackLevel",m_PortDmaIni[i].m_BackLevel);//
				Master.set("BackLevelMax",m_PortDmaIni[i].m_MaxBackLevel);//
				Master.set("ExposureMode",m_PortDmaIni[i].m_ExposureMode);

				Master.set("Linlog",m_PortDmaIni[i].m_Linlog);

				Master.set("PixelFormat",m_PortDmaIni[i].m_PixelFormat);

				Master.set("DataTurn",m_PortDmaIni[i].m_DataTurn);
				Master.set("BayerFilter",m_PortDmaIni[i].m_BayerFilter);
				Master.set("RedGain",m_PortDmaIni[i].m_RedGain);

				Master.set("GreenGain",m_PortDmaIni[i].m_GreenGain);
				Master.set("BlueGain",m_PortDmaIni[i].m_BlueGain);
				Master.set("RedGamma",m_PortDmaIni[i].m_RedGamma);

				Master.set("GreenGamma",m_PortDmaIni[i].m_GreenGamma);
				Master.set("BlueGamma",m_PortDmaIni[i].m_BlueGamma);

				tempstr="";
				tempstr=(LPCTSTR)m_PortDmaIni[i].m_StorageID;
				Master.set("StorageID",tempstr);

				tempstr="";

				str="";

				for(int j=0;j<m_PortDmaIni[i].m_Filenum;j++)
				{
					if(m_PortDmaIni[i].m_Filepath[j]!="")
					{
						if(j>0){
							str+=",";
						}

						str+=m_PortDmaIni[i].m_Filepath[j];
					}
				}

				str+=";";
				tempstr=(LPCTSTR)str;
				Master.set("Filepath",tempstr);

				Master.set("StorageCashNum",m_PortDmaIni[i].m_StorageCashNum);
				Master.set("IsFact",m_PortDmaIni[i].m_IsFact);
				
				tempstr="";
				tempstr=(LPCTSTR)m_PortDmaIni[i].m_FactPath;
				Master.set("FactPath",tempstr);

				Master.set("MinFreeSize",m_PortDmaIni[i].m_MinFreeSize);

				tempstr="";
				Master.set("RevEnable",m_PortDmaIni[i].m_RevEnable);

				tempstr=(LPCTSTR)m_PortDmaIni[i].m_RevID;
				Master.set("RevID",tempstr);
			
				Master.set("Revport",m_PortDmaIni[i].m_RevPort);
				Master.set("RevRate",m_PortDmaIni[i].m_RevRate);
				Master.set("RevLen",m_PortDmaIni[i].m_RevLen);
			}
		}
	}

	Master.save();

	return TRUE;

}


int CIniCtrl::CopyFile(char* src,char*dst){

	
   char ch;

     ifstream in(src);             //打开输入文件
    ofstream out(dst);              //打开输出文件
    if(!in)
    {
       // cout<<"can not open the file"<<endl;
        return 0;
    }
    if(!out)
    {
       // cout<<"can not open the file"<<endl;
        return 0;
    }
    while (in.get(ch))           //实现复制功能
        out.put(ch);
    in.close();
    out.close();
}