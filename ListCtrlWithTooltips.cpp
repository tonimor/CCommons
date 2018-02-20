// ListCtrlWithTooltips.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlWithTooltips.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CListCtrlWithTooltips, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlWithTooltips)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CListCtrlWithTooltips::CListCtrlWithTooltips()
{
}

CListCtrlWithTooltips::~CListCtrlWithTooltips()
{
}

BOOL CListCtrlWithTooltips::PreTranslateMessage(MSG* pMsg)
{
	if(m_tooltip.m_hWnd)
		m_tooltip.RelayEvent(pMsg);

	return CListCtrl::PreTranslateMessage(pMsg);
}

BOOL CListCtrlWithTooltips::Initialize(UINT i_uIDC, CWnd* i_pParent)
{
	BOOL result = FALSE;
	if((result = this->SubclassDlgItem(i_uIDC, i_pParent)) == TRUE)
		CreateToolTips();
	return result;
}

void CListCtrlWithTooltips::CreateToolTips()
{
	if(m_tooltip.m_hWnd == NULL)
	{
		if(m_tooltip.Create((CWnd*)this))
		{
			m_tooltip.AddTool(this, m_tooltipText);
			m_tooltip.Activate(TRUE);
			m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 250);
		}
	}
}

void CListCtrlWithTooltips::OnMouseMove(UINT nFlags, CPoint point)
{
	LV_HITTESTINFO	lvTestInf;

	lvTestInf.pt = point;

	// Ask where I am ! ! !
	HitTest(&lvTestInf);

	if(lvTestInf.flags & LVHT_ONITEM)
	{
		m_tooltipText = GetTooltipInfo(lvTestInf);
		if(m_tooltipText.IsEmpty() == FALSE)
			m_tooltip.UpdateTipText(m_tooltipText, this);
	}

	CallMouseMove(nFlags, point);
	CListCtrl::OnMouseMove(nFlags, point);
}

CString CListCtrlWithTooltips::GetTooltipInfo(LV_HITTESTINFO lvTestInf)
{
	CString info;
	int nRow = lvTestInf.iItem;
	if(nRow >= 0)
		GetTooltipString(lvTestInf, nRow, info);
	return info;
}
