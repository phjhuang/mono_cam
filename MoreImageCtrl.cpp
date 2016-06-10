#include "StdAfx.h"
#include "MoreImageCtrl.h"


CMoreImageCtrl::CMoreImageCtrl(void)
{

    m_pSdData=NULL;
    m_SDFile="";
    for(int i=0; i<2; i++)
        {
            m_pclibImage24[i]=NULL;  ////////////huangjin
			m_W[i]=0;////////////huangjin
			m_H[i]=0;////////////huangjin

            m_pclibImage[i]=NULL;
            m_ClibW[i]=0;
            m_ClibH[i]=0;
            m_ClibDept[i]=0;
            m_ClibImageIsBater[i]=0;

            m_ClibImageFile[i]="";

            for(int k=0; k<15; k++)
                {
                    m_Clibpoint[i][k].x=0;
                    m_Clibpoint[i][k].y=0;
                }


			///TRACE("%s/n", G_IniData.m_MsmtDatID);
			//mono   if(!m_MapCtrl[i].CacheMapCom_Init((LPCTSTR)G_IniData.m_MsmtDatID,NULL))

			CString  cstr = "CCasheMap.BaseCasheMap.1";

			if (!m_MapCtrl[i].CacheMapCom_Init((LPCTSTR)cstr, NULL))
			{
				AfxMessageBox("请检查图像数据控制组件!");
				//return;
			}

            m_MapCtrl[i].SetLoopEnable(FALSE,0);

            long nSize=1024*1024;
            m_MapCtrl[i].SetMinDiskSize(nSize);

            m_pDatImage[i]=NULL;
            m_pShowData[i]=NULL;

            m_CurPos[i]=0;

        }

    m_SmallShowNum=0;
    m_SamllShowPos.x=0;
    m_SamllShowPos.y=0;
    m_SmalShowCount=0;
    m_pclibImage[2]=NULL;

}


CMoreImageCtrl::~CMoreImageCtrl(void)
{
    for(int i=0; i<3; i++)
        {
            if(m_pclibImage[i])
                delete m_pclibImage[i];

            m_pclibImage[i]=NULL;
        }


    for(int i=0; i<2; i++)
        {
            if(m_pclibImage24[i])
                delete[] m_pclibImage24[i];
            m_pclibImage24[i]=NULL;
            ///////////huangjin
        }



}

BOOL CMoreImageCtrl::GetClibFileData()
{
    for(int i=0; i<2; i++)
        {
            if(!FSO_Tools::isFsoExists((LPCTSTR)m_ClibImageFile[i]))
                return FALSE;
        }

    for(int i=0; i<3; i++)
        {
            if(m_pclibImage[i])
                delete m_pclibImage[i];

            m_pclibImage[i]=NULL;
        }


    int k=0;
    for(int i=0; i<2; i++)
        {
            CFile file;

            if(file.Open(m_ClibImageFile[i],CFile::modeRead))
                {
                    m_pclibImage[i]=new BYTE[file.GetLength()];

                    file.Read(m_pclibImage[i],file.GetLength());

                    BITMAPFILEHEADER pfileInfo;
                    BITMAPINFOHEADER pheadInfo;

                    CopyMemory( &pfileInfo, m_pclibImage[i],  14);
                    CopyMemory( &pheadInfo,m_pclibImage[i]+14 , 40);

                    m_ClibW[i]=pheadInfo.biWidth;
                    m_ClibH[i]=pheadInfo.biHeight;
                    m_ClibDept[i]=pheadInfo.biBitCount;

                    m_ClibImageIsBater[i]=FALSE;

                    k++;
                }

            file.Close();
        }

    if(k==2)
        {
            m_pclibImage[2]=new BYTE[100*100*3];
            return TRUE;
        }
    else
        {
            for(int i=0; i<2; i++)
                {
                    if(m_pclibImage[i])
                        delete m_pclibImage[i];

                    m_pclibImage[i]=NULL;
                }

            return FALSE;
        }

}


