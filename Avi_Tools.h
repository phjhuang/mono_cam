
#pragma once

#ifndef __AVIFILE_H
#define __AVIFILE_H
//#include <afx.h>
//#include <vfw.h> 
//#include <aviriff.h>
#include <string>
#include <stdio.h>

//#include <aviriff.h>


#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")
using namespace std;
class Avi_Tools{
public:
		Avi_Tools();
	~Avi_Tools();

	static int GetTotalFrames(string aviFileName);//得到视频的总帧数
	//录制视频开始初始化
	static bool InitAviCreate(PAVIFILE *m_pAVIFile,PAVISTREAM *m_ps,PAVISTREAM *m_psCompressed,string aviFileName, int width,int height,int dwFrameRate,int nBitsPerPixel);
	//停止视频
	static bool StopAvi(PAVIFILE  *m_pAVIFile,PAVISTREAM *m_ps,PAVISTREAM *m_psCompressed);
	//保存视频
	static void SaveAvi(PAVISTREAM *m_psCompressed,unsigned char* pImageBuf, int nImageWidth, int nImageHeight,int m_nTimeFrame,int nBitsPerPixel);
};
#endif