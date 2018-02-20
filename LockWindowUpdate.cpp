// LockWindowUpdate.cpp: implementation of the CLockWindowUpdate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LockWindowUpdate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLockWindowUpdate::CLockWindowUpdate(HWND i_hwnd)
{
	m_hwnd = i_hwnd;
	LockUpdate();
}

CLockWindowUpdate::~CLockWindowUpdate()
{
	UnlockUpdate();
}

void CLockWindowUpdate::LockUpdate()
{
	LockWindowUpdate(m_hwnd);
}

void CLockWindowUpdate::UnlockUpdate()
{
	LockWindowUpdate(NULL);
}
