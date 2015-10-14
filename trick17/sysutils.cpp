#include "stdafx.h"
#include "sysutils.h"
#include <windows.h>
#include <winsvc.h>
#include <wininet.h>
#include <iostream>

#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Advapi32.lib")

using namespace std;


sysutils::sysutils()
{
}

sysutils::~sysutils()
{
}

/* check if a registry key exists */
int sysutils::checkExplorerListOrder()
{
	HKEY hkey;
	LPBYTE dwReturn = NULL;
	DWORD dwSize = sizeof(DWORD);
	char buffer[255] = { 0 };
	DWORD dwBufSize = sizeof(buffer);

	// Buffer to store string read from registry
	TCHAR szValue[1024];
	DWORD cbValueLength = sizeof(szValue);
	int bState = 0;


	if (RegOpenKeyExW(HKEY_CURRENT_USER,
		L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\Bags\\AllFolders\\Shell"
		, 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) 	{
		
		DWORD error = RegQueryValueEx(hkey, L"FolderType", 0, 0, reinterpret_cast<LPBYTE>(&szValue),
			&cbValueLength);

		if (error == ERROR_SUCCESS)	{

			if (CString(szValue) == CString("NotSpecified")) {
				bState = 1;
			}			
		}
		else
		{
			//MessageBox(NULL, CString(szValue), _T("Error"), MB_ICONERROR | MB_OK);
		}
	}
	RegCloseKey(hkey);
	return bState;
}


/* 0 is default (on), 1 is disables Windows automatic folder discovery  */
bool sysutils::setExplorerListOrder(int style)
{
	HKEY hkey;
	DWORD dwDisposition;	
	char* data = "NotSpecified\0";
	
	if (RegCreateKeyEx(HKEY_CURRENT_USER,
		TEXT("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\Bags\\AllFolders\\Shell"),
		0, NULL, 0,
		KEY_WRITE, NULL,
		&hkey, &dwDisposition) == ERROR_SUCCESS) {
		DWORD dwType, dwSize;
		dwType = REG_DWORD;
		dwSize = sizeof(DWORD);
		DWORD rofl = 1;
			RegSetValueExA(hkey, "FolderType", 0, REG_SZ, (BYTE*)data, (DWORD) strlen(data) +1 );
			RegCloseKey(hkey);
	}  
	return false;
}

/*  Registry - Delete "Windows Automatic Folder Discovery" key and reset to Windows default setting */
bool sysutils::DeleteValueKey()
{
	HKEY hkey;
	RegOpenKey(HKEY_CURRENT_USER, L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\Bags\\AllFolders\\Shell", &hkey);
	RegDeleteValue(hkey, L"FolderType");
		RegCloseKey(hkey); 

	return false;
}

/* Check if internet connection is possible at all */
bool sysutils::CheckForConnection()
{
	if (InternetGetConnectedState(NULL, 0))
		return true;
	else
		return false;
}

// run %systemroot%\\system32\\net.exe 
void sysutils::addWindowsuser(CString winuser)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	CString netcmd = netcmd + " user /add " + winuser + "\0";

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char * pSystemRoot;
	pSystemRoot = getenv("SYSTEMROOT");

	si.wShowWindow = SW_MINIMIZE;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	wchar_t wtext[MAX_PATH];
	mbstowcs(wtext, pSystemRoot, strlen(pSystemRoot) + 1);
	LPWSTR ptr = wtext;

	CString foo = wtext;
	LPTSTR cmdArgs = netcmd.GetBuffer(0);

	if (!CreateProcess(foo + "\\system32\\net.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{

		MessageBox(NULL, L"User could not be created", L"Error", MB_ICONERROR);
	}
	else {
		MessageBox(NULL, L"User created or all ready exists. You can use this Account on Windows logon", L"Info", MB_ICONINFORMATION);
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// run %systemroot%\\system32\\net.exe - stop bits service
void sysutils::stopBitsService()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	CString netcmd = netcmd + " stop bits \0";

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char * pSystemRoot;
	pSystemRoot = getenv("SYSTEMROOT");

	si.wShowWindow = SW_MINIMIZE;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	wchar_t wtext[MAX_PATH];
	mbstowcs(wtext, pSystemRoot, strlen(pSystemRoot) + 1);
	LPWSTR ptr = wtext;

	CString foo = wtext;
	LPTSTR cmdArgs = netcmd.GetBuffer(0);

	if (!CreateProcess(foo + "\\system32\\net.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		 MessageBox(NULL, L"Services could not be stopped", L"Error", MB_ICONERROR);
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// run %systemroot%\\system32\\net.exe - start bis services
void sysutils::startBitsService()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	CString netcmd = netcmd + " start bits \0";

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char * pSystemRoot;
	pSystemRoot = getenv("SYSTEMROOT");

	si.wShowWindow = SW_MINIMIZE;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	wchar_t wtext[MAX_PATH];
	mbstowcs(wtext, pSystemRoot, strlen(pSystemRoot) + 1);
	LPWSTR ptr = wtext;

	CString foo = wtext;
	LPTSTR cmdArgs = netcmd.GetBuffer(0);

	if (!CreateProcess(foo + "\\system32\\net.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{

		MessageBox(NULL, L"Services could not be stopped", L"Error", MB_ICONERROR);
	}
	
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// run %systemroot%\\system32\\net.exe - stop windows update service
void sysutils::stopWindowsUpdateService()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	CString netcmd = netcmd + " stop wuauserv \0";

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char * pSystemRoot;
	pSystemRoot = getenv("SYSTEMROOT");

	si.wShowWindow = SW_MINIMIZE;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	wchar_t wtext[MAX_PATH];
	mbstowcs(wtext, pSystemRoot, strlen(pSystemRoot) + 1);
	LPWSTR ptr = wtext;

	CString foo = wtext;
	LPTSTR cmdArgs = netcmd.GetBuffer(0);

	if (!CreateProcess(foo + "\\system32\\net.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
	    MessageBox(NULL, L"Services could not be stopped", L"Error", MB_ICONERROR);
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// run %systemroot%\\system32\\net.exe - start windows update service
void sysutils::startWindowsUpdateService()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	CString netcmd = netcmd + " start wuauserv \0";

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char * pSystemRoot;
	pSystemRoot = getenv("SYSTEMROOT");

	si.wShowWindow = SW_MINIMIZE;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	wchar_t wtext[MAX_PATH];
	mbstowcs(wtext, pSystemRoot, strlen(pSystemRoot) + 1);
	LPWSTR ptr = wtext;

	CString foo = wtext;
	LPTSTR cmdArgs = netcmd.GetBuffer(0);

	if (!CreateProcess(foo + "\\system32\\net.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		MessageBox(NULL, L"Services could not be stopped", L"Error", MB_ICONERROR);
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}



CString sysutils::getDesktopfolder()
{
	wchar_t buffer[MAX_PATH];
	BOOL result = SHGetSpecialFolderPath(NULL
		, buffer
		, CSIDL_DESKTOP
		, false);
	if (!result) throw "Error";


	return buffer;
}


// make folder writetable
VOID sysutils::makeWritable(CONST CString& filename)
{
	DWORD dwAttrs = ::GetFileAttributes(filename);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES) return;

	if (dwAttrs & FILE_ATTRIBUTE_READONLY)
	{
		::SetFileAttributes(filename,
			dwAttrs & (~FILE_ATTRIBUTE_READONLY));
	}
}

// delete folder recursively 
BOOL sysutils::deleteDirectory(CONST CString& sFolder)
{
	CFileFind   ff;
	CString     sCurFile;
	BOOL bMore = ff.FindFile(sFolder + _T("\\*.*"));

	// Empty the folder, before removing it
	while (bMore)
	{
		bMore = ff.FindNextFile();
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
				deleteDirectory(ff.GetFilePath());
		}
		else
		{
			sCurFile = ff.GetFilePath();
			makeWritable(sCurFile);

			if (!::DeleteFile(sCurFile))
			{		
				return FALSE;
			}
		}
	}

	// RemoveDirectory fails without this one!  CFileFind locks file system resources.
	ff.Close();

	if (!::RemoveDirectory(sFolder))
	{		
		return FALSE;
	}
	return TRUE;
}

// Stop a Service
BOOL sysutils::StopService(CString strServiceName)
{
	BOOL bResult = FALSE;

	SC_HANDLE hServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL != hServiceControlManager)
	{
		SC_HANDLE hService = OpenService(hServiceControlManager, strServiceName.GetBuffer(0), SERVICE_STOP | SERVICE_QUERY_STATUS);
		if (hService != NULL)
		{
			SERVICE_STATUS_PROCESS serviceStatus;
			if (ControlService(hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&serviceStatus))
			{
				bResult = TRUE;
			}
			CloseServiceHandle(hService);
		}

		CloseServiceHandle(hServiceControlManager);
	}

	return bResult;
}