#if !defined(AFX_ENHPROGRESSCTRL_H__12909D73_C393_11D1_9FAE_8192554015AD__INCLUDED_)
#define AFX_ENHPROGRESSCTRL_H__12909D73_C393_11D1_9FAE_8192554015AD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GradientProgressCtrl.h : header file

#include "MemDC.h"

class CGradientProgressCtrl : public CProgressCtrl
{
public:

	CGradientProgressCtrl();
	virtual ~CGradientProgressCtrl();

	void SetRange(int nLower, int nUpper);
	int	 SetPos(int nPos);
	int  SetStep(int nStep);
	int  StepIt(void);
	int  GetPos();

	void SetTextColor(COLORREF color)	{m_clrText = color;}
	void SetBkColor(COLORREF color)		 {m_clrBkGround = color;}
	void SetStartColor(COLORREF color)	{m_clrStart = color;}
	void SetEndColor(COLORREF color)	{m_clrEnd = color;}

	// Show the percent caption
	void ActivatePercent(BOOL bActivate = TRUE)	{ m_bPercentActive = bActivate; }

	// Classic CProgressCtrl
	void SetDefaultCtrl(BOOL bDefault = TRUE);

	// Marquee mode
	void SetMarqueeMode(BOOL i_mode = TRUE);

	BOOL IsMarqueeMode(){ return m_marquee > 0; }

	COLORREF GetTextColor(void)	{return m_clrText;}
	COLORREF GetBkColor(void)		 {return m_clrBkGround;}
	COLORREF GetStartColor(void)	{return m_clrStart;}
	COLORREF GetEndColor(void)	{return m_clrEnd;}

protected:

	int			m_nLower, m_nUpper, m_nStep, m_nCurrentPosition;
	COLORREF	m_clrStart, m_clrEnd, m_clrBkGround, m_clrText;
	BOOL		m_bPercentActive;
	BOOL		m_bDefaultCtrl;
	short		m_marquee;

	void DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth);	
	void ShowPercent(CPaintDC *pDC, const RECT &rectClient);
	void CheckMarqueeState();

	//{{AFX_MSG(CGradientProgressCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENHPROGRESSCTRL_H__12909D73_C393_11D1_9FAE_8192554015AD__INCLUDED_)
