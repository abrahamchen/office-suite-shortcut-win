// SpyToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpyTool.h"
#include "SpyToolDlg.h"
#include "afxdialogex.h"
#include "ItemEditDlg.h"
#include "ItemAddDlg.h"
#include "Common.h"

#define REG_LATEST_OPEN_INI_FILE  L"Software\\Abraham"
#define REG_NAME L"SpyTool"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {
	struct PARAMSORT
	{
		PARAMSORT(HWND hWnd, int columnIndex, bool ascending)
			:m_hWnd(hWnd)
			, m_ColumnIndex(columnIndex)
			, m_Ascending(ascending)
		{
		}

		HWND m_hWnd;
		int  m_ColumnIndex;
		bool m_Ascending;
	};

	int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		PARAMSORT& ps = *(PARAMSORT*)lParamSort;
		TCHAR left[256] = _T(""), right[256] = _T("");

		ListView_GetItemText(ps.m_hWnd, lParam1, ps.m_ColumnIndex, left, _countof(left));
		ListView_GetItemText(ps.m_hWnd, lParam2, ps.m_ColumnIndex, right, _countof(right));

		return ps.m_Ascending ? _tcscmp(left, right) : _tcscmp(right, left);
	}
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx) END_MESSAGE_MAP()

	CSpyToolDlg::CSpyToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpyToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpyToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TARGET, m_ListCtrlTarget);
	DDX_Control(pDX, IDC_EDIT_LINKAGE, m_EditLinkage);
	DDX_Control(pDX, IDC_EDIT_USER, m_EditUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_EditPassword);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_ComboFilterType);
	DDX_Control(pDX, IDC_CHECK_AUTO_RUN, m_CheckAutoRun);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_EditUpdate);
	DDX_Control(pDX, IDC_COMBO_FILTER_CATEGORY, m_ComboCategory);
}

BEGIN_MESSAGE_MAP(CSpyToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TARGET, &CSpyToolDlg::OnLvnItemchangedListTarget)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TARGET, &CSpyToolDlg::OnNMDblclkListTarget)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CSpyToolDlg::OnBnClickedButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CSpyToolDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CSpyToolDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CSpyToolDlg::OnBnClickedButtonLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, &CSpyToolDlg::OnCbnSelchangeComboFilterType)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_TARGET, &CSpyToolDlg::OnLvnColumnclickListTarget)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RUN, &CSpyToolDlg::OnBnClickedCheckAutoRun)
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSpyToolDlg::OnBnClickedButtonAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_CATEGORY, &CSpyToolDlg::OnCbnSelchangeComboFilterCategory)
END_MESSAGE_MAP()

