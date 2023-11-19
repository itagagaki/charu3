/*----------------------------------------------------------
    Charu3ヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_CHARU3_H__05C003AF_297C_4BFB_AFB3_822EAE3D3787__INCLUDED_)
#define AFX_CHARU3_H__05C003AF_297C_4BFB_AFB3_822EAE3D3787__INCLUDED_

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
#include "ClipBord.h"
#include "Init.h"

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
typedef bool(*CharuPlugIn)(TCHAR*, TCHAR*, int, STRING_DATA*, void*);

typedef bool (*setHook)(const HWND hWnd, const int msg);
typedef void (*unHook)(void);

//---------------------------------------------------
// ホットキー設定構造体
//---------------------------------------------------
struct HOT_KEY_CODE
{
    UINT m_uModKey{};
    UINT m_uVkCode{};
    bool m_isDoubleClick{};
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

    void popupTreeWindow(POINT pos, bool keepSelection, HTREEITEM hOpenItem = NULL);
    void closeTreeWindow(int nRet);
    void pasteData(CString strPaste, COPYPASTE_KEY key, HWND hWnd);
    void execData(CString strPaste, COPYPASTE_KEY key, HTREEITEM hTargetItem, HWND hWnd);
    void execKeyMacro(CString strKeyMacro);

    CString getSelectString(COPYPASTE_KEY key, HWND hWnd = NULL);
    void keyUpDown(UINT uMod, UINT uVKCode, int nFlag);
    void keyUpDownC2(UINT uMod, UINT uVKCode, int nFlag);
    void keyUpDownMessage(UINT uMod, UINT uVKCode, int nFlag, HWND hWnd);

    void registerHotkey();
    void stopHotkey();
    void setAppendHotKey();
    void stopAppendHotKey();
    CString convertMacro(STRING_DATA* SourceData, CString strSelect, CString strClip, CString strSoftName);

    HWND getAppWnd() { return m_hSelfWnd; }
    bool isCloseKey() { return m_isCloseKey; }
    void popupTreeWinMC(HWND hForeground);

    void onClipboardChanged(CString strClipboard);
    void toggleStockMode();
    bool getStockSW() { return m_isStockMode; }
    int  getPhase() { return m_nPhase; }
    void fifoClipbord();
    void resetTreeDialog();

    bool SelectFile();
    CString NewFile();
    void SaveData();

    CMyTreeDialog* m_pTreeDlg;
    CInit m_ini;
    CClipBoard m_clipboard;
    CCharu3Tree* m_pTree;
    CMainFrame* m_pMainFrame;
    COPYPASTE_KEY m_keySet;
    FOCUS_INFO m_focusInfo;
    HWND m_hSelfWnd;

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CCharu3App)
public:
    virtual BOOL InitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// インプリメンテーション

public:
    //{{AFX_MSG(CCharu3App)
    afx_msg void OnExit();
    afx_msg void OnAbout();
    afx_msg void OnIssues();
    afx_msg void OnDiscussions();
    afx_msg void OnWiki();
    afx_msg void OnOption();
    afx_msg void OnAddData();
    afx_msg void OnChangData();
    afx_msg void OnExport();
    afx_msg void OnStockStop();
    afx_msg void OnResetTree();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    void getPopupPos(POINT* pPos, int nPosType);
    void adjustLocation(POINT* pos);
    void convHotKeyConf(CString strKey, UINT* pMod, UINT* pVK, bool* isDoubleClick);
    int getKeycode(TCHAR* szKeyName, bool scanLayout);
    bool setAppendKeyInit(HWND hTopWindow, COPYPASTE_KEY* keySet);
    void playHotItem(int nTarget);
    void playData(STRING_DATA data, CString strClip, CString strSelect, bool isPaste, bool isChange = true);

    HTREEITEM m_hSelectItemBkup;
    DWORD m_dwDoubleKeyPopTime, m_dwDoubleKeyFifoTime;

    std::vector<HOT_KEY_CODE> m_hotkeyVector;
    KEY_CODE_NAME m_keyStruct[256];
    bool init();
    HANDLE m_hMutex;				//重複起動判別ハンドル
    HWND m_hActiveKeyWnd;
    HINSTANCE m_hLangDll;

    bool m_isCloseKey, m_isStockMode;
    BOOL m_isImeStatus;
    int m_nPhase;
    CString m_strSavedClipboard, m_strPreviousStocked, m_strPreviousRecordedToHistory;
};

extern CCharu3App theApp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHARU3_H__05C003AF_297C_4BFB_AFB3_822EAE3D3787__INCLUDED_)
