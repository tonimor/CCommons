// WaitMessageDlg.cpp

#include "stdafx.h"
#include "CCommonsR.h"
#include "WaitMessageDlg.h"
#include "CCGlobals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern "C" HINSTANCE GetCurrentModuleHandle();

BEGIN_MESSAGE_MAP(WaitMessageDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

WaitMessageDlg::WaitMessageDlg(CWnd* pParent /*=NULL*/) : CDialog()
{
	HINSTANCE hOldModule = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetCurrentModuleHandle());
	LOAD_LOCAL_STRING(m_message, IDS_WAIT);
	Create(WaitMessageDlg::IDD, pParent);
	AfxSetResourceHandle(hOldModule); 
}

WaitMessageDlg::~WaitMessageDlg()
{
}

void WaitMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_WAITMESSAGE_TEXT, m_message);
}

BOOL WaitMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	UpdateData(FALSE);

	MSG	msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}

void WaitMessageDlg::OnClose()
{
}
