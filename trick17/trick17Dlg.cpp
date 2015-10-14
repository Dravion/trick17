/* Mainwindow  */

#include "stdafx.h"
#include "trick17.h"
#include "trick17Dlg.h"
#include "afxdialogex.h"
#include "sysutils.h"
#include <Psapi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <RestartManager.h>
#include "window_supdate_dlg.h"

#pragma comment(lib, "rstrtmgr.lib")
#pragma comment(lib, "psapi.lib")

void restartExplorer();
void runFlushdns();
std::string GetLastErrorStdStr();
RM_UNIQUE_PROCESS GetExplorerApplication();
std::wstring s2ws(const std::string& s);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Running the os ipconfig command
void runFlusdns()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

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
	LPTSTR cmdArgs = L" /flushdns\0";

	if (!CreateProcess(foo+"\\system32\\ipconfig.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		MessageBox(NULL, L"Could not create user", L"Error", MB_ICONERROR);
	}	

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread); 

}

// Create a string with last error message
std::string GetLastErrorStdStr()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			LocalFree(lpMsgBuf);

			return result;
		}
	}
	return std::string();
}

Ctrick17Dlg::Ctrick17Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctrick17Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void Ctrick17Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, ck_explorer_order);
	DDX_Control(pDX, IDC_DNSFLUSH, bt_dns_flush);
	DDX_Control(pDX, IDC_COMBO_dl, combobox_utility_download);
	DDX_Control(pDX, IDC_BUTTON_run_dl, BUTTON_run_dl);
}

BEGIN_MESSAGE_MAP(Ctrick17Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &Ctrick17Dlg::OnBnClickedCheck1)	
	
	ON_BN_CLICKED(IDC_BUTTON1, &Ctrick17Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Ctrick17Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_DNSFLUSH, &Ctrick17Dlg::OnBnClickedDnsflush)
	ON_BN_CLICKED(IDC_BUTTON4, &Ctrick17Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_add_winuser, &Ctrick17Dlg::OnBnClickedButtonaddwinuser)
	ON_CBN_SELCHANGE(IDC_COMBO_dl, &Ctrick17Dlg::OnCbnSelchangeCombodl)
	ON_CBN_EDITUPDATE(IDC_COMBO_dl, &Ctrick17Dlg::OnCbnEditupdateCombodl)
	ON_BN_CLICKED(IDC_BUTTON7, &Ctrick17Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_run_dl, &Ctrick17Dlg::OnBnClickedButtonrundl)
END_MESSAGE_MAP()

// Message handlers											   
BOOL Ctrick17Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialization

	CString str;	
	SetDlgItemText(IDC_EDIT_new_winuser, _T("debug_user1"));
	SetDlgItemText(IDC_COMBO_dl, _T("eg: http://host/myfile.exe "));

	str.Format(_T("ProcExplorer"));
	combobox_utility_download.AddString(str);

	str.Format(_T("Firefox"));
	combobox_utility_download.AddString(str);

	str.Format(_T("MalewarBytes"));
	combobox_utility_download.AddString(str);
		
	str.Format(_T("Teamviewer"));
	combobox_utility_download.AddString(str);

	return TRUE;
}

void Ctrick17Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void Ctrick17Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();
	}

	sysutils* obj = new sysutils();

	if (obj->checkExplorerListOrder() == 1) {
		ck_explorer_order.SetCheck(true);
	}

	delete obj;
}

HCURSOR Ctrick17Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ctrick17Dlg::OnBnClickedCheck1()
{
	sysutils* obj = new sysutils();

	if (ck_explorer_order.GetCheck() == BST_UNCHECKED) {

		if (obj->checkExplorerListOrder() == 1) {
			obj->DeleteValueKey();
			restartExplorer();
		}
	}
	
	if (ck_explorer_order.GetCheck() == BST_CHECKED) {
		obj->setExplorerListOrder(1);
		restartExplorer();
	}	
	 delete obj;
}

