// OptKeySetEditDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "General.h"
#include "OptKeySetEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptKeySetEditDlg ダイアログ

COptKeySetEditDlg::COptKeySetEditDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COptKeySetEditDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptKeySetEditDlg)
    m_pasteMessage = _T("");
    m_copyMessage = _T("");
    m_keyAction = -1;
    m_matchCombo = -1;
    m_caption = _T("");
    m_copyWait = 0;
    m_pasteWait = 0;
    //}}AFX_DATA_INIT
}

void COptKeySetEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptKeySetEditDlg)
    //}}AFX_DATA_MAP
    if (GetDlgItem(IDC_OPT_WINCAP_COMBO)) {
        DDX_CBString(pDX, IDC_OPT_WINCAP_COMBO, m_caption);
    }
    if (GetDlgItem(IDC_OPT_MATCH_COMBO)) {
        DDX_CBIndex(pDX, IDC_OPT_MATCH_COMBO, m_matchCombo);
    }
    if (GetDlgItem(IDC_OPT_COPYKEY2)) {
        DDX_Control(pDX, IDC_OPT_COPYKEY2, m_copyKey);
    }
    if (GetDlgItem(IDC_OPT_PASTKEY2)) {
        DDX_Control(pDX, IDC_OPT_PASTKEY2, m_pasteKey);
    }
    if (GetDlgItem(IDC_OPT_COPY_WAIT2)) {
        DDX_Text(pDX, IDC_OPT_COPY_WAIT2, m_copyWait);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, m_copyWait, 0, 1000);
    }
    if (GetDlgItem(IDC_OPT_PASTE_WAIT2)) {
        DDX_Text(pDX, IDC_OPT_PASTE_WAIT2, m_pasteWait);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, m_pasteWait, 0, 1000);
    }
    if (GetDlgItem(IDC_OPT_HISTORY_SIZE_LIMIT)) {
        DDX_Text(pDX, IDC_OPT_HISTORY_SIZE_LIMIT, m_nHistoryLimit);
    }

    if (!pDX->m_bSaveAndValidate) {
        UINT uMod = m_key.m_sCopyPasteKey.m_uMod_Paste;
        if (m_key.m_sCopyPasteKey.m_uVK_Paste >= 0x60)
            m_pasteKey.SetHotKey(m_key.m_sCopyPasteKey.m_uVK_Paste, CGeneral::mod2Hotkey(uMod));
        else
            m_pasteKey.SetHotKey(m_key.m_sCopyPasteKey.m_uVK_Paste, CGeneral::mod2Hotkey(uMod) | HOTKEYF_EXT);

        uMod = m_key.m_sCopyPasteKey.m_uMod_Copy;
        if (m_key.m_sCopyPasteKey.m_uVK_Copy >= 0x60)
            m_copyKey.SetHotKey(m_key.m_sCopyPasteKey.m_uVK_Copy, CGeneral::mod2Hotkey(uMod));
        else
            m_copyKey.SetHotKey(m_key.m_sCopyPasteKey.m_uVK_Copy, CGeneral::mod2Hotkey(uMod) | HOTKEYF_EXT);
    }
    else {
        CHANGE_KEY key;
        WORD wMod, wVkCode;

        key.m_nHistoryLimit = m_nHistoryLimit;
        //ペーストキー変換
        m_pasteKey.GetHotKey(wVkCode, wMod);
        key.m_sCopyPasteKey.m_uMod_Paste = CGeneral::hotkey2MOD(wMod);
        key.m_sCopyPasteKey.m_uVK_Paste = wVkCode;

        //コピーキー変換
        m_copyKey.GetHotKey(wVkCode, wMod);
        key.m_sCopyPasteKey.m_uMod_Copy = CGeneral::hotkey2MOD(wMod);
        key.m_sCopyPasteKey.m_uVK_Copy = wVkCode;

        _stscanf_s(m_pasteMessage, _T("%x,%x,%x"), &key.m_sCopyPasteKey.m_pasteMessage.Msg,
            &key.m_sCopyPasteKey.m_pasteMessage.wParam,
            &key.m_sCopyPasteKey.m_pasteMessage.lParam);

        _stscanf_s(m_copyMessage, _T("%x,%x,%x"), &key.m_sCopyPasteKey.m_copyMessage.Msg,
            &key.m_sCopyPasteKey.m_copyMessage.wParam,
            &key.m_sCopyPasteKey.m_copyMessage.lParam);

        key.m_sCopyPasteKey.m_nMessage = m_keyAction;
        key.m_nMatch = m_matchCombo;
        key.m_strTitle = m_caption;
        key.m_sCopyPasteKey.m_nCopyWait = m_copyWait;
        key.m_sCopyPasteKey.m_nPasteWait = m_pasteWait;

        m_key = key;
    }
}

//---------------------------------------------------
//関数名	EnumWindowTitle				[friend]
//機能		ウィンドウの名前を列挙
//---------------------------------------------------
BOOL CALLBACK EnumWindowTitle(HWND hwnd, LPARAM lParam)
{
    CComboBox* combo = (CComboBox*)lParam;

    TCHAR WindowName[1024];
    *WindowName = (char)NULL;
    if (IsWindowVisible(hwnd)) {
        int resalt = GetWindowText(hwnd, WindowName, _countof(WindowName));
        if (resalt && _tcsclen(WindowName) > 0) {
            if (combo)	combo->AddString(WindowName);
        }
    }

    return TRUE;  /* 列挙を続ける */
}

BEGIN_MESSAGE_MAP(COptKeySetEditDlg, CDialog)
    //{{AFX_MSG_MAP(COptKeySetEditDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptKeySetEditDlg メッセージ ハンドラ

BOOL COptKeySetEditDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    EnumWindows(&EnumWindowTitle, (LPARAM)GetDlgItem(IDC_OPT_WINCAP_COMBO));
    return TRUE;
}
