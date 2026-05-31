
#pragma once

#include <list>
#include <algorithm>
#include <functional>
#include "afxwin.h"

// CItemEditDlg dialog

class CItemEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CItemEditDlg)

public:
	CItemEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CItemEditDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonBrower();

private:

	CComboBox m_ComboType;
	CEdit m_EditLink;
	CEdit m_EditName;
	CEdit m_EditValue;
	CEdit m_EditUser;
	CEdit m_EditPsw;

public:
	
	TCHAR m_szName[200];
	std::list<LINK_SET> * m_pItemList;

	afx_msg void OnCbnSelchangeComboType();
	CButton m_BtnBrower;
private:
	CButton m_BtnUpdate;
public:
	afx_msg void OnBnClickedButtonUpdata();
	afx_msg void OnEnChangeEditLink();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnEnChangeEditValue();
	afx_msg void OnEnChangeEditUser();
	afx_msg void OnEnChangeEditPsw();
	afx_msg void OnBnClickedOk();
private:
	CComboBox m_ComBoCategory;
public:
	afx_msg void OnCbnSelchangeComboCategory();
};
