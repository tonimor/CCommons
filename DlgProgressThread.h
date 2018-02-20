#if !defined(AFX_DLG_PROGRESS_THREAD_H__6D3079E7_3535_40FE_83F9_0B08C8039450__INCLUDED_)
#define AFX_DLG_PROGRESS_THREAD_H__6D3079E7_3535_40FE_83F9_0B08C8039450__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CDlgProgressThread.h : header file
//

/**********************************************************************************
class to show progress control in a separate thread

Usage in normal mode:
---------------------
CAlertsProgressThread* progressThread = CAlertsProgressThread::Create(total_progress);
while(some_condition)
{
	do some stuff....
	
	progressThread->->PostOneProgressStep();
}
progressThread->Terminate();


Usage in Marquee mode:
----------------------
CAlertsProgressThread* progressThread = CAlertsProgressThread::Create(INFINITE);
while(some_condition)
{
	do some stuff....
}
progressThread->Terminate();

************************************************************************************/

#include "GradientProgressCtrl.h"

#include "CCommons_dfdllh1.h"

class CDlgProgressThread : public CWinThread
{
	DECLARE_DYNCREATE(CDlgProgressThread)

	enum
	{ 
		ON_PROGRESS_STEP = WM_APP + 11, 
	};

	class CProgressDialog : public CDialog
	{
	public:

		CProgressDialog();

		void InitializeProgressBar(long = 0);
		void SetOneProgressStep(long);

		void SetRange(long i_range){ m_range =i_range; }
		void SetMarquee(BOOL i_param ){ m_marquee = i_param; }
		BOOL IsMarqueeMode(){ return m_progress.IsMarqueeMode(); }
		CEvent& GetEvent(){ return m_event; }

		//{{AFX_DATA(CProgressDialog)
		enum { IDD = IDD_PROGRESS_TRHEAD_DIALOG };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(CProgressDialog)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual void PostNcDestroy();
		//}}AFX_VIRTUAL

	protected:

		CGradientProgressCtrl	m_progress;	
		CEvent					m_event;
		HCURSOR					m_cursor;
		BOOL					m_marquee;
		long					m_range;

		afx_msg UINT OnMyNcHitTest(CPoint point);
		afx_msg BOOL OnMySetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

		DECLARE_MESSAGE_MAP()
	};

public:

	CDlgProgressThread();
	virtual ~CDlgProgressThread();

	static CDlgProgressThread* Create(long);
	void PostOneProgressStep(long);
	void PostOneProgressStep();
	void Terminate();

	CProgressDialog& GetProgressDialog(){ return m_dlgProgress; }

protected:

	CProgressDialog	m_dlgProgress;

	void PostMessage(UINT i_uMessage, WPARAM i_wParam, LPARAM i_lParam);

	//{{AFX_VIRTUAL(CDlgProgressThread)
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDBStatementObject)
	afx_msg void OnProgressStep(WPARAM, LPARAM);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#include "CCommons_dfdllh2.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_PROGRESS_THREAD_H__6D3079E7_3535_40FE_83F9_0B08C8039450__INCLUDED_)
