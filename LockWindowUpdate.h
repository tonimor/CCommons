// LockWindowUpdate.h: interface for the CLockWindowUpdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCKWINDOWUPDATE_H__D2AAD8C3_7479_4053_BC8B_C8F2FC46DE46__INCLUDED_)
#define AFX_LOCKWINDOWUPDATE_H__D2AAD8C3_7479_4053_BC8B_C8F2FC46DE46__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CLockWindowUpdate  
{
public:
	CLockWindowUpdate(HWND i_hwnd);
	virtual ~CLockWindowUpdate();

	void UnlockUpdate();
	void LockUpdate();

private:
	HWND	m_hwnd;
};

#endif // !defined(AFX_LOCKWINDOWUPDATE_H__D2AAD8C3_7479_4053_BC8B_C8F2FC46DE46__INCLUDED_)
