#include <stdio.h>
#include <windows.h>
#include  <math.h>
//YUV转RGB
/*输入YUV格式：10位YUV422
输出格式：8位RGB


*/
class CYUV2RGB{
public:

	static int convert_yuv_to_rgb_pixel(int y, int u, int v);
    static int convert_yuv_to_rgb_buffer(BYTE *yuv,BYTE *rgb, int width, int height);
	static int convert_to_gray8(BYTE* src,BYTE* dst,int width,int height,int m_nPixelByte,int m_nAlign,int m_nOff);
    static int convert_to_src(BYTE* src,BYTE* dst,int width,int height,int m_nPixelByte,int m_nAlign,int m_nOff,int nFormat);

};

