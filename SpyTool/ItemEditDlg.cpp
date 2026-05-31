// ItemEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpyTool.h"
#include "Defination.h"
#include "ItemEditDlg.h"
#include "afxdialogex.h"

// CItemEditDlg dialog

IMPLEMENT_DYNAMIC(CItemEditDlg, CDialogEx)

CItemEditDlg::CItemEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CItemEditDlg::IDD, pParent)
{

}

CItemEditDlg::~CItemEditDlg()
{
}

void CItemEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ComboType);
	DDX_Control(pDX, IDC_EDIT_LINK, m_EditLink);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_EditValue);
	DDX_Control(pDX, IDC_EDIT_USER, m_EditUser);
	DDX_Control(pDX, IDC_EDIT_PSW, m_EditPsw);
	DDX_Control(pDX, IDC_BUTTON_BROWER, m_BtnBrower);
	DDX_Control(pDX, IDC_BUTTON_UPDATA, m_BtnUpdate);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_ComBoCategory);
}


BEGIN_MESSAGE_MAP(CItemEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CItemEditDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_BROWER, &CItemEditDlg::OnBnClickedButtonBrower)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CItemEditDlg::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA, &CItemEditDlg::OnBnClickedButtonUpdata)
	ON_EN_CHANGE(IDC_EDIT_LINK, &CItemEditDlg::OnEnChangeEditLink)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CItemEditDlg::OnEnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_VALUE, &CItemEditDlg::OnEnChangeEditValue)
	ON_EN_CHANGE(IDC_EDIT_USER, &CItemEditDlg::OnEnChangeEditUser)
	ON_EN_CHANGE(IDC_EDIT_PSW, &CItemEditDlg::OnEnChangeEditPsw)
	ON_BN_CLICKED(IDOK, &CItemEditDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, &CItemEditDlg::OnCbnSelchangeComboCategory)
END_MESSAGE_MAP()


// CItemEditDlg message handlers


#define COMBO_URL 0
#define COMBO_DOC 1
#define COMBO_EXE 2
#define COMBO_LNK 3
#define COMBO_DIR 4

#define COMBO_WORK 0
#define COMBO_FUN  1
#define COMBO_PRIVATE 2
#define COMBO_LEARN 3




void CItemEditDlg::OnBnClickedButtonNew()
{
	// TODO: Add your control notification handler code here
	
	
	m_ComboType.SetCurSel(-1);
	m_ComboType.SetWindowText(_T("Selection"));
	m_EditName.SetWindowText(_T(""));
	m_EditLink.SetWindowText(_T(""));
	m_EditValue.SetWindowText(_T(""));
	m_EditUser.SetWindowText(_T(""));
	m_EditPsw.SetWindowText(_T(""));


}


void CItemEditDlg::OnBnClickedButtonBrower()
{
	// TODO: Add your control notification handler code here
	
	TCHAR szFilePath[MAX_PATH + 1] = {0};
	TCHAR szFileName[MAX_PATH + 1] = {0};
	TCHAR szFileInitName[MAX_PATH + 1] = {0};
	OPENFILENAME ofn = {0};
	
	int iSel = m_ComboType.GetCurSel();

	wcscpy_s(szFileInitName,  AfxGetApp()->m_pszHelpFilePath);
	* (wcsrchr(szFileInitName, L'\\')) = L'\0';


	if (iSel == COMBO_DOC)
	{
		/*
		CFileDialog FileTarget(TRUE, _T("Documents"), NULL, OFN_OVERWRITEPROMPT, 
		TEXT("Target File (*.*)|*.doc||"), this);
		if(IDOK == FileTarget.DoModal())
		{
		CString pPathName;
		pPathName = FileTarget.GetPathName();
		m_EditLink.SetWindowText(pPathName);
		m_EditName.SetWindowText(wcsrchr(pPathName, _T('\\')) + 1); 
		}
		*/
		
		ofn.lStructSize     = sizeof(OPENFILENAME);
		ofn.hwndOwner       = GetSafeHwnd();
		ofn.lpstrFile       = szFilePath;
		ofn.nMaxFile        = sizeof(TCHAR) * MAX_PATH;
		ofn.lpstrFilter     = _T("Document File (*.*)\0*.*\0\0");
		ofn.lpstrFileTitle  = szFileName;
		ofn.nMaxFileTitle   = sizeof(TCHAR) * MAX_PATH;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrTitle      = _T("Targeted Document File");
		if(0 == ::GetOpenFileName(&ofn))
			return;

		m_EditLink.SetWindowText(ofn.lpstrFile);
		m_EditName.SetWindowText(wcsrchr(ofn.lpstrFile, _T('\\')) + 1); 
		

	} else if (iSel == COMBO_EXE)
	{

		ofn.lStructSize     = sizeof(OPENFILENAME);
		ofn.hwndOwner       = GetSafeHwnd();
		ofn.lpstrFile       = szFilePath;
		ofn.nMaxFile        = sizeof(TCHAR) * MAX_PATH;
		ofn.lpstrFilter     = _T("Document File (*.EXE)\0*.EXE\0\0");
		ofn.lpstrFileTitle  = szFileName;
		ofn.nMaxFileTitle   = sizeof(TCHAR) * MAX_PATH;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrTitle      = _T("Executable File");
		if(0 == ::GetOpenFileName(&ofn))
			return;

		m_EditLink.SetWindowText(ofn.lpstrFile);
		m_EditName.SetWindowText(wcsrchr(ofn.lpstrFile, _T('\\')) + 1); 
		
	} else if (iSel == COMBO_LNK)
	{
		ofn.lStructSize     = sizeof(OPENFILENAME);
		ofn.hwndOwner       = GetSafeHwnd();
		ofn.lpstrFile       = szFilePath;
		ofn.nMaxFile        = sizeof(TCHAR) * MAX_PATH;
		ofn.lpstrFilter     = _T("Document File (*.LNK)\0*.LNK\0\0");
		ofn.lpstrFileTitle  = szFileName;
		ofn.nMaxFileTitle   = sizeof(TCHAR) * MAX_PATH;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrTitle      = _T("Executable Link File");
		if(0 == ::GetOpenFileName(&ofn))
			return;

		m_EditLink.SetWindowText(ofn.lpstrFile);
		m_EditName.SetWindowText(wcsrchr(ofn.lpstrFile, _T('\\')) + 1); 
		
	}	else if (iSel == COMBO_DIR)
	{
		BROWSEINFO lpbi;
		memset(&lpbi,0,sizeof(BROWSEINFO));
		lpbi.hwndOwner = this->GetSafeHwnd();
		TCHAR cDisplayName[MAX_PATH] = L"Abc";
		
		lpbi.pszDisplayName = cDisplayName;
		LPITEMIDLIST lplFolderData = SHBrowseForFolder(&lpbi);
		if (lplFolderData!= NULL)
		{
			TCHAR * pRootPath = new TCHAR[MAX_PATH];
			SHGetPathFromIDList(lplFolderData, pRootPath);
			m_EditLink.SetWindowText(pRootPath);
			m_EditName.SetWindowText(wcsrchr(pRootPath, _T('\\')) + 1); 
			delete pRootPath;		
		}

	}


}