void CMoreImageCtrl::SetClibPreview(int nport, HDC pShowHdc, CRect rect)
{
    if(nport>=3)
        return ;

    int port=nport;

    int nw=0,nh=0,depht=0,formattype=0;


    if(nport<2)
        {
            nw=m_ClibW[nport];
            nh=m_ClibH[nport];
            formattype=m_ClibImageIsBater[nport];
        }
    else
        {
            nw=40;
            nh=40;
            formattype=FALSE;
        }


    formattype=1;      //////////////huangjin


    depht=8;//m_MapCtrl[port].m_pHead->nPixel*

    //	CImgPreview* pview=&m_ImgPreview[port];
    CImgPreview* pview=m_CLBTImgPreview[nport];

    pview->IsOpen=TRUE;

    pview->m_ShowTime.wYear=0;
    pview->m_ShowTime.wMonth=0;
    pview->m_ShowTime.wDay=0;
    pview->m_ShowTime.wHour=0;
    pview->m_ShowTime.wMinute=0;
    pview->m_ShowTime.wSecond=0;
    pview->m_ShowTime.wMilliseconds=0;


    pview->m_CurrtPos=1;

    int nBitCount=8;

    if(nport<2)
        nBitCount=m_ClibDept[nport];

    int nShowBit=8;

    if(formattype==1)
        {
            nShowBit=24;
            nBitCount=24;
        }

    int m_Bild=1;
    if(nBitCount<=8)
        m_Bild=1;
    else if(nBitCount<=16)
        m_Bild=2;
    else if(nBitCount==24)
        m_Bild=3;
    else if(nBitCount==32)
        m_Bild=4;
    else nBitCount=8;
//	printf("Bild:%d\n",m_Bild);

    int nAlign=0;
    int nOff=0;
    if(nBitCount>8)
        {
            nAlign=0;
            nOff=nBitCount-(m_Bild-1)*8;
        }

    pview->SetFormat(nw, nh,nShowBit,formattype,0,4);
    pview->SetHDC(pShowHdc,rect, rect.Width(), rect.Height());//显示新修改部分

    pview->UptImg(nw,nh,nShowBit);

    pview->OpenImgHandle();

    pview->Reset();

    pview->UpdateDrawVariable(FALSE);

    int rectW=rect.Width();


    if( ( rectW<= nw)||(nport==2) )
        {
            pview->m_DestLeftTop.x = 0;
            pview->m_SrcLeftTop.x = 0;
            pview->m_DestSize.cx = rectW;//W1;
            pview->m_SrcSize.cx = nw ;//rect.Width();//;W1;
        }
    else
        {
            pview->m_DestSize.cx = pview->m_SrcSize.cx =nw;
            pview->m_DestLeftTop.x = (rectW - nw)/2;
            pview->m_SrcLeftTop.x = 0;
        }


    int rectH=rect.Height();
    if(( rectH <= nh )||(nport==2))
        {
            pview->m_DestLeftTop.y = 0;
            pview->m_SrcLeftTop.y = 0;
            pview->m_DestSize.cy = rectH;//H1;
            pview->m_SrcSize.cy = nh;//rect.Height();//H1;

        }
    else
        {
            pview->m_DestSize.cy = pview->m_SrcSize.cy = nh;
            pview->m_DestLeftTop.y = (rectH -nh)/2;
            pview->m_SrcLeftTop.y = 0;
        }

    pview->UpDate();

    pview->UpdateDrawVariable(TRUE);

}

