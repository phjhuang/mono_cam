#include "StdAfx.h"
#include "Avi_Tools.h"
#include <iostream>
#include "Str_Tools.h"



Avi_Tools::Avi_Tools(){
}

Avi_Tools::~Avi_Tools(){

}

//�õ���Ƶ����֡��
/*
int Avi_Tools::GetTotalFrames(string Avi_ToolsName){
#define AVIHD 24

	
     FILE* fp = fopen(Avi_ToolsName.c_str(), "r");
	if(fp==NULL) 
	{
		printf("�޷���avi�ļ�\n");
		return 0;
	}
	AVIMAINHEADER avih;        // AVI�ļ�ͷ�ṹ��
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
//��ʼ��ʼ��¼����Ƶ
bool Avi_Tools::InitAviCreate(PAVIFILE *m_pAVIFile,PAVISTREAM *m_ps,PAVISTREAM	*m_psCompressed,string aviFileName, int width,int height,int dwFrameRate,int nBitsPerPixel){
	
	CoInitialize(NULL);		// ��ʼ��COM Lib

	AVISTREAMINFO StreamInfo;//��Ƶͷ�ļ���Ϣ
	HRESULT hr;
//	PAVIFILE    m_pAVIFile;     ///< AVI�ļ��ľ��
//	PAVISTREAM	m_ps;           ///< ͨ��AVI�ļ���������Ƶ��
//	PAVISTREAM	m_psCompressed; ///< ͨ����Ƶ���ͱ�����������ѹ����Ƶ��
	
	AVIFileInit();	
	//����һ��AVI�ļ�	
	hr = AVIFileOpen(&*m_pAVIFile, Str_Tools::GetStr(aviFileName), OF_WRITE | OF_CREATE, NULL); 
	if (hr != AVIERR_OK)
	{
		return false;
	}
	
	if (hr != AVIERR_OK)
	{
		return true;
	}

	// ����һ����Ƶ��
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


	//��ʼ��ѹ����Ƶ����Ҫ�Ľṹ��
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
	opts.fccHandler = mmioFOURCC('D', 'I', 'B', ' '); // 0x20424944 DIB 

	//����ѹ����Ƶ��
	hr = AVIMakeCompressedStream(&*m_psCompressed, *m_ps, &opts, NULL);
	if (hr != AVIERR_OK)
	{
		AVIStreamClose(*m_ps);
		AVIFileClose(*m_pAVIFile);
		AVIFileExit();
		return false;
	}


	//дͷ�ļ���Ϣ
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

	//����ѹ����Ƶ���������ļ���ʽ��
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

//������Ƶ
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
//¼����Ƶ
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