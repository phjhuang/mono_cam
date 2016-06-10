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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "StdAfx.h"
#include "CalibrationCtrl.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
    ON_REGISTERED_MESSAGE(AFX_WM_ON_CHANGE_RIBBON_CATEGORY, &CMainFrame::OnChangeRibbonCategory)
    ON_COMMAND(ID_OPENIMAGE, &CMainFrame::OnOpenimage)
    ON_COMMAND(IDC_CLOSECLIB, &CMainFrame::OnCloseclib)

    ON_COMMAND(ID_CLBT, &CMainFrame::OnCalibration)   //h
    ON_COMMAND(ID_SAVECLBT, &CMainFrame::OnSaveCalibrationReult)   //h
    ON_COMMAND(ID_MSMT, &CMainFrame::OnStartMeasure)   //huangjin


    ON_COMMAND(ID_OPENDATFILE, &CMainFrame::OnOpendatfile)
    ON_COMMAND(ID_CLOSEDATFILE, &CMainFrame::OnClosedatfile)
    ON_COMMAND(ID_POS1, &CMainFrame::OnPos1)
    ON_COMMAND(ID_POS2, &CMainFrame::OnPos2)
    ON_COMMAND(ID_SLIDERPOS1, &CMainFrame::OnSliderpos1)
    ON_COMMAND(ID_SLIDERPOS2, &CMainFrame::OnSliderpos2)
    ON_COMMAND(ID_FRISTFRAME1, &CMainFrame::OnFristframe1)
    ON_COMMAND(ID_UP1, &CMainFrame::OnUp1)
    ON_COMMAND(ID_DOWN1, &CMainFrame::OnDown1)
    ON_COMMAND(ID_ENDFRAM1, &CMainFrame::OnEndfram1)
    ON_COMMAND(ID_FRISTFRAME2, &CMainFrame::OnFristframe2)
    ON_COMMAND(ID_UP2, &CMainFrame::OnUp2)
    ON_COMMAND(ID_DOWN2, &CMainFrame::OnDown2)
    ON_COMMAND(ID_ENDFRAM2, &CMainFrame::OnEndfram2)
    ON_UPDATE_COMMAND_UI(ID_OPENDATFILE, &CMainFrame::OnUpdateOpendatfile)
    ON_UPDATE_COMMAND_UI(ID_CLOSEDATFILE, &CMainFrame::OnUpdateClosedatfile)
    ON_UPDATE_COMMAND_UI(ID_FRISTFRAME1, &CMainFrame::OnUpdateFristframe1)
    ON_UPDATE_COMMAND_UI(ID_UP1, &CMainFrame::OnUpdateUp1)
    ON_UPDATE_COMMAND_UI(ID_POS1, &CMainFrame::OnUpdatePos1)
    ON_UPDATE_COMMAND_UI(ID_SLIDERPOS1, &CMainFrame::OnUpdateSliderpos1)
    ON_UPDATE_COMMAND_UI(ID_DOWN1, &CMainFrame::OnUpdateDown1)
    ON_UPDATE_COMMAND_UI(ID_ENDFRAM1, &CMainFrame::OnUpdateEndfram1)
    ON_UPDATE_COMMAND_UI(ID_FRISTFRAME2, &CMainFrame::OnUpdateFristframe2)
    ON_UPDATE_COMMAND_UI(ID_UP2, &CMainFrame::OnUpdateUp2)
    ON_UPDATE_COMMAND_UI(ID_POS2, &CMainFrame::OnUpdatePos2)
    ON_UPDATE_COMMAND_UI(ID_SLIDERPOS2, &CMainFrame::OnUpdateSliderpos2)
    ON_UPDATE_COMMAND_UI(ID_DOWN2, &CMainFrame::OnUpdateDown2)
    ON_UPDATE_COMMAND_UI(ID_ENDFRAM2, &CMainFrame::OnUpdateEndfram2)
    ON_UPDATE_COMMAND_UI(ID_OPENIMAGE, &CMainFrame::OnUpdateOpenimage)
    ON_UPDATE_COMMAND_UI(IDC_CLOSECLIB, &CMainFrame::OnUpdateCloseclib)


    ON_COMMAND(ID_BUTTON_SHOW, &CMainFrame::OnDataProcessing)
    ON_COMMAND(ID_SHOWGRAPH, &CMainFrame::OnShowGraph)
    ON_COMMAND(ID_BUTTON_XYZTRANSFORM, &CMainFrame::OnXYZTransform)
    ON_COMMAND(ID_SAVEGRAPH, &CMainFrame::OnSaveGraph)
	ON_COMMAND(ID_SHOWMARK, &CMainFrame::OnShowMark)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
    // TODO: 在此添加成员初始化代码
    theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

    m_MsgHWND=NULL;
    m_MsgID=0;
    m_CtrlID=0;//键盘左右键
    m_MsmtIsOpen=FALSE;
    m_ClibIsOpen=FALSE;
    for(int i=0; i<DEVICEMAX; i++)
        {
            //m_isOpen[i]=FALSE;
            //m_isSave[i]=FALSE;
            m_ShowView[i]=NULL;
            m_Title[i]="";
            //m_IsBayer[i]=FALSE;
        }

	 

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    BOOL bNameValid;
    // 基于持久值设置视觉管理器和样式
    OnApplicationLook(theApp.m_nAppLook);

    m_wndRibbonBar.Create(this);
    m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

