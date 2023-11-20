#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "AboutDialog.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CString strInfo(_T(""));

    TCHAR fileName[MAX_PATH + 1];
    if (::GetModuleFileName(NULL, fileName, MAX_PATH + 1)) {
        DWORD bufSize = ::GetFileVersionInfoSize(fileName, NULL);
        BYTE* pVersion = new BYTE[bufSize];
        if (pVersion) {
            if (::GetFileVersionInfo(fileName, NULL, bufSize, pVersion)) {
                UINT queryLen;
                WORD* pLangCode = nullptr;
                CString subBlock;

                VS_FIXEDFILEINFO* pFileInfo = nullptr;
                ::VerQueryValue(pVersion, _T("\\"), (void**)&pFileInfo, &queryLen);
                ::VerQueryValue(pVersion, _T("\\VarFileInfo\\Translation"), (void**)&pLangCode, &queryLen);

                TCHAR* pStrName = nullptr;
                subBlock.Format(_T("\\StringFileInfo\\%04X%04X\\ProductName"), pLangCode[0], pLangCode[1]);
                ::VerQueryValue(pVersion, (LPTSTR)(LPCTSTR)subBlock, (void**)&pStrName, &queryLen);
                if (queryLen > 0) {
                    strInfo += pStrName;
                }

                TCHAR* pStrVersion = nullptr;
                subBlock.Format(_T("\\StringFileInfo\\%04X%04X\\ProductVersion"), pLangCode[0], pLangCode[1]);
                ::VerQueryValue(pVersion, (LPTSTR)(LPCTSTR)subBlock, (void**)&pStrVersion, &queryLen);
                if (queryLen > 0) {
                    strInfo += " Version ";
                    strInfo += pStrVersion;
                }
            }
            delete[] pVersion;
        }
    }
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_VERSION_NAME, strInfo);
}
