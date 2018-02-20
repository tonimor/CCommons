#ifndef CCGLOBALS_H
#define CCGLOBALS_H

#define ON_LOCK_MAINWINDOW_UPDATE		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_SETREDRAW, FALSE, 0)
#define ON_UNLOCK_MAINWINDOW_UPDATE		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_SETREDRAW, TRUE, 0)
#define	ON_BITMAP_BUTTON(uIDC, hbmp)	::SendMessage(GetDlgItem(uIDC)->m_hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbmp)
#define	ON_ICON_BUTTON(uIDC, hicon)		::SendMessage(GetDlgItem(uIDC)->m_hWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hicon)

#include "CCommons_dfdllh1.h"

namespace CCGlobals
{
/* Console colors *****************************************************
    FOREGROUND_BLUE........0x0001: Text color contains blue.
    FOREGROUND_GREEN.......0x0002: Text color contains green.
    FOREGROUND_RED.........0x0004: Text color contains red.
    FOREGROUND_INTENSITY...0x0008: Text color is intensified.
    BACKGROUND_BLUE........0x0010: Background color contains blue.
    BACKGROUND_GREEN.......0x0020: Background color contains green.
    BACKGROUND_RED.........0x0040: Background color contains red.
    BACKGROUND_INTENSITY...0x0080: Background color is intensified.
**********************************************************************/

    static const int m_bkgBLACK = 0x0000;
    static const int m_bkgWHITE = BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE;
    static const int m_frgWHITE = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;

    static const int m_frgBLUE_bkgWHITE = FOREGROUND_BLUE|m_bkgWHITE;
    static const int m_friBLUE_bkgWHITE = m_frgBLUE_bkgWHITE|FOREGROUND_INTENSITY|m_bkgWHITE;

    static const int m_frgWHITE_bkgBLUE = m_frgWHITE|BACKGROUND_BLUE;
    static const int m_friWHITE_bkgBLUE = m_frgWHITE_bkgBLUE|FOREGROUND_INTENSITY;

    static const int m_frgYELLOW_bkgBLUE = FOREGROUND_RED|FOREGROUND_GREEN|BACKGROUND_BLUE;
    static const int m_friYELLOW_bkgBLUE = m_frgYELLOW_bkgBLUE|FOREGROUND_INTENSITY;

    static const int m_frgRED_bkgBLUE = FOREGROUND_RED|BACKGROUND_BLUE;
    static const int m_friRED_bkgBLUE = m_frgRED_bkgBLUE|FOREGROUND_INTENSITY;

