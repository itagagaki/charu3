/*----------------------------------------------------------
    Setting Dialog - Advanced
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OptAdvanced.h"
#include "Charu3.h" // for theApp.m_ini
#include "resource.h"

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptAdvanced::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptEtc)
    if (GetDlgItem(IDC_OPT_CLIPBOARD_OPEN_DELAY)) {
        DDX_Text(pDX, IDC_OPT_CLIPBOARD_OPEN_DELAY, theApp.m_ini.m_nClipboardOpenDelay);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, theApp.m_ini.m_nClipboardOpenDelay, 0, 4000);
    }
    if (GetDlgItem(IDC_OPT_CLIPBOARD_RETRY_INTERVAL)) {
        DDX_Text(pDX, IDC_OPT_CLIPBOARD_RETRY_INTERVAL, theApp.m_ini.m_nClipboardRetryInterval);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, theApp.m_ini.m_nClipboardRetryInterval, 1, 1000);
    }
    if (GetDlgItem(IDC_OPT_CLIPBOARD_RETRY_TIMES)) {
        DDX_Text(pDX, IDC_OPT_CLIPBOARD_RETRY_TIMES, theApp.m_ini.m_nClipboardRetryTimes);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, theApp.m_ini.m_nClipboardRetryTimes, 0, 100);
    }
    if (GetDlgItem(IDC_OPT_WINDOW_CHECK_INTERVAL)) {
        DDX_Text(pDX, IDC_OPT_WINDOW_CHECK_INTERVAL, theApp.m_ini.m_nWindowCheckInterval);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, theApp.m_ini.m_nWindowCheckInterval, 1, 2000);
    }
    if (GetDlgItem(IDC_LIST_STEALTH)) {
        DDX_Control(pDX, IDC_LIST_STEALTH, m_stealthList);
    }
    if (GetDlgItem(IDC_STEALTH_INPUT)) {
        DDX_Control(pDX, IDC_STEALTH_INPUT, m_ctrlStealthInput);
    }
    if (GetDlgItem(IDC_STEALTH_ADD)) {
        DDX_Control(pDX, IDC_STEALTH_ADD, m_ctrlStealthAdd);
    }
    if (GetDlgItem(IDC_STEALTH_DELETE)) {
        DDX_Control(pDX, IDC_STEALTH_DELETE, m_ctrlStealthDelete);
    }
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptAdvanced, CDialog)
    //{{AFX_MSG_MAP(COptAdvanced)
    //}}AFX_MSG_MAP
    ON_WM_DROPFILES()
    ON_LBN_SELCHANGE(IDC_LIST_STEALTH, &COptAdvanced::OnLbnSelchangeListStealth)
    ON_EN_CHANGE(IDC_STEALTH_INPUT, &COptAdvanced::OnEnChangeStealthInput)
    ON_BN_CLICKED(IDC_STEALTH_ADD, &COptAdvanced::OnBnClickedStealthAdd)
    ON_BN_CLICKED(IDC_STEALTH_SELECT, &COptAdvanced::OnBnClickedStealthSelect)
    ON_BN_CLICKED(IDC_STEALTH_DELETE, &COptAdvanced::OnBnClickedStealthDelete)
END_MESSAGE_MAP()


BOOL COptAdvanced::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0) {
        ::PostMessage(::GetParent(this->m_hWnd), pMsg->message, VK_PRIOR, pMsg->lParam);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL COptAdvanced::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_stealthList.SetHorizontalExtent(2000);
    m_stealthList.ResetContent();
    for (const CString& s : theApp.m_ini.m_stealth) {
        m_stealthList.AddString(s);
    }

    return TRUE;
}

BOOL COptAdvanced::DestroyWindow()
{
    theApp.m_ini.m_stealth.clear();
    int stealthCount = m_stealthList.GetCount();
    for (int i = 0; i < stealthCount; i++) {
        CString s;
        m_stealthList.GetText(i, s);
        theApp.m_ini.m_stealth.push_back(s);
    }

    return CDialog::DestroyWindow();
}

void COptAdvanced::OnLbnSelchangeListStealth()
{
    const int index = m_stealthList.GetCurSel();
    m_ctrlStealthDelete.EnableWindow(LB_ERR != index);
}

void COptAdvanced::OnEnChangeStealthInput()
{
    CString input;
    m_ctrlStealthInput.GetWindowText(input);
    input.Trim();
    m_ctrlStealthAdd.EnableWindow(input.GetLength() > 0);
}

void COptAdvanced::OnBnClickedStealthAdd()
{
    CString input;
    m_ctrlStealthInput.GetWindowText(input);
    AddStealthProgram(input);
    m_ctrlStealthInput.SetWindowText(_T(""));
}

void COptAdvanced::OnBnClickedStealthSelect()
{
    CFileDialog* pFileDialog;
    CString strRes;
    (void)strRes.LoadString(APP_INF_FILE_FILTER6);
    pFileDialog = new CFileDialog(TRUE, _T("*.*"), NULL, NULL, strRes);
    if (pFileDialog && IDOK == pFileDialog->DoModal()) {
        AddStealthProgram(pFileDialog->GetPathName());
        delete pFileDialog;
    }
}

void COptAdvanced::OnBnClickedStealthDelete()
{
    const int index = m_stealthList.GetCurSel();
    if (LB_ERR != index) {
        m_stealthList.DeleteString(index);
        m_ctrlStealthDelete.EnableWindow(FALSE);
    }
}

void COptAdvanced::AddStealthProgram(CString name)
{
    name.Trim();
    if (name.GetLength() > 0 && m_stealthList.FindStringExact(0, name) == LB_ERR) {
        m_stealthList.AddString(name);
    }
}
