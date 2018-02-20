#ifndef SQLTASK_H
#define SQLTASK_H

#include "CCommons_dfdllh1.h"

class SQLTask : public CAsyncTask<LPCTSTR, LPVOID, LPVOID>
{
	CDbQuery*	m_pDBQuery;
	CString		m_statement;

public:

	SQLTask(CDbQuery* i_dbQuery, LPCTSTR i_statement, LPVOID o_result) :
		CAsyncTask<LPCTSTR, LPVOID, LPVOID>(i_statement, o_result, NULL)
	{
		m_pDBQuery = i_dbQuery;
		m_statement = CString(i_statement);
	}

	SQLTask(LPCTSTR i_statement, LPVOID o_result) :
		CAsyncTask<LPCTSTR, LPVOID, LPVOID>(i_statement, o_result, NULL)
	{
		m_statement = CString(i_statement);
	}

	void ExecuteStatement()
	{
		TRY_APP
		{
			::DBExecuteStatement(m_statement);
		}
		CATCH_APP(e){}END_CATCH_APP
	}

protected:

    virtual void onPreExecute()
	{
	}

	virtual void onStillRunning()
	{
	}

    virtual void onPostExecute(BOOL i_result)
	{
	}

    virtual void onProgressUpdate(LPVOID* i_progress)
	{
	}

    virtual void doInBackground(DWORD i_input, LPVOID i_progress)
	{
	}

}

#include "CCommons_dfdllh2.h"

#endif // SQLTASK_H

