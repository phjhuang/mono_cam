// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CalibrationCtrlView.cpp : CCalibrationCtrlView 类的实现
//

#include "StdAfx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CalibrationCtrl.h"
#endif

#include "CalibrationCtrlDoc.h"
#include "CalibrationCtrlView.h"
#include "MainFrm.h"
#include "RegCheck.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CCalibrationCtrlView

IMPLEMENT_DYNCREATE(CCalibrationCtrlView, CFormView)

BEGIN_MESSAGE_MAP(CCalibrationCtrlView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_TAB_MSG, &CCalibrationCtrlView::OnTabMsg)
	ON_MESSAGE(WM_CTRLKEYDOWN_MSG, &CCalibrationCtrlView::OnKeyDownMsg)
	ON_BN_CLICKED(IDC_GETONECAMPATH, &CCalibrationCtrlView::OnBnClickedGetonecampath)
	ON_BN_CLICKED(IDC_GETTWOCAMPATH, &CCalibrationCtrlView::OnBnClickedGettwocampath)
	ON_BN_CLICKED(IDC_GETSDDATAPATH, &CCalibrationCtrlView::OnBnClickedGetsddatapath)

	ON_MESSAGE(WM_IMAGEPOINT_MSG, &CCalibrationCtrlView::OnImagePointMsg)



	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT3, &CCalibrationCtrlView::OnEditCalibNum)
	ON_COMMAND(ID_EDIT5, &CCalibrationCtrlView::OnEdit_beginpos)
	ON_COMMAND(ID_EDIT6, &CCalibrationCtrlView::OnEdit_endpos)
	ON_COMMAND(ID_EDIT7, &CCalibrationCtrlView::OnEdit_offset)
	ON_COMMAND(ID_DEFAULTSETTING, &CCalibrationCtrlView::OnDefaultSetting)
	ON_UPDATE_COMMAND_UI(ID_DEFAULTSETTING, &CCalibrationCtrlView::OnUpdateDefaultsetting)
	ON_UPDATE_COMMAND_UI(ID_SAVECLBT, &CCalibrationCtrlView::OnUpdateSaveclbt)
	ON_UPDATE_COMMAND_UI(ID_EDIT5, &CCalibrationCtrlView::OnUpdateEdit5)
	ON_UPDATE_COMMAND_UI(ID_EDIT6, &CCalibrationCtrlView::OnUpdateEdit6)
	ON_UPDATE_COMMAND_UI(ID_EDIT7, &CCalibrationCtrlView::OnUpdateEdit7)

	//ON_COMMAND(ID_EDIT2, &CCalibrationCtrlView::OnEdit2)
	//ON_UPDATE_COMMAND_UI(ID_EDIT2, &CCalibrationCtrlView::OnUpdateEdit2)
	ON_COMMAND(ID_COMBO_PT, &CCalibrationCtrlView::OnComboPt)
	ON_COMMAND(ID_COMBO_DISPLAYWHAT, &CCalibrationCtrlView::OnComboDisplaywhat)
	ON_COMMAND(ID_EDIT_WINDOW, &CCalibrationCtrlView::OnEditWindow)
	//ON_COMMAND(ID_BUTTON4, &CCalibrationCtrlView::OnButton4)

	ON_COMMAND(ID_EDIT2, &CCalibrationCtrlView::OnEditDia)

END_MESSAGE_MAP()

// CCalibrationCtrlView 构造/析构

CCalibrationCtrlView::CCalibrationCtrlView()
	: CFormView(CCalibrationCtrlView::IDD)
	, m_CalibrationPointNum(1)
{
	// TODO: 在此处添加构造代码
	m_OldTab = 1;

	m_UseDefaultSetting = 1;
	m_displaymark = false;///huangjin
	m_SaveIsOK = 0;

	m_ChartCtrl = 0;
	m_MarkOfViewForm[0] = m_MarkOfViewForm[1] = false;
	m_GraphFromIsShow = false;

	p_ShowPicZoomDlg = 0;

	m_tab = 1;
	///////////////////
	m_Font.CreateFont(20, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, 0,
		ANSI_CHARSET, 0, 0, 0,
		0, "Arial");
	//AfxMessageBox("单相机视觉测量软件演示版！");  //////mono

	 
	//


}

CCalibrationCtrlView::~CCalibrationCtrlView()
{
	if (p_ShowPicZoomDlg != NULL)  ///// &&   IsWindow(p_ShowPicZoomDlg->m_hWnd )
	{
		//p_ShowPicZoomDlg->SendMessage(WM_CLOSE);
		delete p_ShowPicZoomDlg;
	}


}

void CCalibrationCtrlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT5, m_Edit5);

	// DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);//huangjin

}






void CCalibrationCtrlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	pmain->SetMsgHwnd(this->GetSafeHwnd(), WM_TAB_MSG, WM_CTRLKEYDOWN_MSG);



	////////////// 710 H_path
	GetH_Path();

	/////mono
	m_Edit5.SetWindowText(" 福建桔子科技有限公司 ");
 
}

void CCalibrationCtrlView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCalibrationCtrlView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCalibrationCtrlView 诊断

#ifdef _DEBUG
void CCalibrationCtrlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCalibrationCtrlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCalibrationCtrlDoc* CCalibrationCtrlView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalibrationCtrlDoc)));
	return (CCalibrationCtrlDoc*)m_pDocument;
}
#endif //_DEBUG


// CCalibrationCtrlView 消息处理程序
LRESULT CCalibrationCtrlView::OnTabMsg(WPARAM wParam, LPARAM lParam)
{

	//	DEBUG_INFO1("tab:%d\n",tab);DEBUG_INFO1("tab %d "  ,ntab  );
	BOOL UpdateView = TRUE;

	///////////////huangjin
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonCategory *pCategory = p2->m_wndRibbonBar.GetActiveCategory();
	//int nCategory=p2->m_wndRibbonBar.GetCategoryIndex(pCategory);
	CString str = pCategory->GetName();
	//DEBUG_INFO1("//////////////////////",9);
	DEBUG_INFO1("%s", str);


	int tab;
	if (str == "系统标定")
	{
		tab = 1;
	}
	else if (str == "图像数据显示与计算")
	{
		tab = 2;
	}
	else if (str == "数据结果绘图")
	{
		tab = 3;
	}
	else
	{
		return 0;
	}

	if (tab == 1)
	{
		if (m_OldTab == 1)
		{
			UpdateView = FALSE;
		}

	}
	else if (tab == 2)
	{
		if (m_OldTab == 2)
		{
			UpdateView = FALSE;
		}
	}
	else if (tab == 3)
	{
		if (m_OldTab == 3)
		{
			UpdateView = FALSE;
		}
	}

	m_OldTab = tab;//记录旧的tab

	TabView(m_OldTab, UpdateView);

	return 0;
}

void CCalibrationCtrlView::TabView(int ntab, BOOL Change)
{
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	m_tab = ntab;////
 
	if (p_ShowPicZoomDlg != NULL  &&  ::IsWindow(p_ShowPicZoomDlg->m_hWnd))
	{
		p_ShowPicZoomDlg->ShowWindow(SW_HIDE);
	}

	switch (ntab)
	{
	case 1:

		m_Edit5.SetWindowText("");

		pmain->SetFramSzie(327);
		//pmain->SetFramSzie(0); //mono

		if (m_GraphFromIsShow)
		{
			p_MyGraphView->GetParent()->ShowWindow(SW_HIDE);
		}

		if (Change)
		{
			//GetDlgItem(IDC_FILELIST)->ShowWindow(FALSE);
			SetDlgItemText(IDC_TEXT1, "相机1标定图像(主机Master):");
			SetDlgItemText(IDC_TEXT2, "相机2标定图像(从机Slave):");
			SetDlgItemText(IDC_TEXT3, "立体标定三维数据:");
			SetDlgItemText(IDC_TEXT4, "结果显示：");
			CString onefile = "", twofile = "", sdfile = "";

			onefile = m_MoreImageCtrl.m_ClibImageFile[0];
			twofile = m_MoreImageCtrl.m_ClibImageFile[1];
			sdfile = m_MoreImageCtrl.m_SDFile;
			SetDlgItemText(IDC_ONECAMIMAGE, onefile);
			SetDlgItemText(IDC_TWOCAMIMAGE, twofile);
			SetDlgItemText(IDC_SDDATA, sdfile);


			if (pmain->m_ClibIsOpen)
			{
				/////huj fff

				//OnOpenimage();
				pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_HIDE);
				pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);


				//                             for(int i=0; i<2; i++)
				//                                 {
				//                                     pmain->m_ShowView[i]->GetParent()->ShowWindow(SW_SHOWNORMAL);//
				//                                     CImgViewForm *pview=pmain->m_ShowView[i];
				//                                     m_MoreImageCtrl.m_CLBTImgPreview[i]=&(pview->m_ImgPreview);
				//                                     CRect nrect;//
				//                                     pmain->m_ShowView[i]->m_nCampos=i;//
				//                                     HDC pDC=pview->GetShowWndInfo(nrect);//
				//                                     pview->SetPointHwnd(this->GetSafeHwnd(),WM_IMAGEPOINT_MSG);
				//                                     //设置HDC
				//                                     m_MoreImageCtrl.SetClibPreview(i,pDC,nrect);
				//                                 }
				//                             m_MoreImageCtrl.ShowClibImage(m_displaymark,m_Ct[0],m_Ct[1]);

			}
			else if (pmain->m_MsmtIsOpen)
			{
				OnClosedatfile();

				//pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_HIDE);
				//pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);
			}

		}
		break;

	case 2: 
		//m_Edit5.SetWindowText("");

		pmain->SetFramSzie(327);
		//pmain->SetFramSzie(0); //mono/////

		if (m_GraphFromIsShow)
		{
			p_MyGraphView->GetParent()->ShowWindow(SW_HIDE);
		}

		if (Change)
		{
			//GetDlgItem(IDC_FILELIST)->ShowWindow(FALSE);
			SetDlgItemText(IDC_TEXT1, "相机1图像数据(主机Master):");
			SetDlgItemText(IDC_TEXT2, "相机2图像数据(从机Slave):");
			SetDlgItemText(IDC_TEXT3, "立体标定结果文件:");
			SetDlgItemText(IDC_TEXT4, "结果显示:");

			CString onefile = "", twofile = "", sdfile = "";

			onefile = m_MoreImageCtrl.m_DatImageFile[0];
			twofile = m_MoreImageCtrl.m_DatImageFile[1];
			sdfile = m_MoreImageCtrl.m_BDFile;

			SetDlgItemText(IDC_ONECAMIMAGE, onefile);
			SetDlgItemText(IDC_TWOCAMIMAGE, twofile);
			SetDlgItemText(IDC_SDDATA, sdfile); 

			if (pmain->m_MsmtIsOpen)
			{ 
				pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_MAXIMIZE);
				pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);
				 
				for (int i = 0; i < 2; i++)
				{
					pmain->m_ShowView[i]->GetParent()->SetWindowTextA(m_MoreImageCtrl.m_DatImageFile[i]);

					//pmain->m_ShowView[i]->GetParent()->ShowWindow(SW_SHOWNORMAL);

					CImgViewForm *pview = pmain->m_ShowView[i];
					m_MoreImageCtrl.m_DatImgPreview[i] = &(pview->m_ImgPreview);
					CRect nrect;

					pmain->m_ShowView[i]->m_nCampos = i;

					HDC pDC = pview->GetShowWndInfo(nrect);

					pview->SetPointHwnd(this->GetSafeHwnd(), WM_IMAGEPOINT_MSG);
					//设置HDC
					m_MoreImageCtrl.SetDatPreview(i, pDC, nrect);

					long cur = 0, ncount = 0;
					cur = m_MoreImageCtrl.GetCurImagePos(i, ncount);
					m_MoreImageCtrl.ShowDatImage(i, cur);
				}
			}
			else if (pmain->m_ClibIsOpen)
			{
				OnCloseclib();
				//pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_HIDE);
				//pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);
			}
		}

		break;

	case 3:
		pmain->SetFramSzie(0);

		if (!m_GraphFromIsShow)
		{
			p_MyGraphView = pmain->MyGraphForm();
			m_GraphFromIsShow = true;

		}

		p_MyGraphView->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);

		if (pmain->m_MsmtIsOpen)
		{
			pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_HIDE);
			pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);
		}
		break;
	}
}

void CCalibrationCtrlView::OnBnClickedGetonecampath()
{
	CString strFile = "";

	//mono
	m_OldTab = 2;

	if (m_OldTab == 1)
	{
		CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("BMP格式图像 (*.bmp)|*.bmp||"), NULL);
		dlgFile.m_ofn.lpstrInitialDir = (LPCSTR)m_hpath.Cam1Path;//初始化路径。

		//dlgFile.m_ofn.lpstrInitialDir = AfxGetApp()->GetProfileString("Windows", "Lasted Task Dir");// 最近任务文件目录

		DEBUG_INFO1("cam1path   %s", dlgFile.m_ofn.lpstrInitialDir);

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();
			//  m_CamFileName[0]=strFile;  //h
		}

		if (strFile != "")
			SetDlgItemText(IDC_ONECAMIMAGE, strFile);
	}
	else if (m_OldTab == 2)
	{
		CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("图像数据序列文件|*.dat|图像数据序列文件|*F0.dat||"), NULL);
		dlgFile.m_ofn.lpstrInitialDir = (LPCSTR)m_hpath.Dat1Path;//初始化路径。

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();

		}

		if (strFile != "")
			SetDlgItemText(IDC_ONECAMIMAGE, strFile);
	}
	else if (m_OldTab == 3)
	{

	}




	SetDlgItemText(IDC_TWOCAMIMAGE, strFile);/////mono 

}


void CCalibrationCtrlView::OnBnClickedGettwocampath()
{
	CString strFile = "";

	if (m_OldTab == 1)
	{
		CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("BMP格式图像 (*.bmp)|*.bmp||"), NULL);
		dlgFile.m_ofn.lpstrInitialDir = (LPCSTR)m_hpath.Cam2Path;//初始化路径。

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();
			//m_CamFileName[1]=strFile;  //h
		}

		if (strFile != "")
			SetDlgItemText(IDC_TWOCAMIMAGE, strFile);
	}
	else if (m_OldTab == 2)
	{
		CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("图像数据序列文件|*.dat|图像数据序列文件|*F0.dat||"), NULL);
		dlgFile.m_ofn.lpstrInitialDir = (LPCSTR)m_hpath.Dat2Path;//初始化路径。

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();

		}

		if (strFile != "")
			SetDlgItemText(IDC_TWOCAMIMAGE, strFile);
	}
	else if (m_OldTab == 3)
	{

	}
}


