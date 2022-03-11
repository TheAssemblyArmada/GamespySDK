// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8A9E3741_2C27_4AC4_AB95_C33A19DEB835__INCLUDED_)
#define AFX_STDAFX_H__8A9E3741_2C27_4AC4_AB95_C33A19DEB835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>   // mfc core and standard components
#include <afxext.h>   // mfc extensions
#include <afxdtctl.h> // mfc support for internet explorer 4 common controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> // mfc support for windows common controls
#endif              // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h> // mfc socket extensions

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "../gp.h"
#include "../../common/gsavailable.h"

#endif // !defined(AFX_STDAFX_H__8A9E3741_2C27_4AC4_AB95_C33A19DEB835__INCLUDED_)
