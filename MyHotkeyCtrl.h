#if !defined(AFX_MYHOTKEYCTRL_H__A6BC19D4_358F_498B_A7DF_CA317C017F1C__INCLUDED_)
#define AFX_MYHOTKEYCTRL_H__A6BC19D4_358F_498B_A7DF_CA317C017F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CMyHotkeyCtrl ウィンドウ

class CMyHotkeyCtrl : public CHotKeyCtrl
{
    // コンストラクション
public:
    CMyHotkeyCtrl();

    // アトリビュート
public:

    // オペレーション
public:
    void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers) {
        if (wVirtualKeyCode >= 0x60 || wVirtualKeyCode == 0x1d || wVirtualKeyCode == 0x1c || wVirtualKeyCode == 0x20)
            CHotKeyCtrl::SetHotKey(wVirtualKeyCode, wModifiers);
        else
            CHotKeyCtrl::SetHotKey(wVirtualKeyCode, wModifiers | HOTKEYF_EXT);
    };

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CMyHotkeyCtrl)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// インプリメンテーション
public:
    virtual ~CMyHotkeyCtrl();

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(CMyHotkeyCtrl)
    // メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    WORD m_lastKey, m_mod;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYHOTKEYCTRL_H__A6BC19D4_358F_498B_A7DF_CA317C017F1C__INCLUDED_)
