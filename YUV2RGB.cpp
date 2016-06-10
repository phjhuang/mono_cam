#include "StdAfx.h"
#include "YUV2RGB.h" 



int CYUV2RGB::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
	unsigned int pixel32 = 0;
	unsigned char *pixel = (unsigned char *)&pixel32;
	int r, g, b;
	//  r = y + (1.370705 * (v-128));
	//g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
	// b = y + (1.732446 * (u-128));
	
	
	r = y + (1.402 * (v-128));
	g = y - (0.34414 * (v-128)) - (0.71414 * (u-128));
	b = y + (1.772 * (u-128));
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	pixel[0] = b;
	pixel[1] = g ;
	pixel[2] = r ;
	return pixel32;
}

int CYUV2RGB::convert_yuv_to_rgb_buffer(BYTE *yuv, BYTE *rgb,  int width,  int height){
	
	long rowbytes=((width+47)/48)*128;
	long imgsize=rowbytes*height;
	
	BYTE * Arraybit=new BYTE[imgsize*8];
	memset(Arraybit,0,imgsize*8);
	int ind=0;
	int count=0;
	int i;
	
	for(i=0;i<imgsize;i++){
		
		for(ind=0;ind<8;ind++){
			Arraybit[count]=((yuv[i]>>ind) & 0x1);
			count++;
			
		}
		
	}
	
	
	
	
	int num=rowbytes*8/128;	
	
	BYTE *buffer=new BYTE[rowbytes*8];
	BYTE *buffer_cell=new BYTE[128];
	
    int ii=0;
	int jj=0;
	int mm=0;
	int kk=0;
	int k1=0;
	int out=0;
	int begin=0;
	int ending=0;
	int eval=10;
	
	short int du0=0,dy0=0,dv1=0,dy1=0;
	int aa=0;
	int bb=0;
	unsigned int pixel32;
	unsigned char pixel_24[3];
	
	for(jj=0;jj<height;jj++){
		memset(buffer,0,rowbytes*8);
		memcpy(buffer,Arraybit+jj*rowbytes*8,rowbytes*8);
		mm=0;
		ii=0;
		begin=0;
		
		
		for(kk=0;kk<num;kk++){
			
			k1=kk*128;	
			
			memcpy(buffer_cell,buffer+k1,128);
			
			
			for(ii=0;ii<3;ii++){
				du0=0;
				dy0=0;
				dv1=0;
				dy1=0;
				begin=ii*42;
				ending=begin+eval-1;
				for(ind=0;ind<10;ind++){
					
					du0=du0+((buffer_cell[begin+ind]&0x1)<<ind);
				}
				
				aa=int((begin+10)/32);
				bb=int((begin+12)/32);
				if(aa==bb)
					begin=ending+1;
				else
					begin=ending+3;
				ending=begin+eval-1;
				
				for(ind=0;ind<10;ind++){
					
					dy0=dy0+((buffer_cell[begin+ind]&0x1)<<ind);
				}
				
				aa=int((begin+10)/32);
				bb=int((begin+12)/32);
				if(aa==bb)
					begin=ending+1;
				else
					begin=ending+3;
				
				ending=begin+eval-1;
				
				
				for(ind=0;ind<10;ind++){
					
					dv1=dv1+((buffer_cell[begin+ind]&0x1)<<ind);
				}
				
				aa=int((begin+10)/32);
				bb=int((begin+12)/32);
				if(aa==bb)
					begin=ending+1;
				else
					begin=ending+3;
				
				ending=begin+eval-1;
				
				
				for(ind=0;ind<10;ind++){
					
					dy1=dy1+((buffer_cell[begin+ind]&0x1)<<ind);
				}
				
				
				du0=du0*256/1024;
				dy0=dy0*256/1024;
				dv1=dv1*256/1024;
				dy1=dy1*256/1024;
				
				
				pixel32 = CYUV2RGB::convert_yuv_to_rgb_pixel(dy0, du0, dv1);
				pixel_24[0] = (pixel32 & 0x000000ff);
				pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
				pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
				rgb[out++] = pixel_24[0];
				rgb[out++] = pixel_24[1];
				rgb[out++] = pixel_24[2];
				
				
				pixel32 = CYUV2RGB::convert_yuv_to_rgb_pixel(dy1, du0, dv1);
				
				pixel_24[0] = (pixel32 & 0x000000ff);
				pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
				pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
				rgb[out++] = pixel_24[0];
				rgb[out++] = pixel_24[1];
				rgb[out++] = pixel_24[2];
				
				
				
			}
			
			
		}
		
	}
	if(Arraybit)
		delete Arraybit;
	Arraybit=NULL;
	if(buffer)
		delete buffer;
	buffer=NULL;
	
	if(buffer_cell)
		delete buffer_cell;
	buffer_cell=NULL;
	
	return 0;
	
}

