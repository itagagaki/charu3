/*----------------------------------------------------------
    設定メインクラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OptMainDialog.h"
#include "Charu3.h"
#include "hotkey.h"
#include "resource.h"
#include "log.h"

//---------------------------------------------------
//関数名	COptMainDialog
//機能		コンストラクタ
//---------------------------------------------------
COptMainDialog::COptMainDialog(CWnd* pParent /*=NULL*/, int nPage)
    : CDialog(COptMainDialog::IDD, pParent)
    , m_pages{ &m_general, &m_style, &m_popup, &m_fifo, &m_key, &m_advanced }
    , m_nPageNo(nPage)
{
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptMainDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptMainDialog)
    if (GetDlgItem(IDC_OPT_TAB))
        DDX_Control(pDX, IDC_OPT_TAB, m_ctrlTab);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptMainDialog, CDialog)
    //{{AFX_MSG_MAP(COptMainDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_OPT_TAB, OnSelchangeOptTab)
    ON_WM_SHOWWINDOW()
    ON_WM_ACTIVATE()
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

    for (int i = 0; i < NUM_PAGES; i++) {
        CString strTabText;
        (void)strTabText.LoadString(APP_INF_TABNAME_01 + i);
        TC_ITEM item = {};
        item.mask = TCIF_TEXT;
        item.pszText = (TCHAR*)LPCTSTR(strTabText);
        m_ctrlTab.InsertItem(i, &item);
    }

    CRect rect;
    m_ctrlTab.GetWindowRect(&rect);
    m_ctrlTab.AdjustRect(FALSE, &rect);
    ScreenToClient(&rect);

    m_nPageNo = theApp.m_ini.m_nOptionPage;
    const int nDialogID[] = { IDD_SETTINGS_01_GENERAL, IDD_SETTINGS_02_STYLE, IDD_SETTINGS_03_DATATREE, IDD_SETTINGS_04_STOCKMODE, IDD_SETTINGS_05_KEYS, IDD_SETTINGS_06_ADVANCED };
    for (int i = 0; i < NUM_PAGES; i++) {
        m_pages[i]->Create(nDialogID[i], this);
        m_pages[i]->MoveWindow(&rect);
        m_pages[i]->ShowWindow(i == m_nPageNo ? SW_SHOW : SW_HIDE);
        m_ctrlTab.HighlightItem(i, i == m_nPageNo);
    }
    m_ctrlTab.SetCurFocus(m_nPageNo);

    return FALSE; // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
                  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

//---------------------------------------------------
//関数名	OnSelchangeOptTab(NMHDR* pNMHDR, LRESULT* pResult)
//機能		タブ切り替え処理
//---------------------------------------------------
void COptMainDialog::OnSelchangeOptTab(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_nPageNo = m_ctrlTab.GetCurSel();
    for (int i = 0; i < NUM_PAGES; i++) {
        m_pages[i]->ShowWindow(i == m_nPageNo ? SW_SHOW : SW_HIDE);
        m_ctrlTab.HighlightItem(i, i == m_nPageNo);
    }
    *pResult = 0;
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		ウィンドウ破棄処理
//---------------------------------------------------
BOOL COptMainDialog::DestroyWindow()
{
    theApp.RedrawDataTreeView();

    for (int i = 0; i < NUM_PAGES; i++) {
        m_pages[i]->UpdateData();
        m_pages[i]->DestroyWindow();
    }
    theApp.m_ini.m_nOptionPage = m_nPageNo;

    return CDialog::DestroyWindow();
}

void COptMainDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void COptMainDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    if (WA_INACTIVE == nState) {
        UnregisterHotKey(this->m_hWnd, HOTKEY_TAB_NEXT);
        UnregisterHotKey(this->m_hWnd, HOTKEY_TAB_PREV);
    }
    else {
        RegisterHotKey(this->m_hWnd, HOTKEY_TAB_NEXT, MOD_CONTROL, VK_TAB);
        RegisterHotKey(this->m_hWnd, HOTKEY_TAB_PREV, MOD_CONTROL | MOD_SHIFT, VK_TAB);
    }
}

//---------------------------------------------------
// メッセージ前処理
//---------------------------------------------------
BOOL COptMainDialog::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_HOTKEY) {
        switch (pMsg->wParam) {
        case HOTKEY_TAB_NEXT:
            if (m_nPageNo < NUM_PAGES - 1) {
                ++m_nPageNo;
            }
            else {
                m_nPageNo = 0;
            }
            m_ctrlTab.SetCurFocus(m_nPageNo);
            break;
        case HOTKEY_TAB_PREV:
            if (m_nPageNo > 0) {
                --m_nPageNo;
            }
            else {
                m_nPageNo = NUM_PAGES - 1;
            }
            m_ctrlTab.SetCurFocus(m_nPageNo);
            break;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}