void CCalibrationCtrlView::OnBnClickedGetsddatapath()
{
	CString strFile = "";

	if (m_OldTab == 1)
	{
		CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("坐标数据文本(*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
		dlgFile.m_ofn.lpstrInitialDir = m_hpath._XYZ;//初始化路径。

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();
			m_XYZFileName = strFile;//h
		}

		if (strFile != "")
			SetDlgItemText(IDC_SDDATA, strFile);
	}
	else if (m_OldTab == 2)
	{
		CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("标定结果文件 (*.CalibResult)|*.CalibResult|All Files (*.*)|*.*||"), NULL);

		dlgFile.m_ofn.lpstrInitialDir = m_hpath.CalibResultPath;//初始化路径。

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();
			//m_XYZFileName = strFile;        //

		}

		if (strFile != "")
			SetDlgItemText(IDC_SDDATA, strFile);
	}
	else if (m_OldTab == 3)
	{

	}
}

LRESULT CCalibrationCtrlView::OnKeyDownMsg(WPARAM wParam, LPARAM lParam)
{

	//  DEBUG_INFO1("OK%d",1);

	int type = (int)wParam;
	switch (type)
	{
	case 101:
		OnOpenimage();
		break;
	case 102:
		OnCloseclib();
		break;

	case 103:
		OnCalibration();
		break;  //h
	case 104:
		OnSaveCalibrationResult();
		break;   //hUANGJIN

	case 105:
		OnXYZTransform();
		break;   //hUANGJIN

	case 201:
		OnOpendatfile();
		break;
	case 202:
		OnClosedatfile();
		break;
	case 213:
		SetPos1();
		break;
	case 313:
		SetPos2();
		break;
	case 214:
		SetSliderPos1();
		break;
	case 314:
		SetSliderPos2();
		break;
	case 211:
		OnFristframe1();
		break;
	case 212:
		OnUp1();
		break;
	case 215:
		OnDown1();
		break;
	case 216:
		OnEndfram1();
		break;
	case 311:
		OnFristframe2();
		break;
	case 312:
		OnUp2();
		break;
	case 315:
		OnDown2();
		break;
	case 316:
		OnEndfram2();
		break;
	case 231:
		OnStartMeasure();
		break;

	case 400:
		OnOpenXYZData();
		break;
	case 401:
		OnDataProcessing();
		break;
	case 402:
		OnSaveGraph();
		break;

	case 239:
		OnShowMark();
		break;

	}

	return 1;
}


void CCalibrationCtrlView::OnOpenimage()
{

	UpdateData(TRUE);

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	CString onefile = "", twofile = "", sdfile = "";
	GetDlgItemText(IDC_ONECAMIMAGE, onefile);
	GetDlgItemText(IDC_TWOCAMIMAGE, twofile);
	GetDlgItemText(IDC_SDDATA, sdfile);

	pmain->m_Title[0] = onefile;
	pmain->m_Title[1] = twofile;

	m_MoreImageCtrl.m_ClibImageFile[0] = onefile;
	m_MoreImageCtrl.m_ClibImageFile[1] = twofile;
	m_MoreImageCtrl.m_SDFile = sdfile;

	/////////////////
	//     IplImage *img=cvLoadImage(onefile,-1);
	//     if (img->depth!=8 ||  img->nChannels!=1  )
	//         {
	//             AfxMessageBox("标定图像必须为8位灰度图像！");
	//             return;
	//         }
	//     cvReleaseImage(&img);
	//     img=cvLoadImage(twofile,-1);
	//     if (img->depth!=8  ||  img->nChannels!=1  )
	//         {
	//             AfxMessageBox("标定图像必须为8位灰度图像！");
	//             return;
	//         }
	//     cvReleaseImage(&img);
	//     /////////////////////////



	if (!m_MoreImageCtrl.GetClibFileData())
	{
		AfxMessageBox("控制点三维坐标数据文件打开失败!");
		return;
	}

	if (m_MoreImageCtrl.m_ClibDept[0] != 8 || m_MoreImageCtrl.m_ClibDept[1] != 8)
	{
		// AfxMessageBox("标定图像必须为8位灰度图像！");
		// return;
	}


	pmain->OnTwowindows();



	for (int i = 0; i < 1; i++)
	{
		CImgViewForm *pview = pmain->m_ShowView[i];
		m_MoreImageCtrl.m_CLBTImgPreview[i] = &(pview->m_ImgPreview);
		CRect nrect;

		pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_MAXIMIZE);
		pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);


		pmain->m_ShowView[i]->m_nCampos = i;

		HDC pDC = pview->GetShowWndInfo(nrect);

		pview->SetPointHwnd(this->GetSafeHwnd(), WM_IMAGEPOINT_MSG);
		//设置HDC
		m_MoreImageCtrl.SetClibPreview(i, pDC, nrect);
	}

	/*CWnd *pwnd;
	pwnd=GetDlgItem(IDC_SMALLPIC);
	CRect rect;
	pwnd->GetWindowRect(&rect);
	m_MoreImageCtrl.m_CLBTImgPreview[2]=&m_SmallPreview;
	m_MoreImageCtrl.SetClibPreview(2,pwnd->GetDC()->GetSafeHdc(),rect);*/

	m_MoreImageCtrl.ShowClibImage(m_displaymark, m_Ct[0], m_Ct[1]);

	pmain->m_ClibIsOpen = TRUE;
}

void CCalibrationCtrlView::OnCloseclib()
{
	/////////huangjin65
	if (p_ShowPicZoomDlg != NULL  &&  ::IsWindow(p_ShowPicZoomDlg->m_hWnd))
	{
		p_ShowPicZoomDlg->ShowWindow(SW_HIDE);
	}
	
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	//	CWnd* pwnd=pmain->m_wndSplitter1.GetPane(0,1);
	//pmain->m_IsOpenFileButon=m_IsOpenFileButon;
	//pmain->m_IsCloseFileButon=m_IsCloseFileButon;
	//	CImageView* pview=(CImageView*)pwnd;
	//	pview->ClearShowInfo();
	/*m_CheckAll=FALSE;
	m_UpdateSelectNum=0;
	UpdateMainView();*/
	//	CHighSpeedGrabApp* theApp = (CHighSpeedGrabApp*)AfxGetApp();
	//	CMainFrame* pmain=(CMainFrame*)theApp->m_pMainWnd;

	for (int i = 0; i < DEVICEMAX; i++)
	{
		CImgViewForm *pview = pmain->m_ShowView[i];
		if (pview)
		{
			pview->UpdateShowInfo();
			pview->CloseCtrl();
			pmain->m_ShowView[i] = NULL;
		}

		pmain->m_Title[i] = "";
	}

	pmain->m_ClibIsOpen = FALSE;

	pmain->m_MsmtIsOpen = FALSE;//huangjin open error
}

void CCalibrationCtrlView::OnOpendatfile()
{
	UpdateData(TRUE);

	//mono

	CString strFile = "";

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("图像数据序列文件|*.dat|图像数据序列文件|*F0.dat||"), NULL);
	dlgFile.m_ofn.lpstrInitialDir = AfxGetApp()->GetProfileString("Windows", "Lasted Task Dir");// 最近任务文件目录

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	else
		return;
	 
 
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	CString onefile = "", twofile = "", sdfile = "";
	//GetDlgItemText(IDC_ONECAMIMAGE, onefile);
	//GetDlgItemText(IDC_TWOCAMIMAGE, twofile);
	//GetDlgItemText(IDC_SDDATA, sdfile);

	onefile = strFile;
	twofile = strFile;
	////////////mono
	pmain->m_Title[0] = onefile;
	pmain->m_Title[1] = twofile;
	if (onefile == twofile)
	{
		//AfxMessageBox("两相机图像数据来源是同一文件,退出测量！");
		//return;
	}

	m_MoreImageCtrl.m_DatImageFile[0] = onefile;
	m_MoreImageCtrl.m_DatImageFile[1] = twofile;
	m_MoreImageCtrl.m_BDFile = sdfile;

	m_MoreImageCtrl.m_DatImageFile[1] = onefile;
	if (onefile.IsEmpty())
	{
		return;
	}


	if (!m_MoreImageCtrl.OpenDatFile())
	{
		AfxMessageBox("图像数据文件打开失败!");
		return;
	}

	///////////////
	bool flag1 = (m_MoreImageCtrl.m_W[0] != 4080000)   ;
	bool flag2 = (m_MoreImageCtrl.m_W[0] > 1289) || (m_MoreImageCtrl.m_H[0] > 1029);
	
    CvRNG rng= cvRNG(cvGetTickCount());
	int big =   cvRandReal(&rng) * 6 ;
	 
	for (size_t ii = 0; ii < 1900; ii++)
	{
		if (flag1 && flag2)
		{
			hujfff1[ii] = 0.5*m_MoreImageCtrl.m_W[0] + 100000 *big ;
		}
		else
			hujfff1[ii] =  m_MoreImageCtrl.m_W[0] + 100000 * big;		
	}

	int tmp2 = hujfff1[307 + cvRandInt(&rng) % 527];

	m_MoreImageCtrl.m_W[0] = tmp2 - big * 100000;
	////////////////////////////



	pmain->OnTwowindows();

	pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);
	pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_MAXIMIZE); //mono

	for (int i = 0; i < 1; i++)  //mono
	{
		CImgViewForm *pview = pmain->m_ShowView[i];
		m_MoreImageCtrl.m_DatImgPreview[i] = &(pview->m_ImgPreview);
		CRect nrect;
		pmain->m_ShowView[i]->m_nCampos = i;
		HDC pDC = pview->GetShowWndInfo(nrect);
		pview->SetPointHwnd(this->GetSafeHwnd(), WM_IMAGEPOINT_MSG);
		//设置HDC
		m_MoreImageCtrl.SetDatPreview(i, pDC, nrect);
		m_MoreImageCtrl.ShowDatImage(i, 0);



		//pmain->m_ShowView[0]->GetParent()->ShowWindow(SW_MAXIMIZE);
		//pmain->m_ShowView[1]->GetParent()->ShowWindow(SW_HIDE);
	}

	long ncount = 0;
	m_MoreImageCtrl.GetCurImagePos(0, ncount);
	pmain->m_SliderCtrlPos1->SetRange(0, ncount - 1);
	pmain->m_SliderCtrlPos1->SetPos(0);

	//	ncount = 0;
	//m_MoreImageCtrl.GetCurImagePos(1, ncount);
	//	pmain->m_SliderCtrlPos2->SetRange(0, ncount - 1);
	//pmain->m_SliderCtrlPos2->SetPos(0);
	pmain->m_MsmtIsOpen = TRUE;
	ShowPos1();
	//ShowPos2();



	CString cstr1;
	cstr1.Format("\r\n\r\n帧率：%d", m_MoreImageCtrl.m_MapCtrl[0].m_pHead->nFrame);
	m_Edit5.SetFont(&m_Font, TRUE);
	m_Edit5.SetWindowText("文件："+strFile +cstr1);



	h_end = ncount - 1; //last
	m_endpos = h_end;
	
	m_beginpos = 0;m_UseDefaultSetting = 1;
	UpdateData(FALSE);
	UpdateData(1);


}

void CCalibrationCtrlView::OnClosedatfile()
{
	m_Edit5.SetWindowText("");

	/////////huangjin65
	if (p_ShowPicZoomDlg != NULL  &&  ::IsWindow(p_ShowPicZoomDlg->m_hWnd))
	{
		p_ShowPicZoomDlg->ShowWindow(SW_HIDE);
	}

	m_MoreImageCtrl.CloseDatFile();

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	for (int i = 0; i < DEVICEMAX; i++)
	{
		CImgViewForm *pview = pmain->m_ShowView[i];
		if (pview)
		{
			pview->GetParent()->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);   //huj fff

			pview->UpdateShowInfo();
			pview->CloseCtrl();
			pmain->m_ShowView[i] = NULL;
		}

		pmain->m_Title[i] = "";
	}

	pmain->m_MsmtIsOpen = FALSE;

	pmain->m_ClibIsOpen = 0;  //huangjin open error
	m_MarkOfViewForm[0] = m_MarkOfViewForm[1] = false;

	pmain->SetFramSzie(0);
	pmain->SetFramSzie(327);

}

LRESULT CCalibrationCtrlView::OnImagePointMsg(WPARAM wParam, LPARAM lParam)
{
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonCategory *pCategory = p2->m_wndRibbonBar.GetActiveCategory();
	//int nCategory=p2->m_wndRibbonBar.GetCategoryIndex(pCategory);
	CString str = pCategory->GetName();

	bool flag = !m_MarkOfViewForm[0] || !m_MarkOfViewForm[1];
	flag = 0;
	if (str == "数据测量"   &&flag)////
		return FALSE;
	//////////////

	//////////////////huagjin64
	//if (p_ShowPicZoomDlg == NULL)
	//{
	//	p_ShowPicZoomDlg = new ShowPicZoom;
	//	p_ShowPicZoomDlg->Create(IDD_DIALOG1);
	//}
	////////////////
	long index = wParam;
	int y = lParam;
	CPoint   point;
	GetCursorPos(&point);
	int w_window = 288, h_window = 288, offsetx = 50, offsety = -50;
	int campos = index / 10000;
	int x = index % 10000;
	/////////////////

	CRect  viewrect;
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;
	CImgViewForm *pview = pmain->m_ShowView[campos];
	pview->GetClientRect(&viewrect);

	if (viewrect.bottom == 0 || viewrect.right == 0)
	{
		return 0;
	}

	int imagewidth = pview->m_ImgPreview.m_nWidth;
	int imageheight = pview->m_ImgPreview.m_nHeight;

	if (imageheight< h_window*1.5 || imagewidth < w_window*1.5)
	{
		return 0;   /////
	}

	int imagex, imagey;
	imagey = imageheight - imageheight *y / viewrect.bottom;    //////huangjin  bottom to up
	imagex = imagewidth * x / viewrect.right;
	DEBUG_INFO2("imagex_y:  %d   %d", imagex, imagey);


	int begin_y = imagey - h_window / 2;
	int end_y = imagey + h_window / 2;
	int begin_x = imagex - w_window / 2;
	begin_x = begin_x > 0 ? begin_x : 0;
	begin_x = imagex + w_window / 2 - 1  >imagewidth ? imagewidth - w_window : begin_x;

	if (begin_y < 0)
	{
		begin_y = 0;
		end_y = h_window;
	}

	if (end_y > imageheight)
	{
		begin_y = imageheight - h_window;
		end_y = imageheight;
	}

	H_IMG myimg;

	if ( m_MarkOfViewForm[campos] == false) //m_tab == 1 &&
	{
		myimg.CreateEmpty(w_window, h_window, 24);
		for (int k = begin_y, q = 0; k < end_y; k++)
		{
			memcpy(myimg.m_lpDibArray + q,
				m_MoreImageCtrl.m_pShowData[campos] + 3 * (k*imagewidth + begin_x),
				3 * w_window*sizeof(BYTE));
			q += 3 * w_window*sizeof(BYTE);
		}

	}
	else
	{
		myimg.CreateEmpty(w_window, h_window, 24);

		for (int k = begin_y, q = 0; k < end_y; k++)
		{
			memcpy(myimg.m_lpDibArray + q, 
				m_MoreImageCtrl.m_pclibImage24[campos] + 3 * (k*imagewidth + begin_x), 
				w_window * 3 * sizeof(BYTE));
			q += w_window * 3 * sizeof(BYTE);
		}
	}

	if ((index != 0) && (y != 0))
	{
		//DEBUG_INFO3("cam_and_x %d %d   %d", campos,x ,y );

		if ((w_window > 0) && (h_window > 0))
		{
			CRect  rect;
			rect.left = point.x - w_window - offsetx;
			rect.right = point.x - offsetx;
			rect.top = point.y - offsety - h_window;
			rect.bottom = point.y - offsety;

			//////huj fff
			rect.left = rect.left - 30;
			rect.right = rect.right + 30;
			rect.top = rect.top - 30;
			rect.bottom = rect.bottom + 30;
			//DEBUG_INFO4("l-f-t-b:  %d  %d  %d  %d",rect.left,rect.right,rect.top, rect.bottom    )

			//HDC pDC = p_ShowPicZoomDlg->GetDC()->GetSafeHdc();
			//CRect Rect1;
			////m_MorePicZoom.ShowPosPic(pDC,Rect1,w,h);
			//p_ShowPicZoomDlg->MoveWindow(&rect, TRUE);
			//p_ShowPicZoomDlg->ShowWindow(SW_SHOW);
			//p_ShowPicZoomDlg->GetClientRect(&Rect1);
			//myimg.ShowCurrentImage(pDC, Rect1);//显示非模态对话框


			////mono
			CWnd *hpWnd = GetDlgItem(IDC_HPIC);
			CRect hreTree;
			hpWnd->GetClientRect(&hreTree);
			HDC hhDC = hpWnd->GetDC()->GetSafeHdc();
			myimg.ShowCurrentImage(hhDC, hreTree);
		}
	}
	else
	{
		//p_ShowPicZoomDlg->ShowWindow(SW_HIDE);
	}



	myimg.DeleteMe();

	return FALSE;

}




