#pragma once
#include <iostream>

using namespace std;

class sysutils
{
	

public:	
	sysutils();
	~sysutils();
	int checkExplorerListOrder();
	bool setExplorerListOrder(int style);
	bool DeleteValueKey();
	bool CheckForConnection();
	void addWindowsuser(CString winuser);
	CString getDesktopfolder();
	VOID makeWritable(CONST CString& filename);
	BOOL deleteDirectory(CONST CString& sFolder);
	void stopBitsService();
	void startBitsService();
	void stopWindowsUpdateService();
	void startWindowsUpdateService();

	BOOL sysutils::StopService(CString strServiceName);

protected:
		
	
};

