================================================================================
MICROSOFT �������: CalibrationCtrl ��Ŀ����
===============================================================================

Ӧ�ó�������Ϊ����������� CalibrationCtrl Ӧ�ó��򡣴�Ӧ�ó��򲻽���ʾ Microsoft ������Ļ���ʹ�÷�����������Ϊ����дӦ�ó������㡣

���ļ���Ҫ������� CalibrationCtrl Ӧ�ó����ÿ���ļ������ݡ�

CalibrationCtrl.vcxproj
����ʹ��Ӧ�ó��������ɵ� VC++ ��Ŀ������Ŀ�ļ��� 
���������ɸ��ļ��� Visual C++ �İ汾��Ϣ���Լ��й�ʹ��Ӧ�ó�����ѡ���ƽ̨�����ú���Ŀ���ܵ���Ϣ��

CalibrationCtrl.vcxproj.filters
    ����ʹ�á�Ӧ�ó����򵼡����ɵ� VC++ ��Ŀɸѡ���ļ���
    �������й���Ŀ�ļ���ɸѡ��֮��Ĺ�����Ϣ���� IDE �У�ͨ�����ֹ��������ض��ڵ����Է�����ʽ��ʾ����������չ�����ļ������磬��.cpp���ļ��롰Դ�ļ���ɸѡ��������

CalibrationCtrl.h
����Ӧ�ó������Ҫͷ�ļ���������������Ŀ�ض���ͷ�ļ�(���� Resource.h)�������� CCalibrationCtrlApp Ӧ�ó����ࡣ

CalibrationCtrl.cpp
���ǰ���Ӧ�ó����� CCalibrationCtrlApp ����ҪӦ�ó���Դ�ļ���

CalibrationCtrl.rc
���ǳ���ʹ�õ����� Microsoft Windows ��Դ���б������� RES ��Ŀ¼�д洢��ͼ�ꡢλͼ�͹�ꡣ���ļ�����ֱ���� Microsoft Visual C++ �н��б༭����Ŀ��Դλ�� 2052 �С�

res\CalibrationCtrl.ico
��������Ӧ�ó���ͼ���ͼ���ļ�����ͼ���������Ҫ��Դ�ļ� CalibrationCtrl.rc �С�

res\CalibrationCtrl.rc2
���ļ��������� Microsoft Visual C++ �н��б༭����Դ����Ӧ�ý���������Դ�༭���༭��������Դ���ڴ��ļ��С�

/////////////////////////////////////////////////////////////////////////////

��������ܴ���:
��Ŀ������׼ MFC ���档

MainFrm.h, MainFrm.cpp
��Щ�ļ���������� CMainFrame������������
CFrameWnd ���������� SDI ��ܹ��ܡ�

/////////////////////////////////////////////////////////////////////////////

Ӧ�ó����򵼴���һ���ĵ����ͺ�һ����ͼ:

CalibrationCtrlDoc.h��CalibrationCtrlDoc.cpp - �ĵ�
��Щ�ļ����� CCalibrationCtrlDoc �ࡣ�༭��Щ�ļ�������������ĵ����ݲ���ʵ���ļ�����ͼ���(ͨ�� CCalibrationCtrlDoc::Serialize)��

CalibrationCtrlView.h��CalibrationCtrlView.cpp - �ĵ�����ͼ
��Щ�ļ����� CCalibrationCtrlView �ࡣ
CCalibrationCtrlView �������ڲ鿴 CCalibrationCtrlDoc ����





/////////////////////////////////////////////////////////////////////////////

��������:

ActiveX �ؼ�
Ӧ�ó��������ʹ�� ActiveX �ؼ���֧�֡�

/////////////////////////////////////////////////////////////////////////////

������׼�ļ�:

StdAfx.h��StdAfx.cpp
��Щ�ļ�����������Ϊ CalibrationCtrl.pch ��Ԥ����ͷ (PCH) �ļ�����Ϊ StdAfx.obj ��Ԥ���������ļ���

Resource.h
���Ǳ�׼ͷ�ļ����������µ���Դ ID��
Microsoft Visual C++ ��ȡ�����´��ļ���

