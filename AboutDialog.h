#pragma once

#include "stdafx.h"

#include "resource.h"

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

private:
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
};
