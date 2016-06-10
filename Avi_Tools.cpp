#include "StdAfx.h"
#include "Avi_Tools.h"
#include <iostream>
#include "Str_Tools.h"



Avi_Tools::Avi_Tools(){
}

Avi_Tools::~Avi_Tools(){

}

//得到视频的总帧数
/*
int Avi_Tools::GetTotalFrames(string Avi_ToolsName){
#define AVIHD 24

	
     FILE* fp = fopen(Avi_ToolsName.c_str(), "r");
	if(fp==NULL) 
	{
		printf("无法打开avi文件\n");
		return 0;
	}
	AVIMAINHEADER avih;        // AVI文件头结构体
	fseek(fp, AVIHD, SEEK_SET);
	int nRet = fread(&avih,  sizeof(avih),  1,  fp);   
	if(nRet <= 0) 
	{
		fclose( fp );
		fp = NULL; 
		return 0;
	}
	fclose( fp );
	return avih.dwTotalFrames;

}
*/
//开始初始化录制视频
bool Avi_Tools::InitAviCreate(PAVIFILE *m_pAVIFile,PAVISTREAM *m_ps,PAVISTREAM	*m_psCompressed,string aviFileName, int width,int height,int dwFrameRate,int nBitsPerPixel){
	
	CoInitialize(NULL);		// 初始化COM Lib

	AVISTREAMINFO StreamInfo;//视频头文件信息
	HRESULT hr;
//	PAVIFILE    m_pAVIFile;     ///< AVI文件的句柄
//	PAVISTREAM	m_ps;           ///< 通过AVI文件创建的视频流
//	PAVISTREAM	m_psCompressed; ///< 通过视频流和编码器创建的压缩视频流
	
	AVIFileInit();	
	//创建一个AVI文件	
	hr = AVIFileOpen(&*m_pAVIFile, Str_Tools::GetStr(aviFileName), OF_WRITE | OF_CREATE, NULL); 
	if (hr != AVIERR_OK)
	{
		return false;
	}
	
	if (hr != AVIERR_OK)
	{
		return true;
	}

	// 创建一个视频流
	  int lineByte=(width * nBitsPerPixel/8+3)/4*4;
	::ZeroMemory(&StreamInfo, sizeof(StreamInfo));
	StreamInfo.fccType                = streamtypeVIDEO;
	StreamInfo.fccHandler             = 0;
	StreamInfo.dwScale                = 1;                                       
	StreamInfo.dwRate                 = dwFrameRate;//
//	StreamInfo.dwSuggestedBufferSize  = DWORD(lineByte * height * 3); 
	StreamInfo.dwSuggestedBufferSize  = DWORD(lineByte*height*4);
	SetRect(&StreamInfo.rcFrame, 0, 0, width, height);
	hr = AVIFileCreateStream(*m_pAVIFile, &*m_ps, &StreamInfo);
	if (hr != AVIERR_OK)
	{
		AVIFileClose(*m_pAVIFile);
		AVIFileExit();
		return false;
	}


	//初始化压缩视频流需要的结构体
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
	opts.fccHandler = mmioFOURCC('D', 'I', 'B', ' '); // 0x20424944 DIB 

	//创建压缩视频流
	hr = AVIMakeCompressedStream(&*m_psCompressed, *m_ps, &opts, NULL);
	if (hr != AVIERR_OK)
	{
		AVIStreamClose(*m_ps);
		AVIFileClose(*m_pAVIFile);
		AVIFileExit();
		return false;
	}


	//写头文件信息
	BITMAPINFO *m_pInfo;


    m_pInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
    for(int i = 0; i < 256; i++)
	{
		m_pInfo->bmiColors[i].rgbRed =(BYTE)i;
		m_pInfo->bmiColors[i].rgbGreen =(BYTE)i;
		m_pInfo->bmiColors[i].rgbBlue =(BYTE)i;
		m_pInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//memset(m_pInfo, 0, sizeof(BITMAPINFOHEADER));
	m_pInfo->bmiHeader.biPlanes		= 1;
	m_pInfo->bmiHeader.biWidth		= width;
	m_pInfo->bmiHeader.biHeight		=height;
	m_pInfo->bmiHeader.biCompression	= BI_RGB;
	m_pInfo->bmiHeader.biBitCount	= nBitsPerPixel;
	m_pInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	m_pInfo->bmiHeader.biXPelsPerMeter   =   0;   
	m_pInfo->bmiHeader.biYPelsPerMeter   =   0;   
	m_pInfo->bmiHeader.biClrUsed   =   0;   
	m_pInfo->bmiHeader.biClrImportant   =   0;  
//	m_pInfo->bmiHeader.biSizeImage	=width *height* nBitsPerPixel/8;
	m_pInfo->bmiHeader.biSizeImage	=lineByte*height;

	//设置压缩视频流的输入文件格式。
	hr = AVIStreamSetFormat(*m_psCompressed, 0,(LPVOID)m_pInfo, sizeof(BITMAPINFOHEADER)+ (256 * sizeof(RGBQUAD)));
	if (hr != AVIERR_OK)
	{
		AVIStreamClose(*m_psCompressed);
		AVIStreamClose(*m_ps);
		AVIFileClose(*m_pAVIFile);
		AVIFileExit();
		return false;
	}



	return true;
}

//结束视频
bool Avi_Tools::StopAvi(PAVIFILE  *m_pAVIFile,PAVISTREAM *m_ps,PAVISTREAM *m_psCompressed){
		if (*m_psCompressed)
	{
		AVIStreamClose(*m_psCompressed);
		*m_psCompressed = NULL;
	}
	if (*m_ps)
	{
		AVIStreamClose(*m_ps);
		*m_ps = NULL;
	}
	if (*m_pAVIFile)
	{
		AVIFileEndRecord(*m_pAVIFile);
		AVIFileClose(*m_pAVIFile);
		*m_pAVIFile = NULL;
	}
	AVIFileExit();


	
	CoUninitialize();
	return true;


}
//录制视频
void Avi_Tools::SaveAvi(PAVISTREAM *m_psCompressed,unsigned char* pImageBuf, int nImageWidth, int nImageHeight,int m_nTimeFrame, int nBitsPerPixel){
  //  m_SaverCriSec.Enter();	
    int lineByte=(nImageWidth * nBitsPerPixel/8+3)/4*4;
	HRESULT hr = AVIStreamWrite(*m_psCompressed,
		m_nTimeFrame,
		1, 
		(BYTE*)pImageBuf,	
		LONG(lineByte * nImageHeight), 
		AVIIF_KEYFRAME, 
		NULL,
		NULL);
	//m_SaverCriSec.Leave();
	if (hr == AVIERR_OK )
	{
		m_nTimeFrame ++;
		
	}
}