BOOL CSpyToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CCommon CommonTool;

	m_ListCtrlTarget.InsertColumn(LINKAGE_NAME, _T("Name"), LVCFMT_LEFT, 120);
	m_ListCtrlTarget.InsertColumn(LINKAGE_TYPE, _T("Type"), LVCFMT_LEFT, 70);
	m_ListCtrlTarget.InsertColumn(LINKAGE_COMMENT, _T("Comments ..."), LVCFMT_LEFT, 175);
	m_ListCtrlTarget.InsertColumn(LINKAGE_TIME, _T("Last Time"), LVCFMT_CENTER, 130);
	m_ListCtrlTarget.InsertColumn(LINKAGE_CATEGORY, _T("Category"), LVCFMT_CENTER, 60);

	TCHAR szINIFileName[MAX_PATH];
	CString Inifile;
	ZeroMemory(szINIFileName, sizeof(szINIFileName));
	bool bNeedExport = false;

	DWORD dwSize = MAX_PATH;
	if (CommonTool.ReadRegistry(HKEY_CURRENT_USER, REG_LATEST_OPEN_INI_FILE, REG_NAME, szINIFileName, &dwSize, REG_SZ))
	{
		Inifile = szINIFileName;
		HANDLE hFile = CreateFile(szINIFileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			bNeedExport = true;
		else
			CloseHandle(hFile);
	}
	else
	{
		bNeedExport = true;
	}

	// Check if Default.INI exists in the application directory
	if (bNeedExport)
	{
		TCHAR szAppPath[MAX_PATH];
		GetModuleFileName(NULL, szAppPath, MAX_PATH);
		PathRemoveFileSpec(szAppPath);
		PathAppend(szAppPath, _T("Default.INI"));

		HANDLE hDefaultFile = CreateFile(szAppPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hDefaultFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hDefaultFile);
			Inifile = szAppPath; // Use Default.INI if it exists

			// Write the Default.INI path to the registry
			CommonTool.WriteRegistry(HKEY_CURRENT_USER, REG_LATEST_OPEN_INI_FILE, REG_NAME, Inifile.GetBuffer(), REG_SZ);

			bNeedExport = false;
		}
	}

	if (bNeedExport)
	{
		TCHAR szFileName[MAX_PATH];
		wcscpy_s(szFileName, theApp.m_pszHelpFilePath);
		*(wcsrchr(szFileName, L'\\') + 1) = L'\0';
		wcscat_s(szFileName, _T("Default.INI"));

		CommonTool.ExportFileFromRec(_T("DEFALUTINIFILE"), szFileName);
		CommonTool.WriteRegistry(HKEY_CURRENT_USER, REG_LATEST_OPEN_INI_FILE, REG_NAME, szFileName, REG_SZ);
		Inifile = szFileName;
	}

	if (!m_IniFile.SetIniFile(Inifile))
	{
		MessageBox(_T("Can not get data from INI file"), _T("Error"), MB_OK);
		PostQuitMessage(1);
		return FALSE;
	}

	if (!FillLinkageListFromIniFile())
	{
		MessageBox(_T("Can not initialize the data list from Ini file"), _T("Error"), MB_OK);
		PostQuitMessage(1);
		return FALSE;
	}

	m_ComboFilterType.SetCurSel(FILETER_TYPE_ALL);
	m_ComboCategory.SetCurSel(FILETER_CATEGORY_ALL);

	if (!RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel()))
	{
		MessageBox(_T("Can not initialize the UI list with data"), _T("Error"), MB_OK);
		PostQuitMessage(1);
		return FALSE;
	}

	m_EditUpdate.EnableWindow(FALSE);
	ResetSortOrder();

	if (CommonTool.IsAutoRun())
		m_CheckAutoRun.SetCheck(BST_CHECKED);
	else
		m_CheckAutoRun.SetCheck(BST_UNCHECKED);

	UINT iHotKey = 'R';
	if (!RegisterHotKey(GetSafeHwnd(), HIDE_SHOW_DIAGLOG, MOD_ALT | MOD_CONTROL, iHotKey))
	{
		MessageBox(_T("Hot Key Register Failed!"), _T("Error"), MB_OK);
	}

	return TRUE;
}

void CSpyToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSpyToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CSpyToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CSpyToolDlg::FillLinkageListFromIniFile(void)
{
	m_LinkageItemList.clear();
	LINK_SET linkdata;
	int iCount = m_IniFile.GetCount(TYPE_ALL);

	for (int i = 1; i <= iCount; i++)
	{
		memset(&linkdata, 0, sizeof(LINK_SET));
		if (!m_IniFile.GetData(i, &linkdata))
		{
			break;
		}
		m_LinkageItemList.push_back(linkdata);
	}

	return true;
}

bool CSpyToolDlg::RefreshList(int iType, int iCategory)
{
	m_ListCtrlTarget.DeleteAllItems();
	CString strSelectCategory;

	switch (iCategory)
	{
	case FILETER_CATEGORY_WORK: strSelectCategory = _T("Work"); break;
	case FILETER_CATEGORY_FUN: strSelectCategory = _T("Fun"); break;
	case FILETER_CATEGORY_PRIVATE: strSelectCategory = _T("Private"); break;
	case FILETER_CATEGORY_LEARN: strSelectCategory = _T("Learn"); break;
	default: strSelectCategory = _T("All"); break;
	}

	int iCount = 0;
	for (auto& link : m_LinkageItemList)
	{
		bool bCategoryMatch = (iCategory == FILETER_CATEGORY_ALL) || (_wcsicmp(link.szCategory, strSelectCategory) == 0);
		if (!bCategoryMatch) continue;

		bool bTypeMatch = false;
		switch (iType)
		{
		case FILETER_TYPE_ALL:
			bTypeMatch = true;
			break;
		case FILETER_TYPE_URL:
			bTypeMatch = (_wcsicmp(link.szType, _T("Web-URL")) == 0);
			break;
		case FILETER_TYPE_DOC:
			bTypeMatch = (_wcsicmp(link.szType, _T("Document")) == 0);
			break;
		case FILETER_TYPE_OTHER:
			bTypeMatch = (_wcsicmp(link.szType, _T("Executable")) == 0) ||
				(_wcsicmp(link.szType, _T("Directory")) == 0) ||
				(_wcsicmp(link.szType, _T("Shortcut")) == 0);
			break;
		}

		if (bTypeMatch)
		{
			m_ListCtrlTarget.InsertItem(iCount, link.szName);
			m_ListCtrlTarget.SetItemText(iCount, LINKAGE_TYPE, link.szType);
			m_ListCtrlTarget.SetItemText(iCount, LINKAGE_COMMENT, link.szComments);
			m_ListCtrlTarget.SetItemText(iCount, LINKAGE_TIME, link.szTime);
			m_ListCtrlTarget.SetItemText(iCount, LINKAGE_CATEGORY, link.szCategory);
			iCount++;
		}
	}
	return true;
}

