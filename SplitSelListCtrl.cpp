// SplitSelListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SplitSelListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_BLACK		RGB(0,0,0);
#define COLOR_WHITE		RGB(255,255,255)
#define COLOR_LIGHTGREY	RGB(245,245,245)
#define COLOR_MUSTARD	RGB(200,200,125)
#define COLOR_CREME		RGB(255,255,225)
#define NULL_SEL		-1

BEGIN_MESSAGE_MAP(CSplitSelListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSplitSelListCtrl)
	ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_NOTIFY(HDN_ITEMCHANGEDW, 0, OnHeaderItemChanged)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSplitSelListCtrl::CSplitSelListCtrl()
{
	m_separatorPos = 0;
	m_itemSel = m_subItemSel = NULL_SEL;
}

CSplitSelListCtrl::~CSplitSelListCtrl()
{
}

BOOL CSplitSelListCtrl::Initialize(int i_numCols, int i_separatorPos, UINT i_uIDC, CWnd* i_pParent)
{
	m_numCols = i_numCols;
	m_separatorPos = i_separatorPos;
	return this->SubclassDlgItem(i_uIDC, i_pParent);
}

void CSplitSelListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);
    LVHITTESTINFO myinfo;
    memset(&myinfo, 0, sizeof(myinfo));
    myinfo.pt = point;
	m_subItemSel = NULL_SEL;
    m_itemSel = ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)&myinfo);
	if(m_itemSel >= 0) {
		m_subItemSel = myinfo.iSubItem;
	}
	Invalidate();
	CallLButtonDown();
}

void CSplitSelListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
 	UINT	lvFlags;
	CPoint	lvPoint = point;
 	int		nIndex = HitTest(lvPoint, &lvFlags);
 	if(nIndex != NULL_SEL) 
	{
		if(lvFlags & LVHT_ONITEM)
			SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	CallRButtonDown();
}

void CSplitSelListCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
 	UINT	lvFlags;
	CPoint	lvClientPoint = point;
	ScreenToClient(&lvClientPoint);
 	int	nIndex = HitTest(lvClientPoint, &lvFlags);
	ClientToScreen(&lvClientPoint);
 	if(nIndex != NULL_SEL)
	{
		if(lvFlags & TVHT_ONITEM)
			CallContextMenu(lvClientPoint);
 	}
}

void CSplitSelListCtrl::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_itemSel =	pNMListView->iItem;
	m_subItemSel = pNMListView->iSubItem;
	CallDblClick(pNMHDR, pResult);
}

void CSplitSelListCtrl::OnHeaderItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	DefWindowProc(WM_NOTIFY, 0, LPARAM(pNMHDR));
	Invalidate();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iColumn = pNMListView->iItem;
	CallHeaderItemChanged(iColumn, pNMHDR, pResult);
}

void CSplitSelListCtrl::OnPaint() 
{
	// First let the control do its default drawing.
	const MSG *msg = GetCurrentMessage();
	DefWindowProc( msg->message, msg->wParam, msg->lParam );
	DrawSeparatorLine();
}

void CSplitSelListCtrl::DrawSeparatorLine()
{
	// Draw the line only for LVS_REPORT mode
	if( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		// Get the number of columns
		CClientDC dc(this );
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		// The bottom of the header corresponds to the top of the line 
		RECT rect;
		pHeader->GetClientRect(&rect);
		int top = rect.bottom+2;

		// Now get the client rect so we know the line length and
		// when to stop
		GetClientRect(&rect);

		// The border of the column is offset by the horz scroll
		int borderx = 0 - GetScrollPos( SB_HORZ );
		for( int i = 0; i < nColumnCount; i++ )
		{
			int color = (i == m_separatorPos ? COLOR_LIGHTGREY : COLOR_WHITE);

			// Get the next border
			borderx += GetColumnWidth(i);

			// if next border is outside client area, break out
			if( borderx >= rect.right ) 
				break;

			if(i == 3) {
				CPen myPen(PS_SOLID, 2, color);
				CPen* oldPen = dc.SelectObject(&myPen);

				// Draw the line.
				dc.MoveTo(borderx, top);
				dc.LineTo(borderx, rect.bottom);

				dc.SelectObject(oldPen);
			}
		}
	}
}

