// VScrollText.cpp : implementation file
//

#include "stdafx.h"
#include "VScrollText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.

typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
	(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

extern lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

BEGIN_MESSAGE_MAP(CVScrollText, CStatic)
	//{{AFX_MSG_MAP(CVScrollText)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CVScrollText::CVScrollText()
{
	m_sTexto = _T("");
	m_flagTimer = false;
	m_nClip = 0;
	m_BkGrColor = ::GetSysColor(COLOR_3DFACE);
}

CVScrollText::~CVScrollText()
{
}

void CVScrollText::AddString(CString& sArray)
{
	if (!m_flagTimer)
	{
		// sólo se ejecutará la primera vez que se añada una cadena de texto
		if (SetTimer(ID_TIMER, MILISEC, NULL))
			m_flagTimer = true;

		GetClientRect(&m_rect);

		// asignamos la primera cadena de texto a mostrar
		m_sTexto = sArray;
	}

	// vamos añadiendo cadenas a mostrar
	m_sStrings.Add(sArray);
}

void CVScrollText::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER)
	{
		// vamos aumentando el rectángulo a dibujar
		m_nClip += SCROLL;
		Invalidate();
	}
	
	CStatic::OnTimer(nIDEvent);
}

void CVScrollText::OnDestroy() 
{
	CStatic::OnDestroy();
	
	if (m_flagTimer)
		KillTimer(ID_TIMER);
}

void CVScrollText::OnPaint() 
{
	RECT		rect;
	CWnd		*pWndParent = NULL;
	CPaintDC	dc(this);
	CDC		memDC;
	CBitmap	bitmap, *oldBitmap;
	SIZE		size;
	int		nCadenas;

	// seleccionamos el color de fondo por defecto del control estático
	CBrush bBrush(m_BkGrColor);
	CBrush* pOldBrush;

	// obtenemos el padre del control, generalmente un diálogo
	pWndParent = GetParent();
	if (pWndParent == NULL)
		return;

	// creamos un contexto de dispositivo en memoria compatible
	// con el del control, en el que se dibujará el texto
	if (!memDC.CreateCompatibleDC(&dc)) 
	{
		TRACE0("Error al crear DC en memoria"); 
		return;
	}

	// inicializamos el bitmap que tendrá la imagen final a copiar en el control
	if (!bitmap.CreateCompatibleBitmap(&dc, m_rect.right, m_rect.bottom)) 
	{
		TRACE0("Error al inicializar bitmap en memoria"); 
		return;
	}

	// seleccionamos la fuente por defecto de la ventana padre
	memDC.SelectObject(pWndParent->GetFont());

	// seleccionadmos el bitmap donde vamos a pintar el texto
	oldBitmap = memDC.SelectObject(&bitmap);

	// seleccionamos el pincel para borrar el contenido del rectángulo
	pOldBrush = memDC.SelectObject(&bBrush);

	// rellenamos el rectángulo con el color seleccionado
	memDC.FillRect(&m_rect, &bBrush);

	// seleccionamos el rectángulo donde DrawText() comenzará a pintar el texto,
	// esto es lo que genera el efecto de desplazamiento del texto, es decir,
	// indicamos a DrawText() que pinte el texto un pixel hacia arriba cada vez
	rect = m_rect;
	rect.top = rect.bottom - m_nClip;

	// obtenemos el tamaño de la cadena para saber dónde debe comenzar la siguiente
	GetTextExtentPoint32(memDC.GetSafeHdc(), m_sTexto, m_sTexto.GetLength(), &size);

	// pintamos el texto en memoria
	memDC.SetBkMode(TRANSPARENT);

	// pintamos cada una de las cadenas
	nCadenas = m_sStrings.GetSize();
	for (int i= 0; i < nCadenas; i++)
	{
		// dibujamos cada cadena
		m_sTexto = m_sStrings.GetAt(i);
		memDC.DrawText(m_sTexto, &rect, DT_CENTER | DT_NOPREFIX | DT_SINGLELINE);

		// indicamos dónde debe dibujarse la siguiente cadena
		rect.top += size.cy;
	}

	// copiamos el contenido del DC en memoria al DC del control
	dc.BitBlt(0, 0, m_rect.right, m_rect.bottom, &memDC, 0, 0, SRCCOPY);

	// tenemos que calcular que m_nClip sea mayor o igual que el tamaño del
	// rectángulo del control (altura) más el tamaño del rectángulo del total
	// de las cadenas, para que acaben de pasar del todo
	if (m_nClip >= m_rect.bottom + size.cy * nCadenas)
		m_nClip = 0;

	// dejamos las cosas como estaban y eliminamos los objetos
	memDC.SelectObject(oldBitmap);
	memDC.SelectObject(&pOldBrush);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	bBrush.DeleteObject();
}

void CVScrollText::SetTransparentColor(BOOL bTrans)
{
	if (g_pSetLayeredWindowAttributes == NULL)
			return;

	if (bTrans)
	{
		//  set layered style for the dialog
		SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//  call it with 0 alpha for the given color
		g_pSetLayeredWindowAttributes(m_hWnd, m_BkGrColor, 0, LWA_COLORKEY);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

		// Ask the window and its children to repaint
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
}
