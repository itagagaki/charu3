#include "stdafx.h"

#include "commonDialog.h"
#include "resource.h"

CString CommonDialog::NewFilePath(LPCWSTR ext)
{
    CString strTitle, strDisplay, strPattern;
    (void)strTitle.LoadString(APP_INF_EXPORT_CAPTION);
    (void)strDisplay.LoadString(APP_INF_FILE_FILTER_C3D_DISPLAY);
    (void)strPattern.LoadString(APP_INF_FILE_FILTER_C3D_PATTERN);
    CString strFilter = strDisplay + _T('\0') + strPattern + _T('\0') + _T('\0') + _T('\0');

    OPENFILENAME param;
    ZeroMemory(&param, sizeof param);
    TCHAR tcPath[MAX_PATH] = _T("");
    param.lStructSize = sizeof param;
    param.hwndOwner = NULL;
    param.lpstrTitle = strTitle.GetBuffer();
    param.lpstrFilter = strFilter.GetBuffer();
    param.lpstrDefExt = ext;
    param.lpstrCustomFilter = NULL;
    param.nFilterIndex = 1;
    param.lpstrFile = tcPath;
    param.nMaxFile = MAX_PATH;
    param.lpstrFileTitle = NULL;
    param.lpstrInitialDir = NULL;
    param.Flags = OFN_OVERWRITEPROMPT;
    param.nFileOffset = 0;
    param.nFileExtension = 0;
    if (GetSaveFileName(&param)) {
        //if (param.Flags & OFN_EXTENSIONDIFFERENT) {
        //	_tcscat_s(tcPath, MAX_PATH, _T("."));
        //	_tcscat_s(tcPath, MAX_PATH, _T(DAT_EXT));
        //}
        return CString(tcPath);
    }
    else {
        return CString(_T(""));
    }
}