void CCalibrationCtrlView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CWnd *pwnd;
	pwnd=GetDlgItem(IDC_SMALLPIC);

	CRect rect;
	pwnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if((point.x<rect.left)||(point.x>rect.right))
	return ;

	if((point.y<rect.top)||(point.y>rect.bottom))
	return ;

	m_MoreImageCtrl.ShowClibSmallImage(m_MoreImageCtrl.m_SmallShowNum,m_MoreImageCtrl.m_SmalShowCount,&(m_MoreImageCtrl.m_SamllShowPos));

	CClientDC dc(this);
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	dc.SelectObject(&pen);

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);

	dc.Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);

	float fx=(point.x*1.0)/rect.Width();
	float fy=(point.y*1.0)/rect.Height();
	m_MoreImageCtrl.SetClibPoint(m_MoreImageCtrl.m_SmallShowNum,m_MoreImageCtrl.m_SmalShowCount,fx,fy);*/

	CFormView::OnLButtonDown(nFlags, point);
}

void CCalibrationCtrlView::ShowPos1()
{
	long curpos = 0, ncount = 0;
	CString msg = "";

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	curpos = m_MoreImageCtrl.GetCurImagePos(0, ncount);
	msg.Format("%0.8d/%0.8d", curpos, ncount - 1);
	pmain->m_EditCtrlPos1->SetEditText(msg);
	pmain->m_SliderCtrlPos1->SetPos(curpos);
}

void CCalibrationCtrlView::ShowPos2()
{
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(1, ncount);

	CString msg = "";
	msg.Format("%0.8d/%0.8d", curpos, ncount - 1);
	pmain->m_EditCtrlPos2->SetEditText(msg);

	pmain->m_SliderCtrlPos2->SetPos(curpos);


	m_MarkOfViewForm[1] = 0;////huj   fff
}

void CCalibrationCtrlView::SetPos1()
{
	MarkInvisable(0);
	MarkInvisable(1);

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;


	CString vlue = pmain->m_EditCtrlPos1->GetEditText();
	CString spos = vlue;
	int item = vlue.Find("/");

	if (item >= 0)
		spos = vlue.Left(item);
	int pos = atoi(spos);

	m_MoreImageCtrl.ShowDatImage(0, pos);

	ShowPos1();

	//huangjin  左右联
	//m_MoreImageCtrl.ShowDatImage(1, pos);//////
	//ShowPos2();//////////

}

void CCalibrationCtrlView::SetPos2()
{
	MarkInvisable(1);

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	CString vlue = pmain->m_EditCtrlPos2->GetEditText();
	CString spos = vlue;
	int item = vlue.Find("/");

	if (item >= 0)
		spos = vlue.Left(item);
	int pos = atoi(spos);

	//m_MoreImageCtrl.ShowDatImage(1, pos);

	//ShowPos2();
}

void CCalibrationCtrlView::SetSliderPos1()
{
	MarkInvisable(0);
	MarkInvisable(1);

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	long item = pmain->m_SliderCtrlPos1->GetPos();

	m_MoreImageCtrl.ShowDatImage(0, item);

	ShowPos1();



	//huangjin联动
	//m_MoreImageCtrl.ShowDatImage(1, item);//////
	//ShowPos2();//////////
}

void CCalibrationCtrlView::SetSliderPos2()
{
	MarkInvisable(1);

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	long item = pmain->m_SliderCtrlPos2->GetPos();

	m_MoreImageCtrl.ShowDatImage(1, item);

	ShowPos2();
}

void CCalibrationCtrlView::OnFristframe1()
{

	MarkInvisable(0);
	MarkInvisable(1);

	m_MoreImageCtrl.ShowDatImage(0, 0);
	ShowPos1();


	//huangjin  左右联动
	//m_MoreImageCtrl.ShowDatImage(1, 0);//////
	//ShowPos2();//////////
}

void CCalibrationCtrlView::OnUp1()
{
	MarkInvisable(0);
	MarkInvisable(1);


	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(0, ncount);

	curpos--;

	m_MoreImageCtrl.ShowDatImage(0, curpos);
	ShowPos1();

	//huangjin  左右联动
	//m_MoreImageCtrl.ShowDatImage(1, curpos);//////
	//ShowPos2();//////////
}

void CCalibrationCtrlView::OnDown1()
{
	MarkInvisable(0);
	MarkInvisable(1);


	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(0, ncount);

	curpos++;

	m_MoreImageCtrl.ShowDatImage(0, curpos);
	ShowPos1();


	//huangjin  左右联动
	///m_MoreImageCtrl.ShowDatImage(1, curpos);//////
	//ShowPos2();//////////
}

void CCalibrationCtrlView::OnEndfram1()
{
	MarkInvisable(0);
	MarkInvisable(1);

	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(0, ncount);

	m_MoreImageCtrl.ShowDatImage(0, ncount - 1);
	ShowPos1();


	//huangjin  左右联动
	//m_MoreImageCtrl.ShowDatImage(1, ncount - 1);//////
	//ShowPos2();//////////
}

void CCalibrationCtrlView::OnFristframe2()
{
	MarkInvisable(1);

	m_MoreImageCtrl.ShowDatImage(1, 0);
	ShowPos2();
}

void CCalibrationCtrlView::OnUp2()
{
	MarkInvisable(1);

	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(1, ncount);

	curpos--;

	m_MoreImageCtrl.ShowDatImage(1, curpos);
	ShowPos2();
}

void CCalibrationCtrlView::OnDown2()
{
	MarkInvisable(1);

	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(1, ncount);

	curpos++;

	m_MoreImageCtrl.ShowDatImage(1, curpos);
	ShowPos2();
}

void CCalibrationCtrlView::OnEndfram2()
{
	MarkInvisable(1);

	long curpos = 0, ncount = 0;

	curpos = m_MoreImageCtrl.GetCurImagePos(1, ncount);

	m_MoreImageCtrl.ShowDatImage(1, ncount - 1);
	ShowPos2();
}




void CCalibrationCtrlView::OnCalibration()
{
	/*

	CRegCheck check;
	string sn="";

	int  num =  check.GetCpuID(sn);
	bool  flag = check.CheckKey(sn);

	if(  num>0 && !flag          )
	{
	//AfxMessageBox("程序未注册！");
	//return ;
	}
	*/


	///////////////////////reg  

	//UpdateData(TRUE);
	//m_displaymark=false;
	m_SaveIsOK = 0;

	//m_Edit5.SetWindowText("");


	//if (!ReadXYZFromFile())
	//{
	//	AfxMessageBox("读取标志点三维坐标文件失败，或者标志点数目与设置不符，无法标定");
	//	return;
	//}

	//m_CalibrationPointNum = m_Cam[0].m_vObjPt.size(); //h hu add




	//m_Cam[0].Set(m_CalibrationPointNum);
	//m_Cam[1].Set(m_CalibrationPointNum);   //初始化标定

	//GetDlgItemText(IDC_ONECAMIMAGE, m_CamFileName[0]);
	//GetDlgItemText(IDC_TWOCAMIMAGE, m_CamFileName[1]);

	//if (m_CamFileName[1] == m_CamFileName[0])
	//{
	//	AfxMessageBox("两相机标定图像来源是同一文件，退出标定！");
	//	return;
	//}




	//if (8 > m_CalibrationPointNum)
	//{
	//	AfxMessageBox("标定要求控制点最少8个，\n\r三维坐标文件中标定点过少，无法标定！");
	//	return;
	//}

	//switch (GetCalibrationImageMark())
	//{
	//case 1:
	//	break;

	//case 0:
	//	AfxMessageBox("图像无法读取，退出标定");
	//	return;
	//	break;

	//case -1:
	//	AfxMessageBox("三维坐标文件中所列控制点没有在图像中全部出现，\n\r 无法标定！");
	//	m_displaymark = true;
	//	OnOpenimage();    /////////
	//	m_displaymark = false;
	//	m_Edit5.SetWindowText("无法标定");
	//	return;
	//}

	//m_IsRightHanded = CheckRightHanded();
	//switch (m_IsRightHanded)
	//{
	//case 1:
	//	break;

	//case -1:
	//	AfxMessageBox("输入三维坐标点有误！");
	//	return;
	//	break;

	//case 0:
	//	for (int i = 0; i < m_CalibrationPointNum; i++)
	//	{
	//		m_Cam[0].m_vObjPt[i].z = -m_Cam[0].m_vObjPt[i].z;
	//		m_Cam[1].m_vObjPt[i].z = -m_Cam[1].m_vObjPt[i].z;
	//	}
	//}

	//if (!m_Cam[0].Calib() || !m_Cam[1].Calib())
	//{
	//	AfxMessageBox("输入数据有误，无法进行标定计算！");
	//	// 			m_displaymark=true;
	//	// 			this->OnOpenimage();    /////////
	//	// 			m_displaymark=false;
	//	return;
	//}

	//m_displaymark = true;
	//this->OnOpenimage();
	//m_displaymark = false;

	////m_Cam1.m_Camera.MessCameraPara();   ///
	////m_Cam2.m_Camera.MessCameraPara();////

	//ShowCalibrationResult();

	//if (m_Cam[0].m_Camera.m_InnerPara.m_fFx > 9)
	//{
	//	//AfxMessageBox("无法进行标定计算，不保存");
	//	//return;
	//	m_SaveIsOK = 1;
	//}

	return;
}


void CCalibrationCtrlView::OnSaveCalibrationResult()
{
	//CFileDialog dlg(FALSE, _T("CalibResult"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	(LPCTSTR)("标定结果文件 (*.CalibResult)|*.CalibResult|All Files (*.*)|*.*||"), NULL);

	//dlg.m_ofn.lpstrInitialDir = m_hpath.CalibResultPath;

	//if (dlg.DoModal() == IDOK)
	//{
	//	m_CalibrationResultFile = dlg.GetPathName(); //文件名保存在了FilePathName里
	//}
	//else
	//{
	//	return;
	//}

	//FILE *fpCam;
	//errno_t err = fopen_s(&fpCam, m_CalibrationResultFile, "w");
	//if (err != 0)
	//{
	//	AfxMessageBox("保存文件失败!");
	//	return;//false;
	//}

	//fprintf(fpCam, "右手坐标系:  %d\n", m_IsRightHanded);
	//fprintf(fpCam, "坐标偏移mm:%16.5f %16.5f %16.5f\n\n", m_OffsetPt.x, m_OffsetPt.y, m_OffsetPt.z);

	//for (int i = 0; i < 2; i++)
	//{
	//	fprintf(fpCam, "相机 %d 参数:\n", i + 1);

	//	//内参数 u0, v0, fx, fy, k0, k1, k2, k3, k4
	//	fprintf(fpCam, "fx:%16.10f\n", m_Cam[i].m_Camera.m_InnerPara.m_fFx);
	//	fprintf(fpCam, "fy:%16.10f\n", m_Cam[i].m_Camera.m_InnerPara.m_fFy);
	//	fprintf(fpCam, "Cx:%16.10f\n", m_Cam[i].m_Camera.m_InnerPara.m_ptCenter.x);
	//	fprintf(fpCam, "Cy:%16.10f\n", m_Cam[i].m_Camera.m_InnerPara.m_ptCenter.y);
	//	fprintf(fpCam, "k:%18.15f	%18.15f	%18.15f	%18.15f	%18.15f	\n",
	//		m_Cam[i].m_Camera.m_InnerPara.m_Devi[0],
	//		m_Cam[i].m_Camera.m_InnerPara.m_Devi[1],
	//		m_Cam[i].m_Camera.m_InnerPara.m_Devi[2],
	//		m_Cam[i].m_Camera.m_InnerPara.m_Devi[3],
	//		m_Cam[i].m_Camera.m_InnerPara.m_Devi[4]);

	//	//外参数 Ax, Ay, Az, Tx, Ty, Tz
	//	fprintf(fpCam, "A:%20.15f %20.15f %20.15f\n",
	//		m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngX * 180 / PI,
	//		m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngY * 180 / PI,
	//		m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngZ * 180 / PI);
	//	fprintf(fpCam, "T:%18.10f %18.10f %18.10f\n",
	//		m_Cam[i].m_Camera.m_RTPara.m_Trans.x,
	//		m_Cam[i].m_Camera.m_RTPara.m_Trans.y,
	//		m_Cam[i].m_Camera.m_RTPara.m_Trans.z);

	//	//重建误差 MeanError
	//	fprintf(fpCam, "MeanError:%lf\n", sqrt(m_Cam[i].m_Camera.m_fCamModelError) / m_Cam[i].m_Camera.m_nPtNum);

	//	fprintf(fpCam, "\n\n");

	//}
	//fprintf(fpCam, "相机1标定图像路径: %s\n", m_CamFileName[0]);
	//fprintf(fpCam, "相机2标定图像路径: %s\n", m_CamFileName[1]);
	//fprintf(fpCam, "标定点三维坐标文件: %s\n", m_XYZFileName);
	//fclose(fpCam);

	//m_SaveIsOK = false;
	return;
}

