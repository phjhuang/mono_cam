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

// CalibrationCtrl.cpp : 定义应用程序的类行为。
//

#include "StdAfx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CalibrationCtrl.h"
#include "MainFrm.h"

#include "CalibrationCtrlDoc.h"
#include "CalibrationCtrlView.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalibrationCtrlApp

BEGIN_MESSAGE_MAP(CCalibrationCtrlApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCalibrationCtrlApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
 
END_MESSAGE_MAP()


// CCalibrationCtrlApp 构造

CCalibrationCtrlApp::CCalibrationCtrlApp()
{
	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	//SetAppID(_T("CalibrationCtrl.AppID.NoVersion"));

	SetAppID(_T("MonoCamera.1.0.1"));  //mono


	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	//m_pMainWnd->SetWindowText("摄影测量软件");   



}

// 唯一的一个 CCalibrationCtrlApp 对象
CIniCtrl    G_IniData;
CCalibrationCtrlApp theApp;


// CCalibrationCtrlApp 初始化

BOOL CCalibrationCtrlApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	


	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CString path ;
	char* pStr = path.GetBuffer( 255 );
	GetModuleFileName( m_hInstance , pStr , 255 );
	path.ReleaseBuffer();
	CString moduleName = m_pszAppName;
	moduleName += ".exe";
	path.Replace( moduleName , "" );

	//CString nfilepath;
	//nfilepath=path+"Master.ini";
	//path = "D:\\hj\\monocam\\Release\\"; // //mono

	//G_IniData.GetIni(path);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCalibrationCtrlDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CCalibrationCtrlView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

 
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

 


	// The one and only window has been initialized, so show and update it.
	//m_nCmdShow = SW_SHOWMAXIMIZED;//这一行是添加的, 其它是上下文,帮助查找
	m_pMainWnd->ShowWindow(2);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();


	//m_pMainWnd->SetWindowText("摄影测量系统 Version 1.0") ;//huangjin


	return TRUE;
}

int CCalibrationCtrlApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CCalibrationCtrlApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CCalibrationCtrlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCalibrationCtrlApp 自定义加载/保存方法

void CCalibrationCtrlApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CCalibrationCtrlApp::LoadCustomState()
{
}

void CCalibrationCtrlApp::SaveCustomState()
{
}

// CCalibrationCtrlApp 消息处理程序





void CAboutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码



	ShellExecute(NULL,"open","双目测量软件使用说明.pdf",NULL,NULL,SW_SHOWNORMAL);   

}


 