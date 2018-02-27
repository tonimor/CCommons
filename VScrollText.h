// VScrollText.h : header file
//

#if !defined(AFX_VSCROLLTEXT_H__2CDF7FD3_E4B3_11D3_9A3B_89E160A8A675__INCLUDED_)
#define AFX_VSCROLLTEXT_H__2CDF7FD3_E4B3_11D3_9A3B_89E160A8A675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

class CVScrollText : public CStatic
{
private:
	enum
	{
		SCROLL		= 1,			// 1 = un píxel, 2 = 2 píxels, etc.
		MILISEC		= 75,			// cada 50 milisegundos
		ID_TIMER	= 101,
	};

	CString			m_sTexto;		// texto a dibujar
	CStringArray	m_sStrings;		// cadenas de texto a mostrar
	RECT			m_rect;			// rectángulo del control
	bool			m_flagTimer;	// indica si el temporizador ha sido establecido
	int				m_nClip;		// cantidad del rectángulo a decrementar

public:
	CVScrollText();
	virtual	~CVScrollText();

	void	AddString(CString& sArray);
	void	SetTransparentColor(BOOL bTrans = TRUE);
	void	SetBackGroundColor(COLORREF	color){ m_BkGrColor = color; }

protected:
	COLORREF	m_BkGrColor;

	//{{AFX_MSG(CCVScrollText)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSCROLLTEXT_H__2CDF7FD3_E4B3_11D3_9A3B_89E160A8A675__INCLUDED_)