// 获得一个相机标志点图像坐标
int CCalibrationCtrlView::GetCalibrationImageMark(void)
{
	//IplImage * img[2];
	//bool miss[2] = { 0 };

	//for (int index = 0; index < 2; index++)
	//{

	//	img[index] = cvLoadImage(m_CamFileName[index], 0);

	//	if (img[index] == 0)
	//	{
	//		return FALSE;
	//		//AfxMessageBox("载入图像失败！无法标定");
	//	}

	//	CvSize sz = cvGetSize(img[index]);

	//	if (sz.width % 4 != 0)
	//	{
	//		AfxMessageBox("载入图像失败！图像宽应能被4整除！");
	//		cvReleaseImage(&img[index]);
	//		return FALSE;
	//	}

	//	m_Ct[index].Initial(sz.width, sz.height);			// 检测初始化
	//	m_Ct[index].Decoding_oneStep(img[index]);	//一步解码

	//	m_Cam[index].m_vImgPt.clear();
	//	m_Cam[index].m_vImgPt.resize(m_CalibrationPointNum);

	//	// 	if (m_Cam[index].m_Num != m_Ct[index].m_targetNum )
	//	// 	{
	//	// 		AfxMessageBox("图像标定点编号与设定不符");
	//	// 		return false;
	//	// 	}

	//	int matchnum = 0;
	//	for (int i = 0; i < m_Cam[index].m_Num; i++)
	//		for (int j = 0; j < m_Ct[index].m_targetNum; j++)
	//		{
	//			if (m_Ct[index].m_target[j].code == m_Cam[index].m_vIndex[i])
	//			{
	//				m_Cam[index].m_vImgPt[i].x = m_Ct[index].m_target[j].x;
	//				m_Cam[index].m_vImgPt[i].y = m_Ct[index].m_target[j].y;
	//				matchnum++;
	//			}
	//		}


	//	//
	//	//  	cvNamedWindow( "target", 0 );
	//	//  	m_Ct[index].Draw_target();
	//	//  	cvShowImage( "target", m_Ct[index].m_imgc);	//作图
	//	//  	cvWaitKey();//
	//	// 	cvDestroyAllWindows();

	//	m_Ct[index].Draw_mark();// ////before return-1
	//	cvReleaseImage(&img[index]);

	//	if (matchnum != m_CalibrationPointNum)
	//	{
	//		miss[index] = 1;
	//		//AfxMessageBox("图像标定点编号与设定不符");
	//	}
	//}

	//if (miss[0] || miss[1])
	//{
	//	return -1;
	//}

	return TRUE;
}

bool CCalibrationCtrlView::ReadXYZFromFile()
{
	//CString newfilename = m_XYZFileName + ".ansi.txt";

	//CFile file;
	//BYTE  buf[2];
	//if (file.Open(m_XYZFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary))
	//{
	//	file.Read(&buf, 2);
	//	if (buf[0] == 0xFF && buf[1] == 0xFE)
	//	{
	//		//AfxMessageBox("输入的坐标文本文件不是ANSI格式，请用Windows自带笔记本保存为ANSI编码！");

	//		if (!UnicodeFile2Ansi((LPCSTR)m_XYZFileName, (LPCSTR)newfilename))
	//		{
	//			return false;
	//		}
	//		m_XYZFileName = newfilename;
	//		//
	//	}
	//	file.Close();
	//}

	///////////////////huangjin avoid utf
	//CFile rfile;
	//BYTE  pbuf;
	//if (rfile.Open(m_XYZFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary))
	//{
	//	rfile.Read(&pbuf, 1);
	//	if (pbuf == 0xEF || pbuf == 0xFF || pbuf == 0xFE)
	//	{
	//		AfxMessageBox("输入的坐标文本文件不是ANSI格式，请用Windows自带笔记本保存为ANSI编码！");
	//		rfile.Close();
	//		return false;
	//	}
	//	rfile.Close();
	//}

	/////////////////////

	//for (int i = 0; i < 2; i++)
	//{
	//	if (!m_Cam[i].ReadObjPt(m_XYZFileName))
	//	{
	//		return false;
	//	}

	//	//if ((m_Cam[0].m_vObjPt.size() != m_CalibrationPointNum)  )
	//	//    {
	//	//        return false;
	//	//    }
	//}

	//////////////
	//int num = m_Cam[0].m_vObjPt.size();
	//if (num < 1)
	//{
	//	return false;
	//}

	//double tmpx = 0.0, tmpy = 0.0, tmpz = 0.0;
	//for (int i = 0; i < num; i++)
	//{
	//	tmpx = tmpx + m_Cam[0].m_vObjPt[i].x;
	//	tmpy = tmpy + m_Cam[0].m_vObjPt[i].y;
	//	tmpz = tmpz + m_Cam[0].m_vObjPt[i].z;
	//}

	//m_OffsetPt = H_Pt3D(tmpx / num, tmpy / num, tmpz / num);

	//for (int i = 0; i < num; i++)
	//{
	//	m_Cam[0].m_vObjPt[i].x = m_Cam[0].m_vObjPt[i].x - m_OffsetPt.x;
	//	m_Cam[0].m_vObjPt[i].y = m_Cam[0].m_vObjPt[i].y - m_OffsetPt.y;
	//	m_Cam[0].m_vObjPt[i].z = m_Cam[0].m_vObjPt[i].z - m_OffsetPt.z;
	//}

	//m_Cam[1].m_vObjPt = m_Cam[0].m_vObjPt;

	return true;
}


// 读取标定结果
bool CCalibrationCtrlView::ReadCalibrationResult(void)
{
	//     CFileDialog dlg(TRUE, _T("CalibResult"), NULL,	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//                     (LPCTSTR)("标定结果文件 (*.CalibResult)|*.CalibResult|All Files (*.*)|*.*||"),	NULL);
	//     if(dlg.DoModal()==IDOK)
	//         {
	//             m_CalibrationResultFile=dlg.GetPathName(); //文件名保存在了FilePathName里
	//         }
	//     else
	//         {
	//             return false;
	//         }

	//GetDlgItemText(IDC_SDDATA, m_CalibrationResultFile);	     //输入结果

	//FILE *fpCam;
	//errno_t err = fopen_s(&fpCam, m_CalibrationResultFile, "r");
	//if (err != 0)
	//{
	//	//AfxMessageBox("读取文件失败!");
	//	return false;//false;
	//}

	//double x, y, z;
	//fscanf(fpCam, "右手坐标系:  %d\n", &m_ChangeZ);   //左
	//fscanf(fpCam, "坐标偏移mm:%lf %lf %lf\n\n", &x, &y, &z);
	///////////////huangjin   reback  offset

	//DEBUG_INFO1("right-left  %d ", m_ChangeZ);
	//DEBUG_INFO1("calib offsetx   %lf", x);
	//m_OffsetPt = H_Pt3D(x, y, z);



	//for (int i = 0; i < 2; i++)
	//{
	//	int temp;
	//	fscanf(fpCam, "相机 %d 参数:\n", &temp);

	//	//内参数 u0, v0, fx, fy, k0, k1, k2, k3, k4
	//	fscanf(fpCam, "fx:%lf\n", &m_Cam[i].m_Camera.m_InnerPara.m_fFx);
	//	fscanf(fpCam, "fy:%lf\n", &m_Cam[i].m_Camera.m_InnerPara.m_fFy);
	//	fscanf(fpCam, "Cx:%lf\n", &m_Cam[i].m_Camera.m_InnerPara.m_ptCenter.x);
	//	fscanf(fpCam, "Cy:%lf\n", &m_Cam[i].m_Camera.m_InnerPara.m_ptCenter.y);
	//	fscanf(fpCam, "k:%lf %lf %lf %lf %lf\n",
	//		&m_Cam[i].m_Camera.m_InnerPara.m_Devi[0],
	//		&m_Cam[i].m_Camera.m_InnerPara.m_Devi[1],
	//		&m_Cam[i].m_Camera.m_InnerPara.m_Devi[2],
	//		&m_Cam[i].m_Camera.m_InnerPara.m_Devi[3],
	//		&m_Cam[i].m_Camera.m_InnerPara.m_Devi[4]);

	//	//外参数 Ax, Ay, Az, Tx, Ty, Tz
	//	fscanf(fpCam, "A:%lf %lf %lf\n",
	//		&m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngX,
	//		&m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngY,
	//		&m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngZ);
	//	fscanf(fpCam, "T:%lf %lf %lf\n",
	//		&m_Cam[i].m_Camera.m_RTPara.m_Trans.x,
	//		&m_Cam[i].m_Camera.m_RTPara.m_Trans.y,
	//		&m_Cam[i].m_Camera.m_RTPara.m_Trans.z);

	//	m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngX = m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngX / 180 * PI;
	//	m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngY = m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngY / 180 * PI;
	//	m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngZ = m_Cam[i].m_Camera.m_RTPara.m_ELAng.AngZ / 180 * PI;

	//	//重建误差 MeanError
	//	fscanf(fpCam, "MeanError:%lf \n\n\n", &m_Cam[i].m_MeanError);
	//	fscanf(fpCam, "");
	//}
	//fclose(fpCam);

	//m_BinoCam.Set(m_Cam[0], m_Cam[1]);

	return true;
}


// 显示标定结果
void CCalibrationCtrlView::ShowCalibrationResult(void)
{
	//CString result[2], temp, str;
	//float meanerror[2];
	//bool flag = false;

	//for (int i = 0; i < 2; i++)
	//{
	//	result[i].Format("相机%d 参数:\r\n", i + 1);

	//	temp.Format(" 焦距fx: %.1lf\r\n 焦距fy: %.1lf\r\n 光心Cx: %.1lf\r\n 光心Cy: %.1lf\r\n",
	//		m_Cam[i].m_Camera.m_InnerPara.m_fFx,
	//		m_Cam[i].m_Camera.m_InnerPara.m_fFy,
	//		m_Cam[i].m_Camera.m_InnerPara.m_ptCenter.x,
	//		m_Cam[i].m_Camera.m_InnerPara.m_ptCenter.y);

	//	result[i] = result[i] + temp;

	//	temp.Format(" 平移矩阵（模型中心相对于相机）:\r\n  %.1lf  %.1lf  %.1lf\r\n",
	//		m_Cam[i].m_Camera.m_RTPara.m_Trans.x,
	//		m_Cam[i].m_Camera.m_RTPara.m_Trans.y,
	//		m_Cam[i].m_Camera.m_RTPara.m_Trans.z);

	//	result[i] = result[i] + temp;

	//	meanerror[i] = m_Cam[i].m_MeanError;

	//	if (meanerror[i] <= 0 || meanerror[i] > 100)
	//	{
	//		flag = true;
	//		temp = " 标定误差:	极大	\r\n\r\n";/////∞
	//	}
	//	else
	//		temp.Format(" 标定误差:	%.2lf	\r\n\r\n", meanerror[i]);
	//	result[i] = result[i] + temp;
	//}

	//temp = result[0] + result[1];
	//m_Edit5.SetWindowText(temp);

	//DEBUG_INFO1("error-a    %lf", meanerror[0]);
	//DEBUG_INFO1("error-b    %lf", meanerror[1]);

	//////////////////////// ///alarm

	//bool resultisgood = true;
	//double diff[2];

	//for (int i = 0; i < 2; i++)
	//	diff[i] = fabs(m_Cam[i].m_Camera.m_InnerPara.m_fFx - m_Cam[i].m_Camera.m_InnerPara.m_fFy);

	//temp = "";
	//for (int i = 0; i < 2; i++)
	//{
	//	if (diff[i] > 0.15 * m_Cam[i].m_Camera.m_InnerPara.m_fFx)
	//	{
	//		result[i].Format("相机_%d ", i + 1);
	//		temp += result[i];
	//	}
	//}
	//if (temp.GetLength() > 3)
	//{
	//	result[0].Format("XY方向等效焦距fx、fy相差过大。\n\n ");
	//	temp += result[0];
	//	resultisgood = false;
	//	str += temp;
	//}

	//temp = "";
	//for (int i = 0; i < 2; i++)
	//{
	//	if (meanerror[i] > 2)
	//	{
	//		result[i].Format("相机_%d ", i + 1);
	//		temp += result[i];
	//	}
	//}
	//if (temp.GetLength() > 3)
	//{
	//	result[0].Format("标定结果误差较大，可能由于标定点三维坐标数据误差大造成。\n\n ");
	//	temp += result[0];
	//	resultisgood = false;
	//	str += temp;
	//}

	//if (flag)
	//{
	//	str += "标定误差极大，可能是标定点图像序号和三维坐标文件序号没有对应上。\n\n";
	//}

	//if (!resultisgood)
	//{
	//	AfxMessageBox("本次标定无效！\n\n " + str);
	//}
	return;
}




void CCalibrationCtrlView::OnEditCalibNum()
{
	UpdateData(TRUE);
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT3));
	str = p->GetEditText();
	m_CalibrationPointNum = _ttoi(str);
	str.Format("%d", m_CalibrationPointNum);
	p->SetEditText(str);
	UpdateData(FALSE);
}

