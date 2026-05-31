#include "StdAfx.h"
#include "defination.h"
#include "IniFileOper.h"

CIniFileOper::CIniFileOper(void)
{
}

CIniFileOper::~CIniFileOper(void)
{
}

bool CIniFileOper::SetIniFile(CString filePathName)
{
	TCHAR szTemp[MAX_PATH];
	HANDLE hFile;

	if ((filePathName.Find(_T(':')) == -1) && (filePathName.Find(_T('\\')) == -1))
	{
		GetModuleFileName(NULL, szTemp, MAX_PATH);
		*(wcsrchr(szTemp, _T('\\')) + 1) = '\0';
		m_szIniFileName = szTemp;
		m_szIniFileName += filePathName;
	}
	else
	{
		m_szIniFileName = filePathName;
	}

	hFile = CreateFile(m_szIniFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		WIN32_FIND_DATA FindData;
		GetModuleFileName(NULL, szTemp, MAX_PATH);
		*(wcsrchr(szTemp, _T('\\')) + 1) = '\0';
		wcscat_s(szTemp, _T("*.INI"));

		HANDLE hFind = FindFirstFile(szTemp, &FindData);
		if (hFind == INVALID_HANDLE_VALUE)
			return false;
		FindClose(hFind);

		*(wcsrchr(szTemp, '\\') + 1) = '\0';
		wcscat_s(szTemp, FindData.cFileName);
		m_szIniFileName = szTemp;

		hFile = CreateFile(m_szIniFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;
	}

	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);

	return SetKeyList();
}

CString CIniFileOper::GetIniFile(void)
{
	return m_szIniFileName;
}

bool CIniFileOper::GetURLData(int iIndex, LPLINK pURL)
{
	TCHAR szTemp[MAX_PATH];
	TCHAR szAppName[20];
	int iCounter = 1;
	int iNum = 0;

	if (iIndex <= 0) return false;
	memset(pURL, 0, sizeof(LINK_SET));

	for (;;)
	{
		wsprintf(szAppName, _T("CON_%05d"), iCounter);
		if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, szTemp, MAX_PATH, m_szIniFileName) == 0)
			return false;

		if (_wcsicmp(szTemp, _T("Web-URL")) == 0)
		{
			if (++iNum == iIndex)
			{
				wcscpy_s(pURL->szType, _T("Web-URL"));
				GetPrivateProfileString(szAppName, _T("DESCRIPTION"), _T(""), pURL->szName, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("LINKAGE"), _T(""), pURL->szLinkage, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("COMMENTS"), _T(""), pURL->szComments, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("USER"), _T(""), pURL->szUSER, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("PSW"), _T(""), pURL->szPSW, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("LASTTIME"), _T(""), pURL->szTime, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("CATEGORY"), _T(""), pURL->szCategory, MAX_PATH, m_szIniFileName);
				return true;
			}
		}
		iCounter++;
	}
	return false;
}

bool CIniFileOper::InsertURLData(int iIndex, LPLINK pURL)
{
	TCHAR szAppName[20];
	int iCounter = 1;

	if (iIndex <= 0) return false;

	for (;;)
	{
		wsprintf(szAppName, _T("CON_%05d"), iCounter);
		if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, NULL, 0, m_szIniFileName) == 0)
		{
			WritePrivateProfileString(szAppName, _T("TYPE"), pURL->szType, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("DESCRIPTION"), pURL->szName, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("LINKAGE"), pURL->szLinkage, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("COMMENTS"), pURL->szComments, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("USER"), pURL->szUSER, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("PSW"), pURL->szPSW, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("LASTTIME"), pURL->szTime, m_szIniFileName);
			WritePrivateProfileString(szAppName, _T("CATEGORY"), pURL->szCategory, m_szIniFileName);
			return true;
		}
		iCounter++;
	}
	return false;
}

int CIniFileOper::GetCount(TARGET_TYPE type)
{
	TCHAR szTemp[MAX_PATH];
	TCHAR szAppName[20];
	int iCounter = 1;
	int iNum = 0;

	switch (type)
	{
	case TYPE_ALL:
		for (;;)
		{
			wsprintf(szAppName, _T("CON_%05d"), iCounter);
			if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, szTemp, MAX_PATH, m_szIniFileName) != 0)
				iCounter++;
			else
				return --iCounter;
		}
		break;

	case TYPE_URL:
		for (;;)
		{
			wsprintf(szAppName, _T("CON_%05d"), iCounter);
			if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, szTemp, MAX_PATH, m_szIniFileName) == 0)
				return iNum;

			if (_wcsicmp(szTemp, _T("Web-URL")) == 0)
				iNum++;

			iCounter++;
		}
		break;

	case TYPE_DOC:
		for (;;)
		{
			wsprintf(szAppName, _T("CON_%05d"), iCounter);
			if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, szTemp, MAX_PATH, m_szIniFileName) == 0)
				return iNum;

			if (_wcsicmp(szTemp, _T("Document")) == 0)
				iNum++;

			iCounter++;
		}
		break;
	}
	return 0;
}

