//#ifndef GLOBAL_H
//#define GLOBAL_H
#define REMOVE_HEIGHT 28
#ifndef	BIF_NEWDIALOGSTYLE
#define		BIF_NEWDIALOGSTYLE		0x0040
#endif

#ifndef	BIF_USENEWUI
#define		BIF_USENEWUI			0x0050
#endif

//	初始化文件夹设定用的回调函数
int CALLBACK _SHBrowseForFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

BOOL SelectFolder(CString& lpstrFolder, CString strInitFolder);
//#endif