BOOL CItemEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	std::list<LINK_SET>::iterator begin, end;
	begin = m_pItemList->begin();
	end = m_pItemList->end();

	if (wcslen(m_szName) > 0) //Item is selected
	{
		for (; begin!=end; begin++) //Scan the item list and initialize the control list -  URL, Doc, Exe
		{
			if (_wcsicmp(begin->szName, m_szName) == 0)
			{
				m_EditName.SetWindowText(begin->szName);
				m_EditLink.SetWindowText(begin->szLinkage);
				m_EditValue.SetWindowText(begin->szComments);
				m_EditUser.SetWindowText(begin->szUSER);
				m_EditPsw.SetWindowText(begin->szPSW);

				if (_wcsicmp(begin->szType, _T("Web-URL")) == 0)
					m_ComboType.SetCurSel(COMBO_URL);
				else if (_wcsicmp(begin->szType, _T("Document")) == 0)
					m_ComboType.SetCurSel(COMBO_DOC);
				else if (_wcsicmp(begin->szType, _T("Executable")) == 0)
					m_ComboType.SetCurSel(COMBO_EXE);
				else if (_wcsicmp(begin->szType, _T("Shortcut")) == 0)
					m_ComboType.SetCurSel(COMBO_LNK);
				else if (_wcsicmp(begin->szType, _T("Directory")) == 0)
					m_ComboType.SetCurSel(COMBO_DIR);
				else m_ComboType.SetCurSel(-1);

				if (_wcsicmp(begin->szCategory, _T("Work")) == 0)
					m_ComBoCategory.SetCurSel(COMBO_WORK);
				else if (_wcsicmp(begin->szCategory, _T("Fun")) == 0)
					m_ComBoCategory.SetCurSel(COMBO_FUN);
				else if (_wcsicmp(begin->szCategory, _T("Private")) == 0)
					m_ComBoCategory.SetCurSel(COMBO_PRIVATE);
				else if (_wcsicmp(begin->szCategory, _T("Learn")) == 0)
					m_ComBoCategory.SetCurSel(COMBO_LEARN);
				else 
					m_ComBoCategory.SetWindowText(_T("Selection"));

			}

		}
	
	} else 
	{
		m_ComboType.SetCurSel(-1);
		m_ComboType.SetWindowText(_T("Selection"));
		m_ComBoCategory.SetCurSel(-1);
		m_ComBoCategory.SetWindowText(_T("Selection"));
	}

	
	m_BtnUpdate.EnableWindow(FALSE);
	
	int iSel = m_ComboType.GetCurSel();

	if ((iSel == COMBO_DOC) || (iSel == COMBO_EXE) || (iSel == COMBO_LNK) || (iSel == COMBO_DIR))
		m_BtnBrower.EnableWindow();
	else m_BtnBrower.EnableWindow(FALSE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}


