#pragma once

#include "ImgViewForm.h"
#include "MapFileCtrl.h"
#include ".\\codedmark\\_codedTarget.h"

class CMoreImageCtrl
{
public:	
	
	
	PBYTE m_pclibImage[3];

	PBYTE m_pclibImage24[2]; //huangjin

	PBYTE m_pSdData;

	int m_ClibW[2];
	int m_ClibH[2];
	int m_ClibDept[2];
	BOOL m_ClibImageIsBater[2];
	
	CPoint m_Clibpoint[2][15];

	


public:
	CMoreImageCtrl(void);
	~CMoreImageCtrl(void);

public:
	///////////////////////////////clib
	CString m_ClibImageFile[2];
	CString m_SDFile;
    CImgPreview* m_CLBTImgPreview[3];
	int m_SmallShowNum;
	int m_SmalShowCount;
	CPoint m_SamllShowPos;

	////////////////////////////dat
	CString m_DatImageFile[2];
	CString m_BDFile;
	CImgPreview* m_DatImgPreview[2];
	CMapFileCtrl m_MapCtrl[2];

	PBYTE m_pDatImage[2];
	PBYTE m_pShowData[2];
	long m_SaveCount[2];
	double m_Snaptime[2];
	int m_W[2];
	int m_H[2];
	int m_Depht[2];

	long m_CurPos[2];
public:

	//////////////////////clib
	BOOL GetClibFileData();
	

	void ShowClibImage(bool displaymark, const CodedTarget& ct1,const CodedTarget& ct2);
	void ShowClibSmallImage(int nport,int showcount,CPoint *point);
	void SetClibPreview(int nport, HDC pShowHdc, CRect rect);
	void SetClibPoint(int nport,int nposnum,float xbz,float ybz);


	////////////////////////////dat
	BOOL OpenDatFile();
	void CloseDatFile();
	void SetDatPreview(int nport, HDC pShowHdc, CRect rect);
	void ShowDatImage(int nport,long imagepos);
	long GetCurImagePos(int nport,long& imagecount);
	static BOOL __stdcall RevMsg( void* pData,long cout, long num,int mstype);




	////////////huangjin65
	
	CImgViewForm *m_ShowView;
	PBYTE m_pSmallData;
	CImgPreview m_PosImgPreview;
	void ShowPosPic(HDC pShowHdc, CRect rect,int w,int h);
	void GetPosPic(int nport,int posX,int posY,int& w,int& h);
};

