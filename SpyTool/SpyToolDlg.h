
// SpyToolDlg.h : header file
//
#pragma once
#include "afxcmn.h"
#include "Defination.h"
#include "IniFileOper.h"

//std namespace for linkage list.
#include <list>
#include <algorithm>
#include <functional>
#include "afxwin.h"

#define HIDE_SHOW_DIAGLOG  166  
#define HOTKEY_CONT_ALT_R  0x52


using namespace std;



typedef enum  type_Link_List
{
	LINKAGE_NAME = 0,
	LINKAGE_TYPE,
	LINKAGE_COMMENT,
	LINKAGE_TIME,
	LINKAGE_CATEGORY, 
} Link_List_Control_Column;


typedef enum  type_filter_List
{
	FILETER_TYPE_ALL = 0,
	FILETER_TYPE_URL,
	FILETER_TYPE_DOC,
	FILETER_TYPE_OTHER, //exe, directory, link 
} Link_List_Filter;


typedef enum  type_filter_Category
{
	FILETER_CATEGORY_ALL = 0,
	FILETER_CATEGORY_WORK,
	FILETER_CATEGORY_FUN,
	FILETER_CATEGORY_PRIVATE, //exe, directory, link 
	FILETER_CATEGORY_LEARN,
} Link_Category_Filter;

// CSpyToolDlg dialog
class CSpyToolDlg : public CDialogEx
{
// Construction
public:
	CSpyToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SPYTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	CListCtrl m_ListCtrlTarget;
	CIniFileOper m_IniFile;
	std::list<LINK_SET> m_LinkageItemList;


	//std::list<LINK_SET>::iterator begin, end;


	bool FillLinkageListFromIniFile(void);
	bool RefreshList(int iType = 0,  int iCategory = 0);
public:
	afx_msg void OnLvnItemchangedListTarget(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTarget(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CEdit m_EditLinkage;
	// User name for specific web
	CEdit m_EditUser;
	// Password for URL
	CEdit m_EditPassword;

	bool m_Ascending;
	int  m_SortCol;


public:
		afx_msg void OnBnClickedButtonFolder();
		afx_msg void OnBnClickedButtonEdit();
		afx_msg void OnBnClickedButtonDel();
		afx_msg void OnBnClickedButtonLoad();
private:
	// Save the data to Inf file
	bool SaveList2InfFile(void);
	CComboBox m_ComboFilterType;
public:
	afx_msg void OnCbnSelchangeComboFilterType();
	afx_msg void OnLvnColumnclickListTarget(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void ResetSortOrder(void);
	CButton m_CheckAutoRun;
public:
	afx_msg void OnBnClickedCheckAutoRun();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedButtonAdd();
private:
	CButton m_EditUpdate;
	CComboBox m_ComboCategory;
public:
	afx_msg void OnCbnSelchangeComboFilterCategory();
};
