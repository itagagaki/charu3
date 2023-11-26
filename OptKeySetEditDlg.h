#if !defined(AFX_OPTKEYSETEDITDLG_H__A0D37959_215D_4FCE_856F_A470CFAB6F9A__INCLUDED_)
#define AFX_OPTKEYSETEDITDLG_H__A0D37959_215D_4FCE_856F_A470CFAB6F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "MyHotkeyCtrl.h"
#include "Init.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// COptKeySetEditDlg ダイアログ

class COptKeySetEditDlg : public CDialog
{
    // コンストラクション
public:
    COptKeySetEditDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
    //{{AFX_DATA(COptKeySetEditDlg)
    enum { IDD = IDD_KEYSET_EXT };
    CMyHotkeyCtrl	m_pasteKey;
    CMyHotkeyCtrl	m_copyKey;
    CString	m_pasteMessage;
    CString	m_copyMessage;
    int		m_keyAction;
    int		m_matchCombo;
    CString	m_caption;
    int m_copyWait;
    int m_pasteWait;
    int		m_nHistoryLimit;
    //}}AFX_DATA
    friend BOOL CALLBACK EnumWindowTitle(HWND hwnd, LPARAM lParam);

    void setKeyInfo(CHANGE_KEY keyInfo) {
        m_key = keyInfo;
    }
    CHANGE_KEY getKeyInfo() {
        return m_key;
    };
    // オーバーライド
        // ClassWizard は仮想関数のオーバーライドを生成します。
        //{{AFX_VIRTUAL(COptKeySetEditDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
    CHANGE_KEY m_key;

    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(COptKeySetEditDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTKEYSETEDITDLG_H__A0D37959_215D_4FCE_856F_A470CFAB6F9A__INCLUDED_)