//////////////////huangjin状态栏   disable  statusbar状态栏
//     if (!m_wndStatusBar.Create(this))
//         {
//             TRACE0("未能创建状态栏\n");
//             return -1;      // 未能创建
//         }

    CString strTitlePane1;
    CString strTitlePane2;
    bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
    ASSERT(bNameValid);
    bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
    ASSERT(bNameValid);
    m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
    m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

      //启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_STANDARD);
    // 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

    m_EditCtrlPos1=DYNAMIC_DOWNCAST(
                       CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_POS1));

    m_EditCtrlPos2=DYNAMIC_DOWNCAST(
                       CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_POS2));

    m_SliderCtrlPos1=DYNAMIC_DOWNCAST(
                         CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDERPOS1));
    m_SliderCtrlPos1->SetRange(1,50);
    m_SliderCtrlPos1->SetPos(1);

	/*
	m_SliderCtrlPos2=DYNAMIC_DOWNCAST(
                         CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDERPOS2));
    m_SliderCtrlPos2->SetRange(1,50);
    m_SliderCtrlPos2->SetPos(1);
	 */

    return 0;
}

 

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
    CWaitCursor wait;

    theApp.m_nAppLook = id;

	theApp.m_nAppLook = ID_VIEW_APPLOOK_OFF_XP; /////huangjin
		

    switch (theApp.m_nAppLook)
        {
        case ID_VIEW_APPLOOK_WIN_2000:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
            m_wndRibbonBar.SetWindows7Look(1);
            break;

        case ID_VIEW_APPLOOK_OFF_XP:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
            m_wndRibbonBar.SetWindows7Look(FALSE);
            break;

        case ID_VIEW_APPLOOK_WIN_XP:
            CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
            m_wndRibbonBar.SetWindows7Look(FALSE);
            break;

        case ID_VIEW_APPLOOK_OFF_2003:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
            CDockingManager::SetDockingMode(DT_SMART);
            m_wndRibbonBar.SetWindows7Look(0);
            break;

        case ID_VIEW_APPLOOK_VS_2005:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
            CDockingManager::SetDockingMode(DT_SMART);
            m_wndRibbonBar.SetWindows7Look(0);
            break;

        case ID_VIEW_APPLOOK_VS_2008:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
            CDockingManager::SetDockingMode(DT_SMART);
            m_wndRibbonBar.SetWindows7Look(1);//1
            break;

        case ID_VIEW_APPLOOK_WINDOWS_7:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
            CDockingManager::SetDockingMode(DT_SMART);
            m_wndRibbonBar.SetWindows7Look(1);//0
            break;

        default:
            switch (theApp.m_nAppLook)
                {
                case ID_VIEW_APPLOOK_OFF_2007_BLUE:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
                    break;

                case ID_VIEW_APPLOOK_OFF_2007_BLACK:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
                    break;

                case ID_VIEW_APPLOOK_OFF_2007_SILVER:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
                    break;

                case ID_VIEW_APPLOOK_OFF_2007_AQUA:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
                    break;
                }

            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
            CDockingManager::SetDockingMode(DT_SMART);
            m_wndRibbonBar.SetWindows7Look(FALSE);   //TRUE
        }

    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

    theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    int cx = GetSystemMetrics(SM_CXSCREEN);
    CRect rect;
    GetClientRect(&rect);//产生第一次静态切分

    if(!m_wndSplitter.CreateStatic(this, 1, 2))
        {
            TRACE("Failed to create splitter.\n");
            return FALSE;
        }
    //	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CHighSpeedGrabView), CSize(cx*192/1000, 0), pContext))

	/////cx1//192-----.180  178   357  cx*178/1000
	//mono  
	int cx1 = rect.Width();
	cx = cx1*327 / 1000;

	if (!m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, CSize(cx, 0), pContext))

        {
            TRACE("Failed to create splitter pane.\n");
            return FALSE;
        }


    /*
    m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CHighSpeedGrabView),//视图类
    CSize(rect.Width()-rect.Width()*4/5,rect.Height()),//初始化大小
    pContext);//父窗口的创建参数
    */


    if(!m_wndSplitter.CreateMDIPane(0, 1, CSize(0, 0), pContext))
        {
            TRACE("Failed to create splitter MDI pane.\n");
            return FALSE;
        }

    return TRUE;//不再调用基类的OnCreateClient函数

}

