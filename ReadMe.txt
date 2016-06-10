================================================================================
MICROSOFT 基础类库: CalibrationCtrl 项目概述
===============================================================================

应用程序向导已为您创建了这个 CalibrationCtrl 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 CalibrationCtrl 应用程序的每个文件的内容。

CalibrationCtrl.vcxproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

CalibrationCtrl.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。
    它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

CalibrationCtrl.h
这是应用程序的主要头文件。它包括其他项目特定的头文件(包括 Resource.h)，并声明 CCalibrationCtrlApp 应用程序类。

CalibrationCtrl.cpp
这是包含应用程序类 CCalibrationCtrlApp 的主要应用程序源文件。

CalibrationCtrl.rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 2052 中。

res\CalibrationCtrl.ico
这是用作应用程序图标的图标文件。此图标包括在主要资源文件 CalibrationCtrl.rc 中。

res\CalibrationCtrl.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。

/////////////////////////////////////////////////////////////////////////////

对于主框架窗口:
项目包含标准 MFC 界面。

MainFrm.h, MainFrm.cpp
这些文件包含框架类 CMainFrame，该类派生自
CFrameWnd 并控制所有 SDI 框架功能。

/////////////////////////////////////////////////////////////////////////////

应用程序向导创建一个文档类型和一个视图:

CalibrationCtrlDoc.h，CalibrationCtrlDoc.cpp - 文档
这些文件包含 CCalibrationCtrlDoc 类。编辑这些文件可以添加特殊文档数据并可实现文件保存和加载(通过 CCalibrationCtrlDoc::Serialize)。

CalibrationCtrlView.h，CalibrationCtrlView.cpp - 文档的视图
这些文件包含 CCalibrationCtrlView 类。
CCalibrationCtrlView 对象用于查看 CCalibrationCtrlDoc 对象。





/////////////////////////////////////////////////////////////////////////////

其他功能:

ActiveX 控件
应用程序包括对使用 ActiveX 控件的支持。

/////////////////////////////////////////////////////////////////////////////

其他标准文件:

StdAfx.h，StdAfx.cpp
这些文件用于生成名为 CalibrationCtrl.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

Resource.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。

CalibrationCtrl.manifest
	应用程序清单文件供 Windows XP 用来描述应用程序
	对特定版本并行程序集的依赖性。加载程序使用此
	信息从程序集缓存加载适当的程序集或
	从应用程序加载私有信息。应用程序清单可能为了重新分发而作为
	与应用程序可执行文件安装在相同文件夹中的外部 .manifest 文件包括，
	也可能以资源的形式包括在该可执行文件中。 
/////////////////////////////////////////////////////////////////////////////

其他注释:

应用程序向导使用“TODO:”指示应添加或自定义的源代码部分。

如果应用程序在共享的 DLL 中使用 MFC，则需要重新发布这些 MFC DLL；如果应用程序所用的语言与操作系统的当前区域设置不同，则还需要重新发布对应的本地化资源 MFC100XXX.DLL。有关这两个主题的更多信息，请参见 MSDN 文档中有关 Redistributing Visual C++ applications (重新发布 Visual C++ 应用程序)的章节。

/////////////////////////////////////////////////////////////////////////////

//画目标
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

		//画椭圆
		cvEllipse(m_imgc,center,size,box.angle,0,360,CV_RGB(20,238,5),2,CV_AA,0);

		/////
		//cs.Format("%d ", m_target[k].sn);

		//cvPutText(m_imgc, cs, center, &font, CV_RGB(0, 255, 0));


		cs.Format("%d", m_target[k].code );
		//center.x = center.x+m_rad;

		cvPutText(m_imgc, cs, center, &font, CV_RGB(255, 75,4 ));

		/////////画中心十字

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