// Returns the process id and create time for the oldest explorer.exe 
RM_UNIQUE_PROCESS GetExplorerApplication()
{
	RM_UNIQUE_PROCESS  result = { 0 };
	DWORD bytesReturned = 0;
	DWORD processIdSize = 4096;
	std::vector<DWORD> processIds;
	processIds.resize(1024);
	EnumProcesses(processIds.data(), processIdSize, &bytesReturned);
	while (bytesReturned == processIdSize)
	{
		processIdSize += processIdSize;
		processIds.resize(processIdSize / 4);
		EnumProcesses(processIds.data(), processIdSize, &bytesReturned);
	}    std::for_each(processIds.begin(), processIds.end(), [&result](DWORD processId) {
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE, processId);
		if (hProcess) {
			std::wstring imageName;
			imageName.resize(4096);
			if (GetProcessImageFileName(hProcess, (LPWSTR)imageName.data(), 4096) > 0)
			{
				if (wcscmp(L"explorer.exe", PathFindFileName(imageName.data())) == 0)
				{
					//this is assmuing the user is not running elevated and won't see explorer processes in other sessions
					FILETIME ftCreate, ftExit, ftKernel, ftUser;
					if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser))
					{
						if (result.dwProcessId == 0)
						{
							result.dwProcessId = processId;
							result.ProcessStartTime = ftCreate;
						}
						else if (CompareFileTime(&result.ProcessStartTime, &ftCreate) > 0)
						{
							result.dwProcessId = processId;
							result.ProcessStartTime = ftCreate;
						}
					}
				}
			}
			CloseHandle(hProcess);
		}
	});
	return result;
}

// Button restart explorer
void Ctrick17Dlg::OnBnClickedButton1()
{
	restartExplorer();
}

/* some changes needs explorer.exe to restart */
void restartExplorer()
{
//Taskbar position calculating code omitted
DWORD dwSession = 0;
WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);
if (dwError == ERROR_SUCCESS) {
	RM_UNIQUE_PROCESS rgApplications[1] = { GetExplorerApplication() };
	dwError = RmRegisterResources(
		dwSession, 0, NULL, 1, rgApplications, 0, NULL);
	DWORD dwReason;
	UINT nProcInfoNeeded;
	UINT nProcInfo = 10;
	RM_PROCESS_INFO rgpi[10];
	dwError = RmGetList(dwSession, &nProcInfoNeeded,
		&nProcInfo, rgpi, &dwReason);
	if (dwReason == RmRebootReasonNone) //now free to restart explorer
	{
		RmShutdown(dwSession, RmForceShutdown, NULL);
		HKEY hKey = { 0 };
		DWORD result = 0;
		result = ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StuckRects2"),
			0, KEY_READ | KEY_WRITE, &hKey);
		if (result == ERROR_SUCCESS)
		{
			std::vector<BYTE> data;
			data.resize(256);
			TCHAR settingValue[] = _T("Settings");
			DWORD dwKeyDataType = 0;
			DWORD dwDataBufSize = (DWORD) data.size();
			result = ::RegQueryValueEx(hKey, settingValue, NULL, &dwKeyDataType,
				(LPBYTE)data.data(), &dwDataBufSize);
			while (ERROR_MORE_DATA == result)
			{
				data.resize(256 + data.size());
				dwDataBufSize = (DWORD) data.size();
				result = ::RegQueryValueEx(hKey, settingValue, NULL, &dwKeyDataType,
					(LPBYTE)data.data(), &dwDataBufSize);
			}
			data.resize(dwDataBufSize);
			if (result == ERROR_SUCCESS)
			{
				switch (dwKeyDataType)
				{
				case REG_BINARY:
					if (data.size() == 40)
					{
						BYTE taskbarPosition = data[12];
						taskbarPosition = EDGE_ETCHED;
						data[12] = taskbarPosition;
						RECT* taskbarRect = (RECT*)&data[24];
						CopyRect(taskbarRect, NULL);
						result = ::RegSetValueEx(hKey, settingValue, 0, REG_BINARY, (LPBYTE)data.data(), dwDataBufSize);
					}
					break;
				}
			}
			::RegCloseKey(hKey);
		}
		RmRestart(dwSession, 0, NULL);
	}
}
RmEndSession(dwSession);
}

// Button - Internet Explorer Settings
void Ctrick17Dlg::OnBnClickedButton2()
{
	ShellExecute(NULL, L"Open", L"Rundll32.exe", L"shell32.dll,Control_RunDLL InetCpl.cpl,ResetIEtoDefaults,", NULL, SW_SHOWNORMAL);
}

// Button - flush DNS-Resolver cache
void Ctrick17Dlg::OnBnClickedDnsflush()
{	
	runFlusdns();
	bt_dns_flush.SetWindowText(_T("DNS-Resolver flushed [OK]"));
    
}


// Button - Test Internet connection
void Ctrick17Dlg::OnBnClickedButton4()
{
	sysutils* obj = new sysutils();

		if (obj->CheckForConnection())	{
			MessageBox(L"Connected", L"Info", MB_ICONINFORMATION);
		}
		else {
			MessageBox(L"Not Connected", L"Warning", MB_ICONWARNING);
		}
	
	delete obj;
}

