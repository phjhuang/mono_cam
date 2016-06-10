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
  //读取文件
  static bool ReadBMP(string bmpName, int *bmpWidth, int *bmpHeight,unsigned char **pBmpBuf,int *biBitCount,RGBQUAD** pColorTable);
  //保存文件
  static bool SaveBMP(string bmpName, unsigned char *imgBuf, int width, int height,int biBitCount, RGBQUAD *pColorTable);


};