// ISO8859_1Codec.h: interface for the CISO8859-1Codec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISO8859_1CODEC_H__8B1A9558_F1DF_4440_B0C5_9B7241EC2AAE__INCLUDED_)
#define AFX_ISO8859_1CODEC_H__8B1A9558_F1DF_4440_B0C5_9B7241EC2AAE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>
using namespace std;

typedef unsigned short   Unicode2Bytes;
typedef unsigned int     Unicode4Bytes;

#include "auxext_dfdllh1.h"


/*****************************************************************************
Converts from CP1252/WE8MSWIN1252 to ISO8859-1 (Latin1)
Usage to encode/decode Strings:
-------------------------------
((LPCTSTR)CISO8859_1Codec::Encode(string_to_encode);
((LPCTSTR)CISO8859_1Codec::Encode(string_to_decode);

******************************************************************************/

class CISO8859_1Codec
{
public:
	CISO8859_1Codec();
	virtual ~CISO8859_1Codec();

	static LPCTSTR Encode(CString &);	
};

#include "auxext_dfdllh2.h"

#endif // !defined(AFX_UTF8CODEC_H__8B1A9558_F1DF_4440_B0C5_9B7241EC2AAE__INCLUDED_)
