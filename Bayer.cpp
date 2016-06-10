// ---------------
// Bayer.cpp
// ---------------

// --------
// Includes
// --------
#include "stdafx.h"
#include "Bayer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define R(x,y, nW, nH) pRGB24[0 + 3*(x) + nW * ((nH) - 1 - (y))]
#define G(x,y, nW, nH) pRGB24[1 + 3*(x) + nW * ((nH) - 1 - (y))]
#define B(x,y, nW, nH) pRGB24[2 + 3*(x) + nW * ((nH) - 1 - (y))]

#define Bay(x,y,nW) pBay[(x) + nW * (y)]


#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

CBayerFilter::CBayerFilter()
{
	nRedGain   = nGreenGain  = nBlueGain = 1;

	nBlueGamma = nGreenGamma = nRedGamma = 0;

	m_nBayerFilter = 0;
	m_nSrcWidth=0;
	m_nSrcHeight=0;
}

CBayerFilter::~CBayerFilter()
{
}

void CBayerFilter::SetSize(int nWidth1, int nWidth2,  int nHeigh1t)
{
	nWidth = nWidth1;
	nFactWidth = nWidth2;
	nHeight = nHeigh1t;
}

void CBayerFilter::SetGain(double dRed, double dBlue, double dGreen)
{
	nBlueGain  = dBlue; 
	nGreenGain = dGreen;
	nRedGain   = dRed;
}

void CBayerFilter::SetGamma(int nRed, int nBlue, int nGreen)
{
	nBlueGamma  = nBlue;
	nGreenGamma = nGreen;
	nRedGamma   = nRed;
}

//-----------------------------------------------------------

void CBayerFilter::bayer_copy1(u8 *pBay, u8 *pRGB24, int x, int y)
{
	double nTmp = 0;
	switch (m_nBayerFilter)
	{
	case 0:
		nTmp = Bay(x + 0, y + 0, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;
		nTmp = Bay(x + 1, y + 1, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		//nTmp = ((u32)Bay(x + 0, y + 0,nWidth), nWidth + (u32)Bay(x + 1, y + 1, nWidth)) / 2;
		nTmp = ((u32)Bay(x + 0, y + 0,nWidth)+ (u32)Bay(x + 1, y + 1, nWidth)) / 2;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 1, y + 0, nWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = R(x + 1, y + 0, nFactWidth, nHeight) = R(x + 1, y + 1, nFactWidth, nHeight) = R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;
		nTmp = Bay(x + 0, y + 1, nWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = B(x + 0, y + 0, nFactWidth, nHeight) = B(x + 0, y + 1, nFactWidth, nHeight) = B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 1:
		nTmp = Bay(x + 0, y + 1, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 1, y + 0, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 1,nWidth) + (u32)Bay(x + 1, y + 0, nWidth)) / 2;
		//nTmp = ((u32)Bay(x + 0, y + 1,nWidth), nWidth + (u32)Bay(x + 1, y + 0, nWidth)) / 2;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 0, nFactWidth, nHeight) = G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 0, y + 0, nWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = R(x + 1, y + 0, nFactWidth, nHeight) = R(x + 1, y + 1, nFactWidth, nHeight) = R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 1, y + 1, nWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = B(x + 0, y + 0, nFactWidth, nHeight) = B(x + 0, y + 1, nFactWidth, nHeight) = B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 2:
		break;
    case 3:
		break;
	}

}
void CBayerFilter::bayer_copy(u8 *pBay, u8 *pRGB24, int x, int y)
{

}
//-----------------------------------------------------------
void CBayerFilter::bayer_copy_BR3(u8 *pBay, u8 *pRGB24, int x, int y)
{

	G(x + 0, y + 0, nFactWidth, nHeight) = Bay(x + 0, y + 0, nWidth);
	G(x + 1, y + 1, nFactWidth, nHeight) = Bay(x + 1, y + 1, nWidth);
	G(x + 0, y + 1, nFactWidth, nHeight) = G(x + 1, y + 0, nFactWidth, nHeight) = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 2;
	R(x + 0, y + 0, nFactWidth, nHeight) = R(x + 1, y + 0, nFactWidth, nHeight) = R(x + 1, y + 1, nFactWidth, nHeight) = R(x + 0, y + 1, nFactWidth, nHeight) = Bay(x + 0, y + 1, nWidth);
	B(x + 1, y + 1, nFactWidth, nHeight) = B(x + 0, y + 0, nFactWidth, nHeight) = B(x + 0, y + 1, nFactWidth, nHeight) = B(x + 1, y + 0, nFactWidth, nHeight) = Bay(x + 1, y + 0, nWidth);

}
//----------------------------------------------------------------------------

void CBayerFilter::bayer_bilinear_BR3(u8 *pBay, u8 *pRGB24, int x, int y)
{

	R(x + 0, y + 0, nFactWidth, nHeight) = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 0, y - 1, nWidth)) / 2;
	G(x + 0, y + 0, nFactWidth, nHeight) = Bay(x + 0, y + 0, nWidth);
	B(x + 0, y + 0, nFactWidth, nHeight) = ((u32)Bay(x - 1, y + 0, nWidth) + (u32)Bay(x + 1, y + 0, nWidth)) / 2;

	R(x + 0, y + 1, nFactWidth, nHeight) = Bay(x + 0, y + 1, nWidth);
	G(x + 0, y + 1, nFactWidth, nHeight) = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 0, y + 2, nWidth)
			 + (u32)Bay(x - 1, y + 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 4;
	B(x + 0, y + 1, nFactWidth, nHeight) = ((u32)Bay(x + 1, y + 0, nWidth) + (u32)Bay(x - 1, y + 0, nWidth)
			 + (u32)Bay(x + 1, y + 2, nWidth) + (u32)Bay(x - 1, y + 2, nWidth)) / 4;

	R(x + 1, y + 0, nFactWidth, nHeight) = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 2, y + 1, nWidth)
			 + (u32)Bay(x + 0, y - 1, nWidth) + (u32)Bay(x + 2, y - 1, nWidth)) / 4;
	G(x + 1, y + 0, nFactWidth, nHeight) = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 2, y + 0, nWidth)
			 + (u32)Bay(x + 1, y - 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 4;
	B(x + 1, y + 0, nFactWidth, nHeight) = Bay(x + 1, y + 0, nWidth);

	R(x + 1, y + 1, nFactWidth, nHeight) = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 2, y + 1, nWidth)) / 2;
	G(x + 1, y + 1, nFactWidth, nHeight) = Bay(x + 1, y + 1, nWidth);
	B(x + 1, y + 1, nFactWidth, nHeight) = ((u32)Bay(x + 1, y + 0, nWidth) + (u32)Bay(x + 1, y + 2, nWidth)) / 2;

}
void CBayerFilter::bayer_bilinear(u8 *pBay, u8 *pRGB24, int x, int y)
{

}