bool CIniFileOper::GetDocData(int iIndex, LPLINK pDoc)
{
	TCHAR szTemp[MAX_PATH];
	TCHAR szAppName[20];
	int iCounter = 1;
	int iNum = 0;

	if (iIndex <= 0) return false;
	memset(pDoc, 0, sizeof(LINK_SET));

	for (;;)
	{
		wsprintf(szAppName, _T("CON_%05d"), iCounter);
		if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, szTemp, MAX_PATH, m_szIniFileName) == 0)
			return false;

		if (_wcsicmp(szTemp, _T("Document")) == 0)
		{
			if (++iNum == iIndex)
			{
				wcscpy_s(pDoc->szType, szTemp);
				GetPrivateProfileString(szAppName, _T("DESCRIPTION"), _T(""), pDoc->szName, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("LINKAGE"), _T(""), pDoc->szLinkage, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("COMMENTS"), _T(""), pDoc->szComments, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("USER"), _T(""), pDoc->szUSER, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("PSW"), _T(""), pDoc->szPSW, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("LASTTIME"), _T(""), pDoc->szTime, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("CATEGORY"), _T(""), pDoc->szCategory, MAX_PATH, m_szIniFileName);
				return true;
			}
		}
		iCounter++;
	}
	return false;
}

bool CIniFileOper::GetExeData(int iIndex, LPLINK pExe)
{
	TCHAR szTemp[MAX_PATH];
	TCHAR szAppName[20];
	int iCounter = 1;
	int iNum = 0;

	if (iIndex <= 0) return false;
	memset(pExe, 0, sizeof(LINK_SET));

	for (;;)
	{
		wsprintf(szAppName, _T("CON_%05d"), iCounter);
		if (GetPrivateProfileString(szAppName, _T("TYPE"), NULL, szTemp, MAX_PATH, m_szIniFileName) == 0)
			return false;

		if ((_wcsicmp(szTemp, _T("Executable")) == 0) ||
			(_wcsicmp(szTemp, _T("Shortcut")) == 0) ||
			(_wcsicmp(szTemp, _T("Directory")) == 0))
		{
			if (++iNum == iIndex)
			{
				wcscpy_s(pExe->szType, szTemp);
				GetPrivateProfileString(szAppName, _T("DESCRIPTION"), _T(""), pExe->szName, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("LINKAGE"), _T(""), pExe->szLinkage, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("COMMENTS"), _T(""), pExe->szComments, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("USER"), _T(""), pExe->szUSER, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("PSW"), _T(""), pExe->szPSW, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("LASTTIME"), _T(""), pExe->szTime, MAX_PATH, m_szIniFileName);
				GetPrivateProfileString(szAppName, _T("CATEGORY"), _T(""), pExe->szCategory, MAX_PATH, m_szIniFileName);
				return true;
			}
		}
		iCounter++;
	}
	return false;
}

bool CIniFileOper::GetData(int iIndex, LPLINK pData)
{
	TCHAR szAppName[20];
	wsprintf(szAppName, _T("CON_%05d"), iIndex);

	GetPrivateProfileString(szAppName, _T("TYPE"), _T(""), pData->szType, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("DESCRIPTION"), _T(""), pData->szName, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("LINKAGE"), _T(""), pData->szLinkage, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("COMMENTS"), _T(""), pData->szComments, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("USER"), _T(""), pData->szUSER, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("PSW"), _T(""), pData->szPSW, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("LASTTIME"), _T(""), pData->szTime, MAX_PATH, m_szIniFileName);
	GetPrivateProfileString(szAppName, _T("CATEGORY"), _T(""), pData->szCategory, MAX_PATH, m_szIniFileName);

	return true;
}

bool CIniFileOper::InsertData(int iIndex, LPLINK pData)
{
	TCHAR szAppName[20];
	wsprintf(szAppName, _T("CON_%05d"), iIndex);

	WritePrivateProfileString(szAppName, _T("TYPE"), pData->szType, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("DESCRIPTION"), pData->szName, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("LINKAGE"), pData->szLinkage, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("COMMENTS"), pData->szComments, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("USER"), pData->szUSER, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("PSW"), pData->szPSW, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("LASTTIME"), pData->szTime, m_szIniFileName);
	WritePrivateProfileString(szAppName, _T("CATEGORY"), pData->szCategory, m_szIniFileName);

	return true;
}

bool CIniFileOper::SetKeyList(void)
{
	TCHAR szKeyName[20];

	if (GetPrivateProfileString(_T("Overall"), _T("APP_NAME"), _T(""), m_szAppName, 20, m_szIniFileName) == 0)
	{
		// 注释掉避免无Overall分区时报错弹窗
		// MessageBox(NULL, _T("Can't got the App_Name from INI file"), _T("Error"), MB_OK);
		// return false;
	}

	memset(m_szKeyList, 0, sizeof(TCHAR) * MAX_KEY_NUM * 20);
	for (m_iKeyNum = 0;; m_iKeyNum++)
	{
		wsprintf(szKeyName, _T("Key_%03d"), m_iKeyNum + 1);
		if (GetPrivateProfileString(_T("Overall"), szKeyName, NULL, m_szKeyList[m_iKeyNum], 20, m_szIniFileName) == 0)
			break;
	}

	return true;
}
