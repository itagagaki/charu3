/*----------------------------------------------------------
    ポップアップ設定クラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTPOPUP_H__3B3D553D_3B12_401F_A02E_A6ED8AF583C8__INCLUDED_)
#define AFX_OPTPOPUP_H__3B3D553D_3B12_401F_A02E_A6ED8AF583C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "resource.h"

//---------------------------------------------------
// COptPopup ダイアログ
//---------------------------------------------------
class COptPopup : public CDialog
{
    // コンストラクション
public:
    COptPopup(CWnd* pParent = NULL);   // 標準のコンストラクタ

    // ダイアログ データ
private:
    //{{AFX_DATA(COptPopup)
    enum { IDD = IDD_SETTINGS_03_DATATREE };
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
public:
    //{{AFX_VIRTUAL(COptPopup)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL DestroyWindow();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
private:
    int m_nSelectByTypingCaseInsensitive;
    int m_nSelectByTypingAutoPaste;
    int m_nSelectByTypingAutoExpand;
    int m_nScrollVertical;
    int m_nScrollHorizontal;
    int m_nSingleExpand;
    int m_nSingleEnter;
    int m_nKeepSelection;
    int m_nKeepFolders;

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(COptPopup)
    virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnCbnSelchangeOptPopupPos();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTPOPUP_H__3B3D553D_3B12_401F_A02E_A6ED8AF583C8__INCLUDED_)
