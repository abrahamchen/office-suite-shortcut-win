#include "StdAfx.h"
#include "Common.h"
#include <shlobj.h>
#include <versionhelpers.h>

CCommon::CCommon(void)
{
    wmemset(m_szPrevWorkDir, 0, MAX_PATH);
}

CCommon::~CCommon(void)
{
}

#define WINDOW_RUN_KEY    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\"
#define AUTO_RUN_NAME     L"AbrahamSpyTool"

bool CCommon::SetAutoRun(bool bFlag)
{
    HKEY hkCfg;
    CString sSubKey = WINDOW_RUN_KEY;
    DWORD dwDisposition;
    long lRet;

    TCHAR szBuffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szBuffer, MAX_PATH);
    CString sFileName = szBuffer;

    if (RegCreateKeyEx(
        HKEY_CURRENT_USER,
        sSubKey,
        0L,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        NULL,
        &hkCfg,
        &dwDisposition) != ERROR_SUCCESS)
    {
        return false;
    }

    if (bFlag)
    {
        lRet = RegSetValueEx(hkCfg, AUTO_RUN_NAME, 0L, REG_SZ,
            (const BYTE*)sFileName.GetString(),
            (sFileName.GetLength() + 1) * sizeof(TCHAR));
    }
    else
    {
        lRet = RegDeleteValue(hkCfg, AUTO_RUN_NAME);
    }

    RegCloseKey(hkCfg);
    return (lRet == ERROR_SUCCESS);
}

bool CCommon::IsAutoRun(void)
{
    HKEY hkCfg;
    CString sSubKey = WINDOW_RUN_KEY;
    DWORD dwDisposition;

    TCHAR szValue[MAX_PATH] = { 0 };
    DWORD dwSize = MAX_PATH * sizeof(TCHAR);
    DWORD dwType = REG_SZ;

    if (RegCreateKeyEx(
        HKEY_CURRENT_USER,
        sSubKey,
        0L,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        NULL,
        &hkCfg,
        &dwDisposition) != ERROR_SUCCESS)
    {
        return false;
    }

    long lRet = RegQueryValueEx(hkCfg, AUTO_RUN_NAME, NULL, &dwType, (LPBYTE)szValue, &dwSize);
    RegCloseKey(hkCfg);

    TCHAR szModule[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szModule, MAX_PATH);

    return (lRet == ERROR_SUCCESS && wcscmp(szValue, szModule) == 0);
}

bool CCommon::EnablePowerChangePrivilege(void)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    bool bSuccess = (GetLastError() == ERROR_SUCCESS);

    CloseHandle(hToken);
    return bSuccess;
}

bool CCommon::ExportFileFromRec(TCHAR* pszResName, TCHAR* pszFileName)
{
    if (!pszResName || !pszFileName) return false;

    HRSRC hResRc = FindResource(AfxGetResourceHandle(), pszResName, pszResName);
    if (!hResRc) return false;

    DWORD dwSize = SizeofResource(AfxGetResourceHandle(), hResRc);
    if (dwSize == 0) return false;

    HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hResRc);
    if (!hGlobal) return false;

    LPVOID pData = LockResource(hGlobal);
    if (!pData) return false;

    SetFileAttributes(pszFileName, FILE_ATTRIBUTE_NORMAL);
    DeleteFile(pszFileName);

    HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwWritten = 0;
    BOOL bOK = WriteFile(hFile, pData, dwSize, &dwWritten, NULL);
    CloseHandle(hFile);

    return bOK && (dwWritten == dwSize);
}

BSTR CCommon::Ascii2BSTR(const char* pszFName)
{
    if (!pszFName) return NULL;
    WCHAR wszBuf[MAX_PATH] = { 0 };
    MultiByteToWideChar(CP_ACP, 0, pszFName, -1, wszBuf, MAX_PATH);
    return SysAllocString(wszBuf);
}

