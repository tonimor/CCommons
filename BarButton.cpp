// BarButton.cpp : implementation file
//

#include "stdafx.h"
#include "BarButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	const UINT nBarButtonTimer = 1;
}

IMPLEMENT_DYNCREATE(CBarButton, CBitmapButton)

BEGIN_MESSAGE_MAP(CBarButton, CBitmapButton)
	//{{AFX_MSG_MAP(CBarButton)
	//ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBarButton::CBarButton()
{
}

CBarButton::~CBarButton()
{
}

void CBarButton::SetPosition()
{
	if(!::IsWindow(m_hWnd))
		return;

	CDockBar* pTopDockBar = GetParentDockBar();
	if(!pTopDockBar)
		return;

	CRect TopDockBarRect;
	pTopDockBar->GetClientRect(TopDockBarRect);

	CRect rect;
	GetWindowRect(rect);
	pTopDockBar->ScreenToClient(rect);	

	int nLeft = TopDockBarRect.right-rect.Width()-::GetSystemMetrics(SM_CXEDGE);
	int nTop  = TopDockBarRect.top+2;

	if(nLeft != rect.left || nTop != rect.top)
		SetWindowPos(
			NULL, 
			nLeft,
			nTop,
			0,
			0,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);

	CRect UnionToolbarRect;

	for(int nI=0;nI<pTopDockBar->m_arrBars.GetSize();nI++)
	{
		CWnd* pWnd = static_cast<CWnd*>(pTopDockBar->m_arrBars[nI]);
		if(!pWnd ||
		   !AfxIsValidAddress(pWnd,sizeof(CWnd)) || 
		   !::IsWindow(*pWnd))
					continue;

		CRect ToolBarRect;
		pWnd->GetWindowRect(&ToolBarRect);
		UnionToolbarRect.UnionRect(UnionToolbarRect,ToolBarRect);
	}

	GetWindowRect(rect);

	UnionToolbarRect.IntersectRect(UnionToolbarRect,rect);
	if(UnionToolbarRect.IsRectEmpty())
	{
		if(!IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if(IsWindowVisible())
		{
			//ShowWindow(SW_HIDE);
		}
	}
}

CDockBar* CBarButton::GetParentDockBar()
{
	CWnd* pParentWnd =GetParent();
	if(!pParentWnd || !pParentWnd->IsKindOf(RUNTIME_CLASS(CDockBar)))
		return NULL;

	return static_cast<CDockBar*>(pParentWnd);
}

void CBarButton::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CBitmapButton::OnMouseMove(nFlags, point);

	TRACKMOUSEEVENT TrackMouseEventStructure;
	ZeroMemory(&TrackMouseEventStructure,sizeof(TrackMouseEventStructure));
	TrackMouseEventStructure.cbSize = sizeof(TrackMouseEventStructure);
	TrackMouseEventStructure.hwndTrack = m_hWnd;
	TrackMouseEventStructure.dwFlags = TME_LEAVE;
	TrackMouseEventStructure.dwHoverTime = HOVER_DEFAULT;

	_TrackMouseEvent(&TrackMouseEventStructure);

	if ((nFlags & MK_LBUTTON)!=0 ||
		(nFlags & MK_RBUTTON)!=0) 
	  return;
	
	CString MessageLine;
	if(MessageLine.LoadString(GetDlgCtrlID()))
	{
		int nDelPos = MessageLine.Find(_T('\n'));
		if(nDelPos != -1)
			MessageLine = MessageLine.Left(nDelPos);
	}

	CWnd*  pMainWnd = ::AfxGetMainWnd();
	if(pMainWnd)
		pMainWnd->SendMessage(WM_SETMESSAGESTRING,0,LPARAM(LPCTSTR(MessageLine)));
}

LRESULT CBarButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_MOUSELEAVE)
	{
		CWnd*  pMainWnd = ::AfxGetMainWnd();
		if(pMainWnd)
			pMainWnd->SendMessage(WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	}

	return CBitmapButton::DefWindowProc(message, wParam, lParam);
}

int CBarButton::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
	CString ToolTipText;

	if(::IsWindow(m_hWnd) && ToolTipText.LoadString(GetDlgCtrlID()))
	{
		int nDelPos = ToolTipText.Find(_T('\n'));
		if(nDelPos != -1)
			ToolTipText = ToolTipText.Right(ToolTipText.GetLength()-nDelPos-1);
	}

	if(ToolTipText.IsEmpty())
		return -1;

	int nLen = ToolTipText.GetLength();
	PTCHAR pText = PTCHAR(::malloc((nLen+1)*sizeof(TCHAR)));
	if(!pText)
		return -1;

	::_tcscpy(pText,ToolTipText.LockBuffer());
	ToolTipText.UnlockBuffer();
	pTI->lpszText = pText;
	pTI->hwnd = m_hWnd;

	CRect ClientRect;
	GetClientRect(ClientRect);
	pTI->rect.left = ClientRect.left;
	pTI->rect.top = ClientRect.top;
	pTI->rect.right = ClientRect.right;
	pTI->rect.bottom = ClientRect.bottom;

	return 1;
}

void CBarButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBitmapButton::OnLButtonDown(nFlags, point);
}

void CBarButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
// 	CWnd* pMainWnd = ::AfxGetMainWnd();
// 	if(pMainWnd)
// 		pMainWnd->PostMessage(ON_BARBUTTON_CLICK);

	CWnd*	pFrame = GetParentFrame();
	if(pFrame)
	{
		ClientToScreen(&point); 
		::PostMessage(pFrame->m_hWnd, ON_BARBUTTON_CLICK,0, 0);
	}

	CBitmapButton::OnLButtonUp(nFlags, point);
}

void CBarButton::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == nBarButtonTimer)
		SetPosition();
	
	CBitmapButton::OnTimer(nIDEvent);
}

int CBarButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBitmapButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ASSERT(GetParentDockBar());

	SetTimer(nBarButtonTimer, 10, NULL);

	EnableToolTips();
		
	return 0;
}

void CBarButton::OnDestroy() 
{
	KillTimer(nBarButtonTimer);

	CBitmapButton::OnDestroy();
}