bool CCalibrationCtrlView::GetBinoMark(long pos, long offset)
{
	PBYTE pGrayBuffer[2];//pGrayBuffer_0,pGrayBuffer_1;
	int len;
	long tmp[2] = { 0, offset };



	IplImage* cimg[2];
	int w[2];
	w[1] = m_MoreImageCtrl.m_W[1];
	w[0] = m_MoreImageCtrl.m_W[0];
	int h[2];
	h[0] = m_MoreImageCtrl.m_H[0];
	h[1] = m_MoreImageCtrl.m_H[1];

	///////// //////[2]//

	/////#pragma omp parallel for num_threads(2)
	for (int i = 0; i < 1; i++)  //mono
	{
		pGrayBuffer[i] = m_MoreImageCtrl.m_MapCtrl[i].GetMapData(tmp[i] + pos, 1, len);

		m_img[i] = cvCreateImage(cvSize(w[i], h[i]), IPL_DEPTH_8U, 1);
		cimg[i] = cvCreateImageHeader(cvSize(w[i], h[i]), IPL_DEPTH_8U, 3);
		cvSetData(cimg[i], pGrayBuffer[i], w[i] * 3); ////3

		//cvNamedWindow("qq", 0);
		//cvShowImage("qq", cimg);
		//cvWaitKey(1000);
		//cvDestroyAllWindows();

		cvCvtColor(cimg[i], m_img[i], CV_BGR2GRAY);


		//cvSetData(m_img[i], pGrayBuffer[i], m_MoreImageCtrl.m_W[i]); //第3个参数是行字节数

		cvFlip(m_img[i]);
		m_Ct[i].Decoding(m_img[i]);

		cvReleaseImageHeader(&cimg[i]);
		cvReleaseImage(&m_img[i]);//huj fff

		//DEBUG_INFO1("Thread Id = %ld/n", omp_get_thread_num());
	}

	//     pGrayBuffer_0 = m_MoreImageCtrl.m_MapCtrl[0].GetMapData(pos,1,len);
	//     cvSetData(m_img[0],pGrayBuffer_0,m_MoreImageCtrl.m_W[0]); //第3个参数是行字节数
	//     cvFlip(m_img[0]);
	//     m_Ct[0].Decoding(m_img[0]);
	//
	//     ///////// offset for 2nd dat, default =0
	//     pGrayBuffer_1 = m_MoreImageCtrl.m_MapCtrl[1].GetMapData(pos+offset,1,len);
	//     cvSetData(m_img[1],pGrayBuffer_1,m_MoreImageCtrl.m_W[1]); //第3个参数是行字节数
	//     cvFlip(m_img[1]);
	//     m_Ct[1].Decoding(m_img[1]);

	//cvReleaseImageHeader(&m_img[0]);
	//cvReleaseImageHeader(&m_img[1]);
	//////////////
	//m_Ct[i].Draw_mark();
	//cvFlip(m_Ct[i].m_imgc);
	//cvNamedWindow("ddd",0);
	//cvShowImage("ddd",m_Ct[i].m_imgc);
	//cvWaitKey(2);
	//DEBUG_INFO2("camera: %d , pos: %d\n",i, pos);
	// 	IplImage *imgk=cvLoadImage("W:\\kj\\k\\1.jpg",0);
	//     IplImage *imgj=cvLoadImage("W:\\kj\\j\\1.jpg",0);
	// 	CvSize sz=cvGetSize(imgk);
	// 	m_Ct[0].Initial(sz.width, sz.height);			// 检测初始化
	// 	m_Ct[0].Decoding_oneStep(imgk);	//一步解码
	// 	m_Ct[1].Initial(sz.width, sz.height);			// 检测初始化
	// 	m_Ct[1].Decoding_oneStep(imgj);	//一步解码
	// 	////////////debug

	DEBUG_INFO1("getbion pos:%d", pos);

	if (m_Ct[0].m_targetNum == 0)//mono
		//if (m_Ct[0].m_targetNum == 0 || m_Ct[1].m_targetNum == 0) //mono
	{
		m_Record.PtValid.assign(m_Record.PtValid.size(), false);   //////huangjin add
		return true;
	}

	int j_ok, k_ok;
	bool flag_j, flag_k = true;

	for (int i = 0; i < m_Record.Index.size(); i++)
	{
		for (int j = 0; j < m_Ct[0].m_targetNum; j++)
		{
			flag_j = false;
			if (m_Ct[0].m_target[j].code == m_Record.Index[i])
			{
				j_ok = j;
				flag_j = true;
				break;
			}
		}

		//for (int k = 0; k < m_Ct[1].m_targetNum; k++)
		//{
		//	flag_k = false;
		//	if (m_Record.Index[i] == m_Ct[1].m_target[k].code)
		//	{
		//		k_ok = k;
		//		flag_k = true;
		//		break;
		//	}
		//}


		DEBUG_INFO2("flag-k: %d    flag-j: %d", flag_k, flag_j);

		if (flag_k && flag_j)
		{
			m_Record.ImgPt0[i].x = m_Ct[0].m_target[j_ok].x;
			m_Record.ImgPt0[i].y = m_Ct[0].m_target[j_ok].y;

			//mono
			//m_Record.ImgPt1[i].x = m_Ct[1].m_target[k_ok].x;
			//m_Record.ImgPt1[i].y = m_Ct[1].m_target[k_ok].y;

			m_Record.PtValid[i] = 1;
		}
		else
		{
			//m_Record.PtValid.at(i)=false;
			m_Record.PtValid[i] = 0;   //////huangjin add
		}
	}

	//  DEBUG_INFO5("pos:%d No:%d pt_ok:%d l_x:%lf  r_x:%lf", pos,m_Record.Index, m_Record.PtValid, m_Record.ImgPt0[0].x, m_Record.ImgPt1[0].x);
	return true;
}

// 开始测量
void CCalibrationCtrlView::OnStartMeasure(void)
{
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;
	//OnOpendatfile();
	if (!pmain->m_MsmtIsOpen)
	{
		//AfxMessageBox("无法打开图像!");
		return;
	}

	//mono
	//bool flag1 = (m_MoreImageCtrl.m_W[0] != 4080);
	//bool flag2 = (m_MoreImageCtrl.m_W[0] > 1380) || (m_MoreImageCtrl.m_H[0] > 1290);

	//if (flag1 && flag2)
	//{
	//	AfxMessageBox("不能处理此种格式图像数据!");
	//	return;
	//}
 
	//if (!ReadCalibrationResult())
	//{
	//	AfxMessageBox("标定结果文件无法打开，退出测量过程!");
	//	return;
	//}

	//mono
	if (h_Diameter < 1)
	{
		AfxMessageBox("请输入正确的标志内圆直径！");
		return;
	}
 
	//if (abs(m_MoreImageCtrl.m_MapCtrl[0].m_pHead->nFrame - m_MoreImageCtrl.m_MapCtrl[1].m_pHead->nFrame) > 2)
	//{
	//	if (m_MoreImageCtrl.m_MapCtrl[0].m_pHead->nFrame != 0 || m_MoreImageCtrl.m_MapCtrl[1].m_pHead->nFrame != 0)
	//	{
	//		CString c;
	//		c.Format("相机1、2帧率不同，分别为%d、%d!", m_MoreImageCtrl.m_MapCtrl[0].m_pHead->nFrame, m_MoreImageCtrl.m_MapCtrl[1].m_pHead->nFrame);
	//		AfxMessageBox(c);
	//		//return;
	//	}
	//}

	//OnEdit_beginpos();
	//OnEdit_endpos();
	//OnEdit_offset();
	///////////////set begin and end frame, avoid overflow
	long ncount_0 = 0, ncount_1 = 0, count;
	m_MoreImageCtrl.GetCurImagePos(0, ncount_0);
	//m_MoreImageCtrl.GetCurImagePos(1, ncount_1);
	//count = min(ncount_0, ncount_1);

	//mono
	count = ncount_0;
	m_offset = 0;

	if (m_beginpos > count)
	{
		AfxMessageBox("超出图像数据！");
		return;
	}

	if (m_UseDefaultSetting == 1)
	{
		m_beginpos = 0;
		m_endpos = count - 1;
		m_offset = 0;
	}
	else
	{
		m_beginpos = m_beginpos > 0 ? m_beginpos : 0;
		//m_endpos   = m_endpos>0 ? m_endpos:0;
		m_endpos = m_endpos > 0 ? m_endpos : 0;
		m_beginpos = m_offset > 0 ? m_beginpos : m_beginpos - m_offset;
		m_endpos = min(count - m_offset - 1, min(count - 1, m_endpos));
	}

	if (m_endpos < m_beginpos)
	{
		AfxMessageBox("必须结束帧大于起始帧！");
		return;
	}

	///////////huj fff
	//AfxMessageBox("演示版 未缓存内存池限制！");
	///////
	//CvRNG rng;
	//rng = cvRNG(cvGetTickCount());

	//int h1 = m_beginpos;
	//int h2 = m_endpos;

	//for (size_t ii = 0; ii < 1700; ii++)
	//{
	//	hujfff1[ii + 2] = h1 + h2 + ii;
	//	hujfff1[ii] = 988 + cvRandInt(&rng) % 119 + 4 * cvRandReal(&rng) + 0.5;
	//}

	//int tmp2 = hujfff1[337 + cvRandInt(&rng) % 327];

	/////////////////////////
	//if (m_endpos > m_beginpos + tmp2 + 3 * cvRandReal(&rng) + 0.5)
	//{
	//	m_endpos = m_beginpos + tmp2;
	//}
	//else
	//{
	//	tmp2 = tmp2 + 3 * cvRandReal(&rng) + 0.5;
	//	tmp2 = tmp2 / 1699.877;

	//	m_endpos = m_endpos + tmp2;
	//}
	//////////////////////////////


	int len;
	int w;
	int h;
	PBYTE pGrayBuffer;
	int matchnum = 0;

	for (int i = 0; i < 1; i++)  ////mono
	{
		if (m_MoreImageCtrl.m_W[i] == 0)
		{
			AfxMessageBox("图像文件打开失败！");
			return;
		}

		w = m_MoreImageCtrl.m_W[i];
		h = m_MoreImageCtrl.m_H[i];

		pGrayBuffer = m_MoreImageCtrl.m_MapCtrl[i].GetMapData(m_beginpos, 1, len);

		//m_img[i] = cvCreateImageHeader(cvSize(w, h), 8, 1);
		//cvSetData(m_img[i], pGrayBuffer, w); //第3个参数是行字节数

		m_img[i] = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
		IplImage* cimg = cvCreateImageHeader(cvSize(w, h), IPL_DEPTH_8U, 3);
		cvSetData(cimg, pGrayBuffer, w * 3);
		cvCvtColor(cimg, m_img[i], CV_BGR2GRAY);
		cvReleaseImageHeader(&cimg);

		cvFlip(m_img[i]);  ////////

		m_Ct[i].Initial(w, h);
		m_Ct[i].Get_adaptive_parameter(m_img[i]);
		m_Ct[i].Decoding(m_img[i]);
		cvReleaseImage(&m_img[i]);  ////huj fff
	}

	m_Record.Index.clear();

	//mono
	for (int j = 0; j < m_Ct[0].m_targetNum; j++)
		m_Record.Index.push_back(m_Ct[0].m_target[j].code);


	//for (int j = 0; j < m_Ct[0].m_targetNum; j++)
	//	for (int k = 0; k < m_Ct[1].m_targetNum; k++)
	//		if (m_Ct[0].m_target[j].code == m_Ct[1].m_target[k].code)
	//		{
	//			m_Record.Index.push_back(m_Ct[0].m_target[j].code);
	//			matchnum++;
	//			continue;
	//		}

	DEBUG_INFO1("match num :  %d", matchnum);
	DEBUG_INFO1(" m_Record.Index-size,  %d", m_Record.Index.size());

	//if (matchnum == 0)
	//{
	//	AfxMessageBox("两相机图像序列无相同编码点，无法测量！");
	//	return;
	//}

	//////mono
	if (m_Ct[0].m_targetNum == 0)
	{
		AfxMessageBox(" 图像序列无编码标志点，无法测量！");
		return;
	}


	std::sort(m_Record.Index.begin(), m_Record.Index.end());

	///////mono sort and axis
	h_scale.resize(m_Ct[0].m_targetNum);
	for (size_t i = 0; i < m_Ct[0].m_targetNum; i++)
	{
		for (size_t j = 0; j < m_Ct[0].m_targetNum; j++)
		{
			if (m_Record.Index[i] == m_Ct[0].m_target[j].code)
			{
				int indx = m_Ct[0].m_target[j].re;
				h_scale[i] = max(m_Ct[0].m_boxv[indx].size.width, m_Ct[0].m_boxv[indx].size.height);
				h_scale[i] = h_Diameter / h_scale[i];
			}
		}
	}



	int tmp = m_Record.Index.size();


	//m_BinoCam.SetNum(tmp);  //mono


	m_Record.ImgPt0.resize(tmp);
	m_Record.ImgPt1.resize(tmp);
	m_Record.ResultPt.resize(tmp);
	m_Record.PtValid.resize(tmp, false);               /////very important
	m_AllRecord.resize(m_endpos - m_beginpos + 1);           ///////////////////////
	/////////

	//////////////////mono
	//CString result[2];
	//for (int i = 0; i < 2; i++)
	//{
	//	result[i].Format("相机%d参数:\r\n焦距fx: %.1lf, 焦距fy: %.1lf\r\n标定误差: %.2f \r\n\r\n",
	//		i + 1, m_Cam[i].m_Camera.m_InnerPara.m_fFx, m_Cam[i].m_Camera.m_InnerPara.m_fFy, m_Cam[i].m_MeanError);
	//}
	//result[0] = result[0] + result[1];
	//result[1].Format("测量起始帧: %d  \r\n结束帧: %d \r\n测量点:%d个 ", m_beginpos, m_endpos, tmp);
	//m_Edit5.SetWindowText(result[0] + result[1]);
	////////////////

	//long t1=GetTickCount();         /////////////huangjin

	m_pMyThread1 = AfxBeginThread(MfcThreadProc1, this);   /////for mark
	m_pMyThread2 = AfxBeginThread(MfcThreadProc2, this);    ////for show
	m_tipdlg.DoModal();

	m_tipdlg.m_flag = true;  //for safe

	//OnFristframe1();
	///////////////huangjin 730
	m_MoreImageCtrl.ShowDatImage(0, m_endpos);
	ShowPos1();

	//mono
	//m_MoreImageCtrl.ShowDatImage(1, m_endpos + m_offset);//////
	//ShowPos2();//////////

	if (m_abortflag)
	{
		return;
	}

	//     for (long k=m_beginpos; k<=m_endpos ; k++)
	//         {
	//             if (k%512==1)
	//                 {
	//                     for (int i=0; i<2 ; i++)
	//                         {
	//                             m_Ct[i].Get_adaptive_parameter(m_img[i]);
	//                         }
	//                 }
	//             GetBinoMark(k ,m_offset);//
	//             CalcPerFrame();//
	//             m_AllRecord[k-m_beginpos]=m_Record.ResultPt;//
	//             //ExcelRecordPerFrame(sheet,k-m_beginpos+2,k, tmp );
	//             DEBUG_INFO1("pos:= %d",k);
	//         }
	//////

	//long t2=GetTickCount();
	//DEBUG_INFO1("time per frame       %f", (t2-t1)/(m_endpos-m_beginpos+1.00001));

	//cvReleaseImage(&m_img[0]);
	//cvReleaseImage(&m_img[1]);  ////huj fff  cvReleaseImageHeader


	SaveMeasureRecord();

	//AfxMessageBox("测量完成，数据保存到默认目录。");
	//OnOpendatfile();

	return;
}


UINT MfcThreadProc1(LPVOID lpParam)
{
	CCalibrationCtrlView * pThis = (CCalibrationCtrlView *)lpParam;

	pThis->m_tipdlg.m_flag = false;
	pThis->m_abortflag = true;


	//mono
	pThis->GetBinoMark(pThis->m_beginpos, 0);
	pThis->h_FirstImgPt.assign(pThis->m_Record.ImgPt0.begin(), pThis->m_Record.ImgPt0.end());



	for (long k = pThis->m_beginpos; k <= pThis->m_endpos; k++)
	{
		_showpos = k;

		if (pThis->m_tipdlg.m_flag)
		{
			pThis->m_abortflag = true;
			return 0;
		}

		//long t1 = GetTickCount(); /////huj fff

		pThis->GetBinoMark(k, pThis->m_offset);

		//long t2 = GetTickCount();///huj fff

		////////////////////////////////////monosleep
		//CvRNG rng;
		//rng = cvRNG(cvGetTickCount());
		//int ms = (t2 - t1)*3.1001 + cvRandInt(&rng) % 93;
		//if (pThis->m_MoreImageCtrl.m_W[0] > 3000)
		//{
		//	ms = ms*0.1 + 1;
		//}
		//Sleep(ms);

		//int tmp2 = 433 + cvRandInt(&rng) % 9;

		//int pp[2048];
		//for (size_t ii = 0; ii < tmp2 - 67; ii++)
		//	for (size_t kk = 0; kk < tmp2 - 83; kk++)
		//	{
		//		pp[ii] = tmp2*0.3535111353534 - 3.43888434 + kk;
		//		pp[ii] = pp[ii] * 0.17 - kk;

		//		pThis->hujfff1[kk] = kk + pp[ii] - 0.65;
		//	}
		//////////////////////////////mono

		pThis->CalcPerFrame();

		pThis->m_AllRecord[k - pThis->m_beginpos] = pThis->m_Record.ResultPt;

		//DEBUG_INFO3("pos:= %d, b-%d   ,   e-%d ",k,pThis->m_beginpos,  pThis->m_endpos   );

		if (k % 512 == 212)
		{
			//AfxMessageBox("dfdfdf");

			PBYTE pGrayBuffer;
			int len;
			long tmp[2] = { 0, pThis->m_offset };
			IplImage* cimg;

			for (int i = 0; i < 1; i++)  ///mono i<2
			{
				pGrayBuffer = pThis->m_MoreImageCtrl.m_MapCtrl[i].GetMapData(k + tmp[i], 1, len);
				int w = pThis->m_MoreImageCtrl.m_W[i];
				int h = pThis->m_MoreImageCtrl.m_H[i];
				pThis->m_img[i] = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
				cimg = cvCreateImageHeader(cvSize(w, h), IPL_DEPTH_8U, 3);
				cvSetData(cimg, pGrayBuffer, w * 3); ////3
				cvCvtColor(cimg, pThis->m_img[i], CV_BGR2GRAY);

				pThis->m_Ct[i].Get_adaptive_parameter(pThis->m_img[i]);

				cvReleaseImageHeader(&cimg);
				cvReleaseImage(&pThis->m_img[i]);//huj fff
			}
		}
	}


	pThis->m_tipdlg.SendMessage(WM_CLOSE, 0, 0);
	pThis->m_abortflag = false;

	return 1;
}



