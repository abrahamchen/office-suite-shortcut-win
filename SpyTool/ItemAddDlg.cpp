// ItemAddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpyTool.h"
#include "Defination.h"
#include "ItemAddDlg.h"
#include "afxdialogex.h"
#include <shlobj.h>

// Item Type Definitions
#define COMBO_URL      0
#define COMBO_DOC      1
#define COMBO_EXE      2
#define COMBO_LNK      3
#define COMBO_DIR      4

// Category Definitions
#define COMBO_WORK     0
#define COMBO_FUN      1
#define COMBO_PRIVATE  2
#define COMBO_LEARN    3

// CItemAddDlg dialog
IMPLEMENT_DYNAMIC(CItemAddDlg, CDialogEx)

CItemAddDlg::CItemAddDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CItemAddDlg::IDD, pParent)
{
}

CItemAddDlg::~CItemAddDlg()
{
}

void CItemAddDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_COMBO_TYPE_ADD, m_ComboType);
    DDX_Control(pDX, IDC_COMBO_CATEGORY_ADD, m_ComboCategory);
    DDX_Control(pDX, IDC_EDIT_LINK_ADD, m_EditLink);
    DDX_Control(pDX, IDC_EDIT_NAME_ADD, m_EditName);
    DDX_Control(pDX, IDC_EDIT_VALUE_ADD, m_EditComments);
    DDX_Control(pDX, IDC_EDIT_USER_ADD, m_EditUser);
    DDX_Control(pDX, IDC_EDIT_PSW_ADD, m_EditPsw);
    DDX_Control(pDX, IDC_BUTTON_BROWER_ADD, m_BtnBrower);
}

BEGIN_MESSAGE_MAP(CItemAddDlg, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &CItemAddDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_ADD_ITEM, &CItemAddDlg::OnBnClickedButtonAddItem)
    ON_BN_CLICKED(IDC_BUTTON_CLEAN, &CItemAddDlg::OnBnClickedButtonClean)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE_ADD, &CItemAddDlg::OnCbnSelchangeComboTypeAdd)
    ON_BN_CLICKED(IDC_BUTTON_BROWER_ADD, &CItemAddDlg::OnBnClickedButtonBrowerAdd)
END_MESSAGE_MAP()

BOOL CItemAddDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Initialize combo boxes with prompt text
    m_ComboType.SetCurSel(-1);
    m_ComboType.SetWindowText(_T("Selection"));

    m_ComboCategory.SetCurSel(-1);
    m_ComboCategory.SetWindowText(_T("Selection"));

    // Disable browse button initially
    m_BtnBrower.EnableWindow(FALSE);

    return TRUE;
}

// Cancel Button
void CItemAddDlg::OnBnClickedCancel()
{
    CDialogEx::OnCancel();
}

// Add Item Button
void CItemAddDlg::OnBnClickedButtonAddItem()
{
    LINK_SET link = { 0 };

    // Validate Item Type
    int iSel = m_ComboType.GetCurSel();
    if (iSel == LB_ERR)
    {
        MessageBox(_T("You must select a type!"), _T("Error"), MB_ICONERROR | MB_OK);
        return;
    }

    // Assign Type String
    switch (iSel)
    {
    case COMBO_URL:   wcscpy_s(link.szType, _T("Web-URL"));    break;
    case COMBO_DOC:   wcscpy_s(link.szType, _T("Document"));   break;
    case COMBO_EXE:   wcscpy_s(link.szType, _T("Executable")); break;
    case COMBO_LNK:   wcscpy_s(link.szType, _T("Shortcut"));   break;
    case COMBO_DIR:   wcscpy_s(link.szType, _T("Directory"));  break;
    default:          wcscpy_s(link.szType, _T("UKN"));       break;
    }

    // Validate Category
    iSel = m_ComboCategory.GetCurSel();
    if (iSel == LB_ERR)
    {
        MessageBox(_T("You must select a category!"), _T("Error"), MB_ICONERROR | MB_OK);
        return;
    }

    // Assign Category String
    switch (iSel)
    {
    case COMBO_WORK:     wcscpy_s(link.szCategory, _T("Work"));     break;
    case COMBO_FUN:      wcscpy_s(link.szCategory, _T("Fun"));      break;
    case COMBO_PRIVATE:  wcscpy_s(link.szCategory, _T("Private"));  break;
    case COMBO_LEARN:    wcscpy_s(link.szCategory, _T("Learn"));    break;
    default:             wcscpy_s(link.szCategory, _T("UKN"));       break;
    }

    // Validate Name
    if (m_EditName.GetWindowText(link.szName, _countof(link.szName)) <= 0)
    {
        MessageBox(_T("You must enter a name!"), _T("Error"), MB_ICONERROR | MB_OK);
        m_EditName.SetFocus();
        return;
    }

    // Validate Link/Path
    if (m_EditLink.GetWindowText(link.szLinkage, _countof(link.szLinkage)) <= 0)
    {
        MessageBox(_T("You must enter a link or path!"), _T("Error"), MB_ICONERROR | MB_OK);
        m_EditLink.SetFocus();
        return;
    }

    // Get optional fields
    m_EditComments.GetWindowText(link.szComments, _countof(link.szComments));
    m_EditUser.GetWindowText(link.szUSER, _countof(link.szUSER));
    m_EditPsw.GetWindowText(link.szPSW, _countof(link.szPSW));

    // Check for duplicate names (SAFE iteration)
    if (m_pItemList && !m_pItemList->empty())
    {
        for (const auto& item : *m_pItemList)
        {
            if (_wcsicmp(item.szName, link.szName) == 0)
            {
                MessageBox(_T("This name already exists! Please use a unique name."),
                    _T("Error"), MB_ICONERROR | MB_OK);
                m_EditName.SetFocus();
                return;
            }
        }
    }

    /*
    // Set current timestamp
    CTime currentTime = CTime::GetCurrentTime();
    currentTime.Format(link.szTime, _T("%Y-%m-%d %H:%M:%S"));
    */
    // Set current timestamp (100% fixed, no errors, no warnings)
    CTime currentTime = CTime::GetCurrentTime();
    CString strTime = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
    wcscpy_s(link.szTime, _countof(link.szTime), strTime);


    // Insert into list
    if (m_pItemList)
    {
        m_pItemList->push_back(link);
    }

    CDialogEx::OnOK();
}