void CSplitSelListCtrl::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPMyNMLVCUSTOMDRAW lpLVCustomDraw = reinterpret_cast<LPMyNMLVCUSTOMDRAW>(pNMHDR);

	switch(lpLVCustomDraw->nmcd.dwDrawStage)
	{
		// Before painting a cell
		case CDDS_ITEMPREPAINT:
		case CDDS_ITEMPREPAINT | 0x00020000: // CDDS_SUBITEM = 0x00020000
		{
			int nRow = lpLVCustomDraw->nmcd.dwItemSpec;
			int nCol = lpLVCustomDraw->iSubItem;
			lpLVCustomDraw->clrTextBk = CLR_DEFAULT;
			lpLVCustomDraw->clrText = COLOR_MUSTARD;
		}
		break;

		// After painting the entire row
		case CDDS_ITEMPOSTPAINT:
		{
			int nRow = lpLVCustomDraw->nmcd.dwItemSpec;
			int nCol = lpLVCustomDraw->iSubItem;

			SetItemState(nRow, LVIS_SELECTED, LVIS_FOCUSED);

            CRect rcHighlight;
            CDC* pDC = CDC::FromHandle(lpLVCustomDraw->nmcd.hdc);
			if(GetRowRect(nRow, m_subItemSel, rcHighlight)) {
				if(m_itemSel == nRow) {

					// Keep subitems Text
					CStringArray strArray;
					for(int iCol = 0; iCol <= 3; iCol++)
					{
						CString str = GetItemText(nRow, m_subItemSel <= 3 ? iCol : iCol+4);
						strArray.Add(str);
					}

					// Bacground
					lpLVCustomDraw->clrTextBk = CLR_DEFAULT;
					int kolor = COLOR_CREME;
					CBrush brush(kolor);
					pDC->FillRect(&rcHighlight, &brush);

					// Frame
					CRect frect = rcHighlight;
					frect.DeflateRect(1,1);
					kolor = COLOR_BLACK;
					CBrush brush2(kolor);
					pDC->FrameRect(&frect, &brush2);

					// Restore subitems text
					for(iCol = 0; iCol <= 3; iCol++)
					{
						CRect rcSubItem;
						GetCellRect(nRow, m_subItemSel <= 3 ? iCol : iCol+4, rcSubItem);
						rcSubItem.DeflateRect(5,2);
						CString str = strArray[iCol];
						kolor = COLOR_BLACK;
						pDC->SetTextColor(kolor);
						pDC->DrawText(str, &rcSubItem, DT_LEFT|DT_END_ELLIPSIS);
					}
				}
			}
		}
		break;

		default: 
			break;    
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;
	*pResult |= CDRF_NOTIFYITEMDRAW;
	*pResult |= 0x00000020;
}

BOOL CSplitSelListCtrl::GetRowRect(int nRow, int nCol, CRect& rect)
{
	CRect rcCell, rcRow;

	if(0 <= m_subItemSel && m_subItemSel <= 3)
	{
		// panel izquierdo -> derivados
		for(int iCol = 0; iCol <= 3; iCol++)
		{
			if(!GetCellRect(nRow, iCol, rcCell))
				return FALSE;
			if(iCol == 0)
				rcRow = rcCell;
			else
				rcRow.right = rcCell.right;
		}
	}
	else if(4 <= m_subItemSel && m_subItemSel <= 7)
	{
		// panel derecho -> procedente
		for(int iCol = 4; iCol <= 7; iCol++)
		{
			if(!GetCellRect(nRow, iCol, rcCell))
				return FALSE;
			if(iCol == 4)
				rcRow = rcCell;
			else
				rcRow.right = rcCell.right;
		}
	}

	rect = rcRow;
	return TRUE;
}

BOOL CSplitSelListCtrl::GetCellRect(int nRow, int nCol, CRect& rect)
{
    CRect rowRect;
    if (GetItemRect(nRow, rowRect, LVIR_BOUNDS) == FALSE)
        return FALSE;

	if(m_subItemSel == NULL_SEL)
		return FALSE;

	CRect colRect;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	pHeader->GetClientRect(&colRect);

	CRect cellrect;
	int   left = 0;
	for( int i = 0; i < m_numCols; i++ ) {
		if(i == nCol) {
			cellrect.left	= (i == 0 ? 16+2/*icon*/ : left);
			cellrect.right	= left+GetColumnWidth(i);
			cellrect.top	= rowRect.top;
			cellrect.bottom = rowRect.bottom;
			break;
		}
 		left += GetColumnWidth(i);
	}

	rect = cellrect;

	return TRUE;
}