void CSpyToolDlg::OnLvnItemchangedListTarget(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	POSITION pos = m_ListCtrlTarget.GetFirstSelectedItemPosition();

	if (pos)
	{
		CString szName;
		int nItem = m_ListCtrlTarget.GetNextSelectedItem(pos);
		szName = m_ListCtrlTarget.GetItemText(nItem, LINKAGE_NAME);

		for (auto& link : m_LinkageItemList)
		{
			if (_wcsicmp(link.szName, szName) == 0)
			{
				m_EditLinkage.SetWindowText(link.szLinkage);
				m_EditUser.SetWindowText(link.szUSER);
				m_EditPassword.SetWindowText(link.szPSW);
				break;
			}
		}
		m_EditUpdate.EnableWindow(TRUE);
	}
	else
	{
		m_EditUpdate.EnableWindow(FALSE);
	}

	*pResult = 0;
}

void CSpyToolDlg::OnNMDblclkListTarget(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POSITION pos = m_ListCtrlTarget.GetFirstSelectedItemPosition();

	if (!pos)
	{
		*pResult = 0;
		return;
	}

	int nItem = m_ListCtrlTarget.GetNextSelectedItem(pos);
	CString szName = m_ListCtrlTarget.GetItemText(nItem, LINKAGE_NAME);

	for (auto& link : m_LinkageItemList)
	{
		if (_wcsicmp(link.szName, szName) == 0)
		{
			CTime t = CTime::GetCurrentTime();
			CString strTime;
			strTime.Format(_T("%d-%02d-%02d; %02d:%02d:%02d"),
				t.GetYear(), t.GetMonth(), t.GetDay(),
				t.GetHour(), t.GetMinute(), t.GetSecond());
			wcscpy_s(link.szTime, strTime);

			m_ListCtrlTarget.SetItemText(nItem, LINKAGE_TIME, strTime);
			SaveList2InfFile();

			if (_wcsicmp(link.szType, _T("Executable")) == 0)
			{
				STARTUPINFO si = { sizeof(si) };
				PROCESS_INFORMATION pi = { 0 };
				si.wShowWindow = SW_SHOWNORMAL;
				si.dwFlags = STARTF_USESHOWWINDOW;

				if (!CreateProcess(link.szLinkage, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
				{
					MessageBox(_T("Can not launch the exe file"), _T("Error"), MB_OK);
				}
				else
				{
					WaitForInputIdle(pi.hProcess, INFINITE);
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
			}
			else
			{
				HINSTANCE hIns = ShellExecute(NULL, _T("open"), link.szLinkage, NULL, NULL, SW_SHOWNORMAL);
				//if ((int)hIns <= 32)
				if (reinterpret_cast<INT_PTR>(hIns) <= 32)
				{
					if (_wcsicmp(link.szType, _T("Web-URL")) == 0)
					{
						CString szHttp = _T("http://") + CString(link.szLinkage);
						hIns = ShellExecute(NULL, _T("open"), szHttp, NULL, NULL, SW_SHOWNORMAL);
						//if ((int)hIns <= 32)
						if (reinterpret_cast<INT_PTR>(hIns) <= 32)
						{
							CString szHttps = _T("https://") + CString(link.szLinkage);
							hIns = ShellExecute(NULL, _T("open"), szHttps, NULL, NULL, SW_SHOWNORMAL);
							//if ((int)hIns <= 32)
							if (reinterpret_cast<INT_PTR>(hIns) <= 32)
								MessageBox(_T("Can not launch URL"), _T("Error"), MB_OK);
						}
					}
					else
					{
						MessageBox(_T("Can not launch the shell application"), _T("Error"), MB_OK);
					}
				}
			}
			break;
		}
	}
	*pResult = 0;
}

void CSpyToolDlg::OnBnClickedButtonFolder()
{
	POSITION pos = m_ListCtrlTarget.GetFirstSelectedItemPosition();
	if (!pos) return;

	CString szName = m_ListCtrlTarget.GetItemText(m_ListCtrlTarget.GetNextSelectedItem(pos), LINKAGE_NAME);

	for (auto& link : m_LinkageItemList)
	{
		if (_wcsicmp(link.szName, szName) == 0)
		{
			if (_wcsicmp(link.szType, _T("Executable")) == 0 ||
				_wcsicmp(link.szType, _T("Document")) == 0 ||
				_wcsicmp(link.szType, _T("Shortcut")) == 0)
			{
				CString szPath = link.szLinkage;
				int nPos = szPath.ReverseFind('\\');
				if (nPos != -1)
				{
					szPath = szPath.Left(nPos + 1);
					ShellExecute(NULL, _T("open"), szPath, NULL, NULL, SW_SHOWNORMAL);
				}
			}
			break;
		}
	}
}

void CSpyToolDlg::OnBnClickedButtonEdit()
{
	CItemEditDlg EditDlg;
	EditDlg.m_pItemList = &m_LinkageItemList;

	POSITION pos = m_ListCtrlTarget.GetFirstSelectedItemPosition();
	if (!pos)
	{
		MessageBox(_T("You must select one item to edit?"), _T("Error"), MB_OK);
		return;
	}

	CString szName = m_ListCtrlTarget.GetItemText(m_ListCtrlTarget.GetNextSelectedItem(pos), LINKAGE_NAME);
	wcscpy_s(EditDlg.m_szName, szName);

	if (EditDlg.DoModal() == IDOK)
	{
		SaveList2InfFile();
		RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel());
	}
}

void CSpyToolDlg::OnBnClickedButtonDel()
{
	POSITION pos = m_ListCtrlTarget.GetFirstSelectedItemPosition();
	if (!pos)
	{
		MessageBox(_T("Please select one item to be deleted"), _T("Error"), MB_OK);
		return;
	}

	if (MessageBox(_T("Are you sure to delete the item you selected?"), _T("Confirmation"), MB_YESNO) == IDNO)
		return;

	CString szName = m_ListCtrlTarget.GetItemText(m_ListCtrlTarget.GetNextSelectedItem(pos), LINKAGE_NAME);

	for (auto it = m_LinkageItemList.begin(); it != m_LinkageItemList.end(); ++it)
	{
		if (_wcsicmp(it->szName, szName) == 0)
		{
			m_LinkageItemList.erase(it);
			break;
		}
	}

	SaveList2InfFile();
	RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel());
}

void CSpyToolDlg::OnBnClickedButtonLoad()
{
	TCHAR szFilePath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };

	CString szInitDir = theApp.m_pszHelpFilePath;
	szInitDir = szInitDir.Left(szInitDir.ReverseFind('\\'));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrFile = szFilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = _T("INI File(*.INI)\0*.INI\0\0");
	ofn.lpstrFileTitle = szFileName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = szInitDir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if (!GetOpenFileName(&ofn)) return;

	if (m_IniFile.SetIniFile(szFilePath))
	{
		m_LinkageItemList.clear();
		FillLinkageListFromIniFile();
		RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel());

		CCommon CommonTool;
		CommonTool.WriteRegistry(HKEY_CURRENT_USER, REG_LATEST_OPEN_INI_FILE, REG_NAME, szFilePath, REG_SZ);
	}
}