// Clear All Fields
void CItemAddDlg::OnBnClickedButtonClean()
{
    m_ComboType.SetCurSel(-1);
    m_ComboType.SetWindowText(_T("Selection"));

    m_ComboCategory.SetCurSel(-1);
    m_ComboCategory.SetWindowText(_T("Selection"));

    m_EditName.SetWindowText(_T(""));
    m_EditLink.SetWindowText(_T(""));
    m_EditComments.SetWindowText(_T(""));
    m_EditUser.SetWindowText(_T(""));
    m_EditPsw.SetWindowText(_T(""));

    m_EditName.SetFocus();
}

// Type Combo Selection Changed
void CItemAddDlg::OnCbnSelchangeComboTypeAdd()
{
    int iSel = m_ComboType.GetCurSel();

    // Enable Browse only for file/system types
    BOOL enableBrowse = (iSel == COMBO_DOC || iSel == COMBO_EXE ||
        iSel == COMBO_LNK || iSel == COMBO_DIR);

    m_BtnBrower.EnableWindow(enableBrowse);
}

// Browse Button Handler
void CItemAddDlg::OnBnClickedButtonBrowerAdd()
{
    int iSel = m_ComboType.GetCurSel();
    TCHAR szFilePath[MAX_PATH] = { 0 };
    TCHAR szFileName[MAX_PATH] = { 0 };

    if (iSel == COMBO_DIR)
    {
        // Browse for Folder
        BROWSEINFO bi = { 0 };
        bi.hwndOwner = GetSafeHwnd();
        bi.pszDisplayName = szFileName;
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

        LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
        if (pidl)
        {
            if (SHGetPathFromIDList(pidl, szFilePath))
            {
                m_EditLink.SetWindowText(szFilePath);

                // Extract folder name
                CString strPath = szFilePath;
                int idx = strPath.ReverseFind(_T('\\'));
                if (idx != -1)
                {
                    m_EditName.SetWindowText(strPath.Mid(idx + 1));
                }
            }
            CoTaskMemFree(pidl);
        }
        return;
    }

    // Browse for Files
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetSafeHwnd();
    ofn.lpstrFile = szFilePath;
    ofn.nMaxFile = _countof(szFilePath);
    ofn.lpstrFileTitle = szFileName;
    ofn.nMaxFileTitle = _countof(szFileName);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    // Set filters per type
    switch (iSel)
    {
    case COMBO_DOC:
        ofn.lpstrFilter = _T("All Files\0*.*\0");
        ofn.lpstrTitle = _T("Select Document");
        break;
    case COMBO_EXE:
        ofn.lpstrFilter = _T("Executable Files\0*.exe\0");
        ofn.lpstrTitle = _T("Select Executable");
        break;
    case COMBO_LNK:
        ofn.lpstrFilter = _T("Shortcut Files\0*.lnk\0");
        ofn.lpstrTitle = _T("Select Shortcut");
        break;
    default:
        return;
    }

    if (GetOpenFileName(&ofn))
    {
        m_EditLink.SetWindowText(szFilePath);
        m_EditName.SetWindowText(szFileName);
    }
}