VARIANT CCommon::VariantString(BSTR Str)
{
    VARIANT var;
    VariantInit(&var);
    V_VT(&var) = VT_BSTR;
    V_BSTR(&var) = SysAllocString(Str);
    return var;
}

void CCommon::GetFileNameListFromDirectory(CString szDir, CStringArray& csFileList)
{
    CFileFind finder;
    CString strPath = szDir + L"\\*.*";

    BOOL bFound = finder.FindFile(strPath);
    while (bFound)
    {
        bFound = finder.FindNextFile();

        if (finder.IsDots())
            continue;

        if (finder.IsDirectory())
        {
            GetFileNameListFromDirectory(finder.GetFilePath(), csFileList);
        }
        else
        {
            csFileList.Add(finder.GetFilePath());
        }
    }
    finder.Close();
}

bool CCommon::SetWorkingDirectory(DIRECTORY_TYPE iType, TCHAR* pDirectory)
{
    if (wcslen(m_szPrevWorkDir) == 0)
        GetCurrentDirectory(MAX_PATH, m_szPrevWorkDir);

    TCHAR szPath[MAX_PATH] = { 0 };

    switch (iType)
    {
    case DIR_CURRENT:
        GetModuleFileName(NULL, szPath, MAX_PATH);
        *(wcsrchr(szPath, L'\\')) = 0;
        break;
    case DIR_SYSTEM:
        GetSystemDirectory(szPath, MAX_PATH);
        break;
    case DIR_DRIVER:
        GetSystemDirectory(szPath, MAX_PATH);
        wcscat_s(szPath, L"\\Drivers");
        break;
    case DIR_WINDOWS:
        GetWindowsDirectory(szPath, MAX_PATH);
        break;
    case DIR_SPECIFIED:
        if (pDirectory) wcscpy_s(szPath, pDirectory);
        break;
    default:
        return false;
    }

    return SetCurrentDirectory(szPath);
}

bool CCommon::RestoreWorkingDirectory(void)
{
    return SetCurrentDirectory(m_szPrevWorkDir);
}

/*
bool CCommon::GetPlatfromInfo(LPFLATFORM_INFO pPlatformInfo)
{
    if (!pPlatformInfo) return false;
    ZeroMemory(pPlatformInfo, sizeof(FLATFORM_INFO));

    OSVERSIONINFOEX osvi = { 0 };
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((OSVERSIONINFO*)&osvi);

    SYSTEM_INFO si = { 0 };
    GetNativeSystemInfo(&si);

    if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        if (osvi.dwMajorVersion == 10)
            wcscpy_s(pPlatformInfo->szOSVersion, L"Windows 10/11");
        else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
            wcscpy_s(pPlatformInfo->szOSVersion, L"Windows 8.1");
        else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
            wcscpy_s(pPlatformInfo->szOSVersion, L"Windows 8");
        else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
            wcscpy_s(pPlatformInfo->szOSVersion, L"Windows 7");
        else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
            wcscpy_s(pPlatformInfo->szOSVersion, L"Vista");
        else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            wcscpy_s(pPlatformInfo->szOSVersion, L"Windows XP");
    }

    wsprintf(pPlatformInfo->szProcessorNumber, L"%d", si.dwNumberOfProcessors);
    DWORD dwSize = MAX_PATH;
    GetComputerName(pPlatformInfo->szComputerName, &dwSize);

    return true;
}
*/


bool CCommon::SeekDirectoryUI(TCHAR* pszDirectory)
{
    if (!pszDirectory) return false;

    BROWSEINFO bi = { 0 };
    TCHAR szDisplayName[MAX_PATH] = { 0 };
    bi.pszDisplayName = szDisplayName;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (!pidl) return false;

    bool bRet = SHGetPathFromIDList(pidl, pszDirectory);
    CoTaskMemFree(pidl);
    return bRet;
}

