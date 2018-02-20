// GradientProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GradientProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MARQUEE_EVENT	 10001
#define	MARQUEE_RANGE	 50
#define	MARQUEE_STEP	 20

BEGIN_MESSAGE_MAP(CGradientProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CGradientProgressCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGradientProgressCtrl::CGradientProgressCtrl()
{
	// Defaults assigned by CProgressCtrl()
	m_nLower = 0;
	m_nUpper = 100;
	m_nCurrentPosition = 0;
	m_nStep = 10;
	m_marquee = 0;
	
	// Initial colors
 	m_clrStart = ::GetSysColor(COLOR_HIGHLIGHT);
 	m_clrEnd = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrBkGround = ::GetSysColor(COLOR_3DFACE);
	m_clrText = COLORREF(RGB(255, 255, 255));

	// Initial percent indicator
	m_bPercentActive = FALSE;

	// Initial style
	m_bDefaultCtrl = FALSE;
}

CGradientProgressCtrl::~CGradientProgressCtrl()
{
}

int CGradientProgressCtrl:: GetPos()
{
	return (CProgressCtrl::OffsetPos(0));
}

void CGradientProgressCtrl:: SetRange(int nLower, int nUpper)
{
	m_nLower = nLower;
	m_nUpper = nUpper;
	m_nCurrentPosition = nLower;
	CProgressCtrl::SetRange(nLower, nUpper);
}

int CGradientProgressCtrl:: SetPos(int nPos)
{
	m_nCurrentPosition = nPos;
	return (CProgressCtrl::SetPos(nPos));
}

int CGradientProgressCtrl:: SetStep(int nStep)
{
	m_nStep = nStep;
	return (CProgressCtrl::SetStep(nStep));
}

int CGradientProgressCtrl:: StepIt()
{
	if(m_marquee > 0)
		CheckMarqueeState();
	m_nCurrentPosition += m_nStep;
	return (CProgressCtrl::StepIt());
}

void CGradientProgressCtrl::CheckMarqueeState()
{
	if(GetPos() == MARQUEE_RANGE-1)
	{
		SetPos(0);
		COLORREF clrBackground, clrForeground;
		switch(m_marquee)
		{
			case 1:
			{
				clrBackground = ::GetSysColor(COLOR_HIGHLIGHT);
				clrForeground = ::GetSysColor(COLOR_3DFACE);
				SetStartColor(clrForeground);
				SetEndColor(clrForeground);
				SetBkColor(clrBackground);
				m_marquee = 2;
				break;
			}
			case 2:
			{
				clrBackground = ::GetSysColor(COLOR_3DFACE);
				clrForeground = ::GetSysColor(COLOR_HIGHLIGHT);
				SetStartColor(clrForeground);
				SetEndColor(clrForeground);
				SetBkColor(clrBackground);
				m_marquee = 1;
				break;
			}
		}
	}
}

void CGradientProgressCtrl::SetMarqueeMode(BOOL i_mode)
{
	m_marquee = (i_mode ? 1 : 0);
	if(m_marquee > 0) {
		SetRange(0, MARQUEE_RANGE);
		SetStep(1);
		SetPos(0);
		SetDefaultCtrl(FALSE); // Default Ctrl not allowed in marquee mode
		SetTimer(MARQUEE_EVENT, MARQUEE_STEP, NULL);
	}
	else
		KillTimer(MARQUEE_EVENT);	
}

void CGradientProgressCtrl::SetDefaultCtrl(BOOL bDefault) 
{ 
	m_bDefaultCtrl = bDefault;
	if(bDefault)
		SetMarqueeMode(FALSE); // Marquee mode not allowed y default ctrl
}

void CGradientProgressCtrl::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == MARQUEE_EVENT) 
		StepIt();
	CProgressCtrl::OnTimer(nIDEvent);
}

/************************************************************************/
// The main drawing routine.  Consists of two parts
// (1) Call the DrawGradient routine to draw the visible part of the progress gradient
// (2) If needed, show the percentage text
/************************************************************************/
void CGradientProgressCtrl::OnPaint() 
{

	if(m_bDefaultCtrl)
	{
		CProgressCtrl::OnPaint();
		return;
	}

	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// If the current position is invalid then we should fade into the  background
	if (m_nCurrentPosition < m_nLower || m_nCurrentPosition > m_nUpper)
	{
		CRect rect;
		GetClientRect(rect);
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect, &brush);
		VERIFY(brush.DeleteObject());
		return;
	}


	// Figure out what part should be visible so we can stop the gradient when needed
	CRect rectClient;
	GetClientRect(&rectClient);
	float maxWidth((float)m_nCurrentPosition/(float)m_nUpper * (float)rectClient.right);


	// Draw the gradient
	DrawGradient(&dc, rectClient, (int)maxWidth);

	// Show percent indicator if needed
	if (m_bPercentActive)
		ShowPercent(&dc, rectClient);

	// Do not call CProgressCtrl::OnPaint() for painting messages
}

