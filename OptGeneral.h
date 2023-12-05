/*----------------------------------------------------------
    一般設定クラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTGENERAL_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_)
#define AFX_OPTGENERAL_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "MyHotkeyCtrl.h"
#include "resource.h"

//---------------------------------------------------
// COptGeneral ダイアログ
//---------------------------------------------------
class COptGeneral : public CDialog
{
    // コンストラクション
public:
    COptGeneral(CWnd* pParent = NULL);   // 標準のコンストラクタ

    // ダイアログ データ
private:
    //{{AFX_DATA(COptGeneral)
    enum { IDD = IDD_SETTINGS_01_GENERAL };
    CMyHotkeyCtrl m_ctrlPopupKey;
    CMyHotkeyCtrl m_ctrlFifoKey;
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
public:
    //{{AFX_VIRTUAL(COptGeneral)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL DestroyWindow();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
private:
    int m_nPutBackClipboard;
    int m_nShowClipboardInNotifyIconTooltip;

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(COptGeneral)
    virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void HotkeyEnablePOP();
    afx_msg void HotkeyDisablePOP();
    afx_msg void HotkeyEnableFIFO();
    afx_msg void HotkeyDisableFIFO();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTGENERAL_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_)