void CMoreImageCtrl::ShowClibImage(bool displaymark, const CodedTarget& ct1,const CodedTarget& ct2 )
{
	for (int i=0; i<2 ; i++)
	{
		if (m_pclibImage24[i]!=NULL)
		{
			delete[] m_pclibImage24[i];			
		}
		m_pclibImage24[i] = new BYTE[m_ClibH[i]*m_ClibW[i]*3];
	}

	if (displaymark==true)
	    {
			memcpy(m_pclibImage24[0],ct1.m_imgc->imageData,m_ClibH[0]*m_ClibW[0]*3*sizeof(BYTE));
			memcpy(m_pclibImage24[1],ct2.m_imgc->imageData,m_ClibH[1]*m_ClibW[1]*3*sizeof(BYTE));

	        goto BYPASS;
	    }


	for (int i=0; i<2 ; i++)
		for(int j=0; j<m_ClibH[i]*m_ClibW[i]; j++)
		{
			*(m_pclibImage24[i] +3*j) = *(m_pclibImage24[i] +3*j+1) =*(m_pclibImage24[i] +3*j+2) = *(m_pclibImage[i] +1078+j);
		}

BYPASS:
    for(int i=0; i<2; i++)
        {
            ////huangjin 24
            //////////m_CLBTImgPreview[i]->SetPicData(m_pclibImage[i]+54+4*256,0);

            m_CLBTImgPreview[i]->SetPicData(m_pclibImage24[i],0);//(BYTE*)ct2.m_imgc->imageData
            m_CLBTImgPreview[i]->DrawPic();
			
        }

}


void CMoreImageCtrl::ShowClibSmallImage(int nport,int showcount,CPoint *point)
{
    if((nport>=2)||(!point))
        return ;

    int left=0,top=0,right=0,bottom=0;
    left=point->x-20;
    right=point->x+20;
    top=point->y-20;
    bottom=point->y+20;

    if(left<0)
        left=0;

    if(top<0)
        top=0;

    if(right>m_ClibW[nport])
        right=m_ClibW[nport];

    if(bottom>m_ClibH[nport])
        bottom=m_ClibH[nport];


    //m_pclibImage[2]
    int w=right-left;

    int k=0;
    for(int i=top; i<bottom; i++)
        {
            CopyMemory( m_pclibImage[2]+k,m_pclibImage[nport]+54+4*256+i*m_ClibW[nport]+left ,w);

            k+=w;
        }

    m_CLBTImgPreview[2]->SetPicData(m_pclibImage[2],0);
    m_CLBTImgPreview[2]->DrawPic();


    m_SmallShowNum=nport;
    m_SmalShowCount=showcount;
    m_SamllShowPos.x=point->x;
    m_SamllShowPos.y=point->y;
}

void CMoreImageCtrl::SetClibPoint(int nport,int nposnum,float xbz,float ybz)
{
    if(nport>=2)
        return ;

    float fx=40.0*xbz;
    float fy=40.0*ybz;

    m_Clibpoint[nport][nposnum].x=m_SamllShowPos.x-20+fx;
    m_Clibpoint[nport][nposnum].y=m_SamllShowPos.y-20+fy;
}

BOOL CMoreImageCtrl::OpenDatFile()
{
    for(int i=0; i<2; i++)   
        {
            if(m_MapCtrl[i].IsOpen())
                m_MapCtrl[i].CloseMapFile();

            if(m_MapCtrl[i].OpenMapFile((LPCTSTR)m_DatImageFile[i]))
                {

                    m_SaveCount[i]=m_MapCtrl[i].m_pHead->nSaveCount;
                    int hour=m_MapCtrl[i].m_pHead->pStart.wHour;
                    int men=m_MapCtrl[i].m_pHead->pStart.wMinute;
                    int sedc=m_MapCtrl[i].m_pHead->pStart.wSecond;
                    int milliseconds=m_MapCtrl[i].m_pHead->pStart.wMilliseconds;
                    m_Snaptime[i]=(hour*3600+men*60+sedc)*1000.0+milliseconds;

                    m_W[i]=m_MapCtrl[i].m_pHead->nWidth;

                    m_H[i]=m_MapCtrl[i].m_pHead->nHeight;;
                    m_Depht[i]=m_MapCtrl[i].m_pHead->nPixel;
                    m_MapCtrl[i].SetCalbackInfo((void*)RevMsg,(void*)this);

                    if(m_pDatImage[i])
                        {
                            delete m_pDatImage[i];
                            m_pDatImage[i]=NULL;
                        }
                    m_pDatImage[i]=new BYTE[m_MapCtrl[i].m_pHead->nWidth*m_MapCtrl[i].m_pHead->nHeight*3];
                }
            else
                {
                    if(i==1)
                        m_MapCtrl[0].CloseMapFile();

                    return FALSE;
                }
        }

    return TRUE;
}

