//---------------------------------------------------
// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//---------------------------------------------------

#if !defined(AFX_STDAFX_H__46E9D15F_F71C_4687_B78A_255288A66BB2__INCLUDED_)
#define AFX_STDAFX_H__46E9D15F_F71C_4687_B78A_255288A66BB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x0600 // Target platform: Windows Vista
#define _USE_32BIT_TIME_T

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <windows.h>
#include <mmsystem.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__46E9D15F_F71C_4687_B78A_255288A66BB2__INCLUDED_)
