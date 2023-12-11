/*----------------------------------------------------------
    Charu3ヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_CHARU3_H__05C003AF_297C_4BFB_AFB3_822EAE3D3787__INCLUDED_)
#define AFX_CHARU3_H__05C003AF_297C_4BFB_AFB3_822EAE3D3787__INCLUDED_

// TODO: Consistent naming

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#if defined(UNICODE) || defined(_UNICODE)
#pragma comment(linker, "/entry:\"wWinMainCRTStartup\"")
#endif

#include "MainFrm.h"
#include "MyTreeDialog.h"
#include "Init.h"
#include "clipboard.h"
#include "window.h"

#define DAT_FORMAT			"Charu3Data1.0"
#define DAT_FORMAT2			"Charu3Data1.1"
#define DAT_EXT				"c3d"

#define TIMER_ACTIVE			606
#define TIMER_MOUSE				607
#define CHARU_QUICK_TIMER		610

#define WM_TASKTRAY    (WM_USER+100)
#define WM_TREE_CLOSE  (WM_USER+102)
#define WM_TIPS_CHANGE (WM_USER+103)
#define WM_KEY_HOOK    (WM_USER+104)
#define WM_FIND_CLOSE  (WM_USER+105)
#define WM_FIND_NEXT   (WM_USER+106)
#define WM_FIND_PREV   (WM_USER+107)
#define WM_FIND_ONCE   (WM_USER+108)

//ポップアップ位置種別
enum {
    POPUP_POS_CARET,
    POPUP_POS_MOUSE,
    POPUP_POS_LEFT_U,
    POPUP_POS_RIGHT_U,
    POPUP_POS_CENTOR,
    POPUP_POS_LEFT_D,
    POPUP_POS_RIGHT_D
};

#define PHASE_START		0x00
#define PHASE_IDOL		0x01
#define PHASE_POPUP		0x02
#define PHASE_OPTION	0x04
#define PHASE_LOCK		0x08

//プラグイン関数宣言
typedef bool(*CharuPlugIn)(TCHAR*, TCHAR*, int, const STRING_DATA*, void*);

typedef bool (*setHook)(const HWND hWnd, const int msg);
typedef void (*unHook)(void);

//---------------------------------------------------
// ホットキー設定構造体
//---------------------------------------------------
struct HOT_KEY_CODE
{
    UINT m_uModKey{};
    UINT m_uVkCode{};
    bool m_bDoubleStroke{};
    HTREEITEM m_hItem{};
    int  m_nDataID{};
    DWORD m_dwDoubleKeyTime{};
    CString m_strMacroName;
};
struct KEY_CODE_NAME
{
    CString strName;
    int nKeyCode{};
};

//---------------------------------------------------
// クラス定義
//---------------------------------------------------
class CCharu3App : public CWinApp
{
public:
    CCharu3App();
    ~CCharu3App();

    HWND GetAppWnd() { return m_hSelfWnd; }
    void CheckFocusInfo(HWND hActiveWnd);
    void OnClick(HWND hActiveWnd);
    int GetPhase() { return m_nPhase; }
    bool isCloseKey() { return m_isCloseKey; }

    void BeForeground();
    void RedrawDataTreeView();

    /**
     * @brief Set text to the clipboard.
     * @param sData Text to set.
     * @return True on success.
     */
    bool SetClipboardText(const CString sData) {
        return m_clipboard.SetClipboardText(sData, m_ini.m_nClipboardRetryTimes, m_ini.m_nClipboardRetryInterval); 
    }

    /**
     * @brief Get text from the clipboard.
     * @param sData Where to store the retrieved text.
     * @return True on success.
     */
    bool GetClipboardText(CString& sData) {
        return m_clipboard.GetClipboardText(sData, m_ini.m_nClipboardRetryTimes, m_ini.m_nClipboardRetryInterval);
    }

    /**
     * @brief Record text as an entry in Stock Mode and as items in the history folders.
     * @param text The text to record.
     */
    void Record(CString text);

    CInit m_ini;
    CCharu3Tree* m_pTree;

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
private:
    //{{AFX_VIRTUAL(CCharu3App)
    virtual BOOL InitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

    // インプリメンテーション
private:
    bool init();
    void resetTreeDialog();
    bool SelectFile();
    void SaveData();

    void popupTreeWindow(POINT pos, bool keepSelection, HTREEITEM hOpenItem = NULL);
    void closeTreeWindow(int nRet);
    void getPopupPos(POINT* pPos, int nPosType);
    void adjustLocation(POINT* pos);

    void ToggleStockMode();
    void PullOneTimeData();

    void pasteData(CString strPaste, COPYPASTE_KEY key, HWND hWnd);
    void execData(CString strPaste, COPYPASTE_KEY key, HTREEITEM hTargetItem, HWND hWnd);
    void execKeyMacro(CString strKeyMacro);
    void playHotItem(int nTarget);
    void playData(const STRING_DATA* dataPtr, CString strClip, CString strSelect, bool isPaste, bool isChange = true);
    CString convertMacro(const STRING_DATA* sourceDataPtr, CString strSelect, CString strClip, CString strSoftName);

    /**
     * @brief Get selected text from the window that has the keyboard focus.
     * @note The retrieved text also remains on the clipboard that was passed through.
     */
    CString GetSelectedText();

    void keyUpDown(UINT uMod, UINT uVKCode, int nFlag);
    void keyUpDownC2(UINT uMod, UINT uVKCode, int nFlag);
    void keyUpDownMessage(UINT uMod, UINT uVKCode, int nFlag, HWND hWnd);

    void ParseHotkeyDescriptor(CString strKey, UINT* pMod, UINT* pVK, bool* pDoubleStroke);
    int getKeycode(TCHAR* szKeyName, bool scanLayout);
    void RegisterHotkeys();
    void UnregisterHotkeys();
    void RegisterAdditionalHotkeys();
    void UnregisterAdditionalHotkeys();
    bool setAppendKeyInit(HWND hTopWindow, COPYPASTE_KEY* keySet);

    HANDLE m_hMutex;				//重複起動判別ハンドル
    HINSTANCE m_hLangDll;
    HWND m_hSelfWnd;
    FOCUS_INFO m_focusInfo;
    CMainFrame* m_pMainFrame;

    bool m_isCloseKey, m_isStockMode;
    BOOL m_isImeStatus;
    int m_nPhase;

    CClipBoard m_clipboard;

    CMyTreeDialog* m_pTreeDlg;
    HTREEITEM m_hSelectItemBkup;
    DWORD m_dwDoubleKeyPopTime, m_dwDoubleKeyFifoTime;
    CString m_strSavedClipboard, m_strPreviousStocked;

    COPYPASTE_KEY m_keySet;
    std::vector<HOT_KEY_CODE> m_hotkeyVector;
    KEY_CODE_NAME m_keyStruct[256];
    HWND m_hActiveKeyWnd;

    // 生成されたメッセージ マップ関数
public:
    //{{AFX_MSG(CCharu3App)
    afx_msg void OnOption();
    afx_msg void OnExit();
protected:
    afx_msg void OnAbout();
    afx_msg void OnIssues();
    afx_msg void OnDiscussions();
    afx_msg void OnWiki();
    afx_msg void OnAddData();
    afx_msg void OnChangData();
    afx_msg void OnExport();
    afx_msg void OnStockStop();
    afx_msg void OnResetTree();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

extern CCharu3App theApp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHARU3_H__05C003AF_297C_4BFB_AFB3_822EAE3D3787__INCLUDED_)
