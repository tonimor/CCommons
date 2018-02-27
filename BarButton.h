#if !defined(AFX_BARBUTTON_H__8D31EEF5_6478_11D4_890F_0000E8E0DC35__INCLUDED_)
#define AFX_BARBUTTON_H__8D31EEF5_6478_11D4_890F_0000E8E0DC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarButton.h : header file
//

const UINT ON_BARBUTTON_CLICK = RegisterWindowMessage(_T("OnBarButtonClick"));

/*********************************************************************************

Usage
-----

// define a member in your frame window
CBarButton m_BarButton;

// message-map
BEGIN_MESSAGE_MAP(CMyFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMyFrame)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(ON_BARBUTTON_CLICK, OnBarButtonClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// create bar-button in yout OnCreate frame
int CMyFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMyFrameParent::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hOldModule = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetCurrentModuleHandle());

	// BarButton Support
	CControlBar* pTopDockBar = GetControlBar(AFX_IDW_DOCKBAR_TOP);
	if(pTopDockBar && m_BarButton.Create(NULL,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		CRect(0,0,0,0),
		pTopDockBar,
		IDC_BAR_BUTTON))
	{	
		m_BarButton.LoadBitmaps(IDB_MY_BARBUTTON_BITMAP, IDB_MY_BARBUTTON_BITMAP_SELECTED);
		m_BarButton.SizeToContent();
	}

	AfxSetResourceHandle(hOldModule); 

	return 0;
}

LRESULT CMyFrame::OnBarButtonClick(WPARAM wParam, LPARAM lParam)
{
	CRect	rect;
	m_BarButton.GetWindowRect(&rect);
	CBDVMovimientoDGTBrowse*	pMovBrowse = NULL;
	CBDVLoadBrowse*				pLoadBrowse = NULL;

	// implement your own button-click code here !!!!

	return 0;
}

*********************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CBarButton window

class CBarButton : public CBitmapButton
{
	DECLARE_DYNCREATE(CBarButton)

public:
	CBarButton();
	virtual ~CBarButton();

	//{{AFX_VIRTUAL(CBarButton)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	
protected:

	virtual CDockBar* GetParentDockBar();
	virtual void SetPosition();

	//{{AFX_MSG(CBarButton)
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARBUTTON_H__8D31EEF5_6478_11D4_890F_0000E8E0DC35__INCLUDED_)
