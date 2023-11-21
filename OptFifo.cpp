/*----------------------------------------------------------
    履歴FIFO設定クラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OptFifo.h"
#include "Charu3.h" // for theApp.m_ini
#include "util.h"
#include "resource.h"

//---------------------------------------------------
//関数名	COptFifo
//機能		コンストラクタ
//---------------------------------------------------
COptFifo::COptFifo(CWnd* pParent /*=NULL*/) : CDialog(COptFifo::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptFifo)
    //}}AFX_DATA_INIT
    m_nAutoOff = Util::BoolToInt(theApp.m_ini.m_bAutoOff);
    m_nCleanup = Util::BoolToInt(theApp.m_ini.m_bCleanupAtTurnOff);
    m_nDontSaveSameDataAsLast = Util::BoolToInt(theApp.m_ini.m_bDontSaveSameDataAsLast);
    //m_strCopySound = theApp.m_ini.m_fifo.m_strCopySound;
    //m_strPasteSound = theApp.m_ini.m_fifo.m_strPasteSound;
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptFifo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptFifo)
    if (GetDlgItem(IDC_OPT_STOCK_NONE))
        DDX_Radio(pDX, IDC_OPT_STOCK_NONE, theApp.m_ini.m_nFifo);
    if (GetDlgItem(IDC_OPT_SOUND_COPY))
        DDX_Text(pDX, IDC_OPT_SOUND_COPY, theApp.m_ini.m_strCopySound);
    if (GetDlgItem(IDC_OPT_SOUND_PASTE))
        DDX_Text(pDX, IDC_OPT_SOUND_PASTE, theApp.m_ini.m_strPasteSound);
    if (GetDlgItem(IDC_OPT_CLEAR_ONETIME))
        DDX_Check(pDX, IDC_OPT_CLEAR_ONETIME, m_nCleanup);
    if (GetDlgItem(IDC_ALL_DEL_OFF))
        DDX_Check(pDX, IDC_ALL_DEL_OFF, m_nAutoOff);
    if (GetDlgItem(IDC_OPT_DUPLICATION))
        DDX_Check(pDX, IDC_OPT_DUPLICATION, m_nDontSaveSameDataAsLast);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptFifo, CDialog)
    //{{AFX_MSG_MAP(COptFifo)
    ON_BN_CLICKED(IDC_PLAY_COPY, OnPlayCopy)
    ON_BN_CLICKED(IDC_PLAY_PASTE, OnPlayPaste)
    ON_BN_CLICKED(IDC_FILE_SOUND_COPY, OnFileCopy)
    ON_BN_CLICKED(IDC_FILE_SOUND_PASTE, OnFilePaste)
    ON_BN_CLICKED(IDC_NOSOUND_COPY, OnNoSoundCopy)
    ON_BN_CLICKED(IDC_NOSOUND_PASTE, OnNoSoundPaste)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptFifo メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL COptFifo::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0) {
        ::PostMessage(::GetParent(this->m_hWnd), pMsg->message, VK_PRIOR, pMsg->lParam);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void COptFifo::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    if (bShow) {
        CWnd* w;
        switch (theApp.m_ini.m_nDoubleKeyPOP) {
        default:
        case 0:
            w = GetDlgItem(IDC_OPT_STOCK_NONE);
            break;
        case 1:
            w = GetDlgItem(IDC_OPT_STOCK_FIFO);
            break;
        case 2:
            w = GetDlgItem(IDC_OPT_STOCK_LIFO);
            break;
        }
        if (w) w->SetFocus();
    }
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		設定を反映
//---------------------------------------------------
BOOL COptFifo::DestroyWindow()
{
    theApp.m_ini.m_bAutoOff = static_cast<CButton*>(GetDlgItem(IDC_ALL_DEL_OFF))->GetCheck() != 0;
    theApp.m_ini.m_bCleanupAtTurnOff = static_cast<CButton*>(GetDlgItem(IDC_OPT_CLEAR_ONETIME))->GetCheck() != 0;
    theApp.m_ini.m_bDontSaveSameDataAsLast = static_cast<CButton*>(GetDlgItem(IDC_OPT_DUPLICATION))->GetCheck() != 0;
    //theApp.m_ini.m_fifo.m_strCopySound = m_strCopySound;
    //theApp.m_ini.m_fifo.m_strPasteSound = m_strPasteSound;

    return CDialog::DestroyWindow();
}

void COptFifo::OnPlayCopy()
{
    Play(IDC_OPT_SOUND_COPY);
}

void COptFifo::OnPlayPaste()
{
    Play(IDC_OPT_SOUND_PASTE);
}

void COptFifo::Play(int id)
{
    CWnd* edit = GetDlgItem(id);
    int len = edit->GetWindowTextLength();
    TCHAR* textBuf = new TCHAR[len + 1];
    if (textBuf) {
        edit->GetWindowText(textBuf, len + 1);
        if (textBuf != _T("")) {
            PlaySound(textBuf, NULL, SND_ASYNC | SND_FILENAME);
        }
        delete[] textBuf;
    }
}

void COptFifo::OnFileCopy()
{
    File(IDC_OPT_SOUND_COPY);
}

void COptFifo::OnFilePaste()
{
    File(IDC_OPT_SOUND_PASTE);
}

void COptFifo::File(int id)
{
    CString strRes;
    (void)strRes.LoadString(APP_INF_FILE_FILTER4);
    CFileDialog openFileDialog(TRUE, _T("wav"), NULL, OFN_FILEMUSTEXIST, strRes);
    openFileDialog.m_ofn.lpstrInitialDir = theApp.m_ini.m_strAppPath;
    if (openFileDialog.DoModal() == IDOK) {
        CString file = openFileDialog.GetPathName();
        int baseLen = theApp.m_ini.m_strAppPath.GetLength();
        if (theApp.m_ini.m_strAppPath.CompareNoCase(file.Left(baseLen)) == 0) {
            file = file.Mid(baseLen); // relative path to under exe
        }
        SetDlgItemText(id, file);
    }
}

void COptFifo::OnNoSoundCopy()
{
    SetDlgItemText(IDC_OPT_SOUND_COPY, _T(""));
}

void COptFifo::OnNoSoundPaste()
{
    SetDlgItemText(IDC_OPT_SOUND_PASTE, _T(""));
}
