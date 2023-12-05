#pragma once

#include "stdafx.h"

#include "resource.h"

class CInitialDialog : public CDialog
{
    DECLARE_DYNAMIC(CInitialDialog)

public:
    CInitialDialog(CWnd* pParent = nullptr) : CDialog(IDD_INITIAL_DIALOG, pParent) {}
    virtual ~CInitialDialog() {}

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

    afx_msg void OnBnClickedNew();
    afx_msg void OnBnClickedOpen();
    afx_msg void OnBnClickedQuit();

    DECLARE_MESSAGE_MAP()
};