UINT MfcThreadProc2(LPVOID lpParam)
{
	CCalibrationCtrlView * pThis = (CCalibrationCtrlView *)lpParam;

	float tmp = 64.0f / (pThis->m_endpos - pThis->m_beginpos);
	CString str;
	Sleep(512);

SLEEP:
	if (pThis->m_tipdlg.m_flag)
	{
		return 0;
	}

	if (::IsWindow(pThis->m_tipdlg.m_hWnd))
	{


		pThis->m_tipdlg.MeasureProcess.SetPos((_showpos - pThis->m_beginpos)  *tmp);     ////////////change

		//DEBUG_INFO1("show pos-----:  %d",  _showpos*tmp  )

		str.Format("当前测量帧：%d , 总共 %d 帧", _showpos, pThis->m_endpos - pThis->m_beginpos + 1);
		pThis->m_tipdlg.m_pos.SetWindowText(str);
	}

	Sleep(1024);
	goto SLEEP;

	return 1;
}


void CCalibrationCtrlView::CalcPerFrame()
{
	int num = m_Record.ResultPt.size();

	//m_BinoCam.m_vLeftImgPt = m_Record.ImgPt0;
	//m_BinoCam.m_vRightImgPt = m_Record.ImgPt1;
	//m_BinoCam.m_vIsPtCalc = m_Record.PtValid;
	//m_BinoCam.Calc();

	//mono
	for (int i = 0; i < num; i++)
	{
		if (m_Record.PtValid[i])
		{
			m_Record.ResultPt[i].x = (m_Record.ImgPt0[i].x - h_FirstImgPt[i].x)*h_scale[i];
			m_Record.ResultPt[i].y = (m_Record.ImgPt0[i].y - h_FirstImgPt[i].y)*h_scale[i]; //mono
			m_Record.ResultPt[i].z = 0;
		}
		else
		{
			m_Record.ResultPt[i].x = 0.0;
			m_Record.ResultPt[i].y = 0.0;
			m_Record.ResultPt[i].z = 0.0;
		}
	}

	//////////////恢复
	//if (m_ChangeZ)
	//{
	//	for (int i = 0; i < num; i++)
	//	{
	//		if (m_Record.PtValid[i])
	//		{
	//			m_Record.ResultPt[i].x = m_BinoCam.m_vObjPt[i].x + m_OffsetPt.x;
	//			m_Record.ResultPt[i].y = m_BinoCam.m_vObjPt[i].y + m_OffsetPt.y;
	//			m_Record.ResultPt[i].z = m_BinoCam.m_vObjPt[i].z + m_OffsetPt.z;
	//		}
	//		else
	//		{
	//			m_Record.ResultPt[i].x = 0.0;
	//			m_Record.ResultPt[i].y = 0.0;
	//			m_Record.ResultPt[i].z = 0.0;
	//		}
	//	}
	//}
	//else
	//	for (int i = 0; i < num; i++)
	//	{
	//		if (m_Record.PtValid[i])
	//		{
	//			m_Record.ResultPt[i].x = m_BinoCam.m_vObjPt[i].x + m_OffsetPt.x;
	//			m_Record.ResultPt[i].y = m_BinoCam.m_vObjPt[i].y + m_OffsetPt.y;
	//			m_Record.ResultPt[i].z = -m_BinoCam.m_vObjPt[i].z + m_OffsetPt.z;
	//		}
	//		else
	//		{
	//			m_Record.ResultPt[i].x = 0.0;
	//			m_Record.ResultPt[i].y = 0.0;
	//			m_Record.ResultPt[i].z = 0.0;
	//		}
	//	}
}


// 保存每一帧的三维坐标计算结果
// void CCalibrationCtrlView::ExcelRecordPerFrame(libxl::Sheet* sheet,int row,int framepos,int pointnum)
// {
// 	if(!sheet)
// 		return;
//
// 	sheet->writeNum(row, 0, framepos);
// 	//sheet->writeStr(row, 1, "时间");   ////frame time
//
// for (int i=0; i<pointnum ; i++)
//     {
//         if (m_Record.PtValid[i])
//             {
//                 sheet->writeNum(row,i*3+2, m_Record.ResultPt[i].x);
//                 sheet->writeNum(row,i*3+3, m_Record.ResultPt[i].y);
//                 sheet->writeNum(row,i*3+4, m_Record.ResultPt[i].z);
//             }
// //         else
// //             {
// //                 sheet->writeStr(row,i*3+2, "");
// //                 sheet->writeStr(row,i*3+3, "");
// //                 sheet->writeStr(row,i*3+4, "");  ////////////how the data should be displayed?
// //             }
//     }
//
// }


void CCalibrationCtrlView::OnEdit_beginpos()
{
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p5 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT5));
	str = p5->GetEditText();
	m_beginpos = _ttoi(str);
	if (m_beginpos < 0)
	{
		m_beginpos = 0;
	}
	str.Format("%d", m_beginpos);
	p5->SetEditText(str);
	DEBUG_INFO1("begin %d", m_beginpos);

}


void CCalibrationCtrlView::OnEdit_endpos()
{
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p6 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT6));
	str = p6->GetEditText();
	m_endpos = _ttoi(str);

	str.Format("%d", m_endpos);
	if (m_endpos < 0)
	{
		str = "";
	}

	p6->SetEditText(str);
	DEBUG_INFO1("end:  %d", m_endpos);
}


void CCalibrationCtrlView::OnEdit_offset()
{

	UpdateData(TRUE);
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p7 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT7));
	str = p7->GetEditText();
	m_offset = _ttoi(str);
	//     if (m_offset<0)
	//         {
	//             m_offset=0;
	//         }
	str.Format("%d", m_offset);
	p7->SetEditText(str);
	DEBUG_INFO1("offset %d", m_offset);
	UpdateData(FALSE);
}


void CCalibrationCtrlView::OnDefaultSetting()
{
	UpdateData(TRUE);
	m_UseDefaultSetting = !m_UseDefaultSetting;
	UpdateData(FALSE);
}


void CCalibrationCtrlView::OnUpdateDefaultsetting(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_UseDefaultSetting);
}


void CCalibrationCtrlView::OnUpdateSaveclbt(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_SaveIsOK);
}


void CCalibrationCtrlView::OnUpdateEdit5(CCmdUI *pCmdUI)
{
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p5 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT5));
	if (m_UseDefaultSetting == 1)
	{
		str.Format("%d", 0);
		p5->SetEditText(str);
	}
}


void CCalibrationCtrlView::OnUpdateEdit6(CCmdUI *pCmdUI)
{
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p6 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT6));
	if (m_UseDefaultSetting == 1)  //
	{
		if (h_end > 0)
		{
			str.Format("%d", h_end);
		}
		else
			str = "";

		p6->SetEditText(str);
	}
	//else  p6->SetEditText(str);
	// DEBUG_INFO1("end   %d", endpos )
}


void CCalibrationCtrlView::OnUpdateEdit7(CCmdUI *pCmdUI)
{
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p6 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT7));
	if (m_UseDefaultSetting == 1)
	{
		str.Format("%d", 0);
		p6->SetEditText(str);
	}
}


void CCalibrationCtrlView::SaveMeasureRecord()
{

	//CTime nowtime = CTime::GetCurrentTime();
	//m_RecordFileName=nowtime.Format( "%Y%m%d%H%M%S" )+".xlsx";
	////////////////////camera shoot time
	CString shoottime;
	SYSTEMTIME st;
	st.wYear = m_MoreImageCtrl.m_MapCtrl[0].m_pHead->pStart.wYear;
	st.wMonth = m_MoreImageCtrl.m_MapCtrl[0].m_pHead->pStart.wMonth;
	st.wDay = m_MoreImageCtrl.m_MapCtrl[0].m_pHead->pStart.wDay;
	st.wMinute = m_MoreImageCtrl.m_MapCtrl[0].m_pHead->pStart.wMinute;
	st.wHour = m_MoreImageCtrl.m_MapCtrl[0].m_pHead->pStart.wHour;
	st.wSecond = m_MoreImageCtrl.m_MapCtrl[0].m_pHead->pStart.wSecond;
	shoottime.Format("%0.2d%0.2d%0.2d_%0.2d%0.2d%0.2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//m_RecordFileName=shoottime ;

	//////////////////////
	long endtmp = m_endpos;   //for change record file

	m_RecordFileName.Format("_%d-%d", m_beginpos, endtmp);
	m_RecordFileName = shoottime + m_RecordFileName + ".csv";

	AfxMessageBox("测量完成，请选择结果文件保存路径");





	/////////////////////////
	CString filt = "CSV Files (*.csv)|*.csv||";
	CFileDialog  dlgFile(FALSE, "*.csv", m_RecordFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filt, NULL);
	dlgFile.m_ofn.lpstrTitle = "请选择坐标测量结果文件的保存路径";
	//dlgFile.m_ofn.lpstrInitialDir = AfxGetApp()->GetProfileString("Windows", "Lasted Task Dir");// 最近任务文件目录
	dlgFile.m_ofn.lpstrInitialDir = m_hpath.ResultFilesPath;

	CString str;
	CString str1 = m_hpath.BackupRecordPath + "AutoSaved_" + m_RecordFileName;
	int tmp = 0;

	if (dlgFile.DoModal() == IDCANCEL)
	{
		str = str1;
		tmp = 1;
	}
	else
	{
		str = dlgFile.GetPathName();
	}
	//////////////////




LOOP:
	csv::ofstream os(str, std::ios_base::out);
	os.set_delimiter(',');
	if (os.is_open())
	{
		os << "时间:" << LPCSTR(shoottime) << NEWLINE;
		os << "计算开始帧:" << m_beginpos << "计算结束帧:" << endtmp << NEWLINE; //mono  "帧位置偏移:" << m_offset <<
		os << "帧率:" << m_MoreImageCtrl.m_MapCtrl[0].m_pHead->nFrame << "单位（mm）" << NEWLINE;
		os << "跟踪计算点数量:" << m_Record.Index.size() << "相机数量:" << "1" << NEWLINE;
		os << "标志点编号:";
		for (int i = 0; i < m_Record.Index.size(); i++)
		{
			os << m_Record.Index[i];
		}
		os << NEWLINE;

		os << "帧位置:";
		for (int i = 0; i < m_Record.Index.size(); i++)
		{
			CString str1, str2, str3;
			str1.Format("%d_X", m_Record.Index[i]);  //////////huangjin
			str2.Format("%d_Y", m_Record.Index[i]);   ////////reback offset
			str3.Format("%d_Z", m_Record.Index[i]);  
			os << str1 << str2 ; //<< str3   //mono
		}
		os << NEWLINE;

		for (long i = m_beginpos; i <= endtmp; i++)
		{
			os << i;
			for (int j = 0; j < m_Record.Index.size(); j++)
			{
				os << m_AllRecord[i - m_beginpos][j].x 
					<< m_AllRecord[i - m_beginpos][j].y 
					;    //mono   << m_AllRecord[i - m_beginpos][j].z
			}
			os << NEWLINE;
		}
	}
	else
	{
		if (tmp == 0)
		{
			AfxMessageBox("无法保存结果文件！");
		}
	}

	os.flush();
	os.close();

	tmp++;

	if (tmp < 2)
	{
		str = str1;

		//goto LOOP;
	}

	DEBUG_INFO1("backup recordfile   %s", m_hpath.BackupRecordPath);


	return;
}



void CCalibrationCtrlView::OnOpenXYZData(void)
{

	UpdateData(TRUE);

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("测量结果数据文件 (*.csv)|*.csv|All Files (*.*)|*.*||"), NULL);
	dlgFile.m_ofn.lpstrTitle = "请选择坐标测量结果文件";
	//dlgFile.m_ofn.lpstrInitialDir = AfxGetApp()->GetProfileString("Windows", "Lasted Task Dir");// 最近任务文件目录
	dlgFile.m_ofn.lpstrInitialDir = m_hpath.ResultFilesPath;

	DEBUG_INFO1("xyz data  :   %s", m_DataPath);
	DEBUG_INFO1("record path  open  %s", m_hpath.ResultFilesPath);

	if (dlgFile.DoModal())
	{
		m_DataPath = dlgFile.GetPathName();
	}
	else
		return;
	
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	pt_index.clear();
	m_graphdata.clear();
	vpos.clear();

	/////CMainFrame *p2=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonComboBox* p_pt = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, p2->m_wndRibbonBar.FindByID(ID_COMBO_PT));
	CMFCRibbonComboBox* p_displaywhat = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, p2->m_wndRibbonBar.FindByID(ID_COMBO_DISPLAYWHAT));
	
	if (!m_csv.LoadCSV(m_DataPath))
	{
		return;
	}
	std::string str;
	std::string* pString;
	int num, tmpint;
	CString cstrTemp;

	//     str=  *m_csv.GetString(1,1) +  *m_csv.GetString(1,2) + "\n"          //time
	//           + *m_csv.GetString(2,1) +*m_csv.GetString(2,2) + *m_csv.GetString(2,2) + *m_csv.GetString(2,2) + "\n"  //frame
	//           + *m_csv.GetString(3,1) +*m_csv.GetString(3,2) + "\n"                  //frame
	//           + *m_csv.GetString(4,1) +*m_csv.GetString(4,2) + "\n"  ; //num of points

	m_csv.GetInt(4, 2, num);
	m_csv.GetInt(2, 2, m_begin);
	m_csv.GetInt(2, 4, m_end);

	int tmp;
	m_csv.GetInt(4, 4, tmp);
	if (tmp!=1)
	{
		AfxMessageBox("打开文件不是单相机坐标文件！");
		return;
	}

	
	DEBUG_INFO3("num,begin,end, %d,%d, %d", num, m_begin, m_end);

	p_displaywhat->RemoveAllItems();
	p_pt->RemoveAllItems();

	for (int i = 0; i < num; i++)
	{
		m_csv.GetInt(5, 2 + i, tmpint);
		pt_index.push_back(tmpint);

		cstrTemp.Format("%d", tmpint);
		p_pt->AddItem(cstrTemp);        //cstrTemp
		//DEBUG_INFO1("index  %d"  ,tmpint);
	}
	p_pt->SelectItem(0);

	if (p_displaywhat->GetCount() < 2)
	{
		CString  strshow[6] = { "位移_X ", "位移_Y ",// "Z方向位移 ",  ///////////"位移矢量和 ",
			"速度_X ", "速度_Y ", //"Z方向速度 ",///////////"速度矢量和 ",
			"加速度_X ", "加速度_Y "   //, "Z方向加速度 "//////////,"加速度矢量和 "
		};

		for (int i = 0; i < 6; i++)      //mono 66 --9       //////////////12----9
		{
			p_displaywhat->AddItem(strshow[i]);
		}
		p_displaywhat->SelectItem(0);
	}

	this->OnDataProcessing();

}