void CMoreImageCtrl::CloseDatFile()
{
    for(int i=0; i<2; i++)
        {
            m_MapCtrl[i].CloseMapFile();

            m_MapCtrl[i].m_pHead=NULL;

            m_W[i]=0;

            m_H[i]=0;

            if(m_pDatImage[i])
                {
                    delete m_pDatImage[i];
                    m_pDatImage[i]=NULL;
                }
        }
}

BOOL CMoreImageCtrl::RevMsg( void* pData,long cout, long num,int msgtype)
{
    CMoreImageCtrl* p=(CMoreImageCtrl*)pData;
    int type=msgtype;



    //::PostMessage(p->m_MsgHWND,p->m_ProcMsg,cout,msgtype);


    return 0;
}

void CMoreImageCtrl::SetDatPreview(int nport, HDC pShowHdc, CRect rect)
{
    if(nport>=2)
        return ;

    int port=nport;

    int nw=0,nh=0,depht=0,formattype=0;



    nw=m_W[nport];
    nh=m_H[nport];

    formattype=1;   ///////huj  0
	formattype = m_MapCtrl[port].m_pHead->nFormat;
	DEBUG_INFO1("f   %d", formattype);
	

    depht=m_Depht[nport];//m_MapCtrl[port].m_pHead->nPixel*

    //	CImgPreview* pview=&m_ImgPreview[port];
    CImgPreview* pview=m_DatImgPreview[nport];

    pview->IsOpen=TRUE;

    pview->m_ShowTime.wYear=0;
    pview->m_ShowTime.wMonth=0;
    pview->m_ShowTime.wDay=0;
    pview->m_ShowTime.wHour=0;
    pview->m_ShowTime.wMinute=0;
    pview->m_ShowTime.wSecond=0;
    pview->m_ShowTime.wMilliseconds=0;


    pview->m_CurrtPos=1;

    int nBitCount=8;

    nBitCount=m_Depht[nport];

    int nShowBit=8;

    if(formattype==1)
        {
            nShowBit=24;
            nBitCount=24;
        }

    int m_Bild=1;
    if(nBitCount<=8)
        m_Bild=1;
    else if(nBitCount<=16)
        m_Bild=2;
    else if(nBitCount==24)
        m_Bild=3;
    else if(nBitCount==32)
        m_Bild=4;
    else nBitCount=8;
//	printf("Bild:%d\n",m_Bild);

    int nAlign=0;
    int nOff=0;
    if(nBitCount>8)
        {
            nAlign=0;
            nOff=nBitCount-(m_Bild-1)*8;
        }

    pview->SetFormat(nw, nh,nShowBit,formattype,0,4);
    pview->SetHDC(pShowHdc,rect, rect.Width(), rect.Height());//显示新修改部分

    pview->UptImg(nw,nh,nShowBit);

    pview->OpenImgHandle();

    pview->Reset();

    pview->UpdateDrawVariable(FALSE);

    int rectW=rect.Width();


    if( ( rectW<= nw)||(nport==2) )
        {
            pview->m_DestLeftTop.x = 0;
            pview->m_SrcLeftTop.x = 0;
            pview->m_DestSize.cx = rectW;//W1;
            pview->m_SrcSize.cx = nw ;//rect.Width();//;W1;
        }
    else
        {
            pview->m_DestSize.cx = pview->m_SrcSize.cx =nw;
            pview->m_DestLeftTop.x = (rectW - nw)/2;
            pview->m_SrcLeftTop.x = 0;
        }


    int rectH=rect.Height();
    if(( rectH <= nh )||(nport==2))
        {
            pview->m_DestLeftTop.y = 0;
            pview->m_SrcLeftTop.y = 0;
            pview->m_DestSize.cy = rectH;//H1;
            pview->m_SrcSize.cy = nh;//rect.Height();//H1;

        }
    else
        {
            pview->m_DestSize.cy = pview->m_SrcSize.cy = nh;
            pview->m_DestLeftTop.y = (rectH -nh)/2;
            pview->m_SrcLeftTop.y = 0;
        }

    pview->UpDate();

    pview->UpdateDrawVariable(TRUE);

}