void CMainFrame::OnSeginalwindows()
{
    // TODO: 在此添加命令处理程序代码
    //	CWnd* pwnd=m_wndSplitter1.GetPane(0,1);

    //	CImageView* pview=(CImageView*)pwnd;

    //	pview->SetWindowsType(0);

    m_ViewNum=1;
    if(m_ShowView[0])
        {
            m_ShowView[0]->CloseCtrl();
            m_ShowView[0]=NULL;

        }

    m_ShowView[0]=(CImgViewForm*)ViewAdd(1,m_Title[0]);
    m_ShowView[0]->m_nIndex=0;
    m_ShowView[0]->SetHwnd(m_hViewWnd,m_ViewMsgID);

    //	CDocument *pDoc = GetActiveDocument();
    //return m_wndSplitter.CreateChildren(pDoc,m_ViewNum,viewtype,framtext);//,viewtype
}


void CMainFrame::OnFourwindows(int num)
{
    /*
    CWnd* pwnd=m_wndSplitter1.GetPane(0,1);
    CImageView* pview=(CImageView*)pwnd;
    pview->SetWindowsType(2);
    */
    //CMFCRibbonBaseElement* pp=m_MainButton.FindByID(ID_SEGINALWINDOWS);
    //	pp->EnableTooltipInfoShortcut(FALSE);
    //m_wndRibbonBar.GetElementsByID(ID_SEGINALWINDOWS);

    m_ViewNum=num;

    for(int i=0; i<m_ViewNum; i++)
        {
            if(m_ShowView[i])
                {
                    m_ShowView[i]->CloseCtrl();
                    m_ShowView[i]=NULL;
                }

            m_ShowView[i]=(CImgViewForm*)ViewAdd(i+1,m_Title[i]);
            m_ShowView[i]->m_nIndex=i;
            m_ShowView[i]->SetHwnd(m_hViewWnd,m_ViewMsgID);
        }
}


