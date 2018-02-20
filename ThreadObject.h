// ***********************************************************************
// ThreadObject.h
//
// ***********************************************************************

#ifndef __ThreadObject_H__
#define __ThreadObject_H__

#include <afxmt.h>

#include "CCommons_dfdllh1.h"

class ThreadObject: public CWinThread
{
protected:

	virtual int	Run() = 0;

	virtual BOOL InitInstance();
	virtual int	 ExitInstance();
	virtual BOOL Start();

public:

	CEvent	m_Event;

	ThreadObject();
	virtual ~ThreadObject();

	virtual void EndThread( DWORD = INFINITE );
	virtual BOOL TerminateThread();
};

#include "CCommons_dfdllh2.h"

#endif


