/*----------------------------------------------------------
    ビジュアル設定クラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTSTYLE_H__CE6482E8_6118_46DF_B05A_C898A00E9122__INCLUDED_)
#define AFX_OPTSTYLE_H__CE6482E8_6118_46DF_B05A_C898A00E9122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "resource.h"
#include "PaletStatic.h"
#include "resource.h"

#define	MAX_FONT 256

//---------------------------------------------------
// COptStyle ダイアログ
//---------------------------------------------------
class COptStyle : public CDialog
{
    // コンストラクション
public:
    COptStyle(CWnd* pParent = NULL);   // 標準のコンストラクタ
    friend int CALLBACK EnumFontProc(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm, int FontType, LPARAM _dwDmy);

    // ダイアログ データ
    //{{AFX_DATA(COptStyle)
    enum { IDD = IDD_SETTINGS_02_STYLE };
    CString m_strBorderColor;
    CString m_strBackgroundColor;
    CString m_strTextColor;
    CPaletStatic m_ctrlBorderPal;
    CPaletStatic m_ctrlTextPal;
    CPaletStatic m_ctrlBackgroundPal;
    CComboBox m_ctrlFontCombo;
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(COptStyle)
public:
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
protected:
    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(COptStyle)
    afx_msg void OnOptBorderColorPal();
    afx_msg void OnOptBackgroundColorPal();
    afx_msg void OnOptTextColorPal();
    virtual BOOL OnInitDialog();
    afx_msg void OnOptVsBrows();
    afx_msg void OnChangeOptTextColor();
    afx_msg void OnChangeOptBorderColor();
    afx_msg void OnChangeOptBackColor();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOptLoadStyle();
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
    void UpdateColor(CEdit* edit, CPaletStatic* stPal);
    void PickColor(CEdit* edit, CPaletStatic* stPal);
    void SetOpacityText(int value);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTSTYLE_H__CE6482E8_6118_46DF_B05A_C898A00E9122__INCLUDED_)