void CGradientProgressCtrl::ShowPercent(CPaintDC* dc, const RECT &rectClient)
{
	float fpercent = 100.0f; 
	fpercent *= (float)(m_nCurrentPosition-m_nLower); 
	fpercent /= (float)(m_nUpper-m_nLower);
	CString spercent;
	spercent.Format("%.0f%%", fpercent);
	dc->SetTextColor(m_clrText);
	dc->SetBkMode(TRANSPARENT);
	dc->DrawText(spercent, CRect(rectClient), DT_VCENTER |  DT_CENTER | DT_SINGLELINE);
}

/*************************************************************************/
// Where most of the actual work is done.  The general version would fill the entire rectangle with
// a gradient, but we want to truncate the drawing to reflect the actual progress control position.
/*************************************************************************/
void CGradientProgressCtrl::DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth)
{
	RECT rectFill;			   // Rectangle for filling band
	float fStep;              // How wide is each band?
	CBrush brush;			// Brush to fill in the bar	

	
	CMemDC memDC(pDC);

	// First find out the largest color distance between the start and end colors.  This distance
	// will determine how many steps we use to carve up the client region and the size of each
	// gradient rect.
	int r, g, b;							// First distance, then starting value
	float rStep, gStep, bStep;		// Step size for each color

	BOOL  bSameColor = TRUE;
	if(m_clrStart == m_clrEnd) 
	{
		r = GetRValue(m_clrStart);
		g = GetGValue(m_clrStart);
		b = GetBValue(m_clrStart);
	}
	else
	{
		// Get the color differences
		bSameColor = FALSE;
		r = (GetRValue(m_clrEnd) - GetRValue(m_clrStart));
		g = (GetGValue(m_clrEnd) - GetGValue(m_clrStart));
		b =  (GetBValue(m_clrEnd) - GetBValue(m_clrStart));
	}

	// Make the number of steps equal to the greatest distance
	int nSteps = max(abs(r), max(abs(g), abs(b)));

	// Determine how large each band should be in order to cover the
	// client with nSteps bands (one for every color intensity level)
	fStep = (float)rectClient.right / (float)nSteps;

	// Calculate the step size for each color
	rStep = r/(float)nSteps;
	gStep = g/(float)nSteps;
	bStep = b/(float)nSteps;

	// Reset the colors to the starting position
	r = GetRValue(m_clrStart);
	g = GetGValue(m_clrStart);
	b = GetBValue(m_clrStart);

	// Start filling bands
	for (int iOnBand = 0; iOnBand < nSteps; iOnBand++) 
	{
		::SetRect(&rectFill,
			(int)(iOnBand * fStep),		// Upper left X
			 0,							// Upper left Y
			(int)((iOnBand+1) * fStep),	// Lower right X
			rectClient.bottom+1);		// Lower right Y		

		// Note: CDC::FillSolidRect is faster, but it does not handle 8-bit color depth
		if(bSameColor) 
			VERIFY(brush.CreateSolidBrush(m_clrStart));
		else
			VERIFY(brush.CreateSolidBrush(RGB(r+rStep*iOnBand, g+gStep*iOnBand, b+bStep*iOnBand)));
		
		memDC.FillRect(&rectFill, &brush);
		VERIFY(brush.DeleteObject());

		// If we are past the maximum for the current position we need to get out of the loop.
		// Before we leave, we repaint the remainder of the client area with the background color.
		if (rectFill.right > nMaxWidth)
		{
			::SetRect(&rectFill, rectFill.right, 0, rectClient.right, rectClient.bottom);
			VERIFY(brush.CreateSolidBrush(m_clrBkGround));
			memDC.FillRect(&rectFill, &brush);
			VERIFY(brush.DeleteObject());
			return;
		}
	}
}

/*************************************************************************/
// All drawing is done in the OnPaint function
/*************************************************************************/
BOOL CGradientProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

