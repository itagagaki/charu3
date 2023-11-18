// MyHotkeyCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MyHotkeyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMyHotkeyCtrl

CMyHotkeyCtrl::CMyHotkeyCtrl()
{
    m_lastKey = NULL;
    m_mod = NULL;
}

CMyHotkeyCtrl::~CMyHotkeyCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyHotkeyCtrl, CHotKeyCtrl)
    //{{AFX_MSG_MAP(CMyHotkeyCtrl)
        // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHotkeyCtrl メッセージ ハンドラ

BOOL CMyHotkeyCtrl::PreTranslateMessage(MSG* pMsg)
{
    if (this->GetFocus() == this) {
#if false
        /*
         * I don't understand the purpose of this code.
         */
        if (!(pMsg->lParam & 0x40000000) && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)) {
            m_mod = 0;
            if (::GetAsyncKeyState(VK_MENU) < 0)		m_mod = m_mod | HOTKEYF_ALT;
            if (::GetAsyncKeyState(VK_CONTROL) < 0)	m_mod = m_mod | HOTKEYF_CONTROL;
            if (::GetAsyncKeyState(VK_SHIFT) < 0)	m_mod = m_mod | HOTKEYF_SHIFT;
            if (pMsg->wParam != 0x11 && pMsg->wParam != 0x12)// m_lastKey = NULL;
                m_lastKey = pMsg->wParam;
        }
        else if (pMsg->message == WM_KEYUP || pMsg->message == WM_SYSKEYUP) {
            SetHotKey(m_lastKey, m_mod);
            return 0;
        }
        if (pMsg->message == WM_SYSKEYDOWN) {
            if (pMsg->wParam == 'C') {
                this->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
                pMsg->message = NULL;
            }
        }
#else
        /*
         * This is enough, I think.
         */
        if (pMsg->message == WM_SYSKEYDOWN) {
            this->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
            return TRUE;
        }
    }
#endif

    return CHotKeyCtrl::PreTranslateMessage(pMsg);
}
