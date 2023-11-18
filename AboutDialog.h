#pragma once

#include "stdafx.h"

#include "resource.h"

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};
