#pragma once

#include "resource.h"

#include <afxwin.h>

class CInitialDialog : public CDialog
{
    DECLARE_DYNAMIC(CInitialDialog)

public:
    CInitialDialog(CWnd* pParent = nullptr) : CDialog(IDD_INITIAL_DIALOG, pParent) {}
    virtual ~CInitialDialog() {}

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedNew();
    afx_msg void OnBnClickedOpen();
    afx_msg void OnBnClickedQuit();
};