void CBayerFilter::bayer_bilinear1(u8 *pBay, u8 *pRGB24, int x, int y)
{
	double nTmp = 0;
	switch (m_nBayerFilter)
	{
	case 0:
		nTmp = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 0, y - 1, nWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 0, y + 0, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
 		G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x - 1, y + 0, nWidth) + (u32)Bay(x + 1, y + 0, nWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 0, y + 1, nWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 0, y + 2, nWidth)
			+ (u32)Bay(x - 1, y + 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 1, y + 0, nWidth) + (u32)Bay(x - 1, y + 0, nWidth)
			+ (u32)Bay(x + 1, y + 2, nWidth) + (u32)Bay(x - 1, y + 2, nWidth)) / 4;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 2, y + 1, nWidth)
			+ (u32)Bay(x + 0, y - 1, nWidth) + (u32)Bay(x + 2, y - 1, nWidth)) / 4;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 2, y + 0, nWidth)
			+ (u32)Bay(x + 1, y - 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 1, y + 0, nWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 2, y + 1, nWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;


		nTmp = Bay(x + 1, y + 1, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;


		nTmp = ((u32)Bay(x + 1, y + 0, nWidth) + (u32)Bay(x + 1, y + 2, nWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 1:
		nTmp = ((u32)Bay(x - 1, y + 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)
				 + (u32)Bay(x - 1, y - 1, nWidth) + (u32)Bay(x + 1, y - 1, nWidth)) / 4;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x - 1, y + 0, nWidth) + (u32)Bay(x + 1, y + 0, nWidth)
				 + (u32)Bay(x + 0, y - 1, nWidth) + (u32)Bay(x + 0, y + 1, nWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
 		G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 0, y + 0, nWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 1, y - 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 0, y + 1, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 0, y + 2, nWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x - 1, y + 1, nWidth) + (u32)Bay(x + 1, y + 1, nWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 1, y + 0, nWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 2, y + 0, nWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(x + 1, y + 1, nWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 1, nWidth) + (u32)Bay(x + 1, y + 2, nWidth)
				 + (u32)Bay(x + 1, y + 0, nWidth) + (u32)Bay(x + 2, y + 1, nWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(x + 0, y + 0, nWidth) + (u32)Bay(x + 0, y + 2, nWidth)
				 + (u32)Bay(x + 2, y + 0, nWidth) + (u32)Bay(x + 2, y + 2, nWidth)) / 4;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 2:
		break;
    case 3:
		break;
	}
}
//-----------------------------------------------------------------------------
void CBayerFilter::bayer_to_rgb24(u8 *pBay, u8 *pRGB24)
{
	IplImage* src = cvCreateImageHeader(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	IplImage* dst = cvCreateImageHeader(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
	cvSetData(src, pBay, nWidth); ////3
	cvSetData(dst, pRGB24, nWidth* 3); ////3 
 
	cvCvtColor(src, dst, CV_BayerGB2RGB);
	cvFlip(dst);
	//cvNamedWindow("window", 1);
	//cvShowImage("window", dst);
	//cvWaitKey();
	//cvDestroyAllWindows();

	cvReleaseImageHeader(&src);
	cvReleaseImageHeader(&dst);//huj fff

 
/////hj

	//int i, j;

	//for (i = 0; i < nWidth; i += 2)
	//	for (j = 0; j < nHeight; j += 2)
	//		if (i == 0 || j == 0 || i == nWidth - 2 || j == nHeight - 2)
	//			bayer_copy1(pBay, pRGB24, i, j);
	//		else
	//			bayer_bilinear1(pBay, pRGB24, i, j);

	////////////
/*
	for( j = 0; j < nHeight; j++ )
		for( i=0; i < nWidth; i++ )
		{
			nTemp = pRGB24[ i*3 + nBitWidth*j]*nRedGain + nRedGamma;
			if(nTemp > 255) nTemp = 255;
			if(nTemp < 0 ) nTemp = 0;
			pRGB24[ i*3 + nBitWidth*j] = nTemp;

			nTemp =  pRGB24[ 1 +  i*3 + nBitWidth*j]*nGreenGain + nGreenGamma;
			if(nTemp > 255) nTemp = 255;
			if(nTemp < 0 ) nTemp = 0;
			pRGB24[ 1 +  i*3 + nBitWidth*j] = nTemp;

			nTemp = pRGB24[ 2 + i*3 + nBitWidth*j]*nBlueGain + nBlueGamma;
			if(nTemp > 255) nTemp = 255;
			if(nTemp < 0 ) nTemp = 0;
			pRGB24[ 2 + i*3 +nBitWidth*j] = nTemp;

		}
*/
}


void CBayerFilter::bayer_to_rgb24_zoomin(u8 *pBay, u8 *pRGB24){
	
	int i,j;


	m_nSrcWidth=2*nWidth;
	m_nSrcHeight=2*nHeight;
	for (i = 0; i < nWidth; i += 2)
		for (j = 0; j < nHeight; j += 2)
			if (i == 0 || j == 0 || i == nWidth - 2 || j == nHeight - 2)
				bayer_copy_zoomin(pBay, pRGB24, 2*i,2*j,i, j);
			else
				bayer_bilinear_zoomin(pBay, pRGB24, 2*i,2*j, i, j);

}

void CBayerFilter::bayer_copy_zoomin(u8 *pBay, u8 *pRGB24, int sx,int sy,int x, int y){

	double nTmp = 0;
	//int nSrcWidth=2*nWidth;
	switch (m_nBayerFilter)
	{
	case 0:
		nTmp = Bay(sx + 0, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;
		nTmp = Bay(sx + 1, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		//nTmp = ((u32)Bay(x + 0, y + 0,nWidth), nWidth + (u32)Bay(x + 1, y + 1, nWidth)) / 2;
		nTmp = ((u32)Bay(sx + 0,sy + 0,m_nSrcWidth)+ (u32)Bay(sx + 1, sy + 1, m_nSrcWidth)) / 2;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 1, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = R(x + 1, y + 0, nFactWidth, nHeight) = R(x + 1, y + 1, nFactWidth, nHeight) = R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;
		nTmp = Bay(sx + 0, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = B(x + 0, y + 0, nFactWidth, nHeight) = B(x + 0, y + 1, nFactWidth, nHeight) = B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 1:
		nTmp = Bay(sx + 0, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 1, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 1,m_nSrcWidth) + (u32)Bay(sx + 1, sy + 0, m_nSrcWidth)) / 2;
		//nTmp = ((u32)Bay(x + 0, y + 1,nWidth), nWidth + (u32)Bay(x + 1, y + 0, nWidth)) / 2;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 0, nFactWidth, nHeight) = G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 0, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = R(x + 1, y + 0, nFactWidth, nHeight) = R(x + 1, y + 1, nFactWidth, nHeight) = R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 1, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = B(x + 0, y + 0, nFactWidth, nHeight) = B(x + 0, y + 1, nFactWidth, nHeight) = B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 2:
		break;
    case 3:
		break;
	}

}
void CBayerFilter:: bayer_bilinear_zoomin(u8 *pBay, u8 *pRGB24,int sx,int sy, int x, int y){

		double nTmp = 0;
	switch (m_nBayerFilter)
	{
	case 0:
		nTmp = ((u32)Bay(sx + 0, sy + 1,m_nSrcWidth) + (u32)Bay(sx + 0, sy - 1, m_nSrcWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 0, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
 		G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx - 1, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 0, m_nSrcWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 0, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 0, sy + 2, m_nSrcWidth)
			+ (u32)Bay(sx - 1,sy + 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 1, m_nSrcWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 1, sy + 0, m_nSrcWidth) + (u32)Bay(sx - 1, sy + 0, m_nSrcWidth)
			+ (u32)Bay(sx + 1, sy + 2, m_nSrcWidth) + (u32)Bay(sx - 1, sy + 2, m_nSrcWidth)) / 4;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 1, m_nSrcWidth) + (u32)Bay(sx + 2, sy + 1, m_nSrcWidth)
			+ (u32)Bay(sx + 0, sy - 1, m_nSrcWidth) + (u32)Bay(sx + 2, sy - 1, m_nSrcWidth)) / 4;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 2, sy + 0, m_nSrcWidth)
			+ (u32)Bay(sx + 1,sy - 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 1, m_nSrcWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 1, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 1, m_nSrcWidth) + (u32)Bay(sx + 2, sy + 1, m_nSrcWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;


		nTmp = Bay(sx + 1, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;


		nTmp = ((u32)Bay(sx + 1, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 2, m_nSrcWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 1:
		nTmp = ((u32)Bay(sx - 1, sy + 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 1, m_nSrcWidth)
				 + (u32)Bay(sx - 1, sy - 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy - 1, m_nSrcWidth)) / 4;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx - 1, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 0, m_nSrcWidth)
				 + (u32)Bay(sx + 0, sy - 1, m_nSrcWidth) + (u32)Bay(sx + 0, sy + 1, m_nSrcWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
 		G(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 0, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 1, sy - 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 1, m_nSrcWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 0, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 0, sy + 2, m_nSrcWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 0, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx - 1, sy + 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 1, m_nSrcWidth)) / 2;
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 1, sy + 0, m_nSrcWidth);
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 2, sy + 0, m_nSrcWidth)) / 2;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 0, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = Bay(sx + 1, sy + 1, m_nSrcWidth);
		nTmp = nTmp * nBlueGain + nBlueGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		B(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 1, m_nSrcWidth) + (u32)Bay(sx + 1, sy + 2, m_nSrcWidth)
				 + (u32)Bay(sx + 1, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 2, sy + 1, m_nSrcWidth)) / 4;
		nTmp = nTmp * nGreenGain + nGreenGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		G(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;

		nTmp = ((u32)Bay(sx + 0, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 0, sy + 2, m_nSrcWidth)
				 + (u32)Bay(sx + 2, sy + 0, m_nSrcWidth) + (u32)Bay(sx + 2, sy + 2, m_nSrcWidth)) / 4;
		nTmp = nTmp * nRedGain + nRedGamma;
		if(nTmp > 255) nTmp = 255;
		if(nTmp < 0) nTmp = 0;
		R(x + 1, y + 1, nFactWidth, nHeight) = (u8)nTmp;
		break;
	case 2:
		break;
    case 3:
		break;
	}
}