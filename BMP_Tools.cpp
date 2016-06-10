#include "StdAfx.h"
#include "BMP_Tools.h"

BMP_Tools::BMP_Tools(){


}
BMP_Tools::~BMP_Tools(){


}
//��ȡbmpͼƬ
//��ɫͼ��ʱBGR�ĸ�ʽ
/*
bmpName:ͼƬ��·��
bmpWidth:�洢ͼƬ�Ŀ��
bmpHeight:�洢ͼƬ�ĸ���
pBmpBuf��ͼƬ��ָ������
biBitCount:ͼƬ��λ��

pColorTable����ɫ��
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
	int lineByte = (width *Count/8+3)/4*4;//����ͼ��ÿ��������ռ���ֽ���

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
string bmpName:�����ļ���·��
imgBuf��ͼƬ������
width:ͼƬ�Ŀ��
height:ͼƬ�ĸ���
biBitCount:ͼƬ������λ��
pColorTable:��ɫ��
*/
bool BMP_Tools::SaveBMP(string bmpName, unsigned char *imgBuf, int width, int height,int biBitCount,RGBQUAD *pColorTable){

	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������

	if(!imgBuf)  return false;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0

	int colorTablesize=0;

	if(biBitCount==8)

		colorTablesize=1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���

	int lineByte=(width * biBitCount/8+3)/4*4;

	//�Զ�����д�ķ�ʽ���ļ�


		FILE *fp=fopen(bmpName.c_str(),"wb");
	//  FILE *fp=fopen(bmpName.c_str(),"wb");

		if(fp==NULL) return false;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp����

	//bfSize��ͼ���ļ�4����ɲ���֮��

	fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)

		+ colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��

	fileHead.bfOffBits=54+colorTablesize;

	//д�ļ�ͷ���ļ�

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ

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

	//дλͼ��Ϣͷ���ڴ�

	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);

	//����Ҷ�ͼ������ɫ��д���ļ� 

	if(biBitCount==8)

		fwrite(pColorTable, sizeof(RGBQUAD),256, fp);

	//дλͼ���ݽ��ļ�

	fwrite(imgBuf, height*lineByte, 1, fp);

	//�ر��ļ�

	fclose(fp);

	return true;



}