	WORD SetConsoleTextAtribute(WORD i_attr)
	{
		CONSOLE_SCREEN_BUFFER_INFO defInfo;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsole, &defInfo);
		SetConsoleTextAttribute(hConsole, i_attr);
		WORD defAttr = defInfo.wAttributes;
		return defAttr;
	}

	CString Trim(LPCTSTR i_str)
	{
		CString s(i_str);
		s.TrimRight();
		s.TrimLeft();
		return s;
	}

	BOOL IsStringNumeric(LPCTSTR i_string)
	{
		CString s(i_str);
		if(s.IsEmpty())
			return FALSE;
		for(int nIndex = 0; nIndex < s.GetLength(); nIndex++)
		{
			TCHAR ch = s.GetAt(nIndex);
			if(isdigit(ch) == 0)
				return FALSE;
		}
		return TRUE;
	}

	void SplitPath(CString& path, CString& drive, CString& dir, CString& fname, CString& ext)
	{
		// 	Parameters:
		// 	path....Full path
		// 	drive...Optional drive letter, followed by a colon (:)
		// 	dir.....Optional directory path, including trailing slash. Forward slashes (/), backslashes (\), or both may be used.
		// 	fname...Base filename (no extension)
		// 	ext.....Optional filename extension, including leading period (.)

		char	chDrive[_MAX_DRIVE]; 
		char	chDir[_MAX_DIR]; 
		char	chFname[_MAX_FNAME]; 
		char	chExt[_MAX_EXT];

		_splitpath((LPCTSTR)path, &chDrive[0], &chDir[0], &chFname[0], &chExt[0]);

		drive = CString(chDrive);
		dir = CString(chDir);
		fname = CString(chFname);
		ext = CString(chExt);
	}

	long Round(double dvalue)
	{
		double	fracc, integ;
		fracc = modf(dvalue, &integ);
		if(abs(fracc) >= 0.5)
			integ++;

		long result = static_cast<long>(integ);  

		return result;
	}

	double Precision(double dvalue, int ndecimals)
	{
		if(ndecimals > 9)
			ndecimals = 9;

		long	factor = 1;
		for(int i = 1; i <= ndecimals; i++)
			factor = 10 * factor;

		double	fracc, integ, epsilon = 0.000000001;

		fracc = modf(dvalue, &integ);
		fracc += epsilon;
		fracc *= factor;
		fracc = ::floor(fracc);
		fracc /= factor;

		double result = integ + fracc;

		return result;
	}

	void SetBitmapOnButton(UINT i_uButtonID, UINT i_uBitmapID)
	{
		HBITMAP		hbmp = 0;
		if(bmp.LoadBitmap(i_uBitmapID, FALSE, RGB(192, 192, 192)))
			hbmp = HBITMAP(bmp);
		ON_BITMAP_BUTTON(i_uButtonID, hbmp);
	}

	void SetIconOnButton(UINT i_uButtonID, UINT i_uIconID)
	{
		CWinApp*		pApp  = AfxGetApp();
		HINSTANCE		hinst = pApp->m_hInstance;
		// HINSTANCE	hinst = ::GetCurrentModuleHandle();
		HICON		hicon = (HICON)LoadImage(hinst, MAKEINTRESOURCE(i_uIconID), IMAGE_ICON, 16, 16, 0);
		ON_ICON_BUTTON(i_uButtonID, hicon);
	}

	BOOL VerifyDLLVersion(LPCTSTR i_dllName, CTimeStamp& o_dllDate)
	{
		CString dllName = CString(i_dllName);
		dllName.MakeLower();
		AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
		for(CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL; pDLL = pDLL->m_pNextDLL)
		{
			char fileName[1024];
			int aux = GetModuleFileName(pDLL->m_hModule, fileName, 1024);
			if (aux == 0)
				continue;

			CString string;
			CString	fname = CString(fileName);
			fname.MakeLower();
			if(strstr(fname, dllName)  != NULL)
			{
				TRY_APP 
				{
					CDate date;
					CFileStatus status;
					CFile::GetStatus(fname, status);
					CTime time = status.m_mtime;
					o_dllDate.SetTimeStamp(
						time.GetYear(), time.GetMonth(), time.GetDay(),
						time.GetHour(), time.GetMinute(), time.GetSecond());
					return TRUE;
				}
				CATCH_APP (e) {} END_CATCH_APP
			}
		}

		return FALSE;
	}

    void print(WORD i_attr, const char* i_string, ...)
	{
		WORD defAttr = TC::SetConsoleTextAtribute(i_attr);
		setlocale(LC_ALL, "");
		if(HasUTF8Chars(i_string)) {
			string strout(i_string);
			UTF8Codec::Decode(strout);
			i_string = strout.c_str();
		}
		char szBuffer[1024] = "";
		va_list	paramList;
		va_start(paramList, i_string);
		vsprintf(szBuffer, i_string, paramList);
		printf(szBuffer);
		va_end(paramList);
		CCGlobals::SetConsoleTextAtribute(defAttr);
	}

    void print(const char* i_string, ...)
	{
		string strout(i_string);
		setlocale(LC_ALL, "");
		UTF8Codec::Decode(strout);
		i_string = strout.c_str();
		char szBuffer[1024] = "";
		va_list	paramList;
		va_start(paramList, i_string);
		vsprintf(szBuffer, i_string, paramList);
		printf(szBuffer);
		va_end(paramList);
	}

    string sprint(const char* i_string, ...)
	{
		char szBuffer[1024] = "";
		va_list	paramList;
		va_start(paramList, i_string);
		vsprintf(szBuffer, i_string, paramList);
		string out(szBuffer);
		va_end(paramList);
		return out;
	}

    wstring wsprint(const wchar_t* i_string, ...)
	{
		wchar_t szBuffer[1024];
		va_list	paramList;
		va_start(paramList, i_string);
		_vsnwprintf(szBuffer, 1024, i_string, paramList);
		wstring out(szBuffer);
		va_end(paramList);
		return out;
	}

    bool HasUTF8Chars(string i_string)
	{
		for(size_t iIndex = 0; iIndex < i_string.size(); iIndex++)
		{
			if(i_string.at(iIndex) & 128)
				return true;
		}
		return false;
	}

	std::string WStringtoASCII(const std::wstring& i_input)
	{
		// This return single byte string (ASCII or ISO-8859-1) from a wide string.
		// Be careful !!!!!
		std::string s(i_input.begin(), i_input.end());
		return s;
	}

	std::wstring ASCIItoWString(const std::string& i_input)
	{
		// This works if i_input is a single byte string (ASCII or ISO-8859-1).
		// For multibyte string (UTF16) this conversion is not valid.
		std::wstring ws(i_input.begin(), i_input.end());
		return ws;
	}

	std::wstring UTF8toWString(const std::string& i_UTF8string)
	{
		// i_UTF8string is a UTF8 encoded estring
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring ws = converter.from_bytes(i_UTF8string);
		return ws;
	}

	std::string WUTF16toString(const std::wstring& i_wideUTF16string)
	{
		// i_wideUTF16string is a wide UTF16 encoded string
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string s = converter.to_bytes(i_wideUTF16string);
		return s;
	}

	std::string to_utf8(const wchar_t* buffer, int len)
	{
		int nChars = ::WideCharToMultiByte(
			CP_UTF8,
			0,
			buffer,
			len,
			NULL,
			0,
			NULL,
			NULL);
		if (nChars == 0) return "";

		string newbuffer;
		newbuffer.resize(nChars) ;
		::WideCharToMultiByte(
			CP_UTF8,
			0,
			buffer,
			len,
			const_cast< char* >(newbuffer.c_str()),
			nChars,
			NULL,
			NULL);

		return newbuffer;
	}

	std::string to_utf8(const std::wstring& str)
	{
		return to_utf8(str.c_str(), (int)str.size());
	}

}

#include "CCommons_dfdllh2.h"

#endif // CCGLOBALS_H