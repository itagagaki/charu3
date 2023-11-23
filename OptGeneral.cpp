/*----------------------------------------------------------
    ��ʐݒ�N���X
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OptGeneral.h"
#include "Charu3.h" // for theApp.m_ini
#include "key.h"
#include "util.h"
#include "resource.h"

//---------------------------------------------------
//�֐���	COptGereral
//�@�\		�R���X�g���N�^
//---------------------------------------------------
COptGeneral::COptGeneral(CWnd* pParent /*=NULL*/) : CDialog(COptGeneral::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptGeneral)
    //}}AFX_DATA_INIT
    m_nPutBackClipboard = Util::BoolToInt(theApp.m_ini.m_bPutBackClipboard);
    m_nShowClipboardInNotifyIconTooltip = Util::BoolToInt(theApp.m_ini.m_bShowClipboardInTooltipOfNofifyIcon);
}

//---------------------------------------------------
//�֐���	DoDataExchange(CDataExchange* pDX)
//�@�\		�f�[�^�G�N�X�`�F���W
//---------------------------------------------------
void COptGeneral::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptGeneral)

    if (GetDlgItem(IDC_OPT_KEYRADIO_POP))
        DDX_Radio(pDX, IDC_OPT_KEYRADIO_POP, theApp.m_ini.m_nDoubleKeyPOP);
    if (GetDlgItem(IDC_OPT_POPUPKEY))
        DDX_Control(pDX, IDC_OPT_POPUPKEY, m_ctrlPopupKey);

    if (GetDlgItem(IDC_OPT_KEYRADIO_FIFO))
        DDX_Radio(pDX, IDC_OPT_KEYRADIO_FIFO, theApp.m_ini.m_nDoubleKeyFIFO);
    if (GetDlgItem(IDC_OPT_FIFO_KEY))
        DDX_Control(pDX, IDC_OPT_FIFO_KEY, m_ctrlFifoKey);

    if (GetDlgItem(IDC_OPT_ICON_POP))
        DDX_Radio(pDX, IDC_OPT_ICON_POP, theApp.m_ini.m_nIconClick);
    if (GetDlgItem(IDC_OPT_TOCLIP))
        DDX_Check(pDX, IDC_OPT_TOCLIP, m_nPutBackClipboard);
    if (GetDlgItem(IDC_OPT_TOOLTIP_TITLE))
        DDX_Check(pDX, IDC_OPT_TOOLTIP_TITLE, m_nShowClipboardInNotifyIconTooltip);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptGeneral, CDialog)
    //{{AFX_MSG_MAP(COptGeneral)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP, hotkeyEnablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP2, hotkeyDisablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP3, hotkeyDisablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP4, hotkeyDisablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO, hotkeyEnableFIFO)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO2, hotkeyDisableFIFO)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO3, hotkeyDisableFIFO)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO4, hotkeyDisableFIFO)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptGeneral ���b�Z�[�W �n���h��
//---------------------------------------------------

//---------------------------------------------------
//�֐���	PreTranslateMessage(MSG* pMsg)
//�@�\		���b�Z�[�W�O����
//---------------------------------------------------
BOOL COptGeneral::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0) {
        ::PostMessage(::GetParent(this->m_hWnd), pMsg->message, VK_PRIOR, pMsg->lParam);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

//---------------------------------------------------
//�֐���	OnInitDialog()
//�@�\		������
//---------------------------------------------------
BOOL COptGeneral::OnInitDialog()
{
    CDialog::OnInitDialog();

    //�z�b�g�L�[
    if (theApp.m_ini.m_nDoubleKeyPOP != 0)	m_ctrlPopupKey.EnableWindow(false);
    if (theApp.m_ini.m_nDoubleKeyFIFO != 0)	m_ctrlFifoKey.EnableWindow(false);

    UINT uPopKey, uPopMod, uRirekiKey, uRirekiMod;
    theApp.m_ini.getHotKey(&uPopKey, &uPopMod, &uRirekiKey, &uRirekiMod);

    //�R���g���[���ɃZ�b�g
    uPopMod = KeyHelper::ModToHotkey(uPopMod);
    m_ctrlPopupKey.SetHotKey(uPopKey, uPopMod);
    uRirekiMod = KeyHelper::ModToHotkey(uRirekiMod);
    m_ctrlFifoKey.SetHotKey(uRirekiKey, uRirekiMod);

    return TRUE;
}

void COptGeneral::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    if (bShow) {
        CWnd* w;
        switch (theApp.m_ini.m_nDoubleKeyPOP) {
        default:
        case 0:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP);
            break;
        case 1:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP2);
            break;
        case 2:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP3);
            break;
        case 3:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP4);
            break;
        }
        if (w) w->SetFocus();
    }
}

//---------------------------------------------------
//�֐���	DestroyWindow()
//�@�\		�ݒ�𔽉f
//---------------------------------------------------
BOOL COptGeneral::DestroyWindow()
{
    WORD wVkCodeP, wModP;
    WORD wVkCodeF, wModF;

    m_ctrlPopupKey.GetHotKey(wVkCodeP, wModP);
    wModP = KeyHelper::HotkeyToMod(wModP);
    m_ctrlFifoKey.GetHotKey(wVkCodeF, wModF);
    wModF = KeyHelper::HotkeyToMod(wModF);
    theApp.m_ini.setHotkey(wVkCodeP, wModP, wVkCodeF, wModF);
    theApp.m_ini.m_bPutBackClipboard = ((CButton*)GetDlgItem(IDC_OPT_TOCLIP))->GetCheck() != 0;
    theApp.m_ini.m_bShowClipboardInTooltipOfNofifyIcon = ((CButton*)GetDlgItem(IDC_OPT_TOOLTIP_TITLE))->GetCheck() != 0;

    return CDialog::DestroyWindow();
}

void COptGeneral::hotkeyEnablePOP()
{
    m_ctrlPopupKey.EnableWindow(true);
    m_ctrlPopupKey.SetFocus();
}

void COptGeneral::hotkeyDisablePOP()
{
    m_ctrlPopupKey.EnableWindow(false);
}

void COptGeneral::hotkeyEnableFIFO()
{
    m_ctrlFifoKey.EnableWindow(true);
    m_ctrlFifoKey.SetFocus();
}

void COptGeneral::hotkeyDisableFIFO()
{
    m_ctrlFifoKey.EnableWindow(false);
}