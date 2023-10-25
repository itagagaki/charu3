/*----------------------------------------------------------
	設定メインクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "OptMainDialog.h"
#include "Charu3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	COptMainDialog
//機能		コンストラクタ
//---------------------------------------------------
COptMainDialog::COptMainDialog(CWnd* pParent /*=NULL*/,int nPage) : CDialog(COptMainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptMainDialog)
	//}}AFX_DATA_INIT
	m_nPage = nPage;
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptMainDialog)
	if(GetDlgItem(IDC_OPT_TAB))
		DDX_Control(pDX, IDC_OPT_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptMainDialog, CDialog)
	//{{AFX_MSG_MAP(COptMainDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_OPT_TAB, OnSelchangeOptTab)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_KEYDOWN, IDC_OPT_TAB, OnKeydownOptTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptMainDialog メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		初期化
//---------------------------------------------------
BOOL COptMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	TC_ITEM    item;
	int i;
    item.mask = TCIF_TEXT;

	//タブを設定
	CString strTabText;
	//{_T("全般"),_T("スタイル"),_T("ポップアップ"),_T("ストックモード"),_T("キーイベント設定")};

	for(i = 0; i <= MAX_OPT_PAGE; i++) {
		strTabText.LoadString(APP_INF_TABNAME_01 + i);
	    item.pszText = (TCHAR*)LPCTSTR(strTabText);
	    m_ctrlTab.InsertItem(i, &item);
	}

    //タブコンのサイズを取得
    CRect rect;
    m_ctrlTab.GetWindowRect(&rect);
    m_ctrlTab.AdjustRect(FALSE, &rect);
    ScreenToClient(&rect);

	//ページを構築
	int nDialogID[] = {IDD_SETTINGS_01_GENERAL, IDD_SETTINGS_02_STYLE, IDD_SETTINGS_03_DATATREE, IDD_SETTINGS_04_STOCKMODE, IDD_SETTINGS_05_KEYS, IDD_SETTINGS_06_ADVANCED };

	m_OptionPage[0] = &m_EtcPage;
	m_OptionPage[1] = &m_VisualPage;
	m_OptionPage[2] = &m_PopupPage;
	m_OptionPage[3] = &m_FifoPage;
	m_OptionPage[4] = &m_KeysetPage;
	m_OptionPage[5] = &m_AdvancedPage;

	//初期設定
	for(i = 0; i <= MAX_OPT_PAGE; i++) {
	    m_OptionPage[i]->Create(nDialogID[i],this);
	    m_OptionPage[i]->MoveWindow(&rect);
		m_OptionPage[i]->ShowWindow(SW_HIDE);
	}
	m_nPage = theApp.m_ini.m_nOptionPage;
    m_OptionPage[m_nPage]->ShowWindow(SW_SHOW);
	m_ctrlTab.SetCurFocus(m_nPage);

	return FALSE; // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

//---------------------------------------------------
//関数名	OnSelchangeOptTab(NMHDR* pNMHDR, LRESULT* pResult)
//機能		タブ切り替え処理
//---------------------------------------------------
void COptMainDialog::OnSelchangeOptTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	for(int i = 0; i <= MAX_OPT_PAGE; i++)
	    m_OptionPage[i]->ShowWindow(SW_HIDE);

	m_nPage = m_ctrlTab.GetCurSel();
    m_OptionPage[m_nPage]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		ウィンドウ破棄処理
//---------------------------------------------------
BOOL COptMainDialog::DestroyWindow()
{
	if (theApp.m_pTreeDlg->IsWindowVisible()) {
		theApp.m_pTreeDlg->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}

	for(int i = 0; i <= MAX_OPT_PAGE; i++) {
		m_OptionPage[i]->UpdateData();
	    m_OptionPage[i]->DestroyWindow();
	}
	theApp.m_ini.m_nOptionPage = m_nPage;

	return CDialog::DestroyWindow();
}

void COptMainDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

//---------------------------------------------------
// メッセージ前処理
//---------------------------------------------------
BOOL COptMainDialog::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN) {
		if(pMsg->wParam == VK_PRIOR) {
			if(m_nPage < MAX_OPT_PAGE) m_nPage++;
			else			m_nPage = 0;
			m_ctrlTab.SetCurFocus(m_nPage);
		}
		else if(pMsg->wParam == VK_NEXT) {
			if(m_nPage > 0) m_nPage--;
			else			m_nPage = MAX_OPT_PAGE;
			m_ctrlTab.SetCurFocus(m_nPage);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void COptMainDialog::OnKeydownOptTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	TC_KEYDOWN* pTCKeyDown = (TC_KEYDOWN*)pNMHDR;
	if(pTCKeyDown->wVKey == VK_TAB && ::GetKeyState(VK_CONTROL) < 0){
		if(m_nPage < 4) m_nPage++;
		else			m_nPage = 0;
		m_ctrlTab.SetCurFocus(m_nPage);
	}
	*pResult = 0;
}
