/*----------------------------------------------------------
	Charu3
	検索ダイアログ
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "SerchDialog.h"
#include "General.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// アプリケーションオブジェクト
extern	CCharu3App theApp;

//---------------------------------------------------
// CSerchDialog ダイアログ
//---------------------------------------------------
BEGIN_MESSAGE_MAP(CSearchDialog, CDialog)
	//{{AFX_MSG_MAP(CSerchDialog)
	ON_BN_CLICKED(IDC_FIND_PREV, OnFindPrev)
	ON_BN_CLICKED(IDC_FIND_NEXT, OnFindNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
//コンストラクタ
//---------------------------------------------------
CSearchDialog::CSearchDialog(CWnd* pParent /*=NULL*/)	: CDialog(CSearchDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerchDialog)
	//}}AFX_DATA_INIT
}

//---------------------------------------------------
//データエクスチェンジ
//---------------------------------------------------
void CSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerchDialog)
	if (GetDlgItem(IDC_SEARCH_TEXT))
		DDX_Control(pDX, IDC_SEARCH_TEXT, m_ctrlSearchText);
	if(GetDlgItem(IDC_SEARCH_NAME))
		DDX_Control(pDX, IDC_SEARCH_NAME, m_ctrlSearchByName);
	if (GetDlgItem(IDC_SEARCH_DATA))
		DDX_Control(pDX, IDC_SEARCH_DATA, m_ctrlSearchByData);
	if(GetDlgItem(IDC_RADIO_AND))
		DDX_Control(pDX, IDC_RADIO_AND, m_ctrlSearchAnd);
	if (GetDlgItem(IDC_RADIO_OR))
		DDX_Control(pDX, IDC_RADIO_OR, m_ctrlSearchOr);
	if (GetDlgItem(IDC_SEARCH_CASE_INSENSITIVE))
		DDX_Control(pDX, IDC_SEARCH_CASE_INSENSITIVE, m_ctrlCaseInsensitive);
	//}}AFX_DATA_MAP
}

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		ダイアログの初期化
//---------------------------------------------------
BOOL CSearchDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctrlSearchText.SetWindowText(theApp.m_ini.m_strSearchKeywords);
	m_ctrlSearchByName.SetCheck((theApp.m_ini.m_nSearchTarget & SEARCH_TARGET_NAME) ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlSearchByData.SetCheck((theApp.m_ini.m_nSearchTarget & SEARCH_TARGET_DATA) ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlSearchAnd.SetCheck(theApp.m_ini.m_nSearchLogic == SEARCH_LOGIC_AND ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlSearchOr.SetCheck(theApp.m_ini.m_nSearchLogic == SEARCH_LOGIC_OR ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCaseInsensitive.SetCheck(CGeneral::BoolToInt(theApp.m_ini.m_bSearchCaseInsensitive));
	return FALSE;
}

void CSearchDialog::OnCancel()
{
	::PostMessage(this->GetParent()->m_hWnd, WM_FIND_CLOSE, 0, 0);
	CDialog::OnCancel();
}

void CSearchDialog::OnOK()
{
	::PostMessage(this->GetParent()->m_hWnd, WM_FIND_ONCE, 0, 0);
}

void CSearchDialog::OnFindPrev()
{
	::PostMessage(this->GetParent()->m_hWnd, WM_FIND_PREV, 0, 0);
}

void CSearchDialog::OnFindNext()
{
	::PostMessage(this->GetParent()->m_hWnd, WM_FIND_NEXT, 0, 0);
}

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL CSearchDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_F3) {
			::PostMessage(this->GetParent()->m_hWnd, ::GetKeyState(VK_SHIFT) < 0 ? WM_FIND_PREV : WM_FIND_NEXT, 0, 0);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

int CSearchDialog::GetTarget()
{
	return (m_ctrlSearchByName.GetCheck() == BST_CHECKED ? SEARCH_TARGET_NAME : 0) | (m_ctrlSearchByData.GetCheck() == BST_CHECKED ? SEARCH_TARGET_DATA : 0);
}

int CSearchDialog::GetSearchLogic()
{
	return GetCheckedRadioButton(IDC_RADIO_AND, IDC_RADIO_OR) - IDC_RADIO_AND;
}

bool CSearchDialog::GetCaseInsensitive()
{
	return m_ctrlCaseInsensitive.GetCheck() == BST_CHECKED;
}

CString CSearchDialog::GetSearchText()
{
	CString text = _T("");
	GetDlgItem(IDC_SEARCH_TEXT)->GetWindowText(text);
	return text;
}
