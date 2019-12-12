// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#define WINVER 0x0502

#if !defined(AFX_STDAFX_H__04C2E4D3_EA75_4C14_9D30_BD244C09576D__INCLUDED_)
#define AFX_STDAFX_H__04C2E4D3_EA75_4C14_9D30_BD244C09576D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>
#include <afxtempl.h>
#include <afxinet.h>
#include <comdef.h>
#include <msxml2.h>
#include <atlbase.h>

#pragma warning(disable:4996)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__04C2E4D3_EA75_4C14_9D30_BD244C09576D__INCLUDED_)
