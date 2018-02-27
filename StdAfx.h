// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(CCOMMONS_AFX_STDAFX_H__E384B99B_C511_4BD2_90AB_CBE3F6685454__INCLUDED_)
#define CCOMMONS_AFX_STDAFX_H__E384B99B_C511_4BD2_90AB_CBE3F6685454__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC OLE automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxmt.h>
#include <afxpriv.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

// Support for specific browse/detail applications
#include "vbctrl.h"
#include "appbrows.h"
#include "browsedc.h"

// Generic includes

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(CCOMMONS_AFX_STDAFX_H__E384B99B_C511_4BD2_90AB_CBE3F6685454__INCLUDED_)