bool CCommon::GetFileName(TCHAR* pszFileName, TCHAR* pszExt)
{
    if (!pszFileName) return false;

    CString strFilter;
    if (pszExt && wcslen(pszExt) > 0)
        strFilter.Format(L"文件 (*.%s)|*.%s|所有文件 (*.*)|*.*||", pszExt, pszExt);
    else
        strFilter = L"所有文件 (*.*)|*.*||";

    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (dlg.DoModal() == IDOK)
    {
        wcscpy_s(pszFileName, MAX_PATH, dlg.GetPathName());
        return true;
    }
    return false;
}

bool CCommon::GetFileAttributes(const TCHAR* pszFileName, LPFILE_VERSION_INFO pFileInfo)
{
    if (!pszFileName || !pFileInfo) return false;
    ZeroMemory(pFileInfo, sizeof(FILE_VERSION_INFO));

    CFileStatus status;
    if (CFile::GetStatus(pszFileName, status))
    {
        /*
        pFileInfo->lsize = status.m_size;
        CTime t;
        t = status.m_ctime;
        t.Format(pFileInfo->szCreatedTime, L"%Y-%m-%d %H:%M:%S");
        t = status.m_mtime;
        t.Format(pFileInfo->szModifiedTime, L"%Y-%m-%d %H:%M:%S");
        */

        // Creation time
        pFileInfo->lsize = (LONG) status.m_size;
        CTime t;
        t = status.m_ctime;
        CString strTime = t.Format(L"%Y-%m-%d %H:%M:%S");
        wcscpy_s(pFileInfo->szCreatedTime, strTime);

        // Modified time
        t = status.m_mtime;
        strTime = t.Format(L"%Y-%m-%d %H:%M:%S");
        wcscpy_s(pFileInfo->szModifiedTime, strTime);

        // Last access time
        t = status.m_atime;
        strTime = t.Format(L"%Y-%m-%d %H:%M:%S");
        wcscpy_s(pFileInfo->szLastAccessTime, strTime);

    }

    DWORD dummy = 0;
    DWORD len = GetFileVersionInfoSize(pszFileName, &dummy);
    if (len == 0) return true;

    BYTE* buf = new BYTE[len];
    if (GetFileVersionInfo(pszFileName, 0, len, buf))
    {
        struct LANGANDCODEPAGE { WORD lang, cp; } *trans;
        UINT trLen;
        VerQueryValue(buf, L"\\VarFileInfo\\Translation", (LPVOID*)&trans, &trLen);

        CString sub;
        sub.Format(L"\\StringFileInfo\\%04x%04x\\FileVersion", trans->lang, trans->cp);

        WCHAR* ver = NULL;
        UINT vLen;
        if (VerQueryValue(buf, (LPCTSTR)sub, (LPVOID*)&ver, &vLen))
            wcscpy_s(pFileInfo->szFileVersion, ver);
    }
    delete[] buf;
    return true;
}

BOOL CCommon::ReadRegistry(HKEY hRoot, TCHAR* szSubKey, TCHAR* szName, TCHAR* szValue, LPDWORD pdwSize, DWORD dwType)
{
    HKEY hk;
    if (RegOpenKeyEx(hRoot, szSubKey, 0, KEY_READ, &hk) != ERROR_SUCCESS)
        return FALSE;

    BOOL bRet = RegQueryValueEx(hk, szName, 0, &dwType, (LPBYTE)szValue, pdwSize) == ERROR_SUCCESS;
    RegCloseKey(hk);
    return bRet;
}

BOOL CCommon::WriteRegistry(HKEY hRoot, TCHAR* szSubKey, TCHAR* szName, TCHAR* szValue, DWORD dwType)
{
    HKEY hk;
    DWORD disp;
    LONG ret = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, 0, KEY_WRITE, NULL, &hk, &disp);
    if (ret != ERROR_SUCCESS) return FALSE;

    DWORD len = (DWORD) (wcslen(szValue) + 1) * sizeof(TCHAR);
    ret = RegSetValueEx(hk, szName, 0, dwType, (const BYTE*)szValue, len);
    RegCloseKey(hk);
    return ret == ERROR_SUCCESS;
}