void CMainFrame::OnSixwindows(int num)
{
    /*
    CWnd* pwnd=m_wndSplitter1.GetPane(0,1);
    CImageView* pview=(CImageView*)pwnd;
    pview->SetWindowsType(3);
    */

    m_ViewNum=num;

    for(int i=0; i<m_ViewNum; i++)
        {
            if(m_ShowView[i])
                {
                    m_ShowView[i]->CloseCtrl();
                    m_ShowView[i]=NULL;
                }

            m_ShowView[i]=(CImgViewForm*)ViewAdd(i+1,m_Title[i]);
            m_ShowView[i]->m_nIndex=i;
            m_ShowView[i]->SetHwnd(m_hViewWnd,m_ViewMsgID);
        }
}

void CMainFrame::OnEightwindows(int num)
{

    m_ViewNum=num;

    for(int i=0; i<m_ViewNum; i++)
        {
            if(m_ShowView[i])
                {
                    m_ShowView[i]->CloseCtrl();
                    m_ShowView[i]=NULL;
                }

            m_ShowView[i]=(CImgViewForm*)ViewAdd(i+1,m_Title[i]);
            m_ShowView[i]->m_nIndex=i;
            m_ShowView[i]->SetHwnd(m_hViewWnd,m_ViewMsgID);
        }
}


void  CMainFrame:: OnTwowindows()
{

    /*
    CWnd* pwnd=m_wndSplitter1.GetPane(0,1);
    CImageView* pview=(CImageView*)pwnd;
    pview->SetWindowsType(1);    */


    m_ViewNum=2;

    for(int i=0; i<m_ViewNum; i++)
        {
            if(m_ShowView[i])
                {
                    m_ShowView[i]->CloseCtrl();
                    m_ShowView[i]=NULL;
                }

            m_ShowView[i]=(CImgViewForm*)ViewAdd(i+1,m_Title[i]);
            m_ShowView[i]->m_nIndex=i;
            m_ShowView[i]->SetHwnd(m_hViewWnd,m_ViewMsgID);
        }
}

void CMainFrame::SetActive(int index)
{

    CMFCRibbonCategory *pCategory2=m_wndRibbonBar.GetCategory(index);
    m_wndRibbonBar.SetActiveCategory(pCategory2);
}

CView* CMainFrame::GetRunningView()
{
    return m_wndSplitter.GetActiveImgView();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // TODO: 在此添加专用代码和/或调用基类

    if(m_wndSplitter.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        {
            return TRUE;
        }

    return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//void CMainFrame::ActivateFrame(int nCmdShow)
//{
//    // TODO: 在此添加专用代码和/或调用基类
//    nCmdShow=SW_SHOWMAXIMIZED;
//    CFrameWndEx::ActivateFrame(nCmdShow);
//}

CView* CMainFrame::ViewAdd(int viewtype,CString str)//int viewtype
{
    CDocument *pDoc = GetActiveDocument();
    return m_wndSplitter.CreateChildren(pDoc,m_ViewNum,viewtype,str);//,viewtype
}

void CMainFrame:: SetViewNum(int num)
{
    m_ViewNum=num;
}

void CMainFrame:: UpdateShowView()
{
    for(int i=0; i<m_ViewNum; i++)
        {
            if(m_ShowView[i])
                {
                    m_ShowView[i]->UpdateShowInfo();
                }
        }
}

void CMainFrame::SetViewCtrlMsg(HWND hWnd,int nMsg)
{
    m_hViewWnd=hWnd;
    m_ViewMsgID=nMsg;
}

afx_msg LRESULT CMainFrame::OnChangeRibbonCategory(WPARAM wParam, LPARAM lParam)
{
    CMFCRibbonCategory *pCategory=m_wndRibbonBar.GetActiveCategory();
    int nCategory=m_wndRibbonBar.GetCategoryIndex(pCategory);

    //DEBUG_INFO1("%s","切换窗口");

    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_MsgID,0,nCategory);
        }
    return nCategory;
}

void CMainFrame::SetMsgHwnd(HWND phwnd, int nmsg,int ctrlid)
{
    m_MsgHWND=phwnd;
    m_MsgID=nmsg;
    m_CtrlID=ctrlid;
}