void CItemEditDlg::OnCbnSelchangeComboType()
{
	// TODO: Add your control notification handler code here

	int iSel = m_ComboType.GetCurSel();

	if ((iSel == COMBO_DOC) || (iSel == COMBO_EXE) || (iSel == COMBO_LNK) || (iSel == COMBO_DIR))
		m_BtnBrower.EnableWindow();
	else m_BtnBrower.EnableWindow(FALSE);

	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();


}


void CItemEditDlg::OnBnClickedButtonUpdata()
{
	// TODO: Add your control notification handler code here

	if (wcslen(m_szName) == 0) //Item is not selected
		return;

	std::list<LINK_SET>::iterator begin, end;
	begin = m_pItemList->begin();
	end = m_pItemList->end();


	for (; begin!=end; begin++) //Scan the item list and initialize the control list -  URL, Doc, Exe
	{
		if (_wcsicmp(begin->szName, m_szName) == 0)
		{
			LINK_SET link;
			memset(&link,  0,  sizeof(LINK_SET));
			int iSel = m_ComboType.GetCurSel();
			if (iSel == LB_ERR)
			{
				MessageBox(_T("You must select one type!"), _T("Error"), MB_OK);
				return;
			};

			if (iSel == COMBO_URL)
				wcscpy_s(link.szType, _T("Web-URL"));
			else if (iSel == COMBO_DOC)
				wcscpy_s(link.szType, _T("Document"));
			else if (iSel == COMBO_EXE)
				wcscpy_s(link.szType, _T("Executable"));
			else if (iSel == COMBO_LNK)
				wcscpy_s(link.szType, _T("Shortcut"));
			else if (iSel == COMBO_DIR)
				wcscpy_s(link.szType, _T("Directory"));
			else wcscpy_s(link.szType, _T("UKN"));

			iSel = m_ComBoCategory.GetCurSel();
			if (iSel == LB_ERR)
			{
				MessageBox(_T("You must select one type!"), _T("Error"), MB_OK);
				return;
			};

			if (iSel == COMBO_WORK)
				wcscpy_s(link.szCategory, _T("Work"));
			else if (iSel == COMBO_FUN)
				wcscpy_s(link.szCategory, _T("Fun"));
			else if (iSel == COMBO_PRIVATE)
				wcscpy_s(link.szCategory, _T("Private"));
			else if (iSel == COMBO_LEARN)
				wcscpy_s(link.szCategory, _T("Learn"));
			else 
				wcscpy_s(link.szCategory, _T("Unknow"));


			if (m_EditName.GetWindowText(link.szName, 50) <= 0 )
			{
				MessageBox(_T("You must enter Name!"), _T("Error"), MB_OK);
				return;
			};

			if (m_EditLink.GetWindowText(link.szLinkage, MAX_PATH) <= 0 )
			{
				MessageBox(_T("You must enter Link!"), _T("Error"), MB_OK);
				return;
			};

			m_EditValue.GetWindowText(link.szComments, 200);
			m_EditUser.GetWindowText(link.szUSER, 32);
			m_EditPsw.GetWindowText(link.szPSW, 24);

			wcscpy_s(begin->szLinkage,  link.szLinkage);
			wcscpy_s(begin->szName,  link.szName);
			wcscpy_s(begin->szPSW,  link.szPSW);
			wcscpy_s(begin->szType,  link.szType);
			wcscpy_s(begin->szUSER,  link.szUSER);
			wcscpy_s(begin->szComments,  link.szComments);
			wcscpy_s(begin->szCategory,  link.szCategory);
			
			CTime lastTime = CTime::GetCurrentTime();
			TCHAR szTimeDate[24];
			wsprintf(szTimeDate, _T("%d-%02d-%02d; %02d:%02d:%02d"), lastTime.GetYear(),  lastTime.GetMonth(), lastTime.GetDay(), lastTime.GetHour(), lastTime.GetMinute(), lastTime.GetSecond());
			wcscpy_s(begin->szTime, szTimeDate);

			
			wcscpy_s(m_szName, link.szName);

			m_BtnUpdate.EnableWindow(FALSE);
			
		}
	}



	CDialogEx::OnOK();

}


void CItemEditDlg::OnEnChangeEditLink()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();
}


void CItemEditDlg::OnEnChangeEditName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();

}


void CItemEditDlg::OnEnChangeEditValue()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();
}


void CItemEditDlg::OnEnChangeEditUser()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();

}


void CItemEditDlg::OnEnChangeEditPsw()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();
}


void CItemEditDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CItemEditDlg::OnCbnSelchangeComboCategory()
{
	// TODO: Add your control notification handler code here
	int iSel = m_ComBoCategory.GetCurSel();

	if ((iSel == COMBO_WORK) || (iSel == COMBO_FUN) || (iSel == COMBO_PRIVATE))
		m_BtnBrower.EnableWindow();
	else m_BtnBrower.EnableWindow(FALSE);

	if (wcslen(m_szName) > 0) //Item is selected
		m_BtnUpdate.EnableWindow();
}