int CYUV2RGB::convert_to_gray8(BYTE* src,BYTE* dst,int m_nWidth,int m_nHeight,int m_nPixelByte,int m_nAlign,int m_nOff){

	int nFactWidth=m_nWidth*m_nPixelByte;
	int nBuf = 0;

	//20150811 宽度不是4的倍数的处理
	int nW=m_nWidth;

	if(nW%4>0)
		nW=(nW/4+1)*4;

		//	for(int i =0; i< m_nHeight; i++)
	for(int i = m_nHeight - 1; i >= 0; i--)
	{
		int j = 0;
		switch(m_nPixelByte)
		{
		case 1:
		    
			for(j = 0; j < m_nWidth; j ++)
			{
				
				/*
				dst[nBuf]=src[i * m_nWidth+j];
				dst[nBuf+1]=src[i * m_nWidth+j];
				dst[nBuf+2]=src[i * m_nWidth+j];
				//memcpy(pBuf + nBuf, PImageData + i * m_nWidth, m_nWidth);
			    nBuf +=3; //m_nWidth;
				*/

				dst[(m_nHeight-1-i)*nW*3+3*j]=src[i * m_nWidth+j];
				dst[(m_nHeight-1-i)*nW*3+3*j+1]=src[i * m_nWidth+j];
				dst[(m_nHeight-1-i)*nW*3+3*j+2]=src[i * m_nWidth+j];
				nBuf +=3;
				//memcpy(dst + nBuf, src + i * nFactWidth, m_nWidth);
			//nBuf += nFactWidth;
			}
			break;
		case 2:
			for(j = 0; j < 2*m_nWidth; j += 2)
			{
				WORD nVal = 0;
				if(m_nAlign)
				{
					nVal = *(src+ i*2*m_nWidth + j);
				    nVal <<= 8; nVal |= *(src + i*2*m_nWidth + j + 1);
				}
				else
				{
					nVal = *(WORD*)(src + i*2*m_nWidth + j);
				}

		
                nVal >>= m_nOff;

				dst[nBuf]=nVal & 0xff;
				dst[nBuf+1]=nVal & 0xff;
				dst[nBuf+2]=nVal & 0xff;
				//memcpy(pBuf + nBuf, PImageData + i * m_nWidth, m_nWidth);
			    nBuf +=3; //m_nWidth;

				//dst[nBuf++] = nVal & 0xff;
			}
			break;
		case 3:

		  memcpy(dst + nBuf, src + i * nFactWidth,nFactWidth);
			nBuf += nFactWidth;
			break;
		default:
			return 0;
		}
	}



return nBuf;
}


int CYUV2RGB::convert_to_src(BYTE* src,BYTE* dst,int m_nWidth,int m_nHeight,int m_nPixelByte,int m_nAlign,int m_nOff,int nFormat){

	int nFactWidth=m_nWidth*m_nPixelByte;
	int nBuf = 0;

	//for(int i =0; i< m_nHeight; i++)
	for(int i = m_nHeight - 1; i >= 0; i--)
	{
		int j = 0;
		switch(m_nPixelByte)
		{
		case 1:
		
			memcpy(dst + nBuf, src + i * nFactWidth, m_nWidth);
			nBuf += nFactWidth;
			break;
		case 2:
			for(j = 0; j < 2*m_nWidth; j += 2)
			{
				WORD nVal = 0;
				if(m_nAlign)
				{
					nVal = *(src+ i*2*m_nWidth + j);
				    nVal <<= 8; nVal |= *(src + i*2*m_nWidth + j + 1);
				}
				else
				{
					nVal = *(WORD*)(src + i*2*m_nWidth + j);
				}

		
                nVal >>= m_nOff;


				dst[nBuf++] = nVal & 0xff;
			}
			break;
		case 3:

		  memcpy(dst + nBuf, src + i * nFactWidth,nFactWidth);
			nBuf += nFactWidth;
			break;
		default:
			return 0;
		}
	}



return nBuf;
}