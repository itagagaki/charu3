/*----------------------------------------------------------
    設定メインクラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTMAINDIALOG_H__46D08C0C_FB56_4508_833C_C6377E38BFE2__INCLUDED_)
#define AFX_OPTMAINDIALOG_H__46D08C0C_FB56_4508_833C_C6377E38BFE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "OptGeneral.h"
#include "OptStyle.h"
#include "OptPopup.h"
#include "OptFifo.h"
#include "OptKeySet.h"
#include "OptAdvanced.h"

//---------------------------------------------------
// COptMainDialog ダイアログ
//---------------------------------------------------
class COptMainDialog : public CDialog
{
    // コンストラクション
public:
    COptMainDialog(CWnd* pParent = NULL, int nPage = 0);

    // ダイアログ データ
    //{{AFX_DATA(COptMainDialog)
    enum { IDD = IDD_SETTINGS };
    CTabCtrl	m_ctrlTab;
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(COptMainDialog)
public:
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
protected:
    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(COptMainDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeOptTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    static const int NUM_PAGES = 6;
    CDialog* m_pages[NUM_PAGES];
    int m_nPageNo;

    COptGeneral m_general;
    COptStyle m_style;
    COptPopup m_popup;
    COptFifo m_fifo;
    COptKeySet m_key;
    COptAdvanced m_advanced;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTMAINDIALOG_H__46D08C0C_FB56_4508_833C_C6377E38BFE2__INCLUDED_)
