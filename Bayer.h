// --------------
// Bayer.h
// --------------

// -------------------
// One-Definition-Rule
// -------------------
#ifndef PREVIEWRECT_H_INCLUDE_NR1
#define PREVIEWRECT_H_INCLUDE_NR1

#if _MSC_VER > 1000
#pragma once
#endif

#define u8 unsigned char 
#define u16 unsigned short
#define u32 unsigned int




#define WIDTH 1312
#define HEIGHT 1082

#include <opencv/cv.h>
#include <opencv/highgui.h>



class CBayerFilter
{

public:

	CBayerFilter();
	virtual ~CBayerFilter();

	void bayer_to_rgb24(u8 *pBay, u8 *pRGB24);
	void bayer_to_rgb24_zoomin(u8 *pBay, u8 *pRGB24);//转变为RGB时候 把宽度高度都缩小二分之一

private:
	int m_nSrcWidth;
	int m_nSrcHeight;
	void bayer_copy(u8 *pBay, u8 *pRGB24, int x, int y);
	
	void bayer_bilinear(u8 *pBay, u8 *pRGB24, int x, int y);

	void bayer_copy1(u8 *pBay, u8 *pRGB24, int x, int y);
	void bayer_bilinear1(u8 *pBay, u8 *pRGB24, int x, int y);


	void bayer_copy_zoomin(u8 *pBay, u8 *pRGB24, int sx,int sy,int x, int y);
	void bayer_bilinear_zoomin(u8 *pBay, u8 *pRGB24,int sx,int sy, int x, int y);

	void bayer_bilinear_BR3(u8 *pBay, u8 *pRGB24, int x, int y);

	void bayer_copy_BR3(u8 *pBay, u8 *pRGB24, int x, int y);
	
	int nWidth;
	int nHeight;
	int nFactWidth;

private:
	
	double nBlueGain;
	double nGreenGain;
	double nRedGain;

	int nBlueGamma;
	int nGreenGamma;
	int nRedGamma;

	int m_nBayerFilter;

public:
	void SetGain(double dRed, double dBule, double dGreen);
	void SetGamma(int nRed, int nBlue, int nGreen);

	void SetBayFilter(int nVal) { m_nBayerFilter = nVal;}

public:
	void SetSize(int nWidth1, int nWidth2,  int nHeigh1t);
};

#endif