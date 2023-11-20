/*----------------------------------------------------------
    ポップアップ設定クラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OptPopup.h"
#include "Charu3.h"
#include "util.h"
#include "resource.h"

//---------------------------------------------------
//関数名	COptPopup
//機能		コンストラクタ
//---------------------------------------------------
COptPopup::COptPopup(CWnd* pParent /*=NULL*/) : CDialog(COptPopup::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptPopup)
    //}}AFX_DATA_INIT
    m_nSelectByTypingCaseInsensitive = Util::BoolToInt(theApp.m_ini.m_pop.m_bSelectByTypingCaseInsensitive);
    m_nSelectByTypingAutoPaste = Util::BoolToInt(theApp.m_ini.m_pop.m_bSelectByTypingAutoPaste);
    m_nSelectByTypingAutoExpand = Util::BoolToInt(theApp.m_ini.m_pop.m_bSelectByTypingAutoExpand);
    m_nScrollVertical = Util::BoolToInt(theApp.m_ini.m_visual.m_bScrollbarVertical);
    m_nScrollHorizontal = Util::BoolToInt(theApp.m_ini.m_visual.m_bScrollbarHorizontal);
    m_nSingleExpand = Util::BoolToInt(theApp.m_ini.m_pop.m_bSingleExpand);
    m_nSingleEnter = Util::BoolToInt(theApp.m_ini.m_pop.m_bSingleEnter);
    m_nKeepSelection = Util::BoolToInt(theApp.m_ini.m_pop.m_bKeepSelection);
    m_nKeepFolders = Util::BoolToInt(theApp.m_ini.m_pop.m_bKeepFolders);
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptPopup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptPopup)
    if (GetDlgItem(IDC_OPT_POPUP_POS))
        DDX_CBIndex(pDX, IDC_OPT_POPUP_POS, theApp.m_ini.m_pop.m_nPopupPos);

    if (GetDlgItem(IDC_OPT_TOOLTIP_01))
        DDX_Radio(pDX, IDC_OPT_TOOLTIP_01, theApp.m_ini.m_visual.m_nToolTip);
    if (GetDlgItem(IDC_OPT_HoseiX))
        DDX_Text(pDX, IDC_OPT_HoseiX, theApp.m_ini.m_pop.m_posCaretHosei.x);
    if (GetDlgItem(IDC_OPT_HoseiY))
        DDX_Text(pDX, IDC_OPT_HoseiY, theApp.m_ini.m_pop.m_posCaretHosei.y);

    if (GetDlgItem(IDC_OPT_SELECT_BY_TYPING_PERIOD))
        DDX_Text(pDX, IDC_OPT_SELECT_BY_TYPING_PERIOD, theApp.m_ini.m_pop.m_nSelectByTypingFinalizePeriod);
    if (GetDlgItem(IDC_OPT_SELECT_BY_TYPING_CASE_INSENSITIVE))
        DDX_Check(pDX, IDC_OPT_SELECT_BY_TYPING_CASE_INSENSITIVE, m_nSelectByTypingCaseInsensitive);
    if (GetDlgItem(IDC_OPT_SELECT_BY_TYPING_AUTO_PASTE))
        DDX_Check(pDX, IDC_OPT_SELECT_BY_TYPING_AUTO_PASTE, m_nSelectByTypingAutoPaste);
    if (GetDlgItem(IDC_OPT_SELECT_BY_TYPING_AUTO_EXPAND))
        DDX_Check(pDX, IDC_OPT_SELECT_BY_TYPING_AUTO_EXPAND, m_nSelectByTypingAutoExpand);

    if (GetDlgItem(IDC_OPT_SCROLLBAR_VERTICAL))
        DDX_Check(pDX, IDC_OPT_SCROLLBAR_VERTICAL, m_nScrollVertical);
    if (GetDlgItem(IDC_OPT_SCROLLBAR_HORIZONTAL))
        DDX_Check(pDX, IDC_OPT_SCROLLBAR_HORIZONTAL, m_nScrollHorizontal);
    if (GetDlgItem(IDC_OPT_SINGLE_EXPAND))
        DDX_Check(pDX, IDC_OPT_SINGLE_EXPAND, m_nSingleExpand);
    if (GetDlgItem(IDC_OPT_SINGLE_ENTER))
        DDX_Check(pDX, IDC_OPT_SINGLE_ENTER, m_nSingleEnter);
    if (GetDlgItem(IDC_OPT_SELECT_SAVE))
        DDX_Check(pDX, IDC_OPT_SELECT_SAVE, m_nKeepSelection);
    if (GetDlgItem(IDC_OPT_FOLDER_OPEN))
        DDX_Check(pDX, IDC_OPT_FOLDER_OPEN, m_nKeepFolders);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptPopup, CDialog)
    //{{AFX_MSG_MAP(COptPopup)
    ON_CBN_SELCHANGE(IDC_OPT_POPUP_POS, OnCbnSelchangeOptPopupPos)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptPopup メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL COptPopup::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0) {
        ::PostMessage(::GetParent(this->m_hWnd), pMsg->message, VK_PRIOR, pMsg->lParam);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		初期化
//---------------------------------------------------
BOOL COptPopup::OnInitDialog()
{
    CDialog::OnInitDialog();

    OnCbnSelchangeOptPopupPos();

    return TRUE;
}

void COptPopup::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    if (bShow) {
        CWnd* w = GetDlgItem(IDC_OPT_POPUP_POS);
        if (w) w->SetFocus();
    }
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		設定を反映
//---------------------------------------------------
BOOL COptPopup::DestroyWindow()
{
    CWnd* cPopupPositionSelector = GetDlgItem(IDC_OPT_POPUP_POS);
    theApp.m_ini.m_pop.m_nPopupPos = static_cast<CComboBox*>(cPopupPositionSelector)->GetCurSel();
    theApp.m_ini.m_pop.m_bSelectByTypingCaseInsensitive = static_cast<CButton*>(GetDlgItem(IDC_OPT_SELECT_BY_TYPING_CASE_INSENSITIVE))->GetCheck() != 0;
    theApp.m_ini.m_pop.m_bSelectByTypingAutoPaste = static_cast<CButton*>(GetDlgItem(IDC_OPT_SELECT_BY_TYPING_AUTO_PASTE))->GetCheck() != 0;
    theApp.m_ini.m_pop.m_bSelectByTypingAutoExpand = static_cast<CButton*>(GetDlgItem(IDC_OPT_SELECT_BY_TYPING_AUTO_EXPAND))->GetCheck() != 0;
    theApp.m_ini.m_visual.m_bScrollbarVertical = static_cast<CButton*>(GetDlgItem(IDC_OPT_SCROLLBAR_VERTICAL))->GetCheck() != 0;
    theApp.m_ini.m_visual.m_bScrollbarHorizontal = static_cast<CButton*>(GetDlgItem(IDC_OPT_SCROLLBAR_HORIZONTAL))->GetCheck() != 0;
    theApp.m_ini.m_pop.m_bSingleExpand = static_cast<CButton*>(GetDlgItem(IDC_OPT_SINGLE_EXPAND))->GetCheck() != 0;
    theApp.m_ini.m_pop.m_bSingleEnter = static_cast<CButton*>(GetDlgItem(IDC_OPT_SINGLE_ENTER))->GetCheck() != 0;
    theApp.m_ini.m_pop.m_bKeepSelection = static_cast<CButton*>(GetDlgItem(IDC_OPT_SELECT_SAVE))->GetCheck() != 0;
    theApp.m_ini.m_pop.m_bKeepFolders = static_cast<CButton*>(GetDlgItem(IDC_OPT_FOLDER_OPEN))->GetCheck() != 0;

    return CDialog::DestroyWindow();
}

void COptPopup::OnCbnSelchangeOptPopupPos()
{
    CWnd* cPopupPositionSelector = GetDlgItem(IDC_OPT_POPUP_POS);
    if (cPopupPositionSelector) {
        bool enable = static_cast<CComboBox*>(cPopupPositionSelector)->GetCurSel() == POPUP_POS_CARET;
        CWnd* control;
        control = GetDlgItem(IDC_OPT_HoseiX);
        if (control) {
            control->EnableWindow(enable);
        }
        control = GetDlgItem(IDC_OPT_HoseiY);
        if (control) {
            control->EnableWindow(enable);
        }
    }
}
