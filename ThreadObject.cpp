// ***********************************************************************
// ThreadObject.cpp
//
// ***********************************************************************

#include "stdafx.h"
#include "ThreadObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************************************************
The folowing code lines shows how to detect WM_QUIT messages without blocking thread !!!

	MSG		msgCur;
	if(( ::PeekMessage( &msgCur, HWND( -1 ), NULL, NULL, PM_REMOVE )) &&
		( msgCur.message == WM_QUIT ))
		Thread should QUIT on ASAP basis !!!!!!!!
***************************************************************************************/


//////////////////////////////////////////////////////////////////////////
// WARNING !!!!!!!!
//
// CWinThread does not get into message loop, remenber to call ExitInstance
// before exiting Run function !!!!!!


// -----------------------------------------------------------------------

ThreadObject::ThreadObject(): m_Event( FALSE, TRUE )
{
	m_bAutoDelete = FALSE;
	m_pMainWnd = NULL;
	m_Event.ResetEvent();
}

// -----------------------------------------------------------------------

ThreadObject::~ThreadObject()
{
}

// -----------------------------------------------------------------------

BOOL ThreadObject::InitInstance()
{
#ifdef _WIN32_DCOM
	if( ::CoInitializeEx( NULL, COINIT_MULTITHREADED ) != S_OK )
#else
	if( ::CoInitialize( NULL ) != S_OK )
#endif
	{
		TRACE( "Could not initialize COM subsystem on thread %d !!!\n", m_nThreadID );
		return FALSE;
	}

	return TRUE;
}

// -----------------------------------------------------------------------

int ThreadObject::ExitInstance()
{
	::CoUninitialize();
	return CWinThread::ExitInstance();
}

// ------------------------------------------------------------------------

void ThreadObject::EndThread( DWORD dwTimeout )
{
// Pre:
//	- Solo se puede llamar a esta funcion cuando el Thread existe, de lo contrario el resultado es impredecible.
//	- Si el calling y el called thread son threads que han llamado a alguna función del subsistema OLE
//    existe un gran peligro de deadlock si el Timeout es INFINITE.

	ASSERT( m_hThread );

	if( m_hThread )
	{
		DWORD dwCurrent = GetCurrentThreadId();

		//Espera a que el Thread que termina indique su finalizacion.
		//SetThreadPriority( THREAD_PRIORITY_HIGHEST );

		m_Event.SetEvent(/*mark event as signaled*/);
		::PostThreadMessage( m_nThreadID, WM_QUIT, 0, 0 );

		// take care, if calling thread is an OLE thread it hangs whole system !!!!!
		TRACE( "THREAD: stopping thread(%08x) from thread(%08x) with timeout(%08x)!!\n !!!!", m_nThreadID, dwCurrent );
		::WaitForSingleObject( m_hThread, dwTimeout );
	}
}

// -----------------------------------------------------------------------

BOOL ThreadObject::TerminateThread()
{
	if( m_hThread != NULL )
		return ::TerminateThread( m_hThread, DWORD( -1 ));

	return TRUE;
}

// -----------------------------------------------------------------------

BOOL ThreadObject::Start()
{
	// Lanza el Thread.
	return CreateThread( CREATE_SUSPENDED );
}
