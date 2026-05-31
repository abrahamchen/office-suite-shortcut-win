#include <list>
#include <algorithm>
#include <functional>


#pragma once


// CItemAddDlg dialog

class CItemAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CItemAddDlg)

public:
	CItemAddDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CItemAddDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAddItem();
	afx_msg void OnBnClickedButtonClean();
	std::list<LINK_SET> * m_pItemList;
	
private:
	CComboBox m_ComboType;
	CComboBox m_ComboCategory;
	CEdit m_EditLink;
	CEdit m_EditName;
	CEdit m_EditComments;
	CEdit m_EditUser;
	CEdit m_EditPsw;
	CButton m_BtnBrower;

public:
	afx_msg void OnCbnSelchangeComboTypeAdd();
	afx_msg void OnBnClickedButtonBrowerAdd();
};
