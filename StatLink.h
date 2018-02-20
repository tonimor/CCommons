#ifndef _STATLINK_H
#define _STATLINK_H

/*************************************************************************************
Usage
-----
Add a member of your own wnd:

In wnd header:
-------------
CStaticLink m_StaticLink; // member


In wnd cpp:
----------
m_StaticLink.m_color = CStaticLink::g_colorUnvisited;
CWnd* pParentWnd = this;
m_StaticLink.SubclassDlgItem(IDC_STATIC_URL, pParentWnd);
CString	strURL = "http:\\www.google.com";
m_StaticLink.SetLink(strURL);

**************************************************************************************/

class CStaticLink : public CStatic 
{
	class CHyperlink : public CString 
	{
	public:
		CHyperlink(LPCTSTR lpLink = NULL) : CString(lpLink) {}
		~CHyperlink() {}

		const CHyperlink& operator=(LPCTSTR lpsz) 
		{
			CString::operator=(lpsz);
			return *this;
		}

		operator LPCTSTR() 
		{
			return CString::operator LPCTSTR(); 
		}

		HINSTANCE Navigate() 
		{ 
			CWaitCursor wait;
			return  IsEmpty() ? NULL :
				ShellExecute(0, _T("open"), *this, 0, 0, SW_SHOWNORMAL);
		}
	};

public:
	DECLARE_DYNAMIC(CStaticLink)

	enum
	{
		ON_STATICLINK_LBUTTONDOWN = WM_USER + 1020,
		ON_STATICLINK_OPENFAILED  = WM_USER + 1021,
	};
	BOOL m_navigate;

	CStaticLink(LPCTSTR lpText = NULL, BOOL bDeleteOnDestroy = FALSE);
	~CStaticLink() {}

	BOOL SubclassDlgItem(UINT nID, CWnd* pParent, LPCTSTR lpszLink = NULL) 
	{
		if (lpszLink)
			m_link = lpszLink;
		return CStatic::SubclassDlgItem(nID, pParent);
	}

	void SetLink(LPCTSTR lpszLink)
	{
		m_link = lpszLink;
	}

	void SetLinkText(LPCTSTR lpszText, LPCTSTR lpszLink)
	{
		m_link = lpszLink;
		SetWindowText(lpszText);
	}

	CHyperlink	m_link;
	COLORREF	m_color;
	CFont		m_font;

	static COLORREF		g_colorUnvisited;
	static COLORREF		g_colorVisited;
	static HCURSOR		g_hCursorLink;

protected:
	BOOL			m_bDeleteOnDestroy;

	virtual void PostNcDestroy();

	//{{AFX_MSG(CStaticLink)
	afx_msg UINT		OnNcHitTest(CPoint point);
	afx_msg HBRUSH	CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL		OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

#endif _STATLINK_H