bool CSpyToolDlg::SaveList2InfFile(void)
{
	int i = 1;
	for (auto& link : m_LinkageItemList)
	{
		m_IniFile.InsertData(i++, &link);
	}
	return true;
}

void CSpyToolDlg::OnCbnSelchangeComboFilterType()
{
	RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel());
}

void CSpyToolDlg::OnLvnColumnclickListTarget(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int colIndex = pNMLV->iSubItem;

	if (m_SortCol == colIndex)
		m_Ascending = !m_Ascending;
	else
	{
		m_SortCol = colIndex;
		m_Ascending = true;
	}

	PARAMSORT paramsort(m_ListCtrlTarget.GetSafeHwnd(), m_SortCol, m_Ascending);
	ListView_SortItemsEx(m_ListCtrlTarget.GetSafeHwnd(), SortFunc, &paramsort);
	*pResult = 0;
}

void CSpyToolDlg::ResetSortOrder(void)
{
	m_Ascending = true;
	m_SortCol = -1;
}

void CSpyToolDlg::OnBnClickedCheckAutoRun()
{
	CCommon CommonTool;
	CommonTool.SetAutoRun(m_CheckAutoRun.GetCheck() == BST_CHECKED);
}

void CSpyToolDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (nHotKeyId == HIDE_SHOW_DIAGLOG)
	{
		// BOOL bVisible = IsWindowVisible(GetSafeHwnd());
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
			ShowWindow(SW_HIDE);
		else
		{
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CSpyToolDlg::OnBnClickedButtonAdd()
{
	CItemAddDlg AddDlg;
	AddDlg.m_pItemList = &m_LinkageItemList;

	if (AddDlg.DoModal() == IDOK)
	{
		SaveList2InfFile();
		RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel());
	}
}

void CSpyToolDlg::OnCbnSelchangeComboFilterCategory()
{
	RefreshList(m_ComboFilterType.GetCurSel(), m_ComboCategory.GetCurSel());
}