void CMoreImageCtrl::ShowDatImage(int nport,long imagepos)
{

    if(nport>=2)
        return ;


    if(m_MapCtrl[nport].IsOpen())
        {
            if((imagepos<0)||(imagepos>=m_MapCtrl[nport].m_pHead->nSaveCount))
                return ;

            int len=0;
            memset(m_pDatImage[nport],0,m_W[nport]*m_H[nport]*3);
            m_pShowData[nport]=m_MapCtrl[nport].GetMapData(imagepos,1,len);


			/////////////////huangjin
// 			int w=m_W[nport], h=m_H[nport];			 
// 			CodedTarget ct;	
// 			ct.Initial(w,h);
// 			IplImage* img =	cvCreateImageHeader(cvSize(w,h), 8, 1);
// 			cvSetData( img,m_pShowData[nport],w); //第3个参数是行字节数
// 			cvFlip(img);
// 			ct.Decoding_oneStep(img);
// 			ct.Draw_mark();


			//cvNamedWindow("aaa",0);		 
			//cvShowImage("aaa",img);


			/////////////////huangjin




            if(len>0)
                {

                    memcpy(m_pDatImage[nport],m_pShowData[nport],len);

                    double cutime=m_Snaptime[nport]+(imagepos*1000.0)/m_MapCtrl[nport].m_pHead->nFrame;

                    SYSTEMTIME ShowTime;
                    ShowTime.wYear=m_MapCtrl[nport].m_pHead->pStart.wYear;
                    ShowTime.wMonth=m_MapCtrl[nport].m_pHead->pStart.wMonth;
                    ShowTime.wDay=m_MapCtrl[nport].m_pHead->pStart.wDay;
                    ShowTime.wHour=cutime/(3600*1000.0);
                    ShowTime.wMinute=(cutime-ShowTime.wHour*3600*1000)/(60*1000.0);
                    ShowTime.wSecond=(cutime-ShowTime.wHour*3600*1000-ShowTime.wMinute*60*1000)/1000.0;
                    ShowTime.wMilliseconds=(cutime-ShowTime.wHour*3600*1000-ShowTime.wMinute*60*1000-ShowTime.wSecond*1000.0)/1.0;



                    CImgPreview *pview=m_DatImgPreview[nport];

                    PBYTE comdata=m_MapCtrl[nport].GetComData(imagepos,0,1);
                    CString msg=(char*)comdata;
                    pview->SetHfInfo(imagepos+1,m_MapCtrl[nport].m_pHead->nSaveCount,ShowTime,msg);

                    pview->SetPicData(m_pDatImage[nport],0);
                    pview->DrawPic();


                    m_CurPos[nport]=imagepos;

                    /*if(m_MapCtrl[nport].m_pHead->nFormat==0){


                    	long ZfT[256];

                    	memset(ZfT,0,256*sizeof(long));// 初始化的时候 要把所有的都初始化

                    	int gray=0;

                    	// 对各像素进行灰度统计

                    	for (int j = 0; j <  m_nW[nport] *  m_nH[nport]; j ++)

                    	{

                    		gray=*(m_pShowData+j);

                    		ZfT[gray]++;


                    	}

                    	// 计算灰度分布密度

                    	for(int i=0;i<256;i++)

                    		m_tongji[nport][i] = ZfT[i] /( m_nW[nport] * m_nH[nport] *1.0f);
                    }

                    m_CurrectPos[nport]=num;
                    ::PostMessage(m_MsgHWND, m_ZFTMsg,0,0);*/


                }
        }

}

long CMoreImageCtrl::GetCurImagePos(int nport,long& imagecount)
{
    imagecount=m_MapCtrl[nport].m_pHead->nSaveCount;

    return m_CurPos[nport];
}


 