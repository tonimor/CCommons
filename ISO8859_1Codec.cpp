// ISO8859_1Codec.cpp: implementation of the CISO8859_1Codec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ISO8859_1Codec.h"


CISO8859_1Codec::CISO8859_1Codec()
{
}

CISO8859_1Codec::~CISO8859_1Codec()
{
}

namespace
{
	int conversion_table[][2]=
	{
		// -1 = UNASSIGNED
		{0x80, 0x0A4},
		{0x81, -1},
		{0x8A, 0x0A6},
		{0x8C, 0x0BC},
		{0x8D, -1},
		{0x8E, 0xB4},
		{0x8F, -1},
		{0x90, -1},
		{0x9A, 0x0A8},
		{0x9C, 0xBD},
		{0x9D, -1},
		{0x9E, 0x0B8},
		{0x9F, 0x0BE},
	};
}



LPCTSTR CISO8859_1Codec::Encode(CString& str)
{	
	for(int i = 0; i < str.GetLength(); i++ ) 
	{
		unsigned int a = (unsigned int) str[i];		
		for (int b=0;b<(sizeof(conversion_table)/(sizeof(int)*2));b++)
		{
			if (conversion_table[b][0]==a)
			{
				if (conversion_table[b][1]==-1)
					a = '?'; 
				else
					a = conversion_table[b][1];
				break;
			}
		}
		str.SetAt(i, (unsigned char) a);
	}

	return (LPCTSTR)str;
}
