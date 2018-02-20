#if !defined(AFX_LISTCTRLWITHTOOLTIPS_H__C70F2A36_782B_4F6F_A0E8_DC4A06FCA73F__INCLUDED_)
#define AFX_LISTCTRLWITHTOOLTIPS_H__C70F2A36_782B_4F6F_A0E8_DC4A06FCA73F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ListCtrlWithTooltips.h : header file
//

/*************************************************************************************
Usage
-----
Derive your own class from CListCtrlWithTooltips.
Implement GetTooltipString and use it as a member of your own wnd and :

void CListCtrlWithTooltipsDerived::GetTooltipString(LV_HITTESTINFO lvTestInf, int i_row, CString& o_strTooltip)
{
	CString strTooltip;

	//
	// init strTooltip depending on i_row
	//

	o_strTooltip = strTooltip
}

In wnd header:
-------------
CListCtrlWithTooltipsDerived m_myListCtrl; // member


In wnd cpp:
----------
CWnd* pParentWnd = this;
m_myListCtrl.Initialize(IDC_LISTCTRL_ID, pParentWnd);

**************************************************************************************/

class CListCtrlWithTooltips : public CListCtrl
{
public:

	CListCtrlWithTooltips();
	virtual ~CListCtrlWithTooltips();

	BOOL Initialize(UINT i_uIDC, CWnd* i_pParent);
	void CreateToolTips();

protected:

	CToolTipCtrl	m_tooltip;
	CString			m_tooltipText;

	CString GetTooltipInfo(LV_HITTESTINFO);

	virtual void GetTooltipString(LV_HITTESTINFO, int i_row, CString& o_strTooltip) = 0;
	virtual void CallMouseMove(UINT nFlags, CPoint point) = 0;

	//{{AFX_VIRTUAL(CListCtrlWithTooltips)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CListCtrlWithTooltips)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLWITHTOOLTIPS_H__C70F2A36_782B_4F6F_A0E8_DC4A06FCA73F__INCLUDED_)
