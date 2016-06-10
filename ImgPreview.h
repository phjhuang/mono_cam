// ImgPreview.h: interface for the CImgPreview class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGPREVIEW_H__F92326CA_3772_4370_BF1E_7B77CBDB905A__INCLUDED_)
#define AFX_IMGPREVIEW_H__F92326CA_3772_4370_BF1E_7B77CBDB905A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>
#include "..\\..\include\Bayer.h"


#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"Vfw32.lib")

struct BayerInfo
{
	int nFilter;
	float nRedGain;
	float nBlueGain;
	float nGreenGain;
	int nRedGamma;
	int nBlueGamma;
	int nGreenGamma; 
};

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

class CImgPreview  
{
public:
	CImgPreview();
	virtual ~CImgPreview();

 
	HDRAWDIB m_DibLib;
    
	HDC m_Hdc;

	CRect m_ShowRect;
	int m_MoveItemX;
	int m_MoveItemY;
	int m_CurposX;
	int m_CurposY;
	volatile BOOL m_bDraw;

	PBITMAPINFO m_pBmpInfo;
	LOGPALETTE* m_pPattle;
	HPALETTE m_hPattle;

	CBayerFilter m_pBayer;
    BayerInfo   m_Bayerinfo;

	int m_nPiexl;	
	int m_Isbayer;	
	int m_nAlign;
	int m_nOff;

	int m_nWidth;
	int m_nHeight;

	int m_ShowLen;

	PBYTE m_pData;
	long m_nData;

    BOOL m_IsShowThread;

	BOOL m_IsRead;
	PBYTE m_pShowData;
	HANDLE m_Exit;//线程退出事件句柄

	int m_Fps;
	int m_nCasheOne;
	int m_nCasheTwo;
	int m_nAcqCount;
	int m_nSaveCount;
	CString m_Msg;
	CString m_ComMsg;


	//结束采集时的显示画面

	BYTE* m_pEndShowData;
	BITMAPINFO*  m_pEndShowInfo;
	int m_EndShowWidth;
	int m_EndShowHeight;
  




public:
	BOOL m_IsFullSceen;
	
	CPoint m_SrcLeftTop;
	CPoint m_DestLeftTop;
	CSize  m_SrcSize;    
	CSize  m_DestSize;

	SYSTEMTIME m_ShowTime;
	int m_CurrtPos;
	BOOL IsOpen;
	BOOL m_IsShow;
	RECT m_oldRect;
	int m_Oldlen;
	/*
	int OldMsgX;
	int OldMsgY;
	int Oldlen;
	*/
	//BOOL m_ShowEnd;
public:
	void SetHDC(HDC hVal,CRect Rect,int nWidth, int nHeight);	
	void UptImg(int nWidth, int nHeight, int nPixel,BOOL IsNew=TRUE);
	void SetDestOffSet(int nLeft, int nTop);
	void SetSrcOffSet(int nLeft, int nTop);
	void Zoomin();
	void Zoomout();
	void GetMoveItem(int& ItemX,int& itemY);
	void GetMovePos(int& posX,int& posY);
	void MoveX(int posX);
	void MoveY(int posY);
	void SetActusize();
	void SetAdaptWindow();

	void SetPattle(HPALETTE pPattle);
	BOOL DrawImg(unsigned char* pData);    

	BOOL IsEnable();

	void UpDate();

	void Reset();

	BOOL GetImagePos(CPoint Destpoint,CPoint& Svcpoint);
public:
	void StopDraw();
	void StartDraw();
	long GetShowData(BYTE* PImageData,PBYTE* PShowData);
	void SetExitData();
	void SetPicData(PBYTE pData, int nData);
	void SetAcqInfo1(int nfps,int nacq,int one,PBYTE msg=NULL);
	void SetAcqInfo2(int nsave,int two,CString msg);
	void SetHfInfo(int CurrtPos,int savecont,SYSTEMTIME ShowTime,CString msg);
	void SetFormat(int nWidth, int nHeight,int nPiexl,int Isbayer, int nAlign,int nOff);
	void SetBayerInfo(BayerInfo pInfo);
	void GetBayerInfo(BayerInfo& pInfo);
	BOOL OpenImgHandle();
	void CloseImgHandle();
	void UpdateDrawVariable(BOOL bVal);


	/**20140212 dlp*/
	void DrawPic();

	void UpdateDraw();//更新
	BOOL m_IsUpdate;
	void ShowGrabPic();


private:
	static UINT ThreadDrawFuc(LPVOID lpParam);//实际处理函数
};

#endif // !defined(AFX_IMGPREVIEW_H__F92326CA_3772_4370_BF1E_7B77CBDB905A__INCLUDED_)