void Ctrick17Dlg::OnBnClickedButtonaddwinuser()
{
	sysutils* obj = new sysutils();

	CString evalue = NULL;
	GetDlgItemText(IDC_EDIT_new_winuser, evalue);
	obj->addWindowsuser(evalue);
	delete obj;
}




// Combobox "selection change" event - if user selected something from dropdown, enable download button
void Ctrick17Dlg::OnCbnSelchangeCombodl()
{
	GetDlgItem(IDC_BUTTON_run_dl)->EnableWindow(TRUE);
}

// Combobox "edit" event - if the value has changed, enable the download button also
void Ctrick17Dlg::OnCbnEditupdateCombodl()
{
	GetDlgItem(IDC_BUTTON_run_dl)->EnableWindow(TRUE); 
}

// Button "click" event - Windowsupdate repair
void Ctrick17Dlg::OnBnClickedButton7()
{
	int Response = 0;
	Response =	MessageBox(L"This step will now remove the Systemfolder\n\nSoftwareDistribution",
						   L"Confirmation",	MB_OKCANCEL|MB_ICONINFORMATION);

	if (Response == IDOK) {
		sysutils* obj = new sysutils();

		int iresult = 0;
		iresult = obj->StopService(L"wuauserv");

		int debug = NULL;

		/*

		char* pSystemRoot;
			pSystemRoot = getenv("SYSTEMROOT");

			wchar_t wtext[MAX_PATH];
			mbstowcs(wtext, pSystemRoot, strlen(pSystemRoot) + 1);
			LPWSTR ptr = wtext;

			CString wrkdir = wtext;

			obj->stopBitsService(); // Windows intelligent Background Transfer Service (BITS)
			obj->stopWindowsUpdateService(); // Windows Update service
		
			MessageBox(L"Services stopped, now deleting "+wrkdir +"\\SoftwareDistribution", L"Info", 0);
			
			obj->deleteDirectory(wrkdir);
			obj->startBitsService();
			obj->startWindowsUpdateService();

			*/

		delete obj;
	}
}

// Download Button event "clicked"
void Ctrick17Dlg::OnBnClickedButtonrundl()
{
	CString download_url = NULL;
	CString output_path = NULL;

	sysutils* obj = new sysutils();
	CString app_exe = NULL;
	CString exec_command = NULL;


	// Firefox
	if (combobox_utility_download.GetCurSel() == 0) {
		download_url = _T("https://download.mozilla.org/?product=firefox-stub&os=win&lang=en-US");
		output_path = obj->getDesktopfolder();
		app_exe = "FF_INSTALL.EXE";
		exec_command = output_path + "\\" + app_exe;
	}

	// Malewarebytes
	if (combobox_utility_download.GetCurSel() == 1) {
		download_url = _T("http://downloads.malwarebytes.org/file/mbam/");
		output_path = obj->getDesktopfolder();
		app_exe = "MALWARE_BYTES.EXE";
		exec_command = output_path + "\\" + app_exe;
	}

	// Sysinternals ProcessExplorer
	if (combobox_utility_download.GetCurSel() == 2) {
		download_url = _T("https://live.sysinternals.com/procexp.exe");
		output_path = obj->getDesktopfolder();
		app_exe = "PE_PROCEXP.EXE";
		exec_command = output_path + "\\" + app_exe;
	}

	// Teamviewer
	if (combobox_utility_download.GetCurSel() == 3) {
		download_url = _T("http://download.teamviewer.com/download/TeamViewer_Setup_de.exe");
		output_path = obj->getDesktopfolder();
		app_exe = "TW_SETUP.EXE";
		exec_command = output_path + "\\" + app_exe;
	}

	int Response = 0;
	Response = MessageBox(L"Download and run this Utility?" + obj->getDesktopfolder() + "\\" + app_exe,
						  L"Confirmation", MB_OKCANCEL|MB_ICONASTERISK);

	if (Response == IDOK) {
		HRESULT hr = URLDownloadToFile(NULL, download_url, output_path + "\\" + app_exe, 0, NULL);
		
		// Start downloaded program
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcess(exec_command, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))	{
			printf("Cannot run the downloaded program (%d).\n", GetLastError());
		} else {
			ASSERT(AfxGetMainWnd() != NULL);
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
		}

		WaitForSingleObject(pi.hProcess, NULL);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	delete obj;
}
