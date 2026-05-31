#pragma once
#include <afxwin.h>
#include <atlstr.h>

typedef enum
{
    DIR_CURRENT,
    DIR_SYSTEM,
    DIR_DRIVER,
    DIR_WINDOWS,
    DIR_SPECIFIED
} DIRECTORY_TYPE;

typedef struct _PLATFORM_INFO
{
    TCHAR szOSVersion[MAX_PATH];
    TCHAR szOSType[MAX_PATH];
    TCHAR szServicePack[MAX_PATH];
    TCHAR szProcessorNumber[32];
    TCHAR szProcessorType[MAX_PATH];
    TCHAR szComputerName[MAX_PATH];
    TCHAR szLocaleInfo[32];
} FLATFORM_INFO, * LPFLATFORM_INFO;

typedef struct _FILE_VERSION_INFO
{
    LONG lsize;
    TCHAR szCreatedTime[MAX_PATH];
    TCHAR szModifiedTime[MAX_PATH];
    TCHAR szLastAccessTime[MAX_PATH];
    TCHAR szFileVersion[MAX_PATH];
    TCHAR szCompanyName[MAX_PATH];
    TCHAR szFileDescription[MAX_PATH];
    TCHAR szInternalName[MAX_PATH];
    TCHAR szOrginalFileName[MAX_PATH];
    TCHAR szProductName[MAX_PATH];
    TCHAR szProductVersion[MAX_PATH];
} FILE_VERSION_INFO, * LPFILE_VERSION_INFO;

class CCommon
{
public:
    CCommon(void);
    ~CCommon(void);

    bool SetAutoRun(bool bFlag);
    bool IsAutoRun(void);
    bool EnablePowerChangePrivilege(void);
    bool ExportFileFromRec(TCHAR* pszResName, TCHAR* pszFileName);
    BSTR Ascii2BSTR(const char* pszFName);
    VARIANT VariantString(BSTR Str);
    void GetFileNameListFromDirectory(CString szDir, CStringArray& csFileList);
    bool SetWorkingDirectory(DIRECTORY_TYPE iType, TCHAR* pDirectory);
    bool RestoreWorkingDirectory(void);
    //bool GetPlatfromInfo(LPFLATFORM_INFO pPlatformInfo);
    bool SeekDirectoryUI(TCHAR* pszDirectory);
    bool GetFileName(TCHAR* pszFileName, TCHAR* pszExt);
    bool GetFileAttributes(const TCHAR* pszFileName, LPFILE_VERSION_INFO pFileInfo);
    BOOL ReadRegistry(HKEY hRootKey, TCHAR* szSubKey, TCHAR* szName, TCHAR* szValue, LPDWORD dwpSize, DWORD dwType);
    BOOL WriteRegistry(HKEY hRootKey, TCHAR* szSubKey, TCHAR* szName, TCHAR* szValue, DWORD dwType);

private:
    TCHAR m_szPrevWorkDir[MAX_PATH];
};