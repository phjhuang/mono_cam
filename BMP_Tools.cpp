#include "StdAfx.h"
#include "BMP_Tools.h"

BMP_Tools::BMP_Tools(){


}
BMP_Tools::~BMP_Tools(){


}
//读取bmp图片
//彩色图像时BGR的格式
/*
bmpName:图片的路径
bmpWidth:存储图片的宽度
bmpHeight:存储图片的高速
pBmpBuf：图片的指针数据
biBitCount:图片的位数

pColorTable：颜色表
*/
bool BMP_Tools::ReadBMP(string bmpName, int *bmpWidth, int *bmpHeight,unsigned char **pBmpBuf,int *biBitCount,RGBQUAD**pColorTable){



		FILE *fp=fopen(bmpName.c_str(),"rb");

	if(fp==NULL){
		printf("cannot open file\n");
		return false;
	}

	fseek(fp,sizeof(BITMAPFILEHEADER),0);
	BITMAPINFOHEADER head;
	fread(&head,sizeof(BITMAPINFOHEADER),1,fp);
	int width = head.biWidth;
	int height = head.biHeight;
	int Count = head.biBitCount;
	int lineByte = (width *Count/8+3)/4*4;//计算图像每行像素所占的字节数

	if(Count == 8)
	{
		*pColorTable = new RGBQUAD[256];
		fread(*pColorTable,sizeof(RGBQUAD),256,fp);
	}

	*pBmpBuf = new unsigned char [lineByte *height];
	fread(*pBmpBuf,1,lineByte *height,fp);
	fclose(fp);
	*bmpWidth=width;
	*bmpHeight=height;
	*biBitCount=Count;



	return true;

}
/*
string bmpName:保存文件的路径
imgBuf：图片的数据
width:图片的宽度
height:图片的高速
biBitCount:图片的像素位数
pColorTable:彩色表
*/
bool BMP_Tools::SaveBMP(string bmpName, unsigned char *imgBuf, int width, int height,int biBitCount,RGBQUAD *pColorTable){

	//如果位图数据指针为0，则没有数据传入，函数返回

	if(!imgBuf)  return false;

	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0

	int colorTablesize=0;

	if(biBitCount==8)

		colorTablesize=1024;

	//待存储图像数据每行字节数为4的倍数

	int lineByte=(width * biBitCount/8+3)/4*4;

	//以二进制写的方式打开文件


		FILE *fp=fopen(bmpName.c_str(),"wb");
	//  FILE *fp=fopen(bmpName.c_str(),"wb");

		if(fp==NULL) return false;

	//申请位图文件头结构变量，填写文件头信息

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp类型

	//bfSize是图像文件4个组成部分之和

	fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)

		+ colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits是图像文件前3个部分所需空间之和

	fileHead.bfOffBits=54+colorTablesize;

	//写文件头进文件

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);

	//申请位图信息头结构变量，填写信息头信息

	BITMAPINFOHEADER head; 

	head.biBitCount=biBitCount;

	head.biClrImportant=0;

	head.biClrUsed=0;

	head.biCompression=0;

	head.biHeight=height;

	head.biPlanes=1;

	head.biSize=40;

	head.biSizeImage=lineByte*height;

	head.biWidth=width;

	head.biXPelsPerMeter=0;

	head.biYPelsPerMeter=0;

	//写位图信息头进内存

	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);

	//如果灰度图像，有颜色表，写入文件 

	if(biBitCount==8)

		fwrite(pColorTable, sizeof(RGBQUAD),256, fp);

	//写位图数据进文件

	fwrite(imgBuf, height*lineByte, 1, fp);

	//关闭文件

	fclose(fp);

	return true;



}