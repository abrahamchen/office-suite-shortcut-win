

#pragma once

#pragma warning (disable: 4700) 
#pragma warning (disable: 4101) 


#define INI_FILE_TYPE_URL 0  //web link URL address list
#define INI_FILE_TYPE_DOC 1  //Doc type and doc file name list. 

#define MAX_KEY_NUM 30   //The INI file key numbers must less or equal 30

class CIniFileOper
{
public:
	CIniFileOper(void);
	~CIniFileOper(void);
private:

	CString m_szIniFileName;
	int m_iKeyNum;
	TCHAR m_szAppName[20];
	TCHAR m_szKeyList[MAX_KEY_NUM][20];

public:
	// Set INI file path&name
	bool SetIniFile(CString filePathName);
	CString GetIniFile(void);

	bool GetURLData(int iIndex, LPLINK pURL);
	bool InsertURLData(int iIndex, LPLINK pURL);
	int GetCount(TARGET_TYPE type);
	bool GetDocData(int iIndex, LPLINK pDoc);
	bool GetExeData(int iIndex, LPLINK pExe);
	bool GetData(int iIndex, LPLINK pData);
	bool InsertData(int iIndex, LPLINK  pData);
	bool SetKeyList(void);
};