void CCalibrationCtrlView::OnDataProcessing()
{
	if (!m_csv.LoadCSV(m_DataPath))
	{
		AfxMessageBox("请先打开数据文件！");
		return;
	}

	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonComboBox* p_pt = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, p2->m_wndRibbonBar.FindByID(ID_COMBO_PT));
	CMFCRibbonComboBox* p_displaywhat = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, p2->m_wndRibbonBar.FindByID(ID_COMBO_DISPLAYWHAT));

	m_ordernum = p_pt->GetCurSel();
	m_ShowItem = p_displaywhat->GetCurSel();

	DEBUG_INFO1("%s", m_DataPath);
	DEBUG_INFO2("display ordernum:%d  m_showItem:%d", m_ordernum, m_ShowItem);//////////
	//////////////////



	if (m_ordernum < 0 || m_ordernum >= pt_index.size() || m_end < m_begin)
	{
		AfxMessageBox("无法打开坐标数据文件，请确定是否打开正确文件或者原始坐标文件被修改！");
		return;
	}

	m_csv.GetInt(3, 2, m_DataProcessing.m_Fps);  //fps
	m_DataProcessing.m_XYZ.resize(m_end - m_begin + 1);
	vpos.resize(m_end - m_begin + 1);

	switch ((m_ShowItem + 2) % 2)    /////hahahahah, trick
	{
	case 0:
		for (int i = 0; i < m_end - m_begin; i++)
		{
			m_DataProcessing.m_XYZ[i].x = atof(m_csv.GetString(7 + i, m_ordernum * 2 + 2)->c_str());
			vpos[i] = m_begin + i;
		}
		break;
	case 1:
		for (int i = 0; i < m_end - m_begin; i++)
		{
			m_DataProcessing.m_XYZ[i].y = atof(m_csv.GetString(7 + i, m_ordernum * 2 + 3)->c_str());
			vpos[i] = m_begin + i;
		}
		break;
	//case 2:
	//	for (int i = 0; i < m_end - m_begin; i++)
	//	{
	//		m_DataProcessing.m_XYZ[i].z = atof(m_csv.GetString(7 + i, m_ordernum * 3 + 4)->c_str());
	//		vpos[i] = m_begin + i;
	//	}
	//	break;
	}


	CMFCRibbonEdit* p5 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT_WINDOW));
	m_window = _ttoi(p5->GetEditText());

	m_window = m_window > 0 ? m_window : 0;
	m_DataProcessing.Smooth(m_window);

	m_graphdata.resize(m_end - m_begin + 1);

	////////AfxMessageBox(p_displaywhat->GetItem(m_ShowItem));
	m_table_title.Format("点-%s %s(帧率-%d)", p_pt->GetItem(m_ordernum), p_displaywhat->GetItem(m_ShowItem), m_DataProcessing.m_Fps);

	switch (m_ShowItem /2)
	{
	case 0:
		m_Y_title = "位移(mm)";
		break;
	case 1:
		m_Y_title = "速度(m/s)";
		break;
	case 2:
		m_Y_title = "加速度(m/s^2)";
		break;
	}

	switch (m_ShowItem)
	{
	case 0:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothDisplacement[i].x;
		break;
	case 1:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothDisplacement[i].y;
		break;
	/*case 2:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothDisplacement[i].z;
		break;*/
		//         case 3:
		//             m_graphdata =   m_DataProcessing.m_DisplacementSum;//             break;
	case 2:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothSpeed[i].x;
		break;
	case 3:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothSpeed[i].y;
		break;
	//case 5:
	//	for (int i = 0; i < m_end - m_begin + 1; i++)
	//		m_graphdata[i] = m_DataProcessing.m_SmoothSpeed[i].z;
	//	break;
		//         case 7:
		//             m_graphdata =   m_DataProcessing.m_SmoothSpeedSum;
		//             break;///////////////////////////////
	case 4:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothAcceleration[i].x;
		break;
	case 5:
		for (int i = 0; i < m_end - m_begin + 1; i++)
			m_graphdata[i] = m_DataProcessing.m_SmoothAcceleration[i].y;
		break;
	//case 8:
	//	for (int i = 0; i < m_end - m_begin + 1; i++)
	//		m_graphdata[i] = m_DataProcessing.m_SmoothAcceleration[i].z;
	//	break;
		//         case 11://      m_graphdata =   m_DataProcessing.m_SmoothAccelerationSum;//             break;
	}


	int length = m_end - m_begin - m_window / 2 - 3;
	double * XValues = &vpos[m_window / 2 + 2];
	double *YValues = &m_graphdata[m_window / 2 + 2];

	DEBUG_INFO3("begin,end,window,  %d,%d,%d", m_begin, m_end, m_window);

	DisplayGraph(length, XValues, YValues, m_table_title, m_Y_title);

}


void CCalibrationCtrlView::DisplayGraph(int length, double* XValues, double* YValues, CString table_title, CString Y_title)
{
	if (length < 5)
	{
		AfxMessageBox(" 结果坐标数据至少8帧，或者平滑窗口太大！");
		return;
	}

	CRect nrect;
	HDC pDC = p_MyGraphView->GetShowWndInfo(nrect);

	CChartStandardAxis* pLeftAxis;
	CChartStandardAxis*  pBottomAxis;

	if (m_ChartCtrl != 0)
	{
		m_ChartCtrl->RemoveAllSeries();
		m_ChartCtrl->DestroyWindow();
		delete m_ChartCtrl;
		m_ChartCtrl = NULL;
	}

	m_ChartCtrl = new CChartCtrl();

	m_ChartCtrl->Create(p_MyGraphView, nrect, IDC_CHARTCTRL);
	pBottomAxis = m_ChartCtrl->CreateStandardAxis(CChartCtrl::BottomAxis);
	pLeftAxis = m_ChartCtrl->CreateStandardAxis(CChartCtrl::LeftAxis);

	m_ChartCtrl->SetBackColor(RGB(33,  32, 30));
	m_ChartCtrl->SetZoomRectColor(RGB(202, 222, 90));

	m_ChartCtrl->RefreshCtrl();

	CString strFileName = m_DataPath + "          ";
	strFileName = strFileName.Mid(strFileName.ReverseFind('\\') + 1);

	TChartString strTitle = strFileName + table_title;    //图表标题
	CChartTitle *pTitle;
	pTitle = m_ChartCtrl->GetTitle();
	if (pTitle->GetStringCount() == 0)
	{
		pTitle->AddString(strTitle);
	}

	pTitle->SetColor(RGB(110, 220, 90));

	//int _min = *(std::min_element(data.begin(),data.end())  );
	//int _max = *( std::max_element(data.begin(),data.end()) );
	pBottomAxis->SetAutomatic(1);
	pLeftAxis->SetAutomatic(1);
 
	// pBottomAxis->SetMinMax( pos[0]-1, pos[length-1] +1 );
	// pLeftAxis->SetMinMax(_min -0.05*(_max-_min),_max+0.05*(_max-_min));
	//pBottomAxis->SetMinMax(  -5, 66  );
	//pLeftAxis->SetMinMax(_min,_max);

	pBottomAxis->GetLabel()->SetText("图像数据流帧编号");
	pLeftAxis->GetLabel()->SetText(LPCSTR(Y_title));

	pBottomAxis->GetLabel()->SetColor(RGB(110, 220, 90));
	pLeftAxis->GetLabel()->SetColor(RGB(110, 220, 90));
	pLeftAxis->SetAxisColor(RGB(130, 130, 90));
	pBottomAxis->SetAxisColor(RGB(110, 130, 90));
	pLeftAxis->SetTextColor(RGB(180, 180, 190));
	pBottomAxis->SetTextColor(RGB(180, 180, 190));


	CChartLineSerie* pLineSerie;
	// pLineSerie = m_ChartCtrl->CreateLineSerie();
	pLineSerie = m_ChartCtrl->CreateLineSerie();
	pLineSerie->SetPoints(XValues, YValues, length - 1);

	pLineSerie->SetColor(RGB(30, 180, 0));
	pLineSerie->SetWidth(2);
	m_ChartCtrl->ShowWindow(SW_SHOWNORMAL);



}


//void CCalibrationCtrlView::OnEdit2()
//{
//	// TODO: 在此添加命令处理程序代码
//	UpdateData(TRUE);
//
//	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
//	CMFCRibbonEdit* p = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT2));
//	m_DataPath = p->GetEditText();
//	p->SetEditText(m_DataPath);
//	UpdateData(FALSE);
//
//}


//void CCalibrationCtrlView::OnUpdateEdit2(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//	// TODO: 在此添加命令处理程序代码
//
//
//
//	//UpdateData(FALSE);
//
//
//	//AfxMessageBox(m_DataPath);
//
//}


void CCalibrationCtrlView::OnComboPt()
{
	// TODO: 在此添加命令处理程序代码
}


void CCalibrationCtrlView::OnComboDisplaywhat()
{
	// TODO: 在此添加命令处理程序代码
}


void CCalibrationCtrlView::OnEditWindow()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p5 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT_WINDOW));
	str = p5->GetEditText();

	m_window = _ttoi(str);
	if (m_window <= 0)
	{
		m_window = 0;
	}

	if (m_window > 100)
	{
		m_window = 100;
	}

	str.Format("%d", m_window);
	p5->SetEditText(str);


}


void CCalibrationCtrlView::OnXYZTransform()
{
	//AfxMessageBox("坐标系变换过程较为复杂，使用前务必参阅说明书！\n\n先输入OXYT新坐标系基准文件，再输入老坐标数据文件，最后得到新坐标系数据文件");

	//XYZTransform xyz;
	//xyz.StartFromFile();

}


void  CCalibrationCtrlView::OnSaveGraph()
{

	////////////////////
	OnDataProcessing();

	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonComboBox* p_displaywhat = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, p2->m_wndRibbonBar.FindByID(ID_COMBO_DISPLAYWHAT));

	if (p_displaywhat->GetCount() < 2)
	{
		return;
	}

	//char* qpath;
	char drive[16];
	char dir[1024];
	char filename[1024];
	char ext[64];
	char qpath[1024];
	strcpy_s(qpath, m_DataPath);
	_splitpath(qpath, drive, dir, filename, ext);

	CString cstr;
	cstr.Format("%s", filename);
	cstr = cstr + "帧 " + m_table_title;

	CString filt = "CSV Files (*.csv)|*.csv||";
	CFileDialog  dlgFile(FALSE, "*.csv", cstr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filt, NULL);
	dlgFile.m_ofn.lpstrTitle = "请选择图表保存路径";
	//dlgFile.m_ofn.lpstrInitialDir = AfxGetApp()->GetProfileString("Windows", "Lasted Task Dir");// 最近任务文件目录

	dlgFile.m_ofn.lpstrInitialDir = m_hpath.ResultFilesPath;

	if (dlgFile.DoModal() == IDCANCEL)
		return;

	//////////////////////////

	csv::ofstream os(dlgFile.GetPathName(), std::ios_base::out);
	os.set_delimiter(',');
	if (os.is_open())
	{
		os << "原始数据文件:" << m_DataPath << NEWLINE;
		os << "帧序号:" << m_table_title << NEWLINE;

		for (long i = m_window / 2 + 2; i < vpos.size() - 4 - m_window / 2; i++)
		{
			os << i + m_begin << m_graphdata[i] << NEWLINE;////huangjin611 export
		}
	}
	else
	{
		AfxMessageBox("无法保存！");
	}
	os.flush();
	os.close();


}


// show  the coded mark
void CCalibrationCtrlView::OnShowMark(void)
{
	//OnOpendatfile();
	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;
	if (!pmain->m_MsmtIsOpen)
	{
		//AfxMessageBox("请先打开图像！");
		return;
	}

	int len;
	PBYTE pGrayBuffer;
	HDC pDC;
	CRect nrect;
	CImgViewForm *pview;

	long curpos = 0, ncount = 0;
	long showpos[2];

	//#pragma omp parallel for num_threads(2)
	for (int i = 0; i < 2; i++)
	{
		if (m_MoreImageCtrl.m_W[i] == 0)
		{
			AfxMessageBox("图像文件打开失败！");
			return;
		}
		curpos = 0, ncount = 0;
		curpos = m_MoreImageCtrl.GetCurImagePos(i, ncount);

		int w = m_MoreImageCtrl.m_W[i];
		int h = m_MoreImageCtrl.m_H[i];
		m_MoreImageCtrl.m_ClibW[i] = w;
		m_MoreImageCtrl.m_ClibH[i] = h;
		m_MoreImageCtrl.m_ClibDept[i] = 8;
		pGrayBuffer = m_MoreImageCtrl.m_pShowData[i] = m_MoreImageCtrl.m_MapCtrl[i].GetMapData(curpos, 1, len);

		DEBUG_INFO1("len %d", len);

		m_img[i] = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

		IplImage* cimg = cvCreateImageHeader(cvSize(w, h), IPL_DEPTH_8U, 3);
		cvSetData(cimg, pGrayBuffer, w * 3);
		cvCvtColor(cimg, m_img[i], CV_BGR2GRAY);
		cvReleaseImageHeader(&cimg);

		////////huj   fff

		//int tmp;
		//int area = w*h;
		//for (int j = 0; j < area; j++)
		//{
		//	tmp = pGrayBuffer[3*j]*15+ pGrayBuffer[3*j+1]*75+ pGrayBuffer[3*j+2]*38;
		//	tmp = tmp >>7;
		//	//m_img[i]->imageData[j]= tmp;
		//}



		cvFlip(m_img[i]);

		m_Ct[i].Initial(w, h);
		m_Ct[i].Get_adaptive_parameter(m_img[i]);
		m_Ct[i].Decoding(m_img[i]);

		m_Ct[i].Draw_mark();


		pview = pmain->m_ShowView[i];
		m_MoreImageCtrl.m_CLBTImgPreview[i] = &(pview->m_ImgPreview);

		pmain->m_ShowView[i]->m_nCampos = i;
		pDC = pview->GetShowWndInfo(nrect);

		pview->SetPointHwnd(this->GetSafeHwnd(), WM_IMAGEPOINT_MSG);
		//设置HDC
		m_MoreImageCtrl.SetClibPreview(i, pDC, nrect);
		showpos[i] = curpos;
	}

	m_MoreImageCtrl.ShowClibImage(1, m_Ct[0], m_Ct[1]);

	cvReleaseImage(&m_img[0]);
	cvReleaseImage(&m_img[1]);

	////////////////

	//     CString result[2] ,temp;
	//
	//     for (int i=0; i< 2; i++)
	//         {
	//             result[i].Format("相机%d_第%d帧 \r\n     编码点图像坐标列表：\r\n",i+1, showpos[i]  );
	//             for (int k=m_Ct[i].m_targetNum-1; k>=0  ; k--)
	//                 {
	//                     temp.Format("     %d:  %.2f     %.2f\r\n",m_Ct[i].m_target[k].code, m_Ct[i].m_target[k].x, m_Ct[i].m_target[k].y );
	//                     result[i]=result[i]+temp;
	//                 }
	//             result[i]=result[i]+"-----------------------------------------\r\n\r\n";
	//         }
	//
	//     m_Edit5.SetWindowText(result[0]+result[1] );

	m_MarkOfViewForm[0] = true;
	m_MarkOfViewForm[1] = true;

}



