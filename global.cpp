#include "stdafx.h"
#include "global.h"

//  �ص�����������
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
		if(strInitFolder.Right(1) == _T("\\"))                           // ɾ��β����"\\"
			strInitFolder = strInitFolder.Left(strInitFolder.GetLength() - 1);
	}
	
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot   = 0;
	sInfo.pszDisplayName = lpszPath;
	sInfo.lpszTitle   = _T("��ѡ������Ҫ��Ŀ���ļ��У�");
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
	
	sInfo.lpfn     = _SHBrowseForFolderCallbackProc;
	sInfo.lParam   = (LPARAM)strInitFolder.GetBuffer(0);
	
	// ��ʾ�ļ���ѡ��Ի���
	lpidlBrowse = ::SHBrowseForFolder(&sInfo);  
	
	if (lpidlBrowse != NULL)
	{
		// ȡ���ļ�����
		if (::SHGetPathFromIDList(lpidlBrowse,lpszPath))   
		{
			lpstrFolder = _T("");
			lpstrFolder = lpszPath;
			
			if(lpstrFolder != "")
			{
				if(lpstrFolder.Right(1) != _T("\\"))
					lpstrFolder += _T("\\");               //��ĩβʱ����"\\"
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


//�����ǻص��������Լ������Ĵ���
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
				//  ������м����Ŀ¼�ؼ���ʹ�����洰���ƶ���С
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
			
			SetWindowText(hwnd, "�ļ����...");
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
