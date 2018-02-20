#if !defined(AFX_SPLITSELLISTCTRL_H__F932127D_3254_42BC_9970_4E66D168C73E__INCLUDED_)
#define AFX_SPLITSELLISTCTRL_H__F932127D_3254_42BC_9970_4E66D168C73E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SplitSelListCtrl.h : header file
//

/*************************************************************************************
Usage
-----
Derive your own class from CSplitSelListCtrl.
Use it as a member of your own wnd with report style:


In wnd header:
------------
CSplitSelListCtrlDerived m_myListCtrl; // member


In wnd cpp:
----------
int numCols = 6; // Number of columns
int separatorPos = 3; // Position of line separator
CWnd* pParentWnd = this;
m_myListCtrl.Initialize(numCols, separatorPos, IDC_LISTCTRL_ID, pParentWnd);

Or, if your prefer:
------------------
m_myListCtrl.SetNumCols(numCols);
m_myListCtrl.SetSeparatorPosition(separatorPos);
m_myListCtrl.SubclassDlgItem(IDC_LISTCTRL_ID, this);

**************************************************************************************/

class CSplitSelListCtrl : public CListCtrl
{
	typedef struct tagMyNMLVCUSTOMDRAW
	{
		NMCUSTOMDRAW nmcd;
		COLORREF clrText;
		COLORREF clrTextBk;
		int      iSubItem;
	} MyNMLVCUSTOMDRAW, *LPMyNMLVCUSTOMDRAW;

public:

	CSplitSelListCtrl();
	virtual ~CSplitSelListCtrl();

	BOOL Initialize(int i_numCols, int i_separatorPos, UINT i_uIDC, CWnd* i_pParent);

	void SetNumCols(int i_numCols)
	{ 
		m_numCols = i_numCols; 
	}

	void SetSeparatorPosition(int i_position)
	{ 
		m_separatorPos = i_position; 
	}

protected:

	int m_numCols;
	int m_separatorPos;
	int m_itemSel;
	int m_subItemSel;

	void DrawSeparatorLine();
	BOOL GetRowRect(int nRow, int nCol, CRect& rect);
	BOOL GetCellRect(int nRow, int nCol, CRect& rect);

	virtual void CallLButtonDown() = 0;
	virtual void CallRButtonDown() = 0;
	virtual void CallContextMenu(CPoint lvClientPoint) = 0;
	virtual void CallDblClick(NMHDR* pNMHDR, LRESULT* pResult) = 0;
	virtual void CallHeaderItemChanged(int i_Column, NMHDR *pNMHDR, LRESULT *pResult) = 0;

	//{{AFX_VIRTUAL(CSplitSelListCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSplitSelListCtrl)
	afx_msg void OnPaint();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITSELLISTCTRL_H__F932127D_3254_42BC_9970_4E66D168C73E__INCLUDED_)
