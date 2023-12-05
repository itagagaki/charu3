/*----------------------------------------------------------
    MainFrmクラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_MAINFRM_H__86B0B4A1_0092_48D1_B396_31664A4314DB__INCLUDED_)
#define AFX_MAINFRM_H__86B0B4A1_0092_48D1_B396_31664A4314DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "ChildView.h"

//---------------------------------------------------
// CMainFrameクラス定義
//---------------------------------------------------
class CMainFrame : public CFrameWnd
{
public:
    CMainFrame();
protected:
    DECLARE_DYNAMIC(CMainFrame)

    // アトリビュート
public:

    // オペレーション
public:

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
public:
    //{{AFX_VIRTUAL(CMainFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // インプリメンテーション
public:
    virtual ~CMainFrame();
    void SwitchNotificationAreaIcon(bool stockmode);
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

private:
    bool CheckTrayPos();
    void OnClipboardChanged();

    HICON m_hIcon, m_hStopIcon;
    NOTIFYICONDATA m_nIcon;
    HWND m_hActive;
    CChildView m_wndView;
    CMenu m_PopupMenu;

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnExit();
    afx_msg BOOL OnQueryEndSession();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__86B0B4A1_0092_48D1_B396_31664A4314DB__INCLUDED_)