void CMainFrame::OnOpenimage()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,101,0);
        }
}


void CMainFrame::OnCloseclib()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,102,0);
        }
}


void CMainFrame::OnCalibration()  //hUANGJIN
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,103,0);
        }

}

void CMainFrame::OnSaveCalibrationReult()  //hUANGJIN
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,104,0);
        }

}


void CMainFrame::OnOpendatfile()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,201,0);
        }
}


void CMainFrame::OnClosedatfile()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,202,0);
        }
}


void CMainFrame::OnPos1()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,213,0);
        }
}


void CMainFrame::OnPos2()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,313,0);
        }
}


void CMainFrame::OnSliderpos1()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,214,0);
        }
}


void CMainFrame::OnSliderpos2()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,314,0);
        }
}


void CMainFrame::OnFristframe1()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,211,0);
        }
}


void CMainFrame::OnUp1()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,212,0);
        }
}


void CMainFrame::OnDown1()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,215,0);
        }
}


void CMainFrame::OnEndfram1()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,216,0);
        }
}


void CMainFrame::OnFristframe2()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,311,0);
        }
}


void CMainFrame::OnUp2()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,312,0);
        }
}


void CMainFrame::OnDown2()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,315,0);
        }
}


void CMainFrame::OnEndfram2()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,316,0);
        }
}


void CMainFrame::OnStartMeasure()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,231,0);
        }
}



void CMainFrame::OnUpdateOpendatfile(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!m_MsmtIsOpen);
}


void CMainFrame::OnUpdateClosedatfile(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateFristframe1(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateUp1(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdatePos1(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateSliderpos1(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateDown1(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateEndfram1(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateFristframe2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateUp2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdatePos2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateSliderpos2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateDown2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateEndfram2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_MsmtIsOpen);
}


void CMainFrame::OnUpdateOpenimage(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!m_ClibIsOpen);


}


void CMainFrame::OnUpdateCloseclib(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_ClibIsOpen);
}

void CMainFrame::OnDataProcessing()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,400,0);
        }
}


void CMainFrame::OnShowGraph()
{
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,401,0);
        }
}


void CMainFrame::OnXYZTransform()
{
    // TODO: 在此添加命令处理程序代码
    if(m_MsgHWND)
        {
            ::PostMessage(m_MsgHWND,m_CtrlID,105,0);  //HUANGJIN
        }
}


void CMainFrame::OnSaveGraph()
{
    // TODO: 在此添加命令处理程序代码
	if(m_MsgHWND)
	{
		::PostMessage(m_MsgHWND,m_CtrlID,402,0);  //HUANGJIN
	}
}


void CMainFrame::OnShowMark()
{
	if(m_MsgHWND)
	{
		::PostMessage(m_MsgHWND,m_CtrlID,239,0);  //HUANGJIN
	}
}


// set the left dialog width
void CMainFrame::SetFramSzie(int width)
{
	CRect rect;
	GetClientRect(&rect);
	int cx1 = rect.Width();
	int cy1 = rect.Height();
	//m_wndSplitter.MoveWindow(-2,-2,cx1,cy1+3);

	int w=cx1*width/1000;
	m_wndSplitter.SetColumnInfo(0, w,0);
	//m_wndSplitter.SetColumnInfo(1, cx1-cx1/2, 0);
	//m_wndSplitter.SetRowInfo(0, cy1/2,0);
	//m_wndSplitter.SetRowInfo(1, cy1-cy1/2, 0);
	m_wndSplitter.RecalcLayout();
}



CImgViewForm*  CMainFrame::MyGraphForm(void)
{
	p_MyGraphForm=(CImgViewForm*)ViewAdd(1,"数据曲线窗口");
	//p_MyGraphForm->m_nIndex=0;
	p_MyGraphForm->SetHwnd(m_hViewWnd,m_ViewMsgID);

	return p_MyGraphForm; 
}


 