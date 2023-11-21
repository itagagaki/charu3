/*----------------------------------------------------------
    設定管理クラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include <fstream>
#include <list>
#include <vector>
#include "nlomann/json.hpp"
#include "jsonHelper.h"

#include "Init.h"
#include "Charu3.h"
#include "search.h"
#include "text.h"
#include "color.h"
#include "log.h"

#define DEFAULT_USERNAME _T("Charu3User")
#define APP_NAME         _T("Charu3")
#define SETTINGS_FILE    _T("settings.json")
#define STATE_FILE       _T("state.json")
#define LOG_FILE         _T("debug.log")

//キャプションのマッチ方法
enum {
    MATCH_FORWARD,
    MATCH_BACKWARD,
    MATCH_PARTIAL,
    MATCH_EXACT
};

#define FILEEXIST(s) (::GetFileAttributes(s) != 0xFFFFFFFF)

namespace {

    void ReadPredefined(std::vector<MACRO_STRUCT>& macro, CString filePath)
    {
        nlohmann::json snippets;
        try { snippets = nlohmann::json::parse(std::ifstream(filePath)); }
        catch (...) { snippets = nlohmann::json(); }
        for (nlohmann::json::iterator it = snippets.begin(); it != snippets.end(); it++) {
            nlohmann::json jnode = it.value();
            MACRO_STRUCT snippet;
            snippet.m_strName = jsonHelper::GetStringPropertyAsCString(jnode, "caption", _T(""));
            snippet.m_strMacro = jsonHelper::GetStringPropertyAsCString(jnode, "data", _T(""));
            snippet.m_cKind = static_cast<int>(jsonHelper::GetNumberProperty(jnode, "mode", 255));
            macro.push_back(snippet);
        }
    }

} // anonymous namespace

//---------------------------------------------------
//関数名	CInit
//機能		コンストラクタ
//---------------------------------------------------
CInit::CInit() : m_hHookDLL(nullptr), m_nOptionPage(0)
{
}

//---------------------------------------------------
//関数名	~CInit
//機能		デストラクタ
//---------------------------------------------------
CInit::~CInit()
{
    if (m_hHookDLL) {
        unHook();
    }
}

//---------------------------------------------------
//関数名	initialize()
//機能		初期化
//---------------------------------------------------
void CInit::initialize()
{
    TCHAR buf[MAX_PATH];
    TCHAR* cpName;

    // locale
    (void)m_locale.LoadString(APP_LOCALE);

    // user name
    DWORD dwSize = _countof(buf);
    if (::GetUserName(buf, &dwSize)) {
        m_strUserName = CString(buf);
    }
    else {
        m_strUserName = DEFAULT_USERNAME;
    }

    // app and plugin path
    GetModuleFileName(NULL, buf, _countof(buf));
    cpName = _tcsrchr(buf, _T('\\'));
    cpName++;
    *cpName = '\0';
    m_strAppPath = CString(buf);
    m_strRwPluginFolder = m_strAppPath + _T("rw_plugin");

    // Decide whether in appdata mode or portable mode
    m_bIsPortableMode = FILEEXIST(m_strAppPath + _T("FORCE_INTO_PORTABLE_MODE"));

    // app data path
    if (m_bIsPortableMode) {
        // portable mode
        m_strAppDataPath = m_strAppPath + _T(".");
    }
    else {
        // appdata mode
        SHGetSpecialFolderPath(NULL, buf, CSIDL_APPDATA, 0);
        m_strAppDataPath = CString(buf) + "\\" + APP_NAME;
        if (!FILEEXIST(m_strAppDataPath)) {
            CreateDirectory(m_strAppDataPath, NULL);
        }
    }

    // file paths for state, settings and log
    m_strStateFile = m_strAppDataPath + "\\" + STATE_FILE;
    m_strSettingsFile = m_strAppDataPath + "\\" + SETTINGS_FILE;
    m_strDebugLog = m_strAppDataPath + "\\" + LOG_FILE;

    // Display a dialog box that allows the user to reset the status and the settings when the Shift key is pressed
    if (::GetKeyState(VK_SHIFT) < 0) {
        CString strMessage;
        (void)strMessage.LoadString(APP_MES_RESET);
        strMessage += +_T("\n\n") + m_strStateFile + _T("\n") + m_strSettingsFile;
        int nRet = AfxMessageBox(strMessage, MB_YESNO | MB_APPLMODAL);
        if (IDYES == nRet) {
            (void)DeleteFile(m_strStateFile);
            (void)DeleteFile(m_strSettingsFile);
        }
    }

    // Read state
    try { m_state = nlohmann::json::parse(std::ifstream(m_strStateFile)); }
    catch (...) { m_state = nlohmann::json(); }

    m_strDataPath = jsonHelper::GetStringPropertyAsCString(m_state, "data.path", _T(""));
    m_strDataFormat = jsonHelper ::GetStringPropertyAsCString(m_state, "data.format", DAT_FORMAT);
    m_bReadOnly = jsonHelper::GetBoolProperty(m_state, "data.readOnly", false);
    m_DialogSize.x = static_cast<LONG>(jsonHelper::GetNumberProperty(m_state, "treeview.width", 250));
    m_DialogSize.y = static_cast<LONG>(jsonHelper::GetNumberProperty(m_state, "treeview.height", 350));
    m_nSearchTarget = static_cast<int>(jsonHelper::GetNumberProperty(m_state, "find.target", SEARCH_TARGET_NAME));
    m_nSearchLogic = static_cast<int>(jsonHelper::GetNumberProperty(m_state, "find.logic", SEARCH_TARGET_DATA));
    m_bSearchCaseInsensitive = jsonHelper::GetBoolProperty(m_state, "find.caseInsensitive", true);
    m_strSearchKeywords = jsonHelper::GetStringPropertyAsCString(m_state, "find.keywords", _T(""));
    m_nSelectID = static_cast<int>(jsonHelper::GetNumberProperty(m_state, "internal.selectId", -1));
    {
        time_t lTime;
        m_nTreeID = static_cast<int>(jsonHelper::GetNumberProperty(m_state, "internal.treeId", time(&lTime)));
    }
    m_nRecNumber = static_cast<int>(jsonHelper::GetNumberProperty(m_state, "internal.recordNumber", 0));

    // Read settings
    try { m_settings = nlohmann::json::parse(std::ifstream(m_strSettingsFile)); }
    catch (...) { m_settings = nlohmann::json(); }

    m_bPutBackClipboard = jsonHelper::GetBoolProperty(m_settings, "clipboard.putBackAfterPasting", false);
    m_nClipboardOpenDelay = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "clipboard.openDelay", 0));
    m_nClipboardRetryInterval = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "clipboard.retryInterval", 50));
    m_nClipboardRetryTimes = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "clipboard.retryTimes", 10));

    m_nDoubleKeyPOP = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "hotkey.popup", 0));
    m_nDCKeyPopTime = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "hotkey.popup.hitTime", 250));
    m_uVK_Pouup = static_cast<UINT>(jsonHelper::GetNumberProperty(m_settings, "hotkey.popup.keyCode", 88)); // X
    m_uMod_Pouup = static_cast<UINT>(jsonHelper::GetNumberProperty(m_settings, "hotkey.popup.keyModifier", MOD_ALT));
    m_nDoubleKeyFIFO = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "hotkey.stockmode", 0));
    m_nDCKeyFifoTime = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "hotkey.stockmode.hitTime", 250));
    m_uVK_Fifo = static_cast<UINT>(jsonHelper::GetNumberProperty(m_settings, "hotkey.stockmode.keyCode", 83)); // S
    m_uMod_Fifo = static_cast<UINT>(jsonHelper::GetNumberProperty(m_settings, "hotkey.stockmode.keyModifier", MOD_CONTROL | MOD_SHIFT));

    m_nIconClick = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "notifyIcon.clickedBehavior", 0));
    m_bShowClipboardInTooltipOfNofifyIcon = jsonHelper::GetBoolProperty(m_settings, "notifyIcon.showClipboard", true);

    m_nFifo = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "stockmode.behavior", 1));
    m_bAutoOff = jsonHelper::GetBoolProperty(m_settings, "stockmode.autoTurnOff", false);
    m_bCleanupAtTurnOff = jsonHelper::GetBoolProperty(m_settings, "stockmode.cleanupAtTurnOff", false);
    m_bDontSaveSameDataAsLast = jsonHelper::GetBoolProperty(m_settings, "stockmode.dontSaveSameDataAsLast", false);
    m_strCopySound = jsonHelper::GetStringPropertyAsCString(m_settings, "stockmode.sound.copy", _T("pu.wav"));
    m_strPasteSound = jsonHelper::GetStringPropertyAsCString(m_settings, "stockmode.sound.paste", _T(""));

    m_nToolTipTime = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "tooltip.autopop", 30000));
    m_nToolTipDelay = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "tooltip.delay", 300));

    m_nPopupPos = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.popupPosition", 0));
    m_posCaretHosei.x = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.popupOffset.x", -20));
    m_posCaretHosei.y = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.popupOffset.y", 0));
    m_nSelectByTypingFinalizePeriod = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.selectByTyping.finalizePeriod", 350));
    m_bSelectByTypingCaseInsensitive = jsonHelper::GetBoolProperty(m_settings, "treeview.selectByTyping.caseInsensitive", true);
    m_bSelectByTypingAutoPaste = jsonHelper::GetBoolProperty(m_settings, "treeview.selectByTyping.autoPaste", false);
    m_bSelectByTypingAutoExpand = jsonHelper::GetBoolProperty(m_settings, "treeview.selectByTyping.autoExpand", false);
    m_nToolTip = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.tooltip", 0));
    m_bScrollbarVertical = jsonHelper::GetBoolProperty(m_settings, "treeview.scrollbar.vertical", true);
    m_bScrollbarHorizontal = jsonHelper::GetBoolProperty(m_settings, "treeview.scrollbar.horizontal", true);
    m_bSingleExpand = jsonHelper::GetBoolProperty(m_settings, "treeview.expandOnSingleClick", false);
    m_bSingleEnter = jsonHelper::GetBoolProperty(m_settings, "treeview.pasteBySingleClick", false);
    m_bKeepSelection = jsonHelper::GetBoolProperty(m_settings, "treeview.keepSelection", true);
    m_bKeepFolders = jsonHelper::GetBoolProperty(m_settings, "treeview.keepFolders", true);

    m_nBorderColor = Color::Parse(jsonHelper::GetStringProperty(m_settings, "treeview.style.borderColor", "#ff9900"));
    m_nBackgroundColor = Color::Parse(jsonHelper::GetStringProperty(m_settings, "treeview.style.backgroundColor", "#ffffee"));
    m_nTextColor = Color::Parse(jsonHelper::GetStringProperty(m_settings, "treeview.style.textColor", "#663300"));
    m_strFontName = jsonHelper::GetStringPropertyAsCString(m_settings, "treeview.style.fontName", "");
    m_nFontSize = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.style.fontSize", 100));
    m_strResourceName = jsonHelper::GetStringPropertyAsCString(m_settings, "treeview.style.iconFile", "");
    m_nOpacity = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "treeview.opacity", 100));

    m_nWindowCheckInterval = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "windowCheckInterval", 400));

    m_stealth.clear();
    jsonHelper::AppendCStringArray(m_settings, "stealthPrograms", m_stealth);

    m_bDebug = jsonHelper::GetBoolProperty(m_settings, "debug", false);

    m_KeyList.clear();
    nlohmann::json keyEventByWindow = m_settings["keyEvent.byWindow"];
    for (nlohmann::json::iterator it = keyEventByWindow.begin(); it != keyEventByWindow.end(); it++) {
        nlohmann::json jnode = it.value();
        CHANGE_KEY node;
        node.m_strTitle = jsonHelper::GetStringPropertyAsCString(jnode, "caption", _T(""));
        node.m_nMatch = static_cast<int>(jsonHelper::GetNumberProperty(jnode, "captionMatchCondition", 0));
        node.m_nHistoryLimit = static_cast<int>(jsonHelper::GetNumberProperty(jnode, "copyLimit", -1));
        node.m_sCopyPasteKey = COPYPASTE_KEY(jnode["keyEvent"]);
        m_KeyList.push_back(node);
    }
    m_defKeySet = COPYPASTE_KEY(m_settings["keyEvent.default"]);
    m_nHistoryLimit = static_cast<int>(jsonHelper::GetNumberProperty(m_settings, "keyEvent.default.copyLimit", -1));

    SaveSettings();

    // Read snippets
    m_vctMacro.clear();
    ReadPredefined(m_vctMacro, m_strAppPath + "_locale\\" + m_locale + "\\snippets.json");
    ReadPredefined(m_vctMacro, m_strAppDataPath + "\\snippets.json");

    // Read option properties
    m_vctDataMacro.clear();
    ReadPredefined(m_vctDataMacro, m_strAppPath + "_locale\\" + m_locale + "\\options.json");

    if (m_bDebug) {
        gLogger.SetLogFile(m_strDebugLog);
    }
}

//---------------------------------------------------
//関数名	SaveSettings()
//機能		書き込み
//---------------------------------------------------
void CInit::SaveSettings()
{
    m_settings["clipboard.putBackAfterPasting"] = m_bPutBackClipboard;
    m_settings["clipboard.openDelay"] = m_nClipboardOpenDelay;
    m_settings["clipboard.retryInterval"] = m_nClipboardRetryInterval;
    m_settings["clipboard.retryTimes"] = m_nClipboardRetryTimes;

    m_settings["hotkey.popup"] = m_nDoubleKeyPOP;
    m_settings["hotkey.popup.hitTime"] = m_nDCKeyPopTime;
    m_settings["hotkey.popup.keyCode"] = m_uVK_Pouup;
    m_settings["hotkey.popup.keyModifier"] = m_uMod_Pouup;
    m_settings["hotkey.stockmode"] = m_nDoubleKeyFIFO;
    m_settings["hotkey.stockmode.hitTime"] = m_nDCKeyFifoTime;
    m_settings["hotkey.stockmode.keyCode"] = m_uVK_Fifo;
    m_settings["hotkey.stockmode.keyModifier"] = m_uMod_Fifo;

    m_settings["notifyIcon.clickedBehavior"] = m_nIconClick;
    m_settings["notifyIcon.showClipboard"] = m_bShowClipboardInTooltipOfNofifyIcon;

    m_settings["stockmode.behavior"] = m_nFifo;
    m_settings["stockmode.autoTurnOff"] = m_bAutoOff;
    m_settings["stockmode.cleanupAtTurnOff"] = m_bCleanupAtTurnOff;
    m_settings["stockmode.dontSaveSameDataAsLast"] = m_bDontSaveSameDataAsLast;
    m_settings["stockmode.sound.copy"] = Text::ConvertUnicodeToUTF8(m_strCopySound);
    m_settings["stockmode.sound.paste"] = Text::ConvertUnicodeToUTF8(m_strPasteSound);

    m_settings["tooltip.autopop"] = m_nToolTipTime;
    m_settings["tooltip.delay"] = m_nToolTipDelay;

    m_settings["treeview.popupPosition"] = m_nPopupPos;
    m_settings["treeview.popupOffset.x"] = m_posCaretHosei.x;
    m_settings["treeview.popupOffset.y"] = m_posCaretHosei.y;
    m_settings["treeview.selectByTyping.finalizePeriod"] = m_nSelectByTypingFinalizePeriod;
    m_settings["treeview.selectByTyping.caseInsensitive"] = m_bSelectByTypingCaseInsensitive;
    m_settings["treeview.selectByTyping.autoPaste"] = m_bSelectByTypingAutoPaste;
    m_settings["treeview.selectByTyping.autoExpand"] = m_bSelectByTypingAutoExpand;
    m_settings["treeview.tooltip"] = m_nToolTip;
    m_settings["treeview.scrollbar.vertical"] = m_bScrollbarVertical;
    m_settings["treeview.scrollbar.horizontal"] = m_bScrollbarHorizontal;
    m_settings["treeview.expandOnSingleClick"] = m_bSingleExpand;
    m_settings["treeview.pasteBySingleClick"] = m_bSingleEnter;
    m_settings["treeview.keepSelection"] = m_bKeepSelection;
    m_settings["treeview.keepFolders"] = m_bKeepFolders;

    m_settings["treeview.style.backgroundColor"] = Color::String(m_nBackgroundColor);
    m_settings["treeview.style.borderColor"] = Color::String(m_nBorderColor);
    m_settings["treeview.style.textColor"] = Color::String(m_nTextColor);
    m_settings["treeview.style.fontName"] = Text::ConvertUnicodeToUTF8(m_strFontName);
    m_settings["treeview.style.fontSize"] = m_nFontSize;
    m_settings["treeview.style.iconFile"] = Text::ConvertUnicodeToUTF8(m_strResourceName);
    m_settings["treeview.opacity"] = m_nOpacity;

    m_settings["windowCheckInterval"] = m_nWindowCheckInterval;

    m_settings["debug"] = m_bDebug;

    nlohmann::json keyEventByWindow;
    std::list<CHANGE_KEY>::iterator it;
    for (it = m_KeyList.begin(); it != m_KeyList.end(); it++) {
        nlohmann::json node;
        node["caption"] = Text::ConvertUnicodeToUTF8(it->m_strTitle);
        node["captionMatchCondition"] = it->m_nMatch;
        node["keyEvent"] = it->m_sCopyPasteKey.ToJson();
        node["copyLimit"] = it->m_nHistoryLimit;
        keyEventByWindow.push_back(node);
    }
    m_settings["keyEvent.byWindow"] = keyEventByWindow;
    m_settings["keyEvent.default"] = m_defKeySet.ToJson();
    m_settings["keyEvent.default.copyLimit"] = m_nHistoryLimit;

    m_settings["stealthPrograms"].clear();
    for (const CString& s : m_stealth) {
        m_settings["stealthPrograms"].push_back(Text::ConvertUnicodeToUTF8(s));
    }

    try { std::ofstream(m_strSettingsFile) << m_settings.dump(1, '\t') << "\n"; }
    catch (...) {}
}

//---------------------------------------------------
//関数名	writeAllInitData()
//機能		書き込み
//---------------------------------------------------
void CInit::writeAllInitData()
{
    SaveSettings();
    writeEnvInitData();
}

//---------------------------------------------------
//関数名	writeEnvInitData()
//機能		環境情報だけ書き込み
//---------------------------------------------------
void CInit::writeEnvInitData()
{
    m_state["data.path"] = Text::ConvertUnicodeToUTF8(m_strDataPath);
    m_state["data.format"] = Text::ConvertUnicodeToUTF8(m_strDataFormat);
    m_state["data.readOnly"] = m_bReadOnly;
    m_state["treeview.width"] = m_DialogSize.x;
    m_state["treeview.height"] = m_DialogSize.y;
    m_state["find.target"] = m_nSearchTarget;
    m_state["find.logic"] = m_nSearchLogic;
    m_state["find.caseInsensitive"] = m_bSearchCaseInsensitive;
    m_state["find.keywords"] = Text::ConvertUnicodeToUTF8(m_strSearchKeywords);
    m_state["internal.selectId"] = m_nSelectID;
    m_state["internal.treeId"] = m_nTreeID;
    m_state["internal.recordNumber"] = m_nRecNumber;
    try { std::ofstream(m_strStateFile) << m_state.dump(1, '\t') << "\n"; }
    catch (...) {}
}

//---------------------------------------------------
//関数名	getPasteHotKey
//機能		貼り付け系キー設定を取得
//---------------------------------------------------
void CInit::getPasteHotKey(UINT* uKey, UINT* uMod, UINT* uCopyKey, UINT* uCopyMod)
{
    *uMod = m_defKeySet.m_uMod_Paste;
    *uKey = m_defKeySet.m_uVK_Paste;

    *uCopyMod = m_defKeySet.m_uMod_Copy;
    *uCopyKey = m_defKeySet.m_uVK_Copy;
}
//---------------------------------------------------
//関数名	setPasteHotkey()
//機能		ホットキー設定をセット
//---------------------------------------------------
void CInit::setPasteHotkey(UINT uKey, UINT uMod, UINT uCopyKey, UINT uCopyMod)
{
    m_defKeySet.m_uMod_Paste = uMod;
    m_defKeySet.m_uVK_Paste = uKey;
    m_defKeySet.m_uMod_Copy = uCopyMod;
    m_defKeySet.m_uVK_Copy = uCopyKey;
}
//---------------------------------------------------
//関数名	setHotkey()
//機能		ホットキー設定をセット
//---------------------------------------------------
void CInit::setHotkey(UINT uPopKey, UINT uPopMod, UINT uFifoKey, UINT uFifoMod)
{
    m_uMod_Pouup = uPopMod;
    m_uVK_Pouup = uPopKey;
    m_uMod_Fifo = uFifoMod;
    m_uVK_Fifo = uFifoKey;
}
//---------------------------------------------------
//関数名	getHotkey()
//機能		ホットキー設定を取得
//---------------------------------------------------
void CInit::getHotKey(UINT* uPopKey, UINT* uPopMod, UINT* uFifoKey, UINT* uFifoMod)
{
    *uPopMod = m_uMod_Pouup;
    *uPopKey = m_uVK_Pouup;
    *uFifoMod = m_uMod_Fifo;
    *uFifoKey = m_uVK_Fifo;
}

//---------------------------------------------------
//関数名	setHookKey()
//機能		キーフックを設定
//---------------------------------------------------
bool CInit::setHookKey(HWND hWnd)
{
    bool isRet = false;
    if (!isWindows98()) return true;
    setHook pSetHook;

    if (!m_hHookDLL) {
        m_hHookDLL = LoadLibrary(m_strAppPath + _T("\\HookSet.dll"));
        if (!m_hHookDLL) return isRet;
        pSetHook = (setHook)GetProcAddress(m_hHookDLL, "SetHook");
        if (!pSetHook) return isRet;
        pSetHook(hWnd, WM_KEY_HOOK);

        isRet = true;
    }
    return isRet;
}

//---------------------------------------------------
//関数名	unHookKey()
//機能		フックを解除
//---------------------------------------------------
void CInit::unHookKey()
{
    if (!isWindows98()) return;
    FARPROC pUnHook;
    //フックの解除
    if (m_hHookDLL) {
        pUnHook = ::GetProcAddress(m_hHookDLL, "UnHook");
        if (pUnHook) pUnHook();
        FreeLibrary(m_hHookDLL);
        m_hHookDLL = nullptr;
    }
}

//---------------------------------------------------
//関数名	getAppendKeyInit()
//機能		追加キー設定を取得
//---------------------------------------------------
COPYPASTE_KEY CInit::getAppendKeyInit(CString strWinName, int nNumber)
{
    bool isMatch = false;

    //キー設定リストから適合するものを探す
    std::list<CHANGE_KEY>::iterator it;
    CHANGE_KEY key;
    COPYPASTE_KEY ret;
    int nFindPoint;

    ret.m_nMessage = -1;
    for (it = m_KeyList.begin(); it != m_KeyList.end(); it++) {
        key = *it;
        if (key.m_nMatch != MATCH_EXACT) {
            nFindPoint = strWinName.Find(key.m_strTitle);
        }

        // 前方一致
        if (key.m_nMatch == MATCH_FORWARD) {
            if (nFindPoint == 0) isMatch = true;
        }
        // 後方一致
        else if (key.m_nMatch == MATCH_BACKWARD && nFindPoint >= 0) {
            if (nFindPoint == (strWinName.GetLength() - key.m_strTitle.GetLength())) isMatch = true;
        }
        // 部分一致
        else if (key.m_nMatch == MATCH_PARTIAL) {
            if (nFindPoint >= 0) isMatch = true;
        }
        // 完全一致
        else if (key.m_nMatch == MATCH_EXACT) {
            if (strWinName == key.m_strTitle) isMatch = true;
        }

        if (isMatch) { //見つかった
            if (nNumber > 0) {
                nNumber--;
                isMatch = false;
            }
            else {
                ret = key.m_sCopyPasteKey;
                break;
            }
        }
    }
    //	if(!isMatch)		ret = m_key.m_defKeySet;//デフォルトキー設定

    return ret;
}

//---------------------------------------------------
//関数名	getAppendKeyInit()
//機能		追加キー設定を取得
//---------------------------------------------------
CHANGE_KEY CInit::getAppendKeyInit2(CString strWinName)
{
    bool isMatch = false;

    //キー設定リストから適合するものを探す
    std::list<CHANGE_KEY>::iterator it;
    CHANGE_KEY key;
    int nFindPoint;

    for (it = m_KeyList.begin(); it != m_KeyList.end(); it++) {
        key = *it;
        if (key.m_nMatch != MATCH_EXACT) {
            nFindPoint = strWinName.Find(key.m_strTitle);
        }

        // 前方一致
        if (key.m_nMatch == MATCH_FORWARD) {
            if (nFindPoint == 0) isMatch = true;
        }
        // 後方一致
        else if (key.m_nMatch == MATCH_BACKWARD && nFindPoint >= 0) {
            if (nFindPoint == (strWinName.GetLength() - key.m_strTitle.GetLength())) isMatch = true;
        }
        // 部分一致
        else if (key.m_nMatch == MATCH_PARTIAL) {
            if (nFindPoint >= 0) isMatch = true;
        }
        // 完全一致
        else if (key.m_nMatch == MATCH_EXACT) {
            if (strWinName == key.m_strTitle) isMatch = true;
        }

        if (isMatch) { //見つかった
            return key;
        }
    }

    key.m_nMatch = -1;
    return key;
}
