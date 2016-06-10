// ImgPreview.cpp: implementation of the CImgPreview class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CalibrationCtrl.h"
#include "ImgPreview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImgPreview::CImgPreview()
{
	m_IsShow=TRUE;
	m_Fps=0;
	m_nCasheOne=0;
	m_nCasheTwo=0;
	m_nAcqCount=0;
	m_nSaveCount=0;
	m_MoveItemX=0;
	m_MoveItemY=0;

	m_CurposX=0;
	m_CurposY=0;
	m_nPiexl=8;
	m_Isbayer=0;
	m_nAlign=0;
	m_nOff=4;
	m_pShowData=NULL;
	m_nWidth=512;
	m_nHeight=512;
	m_ShowLen=0;
	m_DibLib = NULL;
	m_Msg=_T("");
	m_ComMsg=_T("");
	m_pData= NULL;
	m_nData=0;
	m_IsShowThread=FALSE;
	m_IsRead=FALSE;
	m_pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)];
	m_pPattle = (LOGPALETTE*)new char[sizeof(LOGPALETTE)+ 256*sizeof(PALETTEENTRY)];
	ASSERT(m_pBmpInfo);
	ASSERT(m_pPattle);
	m_pPattle->palNumEntries = 256;
	m_pPattle->palVersion = 0x300;
	for(int i = 0; i < 256; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed =(BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbGreen =(BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbBlue =(BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;
		m_pPattle->palPalEntry[i].peRed =  m_pBmpInfo->bmiColors[i].rgbRed;
		m_pPattle->palPalEntry[i].peGreen = m_pBmpInfo->bmiColors[i].rgbGreen;
		m_pPattle->palPalEntry[i].peBlue = m_pBmpInfo->bmiColors[i].rgbBlue;
		m_pPattle->palPalEntry[i].peFlags = 0;
	}
	m_hPattle = CreatePalette(m_pPattle);

	Reset();

	m_bDraw = TRUE;

	m_Exit    = CreateEvent(NULL, TRUE, FALSE, NULL);

	IsOpen=FALSE;
	m_Oldlen=0;

	m_IsFullSceen=FALSE;

}

CImgPreview::~CImgPreview()
{

	CloseHandle(m_Exit);
	m_Exit=NULL;

	CloseImgHandle();
	if(m_pBmpInfo) { delete m_pBmpInfo; m_pBmpInfo = NULL; }
	DeleteObject(m_hPattle);
	if(m_pPattle) { delete m_pPattle; m_pPattle = NULL; }

	if(m_pShowData)
		delete m_pShowData;

	m_pShowData=NULL;
}



void CImgPreview::SetHDC(HDC hVal,CRect Rect,int nWidth, int nHeight)
{
	BOOL bVal = TRUE;
	m_Hdc = hVal;
	m_DestSize.cx = nWidth;
	m_DestSize.cy = nHeight;

	m_ShowRect=Rect;
	if(m_DibLib)
	{
		bVal = DrawDibEnd(m_DibLib);
		bVal = DrawDibBegin(m_DibLib, m_Hdc, m_DestSize.cx, m_DestSize.cy, &m_pBmpInfo->bmiHeader, m_SrcSize.cx, m_SrcSize.cy, DDF_SAME_DRAW);
	}
}

void CImgPreview::UptImg(int nWidth, int nHeight, int nPixel,BOOL IsNew)
{
	LPBITMAPINFOHEADER pHead = NULL;
	pHead = &m_pBmpInfo->bmiHeader;
	pHead->biBitCount = nPixel;
	pHead->biClrImportant = 0;
	pHead->biClrUsed = 0;
	pHead->biCompression = 0;
	pHead->biHeight = nHeight;
	pHead->biPlanes = 1;
	pHead->biSize = sizeof(BITMAPINFOHEADER);
	pHead->biSizeImage = WIDTHBYTES(nWidth * nPixel) * nHeight;
	pHead->biWidth = nWidth;
	pHead->biXPelsPerMeter = 0;
	pHead->biYPelsPerMeter = 0;

	if(IsNew)
	{
		m_SrcSize.cx = nWidth;
		m_SrcSize.cy = nHeight;
	}

	if(m_DibLib)
	{
		DrawDibEnd(m_DibLib);
		DrawDibBegin(m_DibLib, m_Hdc, m_DestSize.cx, m_DestSize.cy, &m_pBmpInfo->bmiHeader, m_SrcSize.cx, m_SrcSize.cy, DDF_SAME_DRAW);
	}
}

void CImgPreview::SetSrcOffSet(int nLeft, int nTop)
{
	m_SrcLeftTop.x = nLeft;
	m_SrcLeftTop.y = nTop;
}

void CImgPreview::SetDestOffSet(int nLeft, int nTop)
{
	m_DestLeftTop.x = nLeft;
	m_DestLeftTop.y = nTop;
}

void CImgPreview::SetPattle(HPALETTE pPattle)
{
	if(m_DibLib) DrawDibSetPalette(m_DibLib, pPattle);
}

BOOL CImgPreview::OpenImgHandle()
{
	BOOL bVal = TRUE;
	if(m_DibLib) CloseImgHandle();
	m_DibLib = DrawDibOpen();
	if(m_DibLib == NULL) return FALSE;
	if(!DrawDibBegin(m_DibLib, m_Hdc, m_DestSize.cx, m_DestSize.cy, &m_pBmpInfo->bmiHeader, m_SrcSize.cx, m_SrcSize.cy, DDF_SAME_DRAW))
	{
		DrawDibClose(m_DibLib); m_DibLib = NULL; return FALSE;
	}
	bVal = DrawDibSetPalette(m_DibLib, m_hPattle);



	return TRUE;
}

void CImgPreview::CloseImgHandle()
{
	if(m_DibLib == NULL) return;
	DrawDibEnd(m_DibLib); 
	DrawDibClose(m_DibLib); m_DibLib = NULL;
}

BOOL CImgPreview::IsEnable()
{
	return (m_DibLib != NULL);
}

BOOL CImgPreview::DrawImg(unsigned char* pData)
{
	//InvalidateRect(m_Hdc);
	BOOL bSuc = TRUE;
	if(m_DibLib == NULL || !m_bDraw) return FALSE;
	bSuc = DrawDibDraw(m_DibLib,
	                   m_Hdc,
	                   m_DestLeftTop.x, m_DestLeftTop.y,
	                   m_DestSize.cx, m_DestSize.cy,
	                   &m_pBmpInfo->bmiHeader,
	                   pData,
	                   m_SrcLeftTop.x, m_SrcLeftTop.y,
	                   m_SrcSize.cx, m_SrcSize.cy, DDF_SAME_DRAW|DDF_SAME_HDC);

	 /*CPen pen(PS_SOLID,1,RGB(0,0,0)); //红色实线画笔，决定画图的边线
	 SelectObject(m_Hdc,&pen);

	 CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	 SelectObject(m_Hdc,pBrush);

	Ellipse(m_Hdc,95,95,115,115);*/

	//FillRect(m_Hdc,CRect(95,95,115,115),(HBRUSH)GetStockObject(NULL_BRUSH));
/*	if(m_IsFullSceen){
	
		return TRUE;
	}
	RECT rr;

	rr.left=0;
	rr.top=(m_DestLeftTop.y+m_DestSize.cy)-20;
	rr.right=m_DestSize.cx;
	rr.bottom=(m_DestLeftTop.y+m_DestSize.cy);
	CBrush brush(RGB(255,255,255)); 


	CString msg=_T("");

	if(!IsOpen)
	{


		//msg.Format("帧率：%0.4d Hz,采集个数:%0.7d,一级缓冲:%0.4d,二级缓冲:%0.4d,存储个数:%d   %s %s",m_Fps,m_nAcqCount,m_nCasheOne,m_nCasheTwo,m_nSaveCount,m_Msg,m_ComMsg);//
	
		msg.Format("帧率：%0.4d Hz,采集个数:%0.7d,一级缓冲:%0.4d,二级缓冲:%0.4d,存储个数:%d",m_Fps,m_nAcqCount,m_nCasheOne,m_nCasheTwo,m_nSaveCount,m_Msg,m_ComMsg);
		
		if(m_Oldlen!=msg.GetLength()&&m_Oldlen>0){
		
			FillRect(m_Hdc,&m_oldRect,brush);
		}


		if(m_Oldlen>0&& rr.top!=m_oldRect.top){
			
			FillRect(m_Hdc,&m_oldRect,brush);

		}
		

	}
	else
	{
	
		msg.Format("时间：%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d.%0.3d,位置:%0.10d/%0.10d %s %s",m_ShowTime.wYear,m_ShowTime.wMonth,m_ShowTime.wDay,m_ShowTime.wHour,m_ShowTime.wMinute,m_ShowTime.wSecond,m_ShowTime.wMilliseconds,m_CurrtPos,m_nSaveCount,m_Msg,m_ComMsg);//
	}
	TextOut(m_Hdc,rr.left,rr.top,(LPCTSTR)msg,msg.GetLength());//

	m_Oldlen=msg.GetLength();
	m_oldRect.left=rr.left;
	m_oldRect.top=rr.top;
	m_oldRect.right=rr.right+10;
	m_oldRect.bottom=rr.bottom;
*/

	//
	return bSuc;
}

void CImgPreview::UpDate()
{
	if(m_DibLib)
	{
		DrawDibEnd(m_DibLib);
		DrawDibBegin(m_DibLib, m_Hdc, m_DestSize.cx, m_DestSize.cy, &m_pBmpInfo->bmiHeader, m_SrcSize.cx, m_SrcSize.cy, DDF_SAME_DRAW);
	}
}

void CImgPreview::UpdateDrawVariable(BOOL bVal)
{
	m_bDraw = bVal;
}

void CImgPreview::Reset()
{
	m_SrcSize.cx = m_SrcSize.cy = 0;
	m_SrcLeftTop.x = m_SrcLeftTop.y = 0;
	m_DestSize.cx = 0;
	m_DestSize.cy = 0;
	m_DestLeftTop.x = m_DestLeftTop.y = 0;
}

void CImgPreview::GetBayerInfo(BayerInfo &pInfo)
{
	pInfo.nFilter=m_Bayerinfo.nFilter;
	pInfo.nRedGain=m_Bayerinfo.nRedGain;
	pInfo.nBlueGain=m_Bayerinfo.nBlueGain;
	pInfo.nGreenGain=m_Bayerinfo.nGreenGain;
	pInfo.nRedGamma=m_Bayerinfo.nRedGamma;
	pInfo.nBlueGamma=m_Bayerinfo.nBlueGamma;
	pInfo.nGreenGamma=m_Bayerinfo.nGreenGamma;
}

void CImgPreview::SetBayerInfo(BayerInfo pInfo)
{
    m_IsShow=FALSE;
	m_Bayerinfo.nFilter=pInfo.nFilter;
	m_Bayerinfo.nRedGain=pInfo.nRedGain;
	m_Bayerinfo.nBlueGain=pInfo.nBlueGain;
	m_Bayerinfo.nGreenGain=pInfo.nGreenGain;
	m_Bayerinfo.nRedGamma=pInfo.nRedGamma;
	m_Bayerinfo.nBlueGamma=pInfo.nBlueGamma;
	m_Bayerinfo.nGreenGamma=pInfo.nGreenGamma;
	Sleep(10);
	m_pBayer.SetBayFilter(pInfo.nFilter);
	m_pBayer.SetGain(pInfo.nRedGain, pInfo.nBlueGain, pInfo.nGreenGain);
	m_pBayer.SetGamma(pInfo.nRedGamma, pInfo.nBlueGamma, pInfo.nGreenGamma);
	m_IsShow=TRUE;
}

void CImgPreview::SetFormat(int nWidth, int nHeight,int nPiexl, int Isbayer, int nAlign, int nOff)
{
	m_nPiexl=nPiexl;	
	m_Isbayer=Isbayer;	
	m_nAlign=nAlign;
	m_nOff=nOff;

	m_nWidth=nWidth;
	m_nHeight=nHeight;
}

void CImgPreview::SetPicData(PBYTE pData, int nData)
{
	if(!m_IsRead)
	{
		//DEBUG_INFO1("ddddddd, %d", 3);
		
		m_pData = pData; 
		m_nData = nData; 
	}

}

void  CImgPreview::SetAcqInfo1(int nfps,int nacq,int one,PBYTE msg)
{
	m_Fps=nfps;
	m_nAcqCount=nacq;
	m_nCasheOne=one;

	if(msg)
	  m_ComMsg=msg;
	else m_ComMsg=_T("");
}

void  CImgPreview::SetAcqInfo2(int nsave,int two,CString msg)
{
	m_nCasheTwo=two;
	m_nSaveCount=nsave;
	m_Msg=msg;
}

void CImgPreview::SetHfInfo(int CurrtPos,int savecont,SYSTEMTIME ShowTime,CString msg)
{
	m_nSaveCount=savecont;

	m_ShowTime=ShowTime;
	m_CurrtPos=CurrtPos;
	m_Msg=msg;
}

void CImgPreview::SetExitData()
{
	SetEvent(m_Exit);
}

long CImgPreview::GetShowData(BYTE *PImageData, PBYTE* PShowData)
{
	// TODO: Add your implementation code here
	if(!PImageData) return 0;
	int nBuf = 0, nSize = 0, nFactWidth = 0;
	PBYTE pBuf = NULL;
	if(m_Isbayer==1||(m_nPiexl==24))
	{
		nFactWidth = WIDTHBYTES(m_nWidth * 24);
		nSize = nFactWidth  * m_nHeight;
	}
	else
	{
		if(m_nPiexl==32){
			nFactWidth = WIDTHBYTES(m_nWidth*m_nPiexl);
			nSize = nFactWidth  * m_nHeight;
		}
		else{
			nFactWidth = WIDTHBYTES(m_nWidth*8);
			nSize = nFactWidth  * m_nHeight;
		}
	}

	if(nSize!=m_ShowLen)
	{
		if(*PShowData)
		{
			delete *PShowData;
			*PShowData=NULL;
		}

		*PShowData=new BYTE[nSize];

		m_ShowLen=nSize;
	}

	pBuf=*PShowData;
	if(m_Isbayer==1)
	{
		if(m_IsFullSceen){
			m_pBayer.SetSize(m_nWidth, nFactWidth, m_nHeight);
			m_pBayer.bayer_to_rgb24(PImageData, pBuf);
		}
		else{

			m_pBayer.SetSize(m_nWidth, nFactWidth, m_nHeight);
			m_pBayer.bayer_to_rgb24_zoomin(PImageData, pBuf);
		}
		return nSize;
	}
	else if(m_nPiexl==24)
	{
		memcpy(pBuf, PImageData, nSize);

		return nSize;
	}

	if(!m_IsFullSceen){
		//for(int i = m_nHeight - 1; i >= 0; i--)
		for(int i = m_nHeight*2 - 1; i >= 0; i-=2)
		{
			int j = 0;
			switch(m_nPiexl)
			{
			case 8:
				for(j = 0; j < m_nWidth*2; j += 2)
				{
					memcpy(pBuf + nBuf, PImageData + i * m_nWidth*2+j, 1);
					nBuf++;
				}
				//memcpy(pBuf + nBuf, PImageData + i * m_nWidth, m_nWidth);
				//nBuf += m_nWidth;
				break;
			case 10:
				/*
				memcpy(pBuf + nBuf, PImageData + i * m_nWidth, m_nWidth);
				nBuf += m_nWidth;
				break;
				*/
			case 12:
			case 14:
			case 16:
				//for(j = 0; j < 2*m_nWidth; j += 2)
				for(j = 0; j < 4*m_nWidth; j += 4)
				{
					WORD nVal = 0;
					if(m_nAlign)
					{
						nVal = *(PImageData + i*2*m_nWidth + j);
						nVal <<= 8; nVal |= *(PImageData + i*2*m_nWidth + j + 1);
					}
					else
					{
						nVal = *(WORD*)(PImageData + i*2*m_nWidth + j);
					}


					nVal >>= m_nOff;


					pBuf[nBuf++] = nVal & 0xff;
				}
				break;
			case 32:

				for(j = 0; j < m_nWidth*2; j += 2)
				{
					memcpy(pBuf + nBuf, PImageData + i * nFactWidth*2+j, 1);
					nBuf++;
				}
				break;
				//  memcpy(pBuf + nBuf, PImageData + i * nFactWidth,nFactWidth);
				//nBuf += nFactWidth;
				break;
			default:
				return 0;
			}
		}

	}
	else{

		for(int i = m_nHeight - 1; i >= 0; i--)
			//for(int i = m_nHeight*2 - 1; i >= 0; i-=2)
		{
			int j = 0;
			switch(m_nPiexl)
			{
			case 8:

				memcpy(pBuf + nBuf, PImageData + i * m_nWidth, m_nWidth);
				nBuf += m_nWidth;
				break;
			case 10:
				/*
				memcpy(pBuf + nBuf, PImageData + i * m_nWidth, m_nWidth);
				nBuf += m_nWidth;
				break;
				*/
			case 12:
			case 14:
			case 16:
				for(j = 0; j < 2*m_nWidth; j += 2)
					//	for(j = 0; j < 4*m_nWidth; j += 4)
				{
					WORD nVal = 0;
					if(m_nAlign)
					{
						nVal = *(PImageData + i*2*m_nWidth + j);
						nVal <<= 8; nVal |= *(PImageData + i*2*m_nWidth + j + 1);
					}
					else
					{
						nVal = *(WORD*)(PImageData + i*2*m_nWidth + j);
					}


					nVal >>= m_nOff;


					pBuf[nBuf++] = nVal & 0xff;
				}
				break;
			case 32:

				memcpy(pBuf + nBuf, PImageData + i * nFactWidth,nFactWidth);
				nBuf += nFactWidth;
				break;
			default:
				return 0;
			}
		}
	}
	return nBuf;
}

//显示线程处理函数
UINT CImgPreview::ThreadDrawFuc(LPVOID lpParam)
{
	CImgPreview* p = (CImgPreview*)lpParam;
	if( p == NULL ) return 0;
	p->m_IsShowThread = TRUE;
	while( p->m_IsShowThread )
	{
	
		if(!p->IsOpen)
		{
			p->m_IsRead=TRUE;
			if((p->m_pData)&&(p->m_IsShow))
			{ 

				if(p->GetShowData(p->m_pData,&(p->m_pShowData))>0)
					p->DrawImg(p->m_pShowData);

				p->m_pData = NULL;
			}
			p->m_IsRead=FALSE;

			Sleep(50);
		}
		else 
		{	

			if((p->m_IsShow)&&(p->m_pData))
			{

				// if(p->GetShowData(p->m_pData,&(p->m_pShowData))>0)
				//	p->DrawImg(p->m_pShowData);
				//p->UpdateDraw();
				p->DrawImg(p->m_pData);
			}


			Sleep(200);
		}

		if(!p->m_IsShowThread)
			break;
	}

	p->m_IsShowThread=FALSE;
	return 0;
}

void CImgPreview::StartDraw()
{
	AfxBeginThread(ThreadDrawFuc, this, THREAD_PRIORITY_NORMAL);//启动显示线程
}

void CImgPreview::StopDraw()
{
	m_IsShowThread=FALSE;

	Sleep(100);

	m_pData = NULL;

	m_Oldlen=0;

	m_ShowLen=0;

	/*
	if(m_ShowEnd)
	{
	ShowEndPic();
	}
	m_ShowEnd=FALSE;
	*/
}

void CImgPreview::Zoomin()
{
	if((m_DestSize.cx-m_DestLeftTop.x)<=10000)
	{
		m_DestLeftTop.x -= 10;
		m_DestSize.cx +=20;
		m_SrcLeftTop.x = 0;
		m_SrcSize.cx=m_nWidth;
		if(m_DestLeftTop.x<=0){

			m_MoveItemX+=2;

			if(abs(m_DestLeftTop.x)!=(m_CurposX*10))
				m_CurposX=abs(m_DestLeftTop.x)/10;
		}
		else
		{
			m_MoveItemX=0;

		}
	}

	if((m_DestSize.cy-m_DestLeftTop.y)<=10000)
	{
		m_DestLeftTop.y -= 10;
		m_DestSize.cy += 20;
		m_SrcLeftTop.y = 0;
		m_SrcSize.cy=m_nHeight;

		if(m_DestLeftTop.y<=0){
			m_MoveItemY+=2;

			if(abs(m_DestLeftTop.y)!=(m_CurposY*10))
				m_CurposY=abs(m_DestLeftTop.y)/10;
		}
		else
		{

			m_MoveItemY=0;
		}
	}
	/*20140212 dlp*/
	// DrawPic();
	//StretchDIBits(m_Hdc, m_ShowRect.left , m_ShowRect.top, m_ShowRect.Width(),m_ShowRect.Height(), 0, 0, m_EndShowWidth, m_EndShowHeight, m_pEndShowData, m_pEndShowInfo, DIB_RGB_COLORS, SRCCOPY);
	UpDate();
	UpdateDrawVariable(TRUE);



}

void CImgPreview::Zoomout()
{
	if((m_DestSize.cx-m_DestLeftTop.x)>m_ShowRect.Width())
	{
		m_DestLeftTop.x += 10;
		m_DestSize.cx -=20;
		m_SrcLeftTop.x = 0;
		m_SrcSize.cx=m_nWidth;

		m_MoveItemX-=2;
	}

	else{
		m_DestLeftTop.x += 10;
		m_DestSize.cx -=20;
		m_SrcLeftTop.x = 0;
		m_SrcSize.cx=m_nWidth;
		m_MoveItemX=0;


	}
	if((m_DestSize.cy-m_DestLeftTop.y)>m_ShowRect.Height())
	{
		m_DestLeftTop.y += 10;
		m_DestSize.cy -= 20;
		m_SrcLeftTop.y = 0;
		m_SrcSize.cy=m_nHeight;

		m_MoveItemY-=2;
	}
	else{

		m_DestLeftTop.y += 10;
		m_DestSize.cy -= 20;
		m_SrcLeftTop.y = 0;
		m_SrcSize.cy=m_nHeight;

		m_MoveItemY=0;

	}

	UpDate();
	UpdateDrawVariable(TRUE);
	/*20140212 dlp*/
	//    DrawPic();
}

void CImgPreview::GetMoveItem(int& ItemX,int& itemY)
{
	ItemX=m_MoveItemX;
	itemY=m_MoveItemY;
}

void CImgPreview::MoveX(int posX)
{
	if(m_MoveItemX<=0)
		return ;

	if((posX>=0)&&(posX<=m_MoveItemX))
	{
		int pos=posX*10;

		m_DestLeftTop.x=-pos;
		//m_DestLeftTop.x-=pos;

		m_CurposX=posX;

		UpDate();
		UpdateDrawVariable(TRUE);

	}
	/*20140212 dlp*/
	// DrawPic();

}

void CImgPreview::MoveY(int posY)
{
	if(m_MoveItemY<=0)
		return ;

	if((posY>=0)&&(posY<=m_MoveItemY))
	{
		int pos=posY*10;

		m_DestLeftTop.y=-pos;

		//m_DestLeftTop.y-=pos;

		m_CurposY=posY;


		UpDate();
		UpdateDrawVariable(TRUE);
	}

	/*20140212 dlp*/
	// DrawPic();


}

void CImgPreview::GetMovePos(int& posX,int& posY)
{
	posX=m_CurposX;
	posY=m_CurposY;
}

void CImgPreview::SetActusize()
{

	//DrawPic();
	int w1=m_ShowRect.Width();

	int cx=(m_nWidth-w1)/10;

	if(cx>0)
	{
		m_DestLeftTop.x = -cx/2;
		m_DestSize.cx =m_nWidth;
		m_SrcLeftTop.x = 0;
		m_SrcSize.cx=m_nWidth;

		m_MoveItemX=cx;

		if(abs(m_DestLeftTop.x)!=(m_CurposX*10))
			m_CurposX=abs(m_DestLeftTop.x)/10;
	}
	else{

		m_DestLeftTop.x=(w1-m_nWidth)/2;
		m_DestSize.cx=m_nWidth;
		m_SrcLeftTop.x = 0;
		m_SrcSize.cx=m_nWidth;
		m_MoveItemX=0;
		m_CurposX=0;

	}

	int h1=m_ShowRect.Height();

	int cy=(m_nHeight-h1)/10;

	if(cy>0)
	{
		m_DestLeftTop.y = -cy/2;
		m_DestSize.cy =m_nHeight;
		m_SrcLeftTop.y = 0;
		m_SrcSize.cy=m_nHeight;

		m_MoveItemY=cy;

		if(abs(m_DestLeftTop.y)!=(m_CurposY*10))
			m_CurposY=abs(m_DestLeftTop.y)/10;
	}

	else{

		m_DestLeftTop.y =(h1-m_nHeight)/2;
		m_DestSize.cy =m_nHeight;
		m_SrcLeftTop.y = 0;
		m_SrcSize.cy=m_nHeight;
		m_MoveItemY=0;
		m_CurposY=0;

	}

	/*20140212 dlp*/
	// DrawPic();
	UpDate();
	UpdateDrawVariable(TRUE);
}

void CImgPreview::SetAdaptWindow()
{


	m_DestLeftTop.x =0;
	m_DestSize.cx =m_ShowRect.Width();
	m_SrcLeftTop.x = 0;
	m_SrcSize.cx=m_nWidth;

	m_MoveItemX=0;
	m_CurposX=0;

	m_DestLeftTop.y = 0;
	m_DestSize.cy =m_ShowRect.Height();
	m_SrcLeftTop.y = 0;
	m_SrcSize.cy=m_nHeight;

	m_MoveItemY=0;
	m_CurposY=0;

	/*20140212 dlp*/
	// DrawPic();
	//	StretchDIBits(m_Hdc, m_ShowRect.left , m_ShowRect.top, m_ShowRect.Width(),m_ShowRect.Height(), 0, 0, m_EndShowWidth, m_EndShowHeight, m_pEndShowData, m_pEndShowInfo, DIB_RGB_COLORS, SRCCOPY);
	UpDate();
	UpdateDrawVariable(TRUE);

}
/*绘图*/
void CImgPreview::DrawPic(){ 


	m_IsShow=TRUE;
	if((m_IsShow)&&(m_pData))
	{
		DrawImg(m_pData);
	}
	m_IsShow=FALSE;
}
void CImgPreview::UpdateDraw(){
	    
	if(!IsOpen){
	
		if((!m_IsShow)&&(m_pData)&&m_IsUpdate){
		//	if(GetShowData(m_pData,&(m_pShowData))>0)
			DrawImg(m_pShowData);
		}
	}
	else
	{
	  if((!m_IsShow)&&(m_pData)&&IsOpen)
		{

		DrawImg   (m_pData);
		}
	}

}
void CImgPreview::ShowGrabPic(){

	m_IsShow=TRUE;

	if((m_IsShow)&&(m_pData))
	{
		if(GetShowData(m_pData,&(m_pShowData))>0)
			DrawImg(m_pShowData);

	}
	m_IsShow=FALSE;
}

BOOL CImgPreview::GetImagePos(CPoint Destpoint,CPoint& Svcpoint)
{
	Svcpoint.x=((m_SrcSize.cx-m_SrcLeftTop.x)*(Destpoint.x-m_DestLeftTop.x))/m_DestSize.cx;

	Svcpoint.y=m_SrcSize.cy-((m_SrcSize.cy-m_SrcLeftTop.y)*(Destpoint.y-m_DestLeftTop.y))/m_DestSize.cy;

	if((Svcpoint.x<m_SrcLeftTop.x)||(Svcpoint.x>=m_SrcSize.cx))
		return FALSE;

	if((Svcpoint.y<m_SrcLeftTop.y)||(Svcpoint.y>=m_SrcSize.cy))
		return FALSE;

	return TRUE;
}