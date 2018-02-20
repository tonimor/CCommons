// DlgProgressThread.cpp : implementation file
//

#include "stdafx.h"
#include "CCommonsR.h"
#include "DlgProgressThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" HINSTANCE GetCurrentModuleHandle();

IMPLEMENT_DYNCREATE(CDlgProgressThread, CWinThread)

BEGIN_MESSAGE_MAP(CDlgProgressThread, CWinThread)
	//{{AFX_MSG_MAP(CDlgProgressThread)
	ON_MESSAGE(ON_PROGRESS_STEP, OnProgressStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgProgressThread::CDlgProgressThread()
{
}

CDlgProgressThread::~CDlgProgressThread()
{
}

CDlgProgressThread* CDlgProgressThread::Create(long i_range)
{
	 CDlgProgressThread* progressThread = 
		 (CDlgProgressThread* )AfxBeginThread(
			RUNTIME_CLASS(CDlgProgressThread),
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED);

	CProgressDialog& dlg = progressThread->GetProgressDialog();
	if(i_range == INFINITE)
		dlg.SetMarquee(TRUE);
	else
		dlg.SetRange(i_range);
	CEvent& event = dlg.GetEvent();
	progressThread->ResumeThread();
	::WaitForSingleObject(event, INFINITE);
	return progressThread;
}

BOOL CDlgProgressThread::InitInstance()
{
	HINSTANCE hOldModule = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetCurrentModuleHandle());
	m_pMainWnd=&m_dlgProgress;
	m_dlgProgress.Create(CProgressDialog::IDD);
	m_dlgProgress.InitializeProgressBar();
	m_dlgProgress.UpdateWindow();
	m_dlgProgress.ShowWindow(SW_SHOW);
	AfxSetResourceHandle(hOldModule); 
	return TRUE;
}

int CDlgProgressThread::ExitInstance()
{
	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
	{
		// Dispatches pending messages
		if(!AfxGetApp()->PumpMessage()) 
			break; 
	} 

	Sleep(250);
	CEvent& event = m_dlgProgress.GetEvent();
	event.SetEvent();
	m_dlgProgress.DestroyWindow();
	return CWinThread::ExitInstance();
}

void CDlgProgressThread::OnProgressStep(WPARAM wParam, LPARAM lParam)
{
	long currentCount = lParam;
	m_dlgProgress.SetOneProgressStep(currentCount);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgProgressThread::PostOneProgressStep()
{
	PostOneProgressStep(0);
}

void CDlgProgressThread::PostOneProgressStep(long i_currentCount)
{
	if(!m_dlgProgress.IsMarqueeMode())
		PostMessage(ON_PROGRESS_STEP, WPARAM(0), LPARAM(i_currentCount));
}

void CDlgProgressThread::Terminate()
{
	CEvent& event = m_dlgProgress.GetEvent();
	event.ResetEvent();
	PostMessage(WM_QUIT, WPARAM(0), LPARAM(0));
	::WaitForSingleObject(event, INFINITE);
}

void CDlgProgressThread::PostMessage(UINT i_uMessage, WPARAM i_wParam, LPARAM i_lParam)
{
	Sleep(1);
	while(!::PostThreadMessage(m_nThreadID, i_uMessage, i_wParam, i_lParam))
	{
		Sleep(100); // let me breath !!!!!
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ON_MY_WM_NCHITTEST() \
	{ WM_NCHITTEST, 0, 0, 0, AfxSig_wp, (AFX_PMSG)(AFX_PMSGW)(UINT (AFX_MSG_CALL CWnd::*)(CPoint))&CProgressDialog::OnMyNcHitTest },
#define ON_MY_WM_SETCURSOR() \
	{ WM_SETCURSOR, 0, 0, 0, AfxSig_bWww, (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))&CProgressDialog::OnMySetCursor },

BEGIN_MESSAGE_MAP(CDlgProgressThread::CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CDlgProgressThread::CProgressDialog)
	ON_MY_WM_NCHITTEST()
	ON_MY_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgProgressThread::CProgressDialog::CProgressDialog() : m_event(FALSE, TRUE)
{
	m_range = 0;
	m_marquee = 0;
	m_event.ResetEvent();
}

void CDlgProgressThread::CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	//}}AFX_DATA_MAP
}

BOOL CDlgProgressThread::CProgressDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	m_progress.SubclassDlgItem(IDC_PROGRESS_THREAD_CONTROL, this);
	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	m_cursor = AfxGetApp()->LoadCursor(IDC_MOVE_CURSOR);
	m_event.SetEvent();
	return TRUE;
}

void CDlgProgressThread::CProgressDialog::InitializeProgressBar(long i_range)
{
	if(m_marquee) {
		m_progress.SetMarqueeMode();
		return;
	}

	m_progress.SetRange(0, m_range);
	m_progress.SetStep(1);
	m_progress.SetPos(0);
}

void CDlgProgressThread::CProgressDialog::SetOneProgressStep(long i_currentCount)
{
	m_progress.StepIt();
	UpdateWindow();
}

void CDlgProgressThread::CProgressDialog::PostNcDestroy()
{
	delete this;
}

UINT CDlgProgressThread::CProgressDialog::OnMyNcHitTest(CPoint point)
{
	UINT nHitTest = CDialog::OnNcHitTest(point);
	return (nHitTest == HTCLIENT ? HTCAPTION : nHitTest);
}

BOOL CDlgProgressThread::CProgressDialog::OnMySetCursor (CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCAPTION) 
	{
		::SetCursor(m_cursor);
		return TRUE;
	}

	return CDialog::OnSetCursor (pWnd, nHitTest, message);
}