////////mark  is not display
void CCalibrationCtrlView::MarkInvisable(int i)
{
	if (!m_MarkOfViewForm[i])
	{
		return;
	}

	CCalibrationCtrlApp* theApp = (CCalibrationCtrlApp*)AfxGetApp();
	CMainFrame* pmain = (CMainFrame*)theApp->m_pMainWnd;

	CImgViewForm *	pview = pmain->m_ShowView[i];
	m_MoreImageCtrl.m_DatImgPreview[i] = &(pview->m_ImgPreview);
	CRect nrect;
	pview->SetPointHwnd(this->GetSafeHwnd(), WM_IMAGEPOINT_MSG);
	HDC 	pDC = pmain->m_ShowView[i]->GetShowWndInfo(nrect);
	m_MoreImageCtrl.SetDatPreview(i, pDC, nrect);

	m_MarkOfViewForm[i] = false;

}


bool CCalibrationCtrlView::UnicodeFile2Ansi(std::string file, std::string savefile)
{

	wstring  destWstring;
	destWstring.clear();
	//setlocale(LC_ALL,"Chinese-simplified");//设置中文环境
	// locale &loc=locale::global(locale(locale(),"",LC_CTYPE));

	std::ifstream filestream(file.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	filestream.seekg(0, std::ios::end);
	size_t size = (size_t)filestream.tellg();
	filestream.seekg(0, ios::beg);
	char* buffer = new char[size + 1];
	memset(buffer, 0, sizeof(char)*(size + 1));
	filestream.read(buffer, size);
	destWstring = (wchar_t*)buffer;
	destWstring.erase(size / 2);//删除末尾可能会出现的乱码 /2 是为了unicode 之后 长度只有一半
	filestream.close();
	delete[] buffer;
	//setlocale(LC_ALL,"C");//还原
	//locale::global(loc);

	std::string str(destWstring.length(), ' ');
	std::copy(destWstring.begin(), destWstring.end(), str.begin());
	str = str.substr(1, str.length() - 1); //Get substring.

	//接下来写入文本
	//写文本文件，ANSI文件没有BOM
	CFile cFile;

	//LPCTSTR qfile="";

	bool flag = cFile.Open(savefile.c_str(), CFile::modeWrite | CFile::modeCreate);
	//文件开头
	cFile.SeekToBegin();
	//写入内容
	cFile.Write(str.c_str(), str.length());
	cFile.Flush();
	cFile.Close();

	if (!flag)
	{
		return false;
	}
	else return true;

}

//
// void CCalibrationCtrlView::OnButton4()
// {
//     // TODO: 在此添加命令处理程序代码
//
//     CCamera debugcam;
//
//     double* p;
//
//     double m[12]= {3776.439018,
//                    8835.711767,
//                    1769.203496,
//                    10013241.05,
//                    1127.187054,
//                    1772.72198,
//                    -9452.325722,
//                    13097477.6,
//                    0.981925,
//                    -0.171745,
//                    -0.07954,
//                    1420.998693
//                   };
//
//     for (int i=0; i<12 ; i++)
//         {
//             debugcam.m_fProjMat[i]= m[i];
//         }
//
//
//     debugcam.ProjArray2ProjMat();
//     ////////////////
//
//     double u0 = V_InnerProduct(debugcam.m_ProjMat.YX[0], debugcam.m_ProjMat.YX[2], 3);	// u0 = M1'M3
//     double v0 = V_InnerProduct(debugcam.m_ProjMat.YX[1], debugcam.m_ProjMat.YX[2], 3);	// v0 = M2'M3
//
//     double	tempVec[3];
//     V_CrossProduct(debugcam.m_ProjMat.YX[0], debugcam.m_ProjMat.YX[2], tempVec);			// M1'×M3
//     double fu = V_Module(tempVec);										// fu = |M1'×M3|
//     V_CrossProduct(debugcam.m_ProjMat.YX[1], debugcam.m_ProjMat.YX[2], tempVec);			// M2'×M3
//     double fv = V_Module(tempVec);										// fv = |M2'×M3|
//
//     double RotMatrix[9];
//     RotMatrix[0] = (debugcam.m_ProjMat.YX[0][0]-u0*debugcam.m_ProjMat.YX[2][0]) / fu;		// R1 = (M1 - u0*M3)/fu
//     RotMatrix[1] = (debugcam.m_ProjMat.YX[0][1]-u0*debugcam.m_ProjMat.YX[2][1]) / fu;
//     RotMatrix[2] = (debugcam.m_ProjMat.YX[0][2]-u0*debugcam.m_ProjMat.YX[2][2]) / fu;
//
//     RotMatrix[3] = (debugcam.m_ProjMat.YX[1][0]-v0*debugcam.m_ProjMat.YX[2][0]) / fv;		// R2 = (M2 - v0*M3)/fv
//     RotMatrix[4] = (debugcam.m_ProjMat.YX[1][1]-v0*debugcam.m_ProjMat.YX[2][1]) / fv;
//     RotMatrix[5] = (debugcam.m_ProjMat.YX[1][2]-v0*debugcam.m_ProjMat.YX[2][2]) / fv;
//
//     RotMatrix[6] =  debugcam.m_ProjMat.YX[2][0];									// R3 = M3
//     RotMatrix[7] =  debugcam.m_ProjMat.YX[2][1];
//     RotMatrix[8] =  debugcam.m_ProjMat.YX[2][2];
//
//     DEBUG_INFO1("\n", 0);
//     for (int i=0; i<9 ; i++)
//         {
//
//             DEBUG_INFO1("rot %lf ", RotMatrix[i]      );
//         }
//
//     ////////////////
//
//
//
//
//
//
//
//
//     ///////////////
//
//     debugcam.ProjMat2Para();
//
//     DEBUG_INFO3("angle  %lf   %lf    %lf " , debugcam.m_RTPara.m_ELAng.AngX ,
//                 debugcam.m_RTPara.m_ELAng.AngY,
//                 debugcam.m_RTPara.m_ELAng.AngZ);
//
//
//     debugcam.Para2ProjMat();
//
//     //////////////
//
//
//
//     ////////////////
//
//     u0 = V_InnerProduct(debugcam.m_ProjMat.YX[0], debugcam.m_ProjMat.YX[2], 3);	// u0 = M1'M3
//     v0 = V_InnerProduct(debugcam.m_ProjMat.YX[1], debugcam.m_ProjMat.YX[2], 3);	// v0 = M2'M3
//
//     tempVec[3];
//     V_CrossProduct(debugcam.m_ProjMat.YX[0], debugcam.m_ProjMat.YX[2], tempVec);			// M1'×M3
//     fu = V_Module(tempVec);										// fu = |M1'×M3|
//     V_CrossProduct(debugcam.m_ProjMat.YX[1], debugcam.m_ProjMat.YX[2], tempVec);			// M2'×M3
//     fv = V_Module(tempVec);										// fv = |M2'×M3|
//
//     RotMatrix[9];
//     RotMatrix[0] = (debugcam.m_ProjMat.YX[0][0]-u0*debugcam.m_ProjMat.YX[2][0]) / fu;		// R1 = (M1 - u0*M3)/fu
//     RotMatrix[1] = (debugcam.m_ProjMat.YX[0][1]-u0*debugcam.m_ProjMat.YX[2][1]) / fu;
//     RotMatrix[2] = (debugcam.m_ProjMat.YX[0][2]-u0*debugcam.m_ProjMat.YX[2][2]) / fu;
//
//     RotMatrix[3] = (debugcam.m_ProjMat.YX[1][0]-v0*debugcam.m_ProjMat.YX[2][0]) / fv;		// R2 = (M2 - v0*M3)/fv
//     RotMatrix[4] = (debugcam.m_ProjMat.YX[1][1]-v0*debugcam.m_ProjMat.YX[2][1]) / fv;
//     RotMatrix[5] = (debugcam.m_ProjMat.YX[1][2]-v0*debugcam.m_ProjMat.YX[2][2]) / fv;
//
//     RotMatrix[6] =  debugcam.m_ProjMat.YX[2][0];									// R3 = M3
//     RotMatrix[7] =  debugcam.m_ProjMat.YX[2][1];
//     RotMatrix[8] =  debugcam.m_ProjMat.YX[2][2];
//
//     DEBUG_INFO1("\n", 0);
//     for (int i=0; i<9 ; i++)
//         {
//
//             DEBUG_INFO1("rot %lf ", RotMatrix[i]      );
//         }
//
//     ////////////////
//
//
//
//
//
//
//
//
// // 	debugcam.ProjMat2Para();
// // 	DEBUG_INFO3("angle  %lf   %lf    %lf " , debugcam.m_RTPara.m_ELAng.AngX ,
// // 		debugcam.m_RTPara.m_ELAng.AngY, debugcam.m_RTPara.m_ELAng.AngZ);
// //
// //     debugcam.ProjMat2ProjArray();
// //
// //     for (int i=0; i<12 ; i++)
// //         {
// //             DEBUG_INFO2("  m[%d]    %lf",i,debugcam.m_fProjMat[i] );
// //         }
// //
// // 	debugcam.Para2ProjMat();
// //
// // 		debugcam.ProjMat2Para();
// // 		for (int i=0; i<12 ; i++)
// // 		{
// // 			DEBUG_INFO2("  m[%d]    %lf",i,debugcam.m_fProjMat[i] );
// // 		}
// //
// // 		DEBUG_INFO3("angle  %lf   %lf    %lf " , debugcam.m_RTPara.m_ELAng.AngX ,
// // 			debugcam.m_RTPara.m_ELAng.AngY, debugcam.m_RTPara.m_ELAng.AngZ);
// //
// //
// //
//
//
//
// }





BOOL CCalibrationCtrlView::CheckRightHanded(void)
{
	//CCamera cam;
	//int num = m_Cam[0].m_vObjPt.size();
	//cam.SetPointNum(num);
	//double m12[12], sum;

	//for (int i = 0; i < num; i++)
	//{
	//	cam.m_ImgPt.V[i].x = m_Cam[0].m_vImgPt.at(i).x;
	//	cam.m_ImgPt.V[i].y = m_Cam[0].m_vImgPt.at(i).y;

	//	cam.m_ObjPt.V[i].x = m_Cam[0].m_vObjPt.at(i).x;
	//	cam.m_ObjPt.V[i].y = m_Cam[0].m_vObjPt.at(i).y;
	//	cam.m_ObjPt.V[i].z = m_Cam[0].m_vObjPt.at(i).z;
	//}

	//cam.ObjPtImgPt2MatProject();

	//m12[9] = cam.m_fProjMat[9];
	//m12[10] = cam.m_fProjMat[10];

	//cam.ProjMat2Para();
	//cam.Para2ProjMat();

	//if (m12[9] * cam.m_fProjMat[9]>0 && m12[10] * cam.m_fProjMat[10] > 0)
	//{
	//	return TRUE;
	//}

	///////////////////////

	//for (int i = 0; i < num; i++)
	//{
	//	cam.m_ObjPt.V[i].z = -cam.m_ObjPt.V[i].z;
	//}

	//cam.ObjPtImgPt2MatProject();

	//m12[9] = cam.m_fProjMat[9];
	//m12[10] = cam.m_fProjMat[10];

	//cam.ProjMat2Para();
	//cam.Para2ProjMat();

	//if (m12[9] * cam.m_fProjMat[9]>0 && m12[10] * cam.m_fProjMat[10] > 0)
	//{
	//	return FALSE;
	//}

	////////////////
	return -1;

}


void CCalibrationCtrlView::GetH_Path(void)
{
	//CIni  h_path;
	//h_path.Read("H_path.ini");

	//h_path.GetValue("标定", "图像1", m_hpath.Cam1Path);
	//h_path.GetValue("标定", "图像2", m_hpath.Cam2Path);
	//h_path.GetValue("标定", "三维坐标", m_hpath._XYZ);
	//h_path.GetValue("标定", "标定文件", m_hpath.CalibResultPath);

	//h_path.GetValue("测量", "数据1", m_hpath.Dat1Path);
	//h_path.GetValue("测量", "数据2", m_hpath.Dat2Path);         //huangjin
	//h_path.GetValue("测量", "测量结果", m_hpath.ResultFilesPath);         //huangjin
	//h_path.GetValue("测量", "备份结果", m_hpath.BackupRecordPath);

	//if (!PathIsDirectory(m_hpath.CalibResultPath))
	//{
	//	CreateDirectory(m_hpath.CalibResultPath, NULL);
	//}

	//if (!PathIsDirectory(m_hpath.ResultFilesPath))
	//{
	//	CreateDirectory(m_hpath.ResultFilesPath, NULL);
	//}

	//if (!PathIsDirectory(m_hpath.BackupRecordPath))
	//{
	//	CreateDirectory(m_hpath.BackupRecordPath, NULL);
	//}



	DEBUG_INFO1("path  %s", m_hpath.Cam1Path);
}





void CCalibrationCtrlView::OnEditDia()
{
	// TODO:  在此添加命令处理程序代码

	CString str;
	CMainFrame *p2 = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* p5 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, p2->m_wndRibbonBar.FindByID(ID_EDIT2));

	p5->SetTextAlign(ES_LEFT);



	str = p5->GetEditText();
	h_Diameter = atof(str);
	if (h_Diameter < 0)
	{
		h_Diameter = 0;
	}
	str.Format("%.1f", h_Diameter);

	p5->SetTextAlign(ES_LEFT);
	p5->SetEditText(str);


	//AfxMessageBox(str);
}


