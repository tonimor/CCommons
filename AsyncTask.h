#ifndef ASYNCTASK_H
#define ASYNCTASK_H

#include "CCommons_dfdllh1.h"

/************************************************************************
Usage:
------

class CSampleTask : public CAsyncTask<DWORD, int, LPVOID>
{
public:

	CSampleTask(DWORD i_input, int i_result, LPVOID i_progress) :
		CAsyncTask<DWORD, int, LPVOID>(i_input, i_result, i_progress) {
	}

protected:

    virtual void onPreExecute();
	virtual void onStillRunning();
    virtual void onPostExecute(BOOL i_result);
    virtual void onProgressUpdate(LPVOID* i_progress);
    virtual void doInBackground(DWORD i_input, LPVOID i_progress);
};

CSampleTask::CSampleTask(DWORD i_input, int i_result, LPVOID i_progress) :
	CAsyncTask<DWORD, int, LPVOID>(i_input, i_result, i_progress) {
}

virtual void CSampleTask::onPreExecute()
{
}

virtual void CSampleTask::onStillRunning() 
{
}

virtual void CSampleTask::onPostExecute(BOOL i_result) 
{
}

virtual void CSampleTask::onProgressUpdate(LPVOID* i_progress) 
{
}

virtual void doInBackground(DWORD i_input, LPVOID i_progress) 
{
	// do background stuff
	// ...............

	m_resultObj = 123; // result
}

// declaration
DWORD	input  = 0;
int		result = 0;
CSampleTask myTask(input, result, NULL);
myTask.execute();

*************************************************************************/

