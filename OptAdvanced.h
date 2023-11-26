/*----------------------------------------------------------
    Setting Dialog - Advanced
----------------------------------------------------------*/

#if !defined(AFX_OPTADVANCED_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_)
#define AFX_OPTADVANCED_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "MyEditCtrl.h"
#include "resource.h"

//---------------------------------------------------
// COptAdvanced ダイアログ
//---------------------------------------------------
class COptAdvanced : public CDialog
{
    // コンストラクション
public:
    COptAdvanced(CWnd* pParent = NULL) : CDialog(IDD, pParent) {}

    // ダイアログ データ
    //{{AFX_DATA(COptAdvanced)
    enum { IDD = IDD_SETTINGS_06_ADVANCED };
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(COptAdvanced)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL DestroyWindow();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
protected:
    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(COptAdvanced)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    virtual BOOL OnInitDialog();
    afx_msg void OnLbnSelchangeListStealth();
    afx_msg void OnEnChangeStealthInput();
    afx_msg void OnBnClickedStealthAdd();
    afx_msg void OnBnClickedStealthSelect();
    afx_msg void OnBnClickedStealthDelete();
    void AddStealthProgram(CString name);

    CMyEditCtrl	m_ctrlStealthInput;
    CButton m_ctrlStealthAdd;
    CButton m_ctrlStealthDelete;
    CListBox m_stealthList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTADVANCED_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_)
