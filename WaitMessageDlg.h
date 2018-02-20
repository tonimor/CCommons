// WaitMessageDlg.h

#ifndef _WAITMESSAGEDLG_H_
#define _WAITMESSAGEDLG_H_

#include "ccommons_dfdllh1.h"

class WaitMessageDlg : public CDialog
{
public:
	enum { IDD = IDD_WAITMESSAGE_DLG };

	WaitMessageDlg(CWnd* = NULL);
	virtual ~WaitMessageDlg();
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnClose();

private:

	CString	m_message;

	DECLARE_MESSAGE_MAP()
};

#include "ccommons_dfdllh2.h"

#endif // _WAITMESSAGEDLG_H_
