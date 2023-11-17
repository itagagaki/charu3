#include "resource.h"
#include "InitialDialog.h"

IMPLEMENT_DYNAMIC(CInitialDialog, CDialog)

void CInitialDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInitialDialog, CDialog)
    ON_BN_CLICKED(IDC_NEW, OnBnClickedNew)
    ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
    ON_BN_CLICKED(IDC_QUIT, OnBnClickedQuit)
END_MESSAGE_MAP()

// CSelectFile メッセージ ハンドラー

void CInitialDialog::OnBnClickedNew()
{
    EndDialog(IDC_NEW);
}

void CInitialDialog::OnBnClickedOpen()
{
    EndDialog(IDC_OPEN);
}

void CInitialDialog::OnBnClickedQuit()
{
    EndDialog(IDC_QUIT);
}
