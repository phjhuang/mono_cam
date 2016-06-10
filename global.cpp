#include "stdafx.h"
#include "global.h"

//  回调函数的声明
int CALLBACK _SHBrowseForFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

BOOL SelectFolder(CString& lpstrFolder, CString strInitFolder)
{
	BOOL          bRes = FALSE;
	
	char          lpszPath[MAX_PATH];
	LPMALLOC      lpMalloc;
	BROWSEINFO    sInfo;
	LPITEMIDLIST lpidlBrowse;
	
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return FALSE;
	
	if (strInitFolder != _T(""))
	{
		if(strInitFolder.Right(1) == _T("\\"))                           // 删除尾部的"\\"
			strInitFolder = strInitFolder.Left(strInitFolder.GetLength() - 1);
	}
	
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot   = 0;
	sInfo.pszDisplayName = lpszPath;
	sInfo.lpszTitle   = _T("请选择您需要的目的文件夹：");
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
	
	sInfo.lpfn     = _SHBrowseForFolderCallbackProc;
	sInfo.lParam   = (LPARAM)strInitFolder.GetBuffer(0);
	
	// 显示文件夹选择对话框
	lpidlBrowse = ::SHBrowseForFolder(&sInfo);  
	
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名
		if (::SHGetPathFromIDList(lpidlBrowse,lpszPath))   
		{
			lpstrFolder = _T("");
			lpstrFolder = lpszPath;
			
			if(lpstrFolder != "")
			{
				if(lpstrFolder.Right(1) != _T("\\"))
					lpstrFolder += _T("\\");               //在末尾时附加"\\"
			}
		}
		
		bRes = TRUE;
	}
	
	if(lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	
	lpMalloc->Release();
	
	return bRes;
}


//下面是回调函数，以及函数的处理
int CALLBACK _SHBrowseForFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	static HWND hWndEdit = NULL;
	CString strDir;
	
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		{
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			
			CRect rect;   
			HWND  hChild = GetWindow(hwnd, GW_CHILD);
			while (hChild) 
			{
				TCHAR szClass[256];
				GetClassName(hChild, szClass, 255);
				//  如果是中间的树目录控件，使可以随窗口移动大小
				if (strcmp(szClass, "SHBrowseForFolder ShellNameSpace Control") == 0)
				{
					GetWindowRect(hChild, rect);
					rect.top -= REMOVE_HEIGHT;
					
					CPoint pt = rect.TopLeft();
					ScreenToClient(hwnd, &pt);
					MoveWindow(hChild, pt.x, pt.y, rect.Width(), rect.Height(), TRUE);
				}
				
				if (strcmp(szClass, "Edit") == 0)
				{
					hWndEdit = hChild;
				}
				
				hChild = GetNextWindow(hChild, GW_HWNDNEXT);
			}
			
			SetWindowText(hwnd, "文件浏览...");
		}
		break;
		
	case BFFM_SELCHANGED:
		if(hWndEdit)
		{
			if(::SHGetPathFromIDList((LPITEMIDLIST)lParam, strDir.GetBufferSetLength(MAX_PATH)))
			{
				SetWindowText(hWndEdit, strDir);
			}
			else
			{
				SetWindowText(hWndEdit, NULL);
				SendMessage(hwnd, BFFM_VALIDATEFAILED, 0, 0);
			}
			strDir.ReleaseBuffer();
		}
		break;
		
	default:
		break;
		
	}
	
	return 0;
}
