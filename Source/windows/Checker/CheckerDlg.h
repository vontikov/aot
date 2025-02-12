// CheckerDlg.h : header file
//

#if !defined(AFX_CHECKERDLG_H__506BF516_3AE1_11D5_92A5_005004035BFC__INCLUDED_)
#define AFX_CHECKERDLG_H__506BF516_3AE1_11D5_92A5_005004035BFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4786)
#include "morph_dict/common/utilit.h"
#include "windows/common/COMSyntaxHolder.h"
#include "afxwin.h"
#include "list"

/////////////////////////////////////////////////////////////////////////////
//#include <stdio.h>

struct CBase
{
	int    num;
	std::string txt;

	CBase() : num(0),txt("") {}

	bool operator == (const CBase& X) const
	{
		return( num==X.num || txt==X.txt );
	}

	bool operator < (const CBase& X) const
	{
		if( num==X.num )
			return( txt<X.txt );
		else
			return( num<X.num );
	}
};

struct CSourceExample
{
	std::string	m_Text;
	std::string	m_Comments;

};
/////////////////////////////////////////////////////////////////////////////
// CCheckerDlg dialog

enum CheckerTypeEnum {SentenceBreakerCheck, SyntaxCheck, TranslatorCheck, 
	RusSynthesisCheck, PredictionCheck, 
	ExternalExeFileCheck, PostMorphCheck}; 
class CCheckerDlg : public CDialog
{
// Construction
public:
	CFont Font;
	CheckerTypeEnum m_CheckerType;
	CCheckerDlg(CWnd* pParent = NULL);	// standard constructor
	~CCheckerDlg();
	bool	m_bLoadCOM;
	std::list<CString>				m_LastQueries;
	bool	m_bResultsAreSmall;

// Dialog Data
	//{{AFX_DATA(CCheckerDlg)
	enum { IDD = IDD_CHECKER_DIALOG };
	//CString	m_file;
	int		m_from;
	int		m_numb;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;
	std::string m_CommandLine;
	std::string m_ExternalOutFile;
	std::string m_ExternalCatalog;
// Data
public:
	MorphLanguageEnum				m_Language;
	SEMANLib::ISemStructurePtr piSeman;
	CCOMSyntaxHolder m_RusSyntaxHolder;
	CCOMSyntaxHolder m_GerSyntaxHolder;

	

	std::vector<CSourceExample> m_CheckExamples;

	std::vector<CBase>  m_base;
	BOOL m_changed;

	StringVector m_tran;
	std::vector<int>    m_nbad;

// Implementation
	BOOL InitCom();
	void ExitCom();

	BOOL LoadData();
	BOOL LoadBase();
	
	BOOL Operate();
	void Proceed();

	BOOL SaveBase();
	std::string GetBaseFileName() const;
	bool PerformExternalExeFileCheck(std::vector<std::string>& Result);
	int		GetNumberOfExamplesToCheck() const;
	void	ChangeHistory(const CString& query);
	CString GetCurrFileName() const;
	void	LoadHistory();
	void	SaveHistory();

// Implementation
protected:
	// Generated message  functions
	//{{AFX_MSG(CCheckerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDeleteBase();
	CComboBox m_comboHistory;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKERDLG_H__506BF516_3AE1_11D5_92A5_005004035BFC__INCLUDED_)