CalibrationCtrl.manifest
	Ӧ�ó����嵥�ļ��� Windows XP ��������Ӧ�ó���
	���ض��汾���г��򼯵������ԡ����س���ʹ�ô�
	��Ϣ�ӳ��򼯻�������ʵ��ĳ��򼯻�
	��Ӧ�ó������˽����Ϣ��Ӧ�ó����嵥����Ϊ�����·ַ�����Ϊ
	��Ӧ�ó����ִ���ļ���װ����ͬ�ļ����е��ⲿ .manifest �ļ�������
	Ҳ��������Դ����ʽ�����ڸÿ�ִ���ļ��С� 
/////////////////////////////////////////////////////////////////////////////

����ע��:

Ӧ�ó�����ʹ�á�TODO:��ָʾӦ��ӻ��Զ����Դ���벿�֡�

���Ӧ�ó����ڹ���� DLL ��ʹ�� MFC������Ҫ���·�����Щ MFC DLL�����Ӧ�ó������õ����������ϵͳ�ĵ�ǰ�������ò�ͬ������Ҫ���·�����Ӧ�ı��ػ���Դ MFC100XXX.DLL���й�����������ĸ�����Ϣ����μ� MSDN �ĵ����й� Redistributing Visual C++ applications (���·��� Visual C++ Ӧ�ó���)���½ڡ�

/////////////////////////////////////////////////////////////////////////////

//��Ŀ��
void CodedTarget::Draw_mark()
{
	if (!m_init)
		return;	 

	cvCvtColor(m_img_orig,m_imgc,CV_GRAY2RGB );	///////

	vector <CvBox2D>::iterator it;
	it = m_boxv.begin( );

	CvBox2D box;
	CvPoint center;
	CvSize size;

	CvFont font;

	//	int pn=m_w*m_h;
	if ( m_dia < 48 )
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,1.9, 1.7, 0, 4);
	else
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, m_dia/10, m_dia/36, 0, m_dia/8);


	CString cs;
	int indx;

	for (int k=0; k< m_targetNum; k++)
	{
		indx=m_target[k].re;

		box=*(it+indx);

		center.x = cvRound(box.center.x);
		center.y = cvRound(box.center.y);
		size.width = cvRound(box.size.width*0.7);
		size.height = cvRound(box.size.height*0.7);
		//box.angle = -box.angle;

		//����Բ
		cvEllipse(m_imgc,center,size,box.angle,0,360,CV_RGB(20,238,5),2,CV_AA,0);

		/////
		//cs.Format("%d ", m_target[k].sn);

		//cvPutText(m_imgc, cs, center, &font, CV_RGB(0, 255, 0));


		cs.Format("%d", m_target[k].code );
		//center.x = center.x+m_rad;

		cvPutText(m_imgc, cs, center, &font, CV_RGB(255, 75,4 ));

		/////////������ʮ��

		center.x=m_target[k].x+0.5;
		center.y=m_target[k].y+0.5;

		//		cvCircle(m_imgc, center,2, CV_RGB(255, 0, 0),2,8, 0);

		CvPoint  cp1, cp2, cp3, cp4;

		int s=size.width*0.4;

		cp1.x=center.x-s;
		cp1.y=center.y;

		cp2.x=center.x+s;
		cp2.y=center.y;

		cp3.x=center.x;
		cp3.y=center.y-s;

		cp4.x=center.x;
		cp4.y=center.y+s;


		//		cvCircle(img, cpc, s, CV_RGB(c, 0 , 0));  

		cvLine( m_imgc, cp1, cp2, CV_RGB(0,195,188), 2,8-8 , 0 );	
		cvLine( m_imgc, cp3, cp4, CV_RGB(0,195,188), 2,8-8 , 0 );

	}

	//m_imgc->origin=1;
	cvFlip(m_imgc); 

	//cvNamedWindow("aaa",0);cvShowImage("aaa",m_imgc);

	//cvSaveImage("qqqqq.bmp",m_imgc);

}