template <class T1,	class T2, class T3> class CAsyncTask
{
    DWORD   m_threadId;
    HANDLE  m_hThread;
    HANDLE  m_hMutex;

protected:

    enum
    {
		ON_FINALIZE_THREAD  = 32001,
        ON_PUBLISH_PROGRESS = 32002,
    };

    T1 m_inputObj;
	T2 m_resultObj;
    T3 m_progressObj;

	// methods in calling thread
    virtual void onPreExecute() = 0;
	virtual void onStillRunning() = 0;
	virtual void onProgressUpdate(T3* i_progress) = 0;
	virtual void onPostExecute(T2 i_result) = 0;

	// methods in background thread
    virtual void doInBackground(T1 i_input, T3 i_progress) = 0;

public:

    CAsyncTask(T1 i_input, T2 i_result, T3 i_progress) : 
		m_inputObj(i_input), m_resultObj(i_result), m_progressObj(i_progress)
    {
        m_hMutex = CreateMutex(NULL, FALSE, NULL);
        m_threadId = GetCurrentThreadId();
        m_hThread  = NULL;
    }

    virtual ~CAsyncTask()
    {
		if (m_hThread)
			terminateTask();
		if(m_hMutex)
			CloseHandle(m_hMutex);
    }

    inline void join()
    {
        // blocks the calling thread until m_hThread has finished.
        WaitForSingleObject(m_hThread, INFINITE);
    }

	inline BOOL isTaskRunning()
	{
		BOOL result = FALSE;
		if(m_hThread)
		{

			DWORD dwStatus = WaitForSingleObject(m_hThread, 0);
			switch(dwStatus)
			{
				case WAIT_OBJECT_0:
					// WAIT_OBJECT_0 means that the thing you were waiting to happen HAPPENED !!!
					// Therefore, the process m_hThread has finished
					break;
				case WAIT_TIMEOUT:
					// the process m_hThread is still executing
					result = TRUE;
					break;
				case WAIT_FAILED:
					// failure
					break;
			}
		}

		return result;
	}

    void publishProgress(long i_wait = 0)
    {
        const long messageId = ON_PUBLISH_PROGRESS;
        if(::PostThreadMessage(m_threadId, messageId, WPARAM(&m_progressObj), LPARAM(i_wait))) {
            ProcessThreadMessage(messageId);
        }
    }

	inline void terminateTask(BOOL i_hardMode = FALSE)
	{
        if(m_hThread) 
		{
			if(i_hardMode == FALSE)
			{
				// wait until thread has properly ended
				DWORD dwStatus = WaitForSingleObject(m_hThread, INFINITE);
				switch(dwStatus)
				{
					case WAIT_OBJECT_0:
						TRACE( _T("worker thread just finished") ); 
						break;
					case WAIT_TIMEOUT:
						TRACE( _T("timed out -> worker thread is still busy") ); 
						break;
					case WAIT_FAILED:
						TRACE( _T("failure !!!") ); 
						break;
				}
				CloseHandle(m_hThread);
				m_hThread = NULL;
				return;
			}

			DWORD exitCode;
			if (::GetExitCodeThread(m_hThread, &exitCode)) {
				if(exitCode == STILL_ACTIVE){
					// the thread has not terminated
				}
				
			}
			CloseHandle(m_hThread);
			m_hThread = NULL;

			// here finish the thread, hard way, be careful !!!
			ExitThread(exitCode);
		}
	}

    inline BOOL executeTask()
    {
		WaitForSingleObject(m_hMutex, INFINITE/*critical section behaviour*/);

		try
		{
			if(m_hThread /*Thread had been started sometime in the past*/)
			{
				if(isTaskRunning())
				{   // if thread's still running -> return
					onStillRunning();
					return FALSE;
				}
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}

			onPreExecute();
			
			LPTHREAD_START_ROUTINE start_routine = (LPTHREAD_START_ROUTINE)CAsyncTask<T1,T2,T3>::run;
			m_hThread = ::CreateThread(
				NULL /*default security*/, 0 /*default stack size*/,
				reinterpret_cast<LPTHREAD_START_ROUTINE>(start_routine) /*LPTHREAD_START_ROUTINE method*/,
				reinterpret_cast<LPVOID>(this) /*argument to run method*/,
				0 /*creation flags: 0 -> starts inmediatily*/, 
				&m_threadId /*current thread Id*/);
		}
		catch(...)
		{
		}

		ReleaseMutex(m_hMutex);

		return TRUE;
    }

private:

    static void run(LPVOID task_obj)
    {
        CAsyncTask<T1,T2,T3>* task = reinterpret_cast<CAsyncTask<T1,T2,T3>*>(task_obj);
        task->doInBackground(task->m_inputObj, task->m_progressObj);
		task->finalize();
    }

    void finalize()
    {
        const long messageId = ON_FINALIZE_THREAD;
        if(::PostThreadMessage(m_threadId, messageId, WPARAM(&m_resultObj), LPARAM(0))) {
			CloseHandle(m_hThread);
			m_hThread = NULL;
            ProcessThreadMessage(messageId);
        }
    }

    inline boolean ProcessThreadMessage(unsigned int i_messageId)
    {
        MSG msg;
        boolean msg_processed = false;
        while(GetMessage(&msg, NULL, 0, 0))
        {
            if(msg.message == i_messageId) {
                switch(i_messageId)
                {
                    case ON_FINALIZE_THREAD: {
                        T2* param = reinterpret_cast<T2*>(msg.wParam);
                        onPostExecute(*param);
                        msg_processed = true;
                        break;
                    }
                    case ON_PUBLISH_PROGRESS: {
                        T3* param  = reinterpret_cast<T3*>(msg.wParam);
                        long wait = msg.lParam;
                        if(wait > 0)
                            Sleep(wait);
                        onProgressUpdate(param);
                        msg_processed = true;
                        break;
                    }
                } // end switch

                if(msg_processed) {
                    while(true) {
						// removes "i_messageId" from the message queue
                        if(!PeekMessage(&msg, NULL, i_messageId, i_messageId, PM_REMOVE))
                            return true;
                    }
                }

            } // end if

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } // end while

        return false;
    }
};

#include "CCommons_dfdllh2.h"

#endif // ASYNCTASK_H
