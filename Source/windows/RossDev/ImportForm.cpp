// ImportForm.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"

#include "ImportForm.h"
#include "io.h"
#include "morph_dict/common/utilit.h"
#include "SchemeRossChoicer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// СImportForm dialog


CImportForm::CImportForm(CRossDoc* pRossDoc,
                          CWnd* pParent /*=NULL*/)
	: CDialog(CImportForm::IDD, pParent),
	  m_pRossDoc(pRossDoc)
	  
{
	//{{AFX_DATA_INIT(СImportForm)
	m_OverWrite = -1;
	m_Append = -1;
	m_Skip = 0;
	m_NumberOfArticle = _T("1");
	//}}AFX_DATA_INIT
	try { 
       std::string strPath = GetRegistryString( g_strRegLastFile );
	   m_FileName = _T(strPath.c_str());
	  }
	catch (...)
	 {
		m_FileName = _T("obor.dic");
	 };
}
CImportForm::~CImportForm()
{
};

void CImportForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(СImportForm)
	DDX_Control(pDX, IDC_HOTKEY1, m_HotKeys);
	DDX_Radio(pDX, IDC_RADIO1, m_OverWrite);
	DDX_Radio(pDX, IDC_RADIO2, m_Append);
	DDX_Radio(pDX, IDC_RADIO3, m_Skip);
	DDX_Text(pDX, IDC_EDIT2, m_FileName);
	DDX_Text(pDX, IDC_EDIT3, m_NumberOfArticle);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RICHEDIT21, m_MainRichEdit);
	DDX_Control(pDX, IDC_RICHEDIT22, m_ProtocolRichEdit);
}


BEGIN_MESSAGE_MAP(CImportForm, CDialog)
	//{{AFX_MSG_MAP(СImportForm)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_BUTTON1, OnRun)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// СImportForm message handlers

void CImportForm::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_OverWrite = 0;
    m_Append = 1;
	m_Skip = 1;
    UpdateData(FALSE);
}


void CImportForm::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_Append = 0;
    m_OverWrite = 1;
	m_Skip = 1;
    UpdateData(FALSE);
}

void CImportForm::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_Skip = 0;
    m_Append = 1;
	m_OverWrite = 1;
    UpdateData(FALSE);
	
}


void CImportForm::SendInfo (CString S, long LineNo)
{
	UpdateData (TRUE);
	S.Trim();
	CString Q;
	if (LineNo != -1)
		Q.Format(" (line %i)\r\n", LineNo);
	else
		Q.Format("\r\n");
	CString SS;
	m_ProtocolRichEdit.GetWindowText(SS);
	SS += S + Q;
	m_ProtocolRichEdit.SetWindowText(SS);
	UpdateData (FALSE);
};

inline bool SaveToFile(const CRichEditCtrl& C, CString FileName)
{
	FILE* fp = fopen(FileName, "wb");
	if (!fp) return false;
	CString S;
	C.GetWindowText(S);
	fprintf(fp, "%s", (const char*)S);
	fclose(fp);
	return true;
};



void CImportForm::OnRun() 
{
	// TODO: Add your control notification handler code here
	CWaitCursor C;


	UpdateData(TRUE);
	if  (    (m_OverWrite != 0)
		  && (m_Append != 0)
		  && (m_Skip != 0) )
	{
		AfxMessageBox ("You should choose how to solve entries conflict (\"overwrite\", \"add\" or \"skip\") ");
		return;
	};

	if (access(m_FileName, 04) != 0)
	{
		CString S;
		S.Format ("Cannot read  %s ", m_FileName);
		AfxMessageBox (S);
		return;
	};

	SaveToFile(m_MainRichEdit, m_FileName);

	m_ProtocolRichEdit.SetSel(0, -1);
	m_ProtocolRichEdit.Clear();

    m_pRossDoc->SetModifiedFlag(TRUE);


	std::string Messages;
	GetRoss()->ImportFromText(
		(const char*)m_FileName, 
		atoi (m_NumberOfArticle),
		Messages
		);
	m_ProtocolRichEdit.SetWindowText(Messages.c_str());
	UpdateData(FALSE);
}

inline bool LoadFromFile(CRichEditCtrl& C, CString FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp) return false;
	char buffer[1000];
	CString S;
	while (fgets(buffer, 1000, fp))
		S += buffer;
	fclose(fp);

	C.SetWindowText(S);
	return true;
}

//выбор файла
void CImportForm::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
   	CFileDialog D(TRUE, "txt", "ross.txt");
	if (D.DoModal() != IDOK) return;
	m_FileName = D.GetPathName();
	UpdateData(FALSE);
	LoadFromFile(m_MainRichEdit, m_FileName);

}



BOOL CImportForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
    LoadFromFile(m_MainRichEdit, m_FileName);
	return TRUE;
}


//сохранение файла 
void CImportForm::OnButton3() 
{
	UpdateData(TRUE);
	SaveToFile(m_MainRichEdit, m_FileName);
}



//сохранение файла 
void CImportForm::OnButton4() 
{
	// TODO: Add your control notification handler code here
   SchemeRossChoicer dlg (m_pRossDoc);
   dlg.DoModal();
   m_pRossDoc->BuildBasicDomItems();
}



IMPLEMENT_DYNCREATE(CProtocolCtrl, CRichEditCtrl)
BEGIN_MESSAGE_MAP(CProtocolCtrl, CRichEditCtrl)
		ON_WM_LBUTTONDBLCLK()
		ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void CProtocolCtrl::ShowError()
{
	CImportForm* V = (CImportForm*)GetParent();
	long nStart, nEnd;
	GetSel(nStart, nEnd);
	int LineNo = LineFromChar(nStart);
	
	char buffer[200];
	GetLine(LineNo,buffer, 200);
	CString S = buffer;
	long l = S.Find("(line");
	if (l != -1)
	{
		l+= 5;
		CString Q = S.Mid (l, 10); 
		long LineNo = atoi(Q);
		long nStartChar = V->m_MainRichEdit.LineIndex(LineNo);
		V->m_MainRichEdit.SetSel(nStartChar, nStartChar);
		SelectLine(V->m_MainRichEdit, LineNo);
	};

};
void CProtocolCtrl::NextError()
{
	long nStart, nEnd;
	GetSel(nStart, nEnd);
	int LineNo = LineFromChar(nStart);
	if (LineNo+1 == this->GetLineCount())
		LineNo = -1;
	SetSel(LineIndex(LineNo+1), LineIndex(LineNo+2));
	ShowError();
};

void CProtocolCtrl::OnLButtonDblClk( UINT nFlags,  CPoint point )
{
	CRichEditCtrl::OnLButtonDblClk (nFlags, point);
	ShowError();
};

void CProtocolCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F4)
		NextError();

	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

IMPLEMENT_DYNCREATE(CSourceCtrl, CRichEditCtrl)
BEGIN_MESSAGE_MAP(CSourceCtrl, CRichEditCtrl)
		ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CSourceCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F4)
	{	
		CImportForm* V = (CImportForm*)GetParent();
		V->m_ProtocolRichEdit.NextError();
	};	
	
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
