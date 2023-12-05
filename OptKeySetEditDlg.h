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
private:
    //{{AFX_DATA(COptKeySetEditDlg)
    enum { IDD = IDD_KEYSET_EXT };
    CMyHotkeyCtrl m_pasteKey;
    CMyHotkeyCtrl m_copyKey;
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
protected:
    //{{AFX_VIRTUAL(COptKeySetEditDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
public:
    void SetKeyInfo(CHANGE_KEY keyInfo) { m_key = keyInfo; }
    CHANGE_KEY GetKeyInfo() const { return m_key; };

    CString m_pasteMessage;
    CString m_copyMessage;
    int m_keyAction;
    int m_matchCombo;
    CString m_caption;
    int m_copyWait;
    int m_pasteWait;
    int m_nHistoryLimit;

private:
    CHANGE_KEY m_key;

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(COptKeySetEditDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTKEYSETEDITDLG_H__A0D37959_215D_4FCE_856F_A470CFAB6F9A__INCLUDED_)
