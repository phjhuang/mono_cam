//

#include "StdAfx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

class BMP_Tools{
public:
  BMP_Tools();
  ~BMP_Tools();
  //��ȡ�ļ�
  static bool ReadBMP(string bmpName, int *bmpWidth, int *bmpHeight,unsigned char **pBmpBuf,int *biBitCount,RGBQUAD** pColorTable);
  //�����ļ�
  static bool SaveBMP(string bmpName, unsigned char *imgBuf, int width, int height,int biBitCount, RGBQUAD *pColorTable);


};