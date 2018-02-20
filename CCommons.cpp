
#include "stdafx.h"  
#include "CCommons.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif  
              
#include "afxdllx.h"    // standard MFC Extension DLL routines       

static AFX_EXTENSION_MODULE NEAR extensionDLL = { NULL, NULL };
static HINSTANCE hRGEXEUIDLL=NULL;
static HINSTANCE hRGDEFUIDLL=NULL;


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("COMMONS.DLL Initializing!\n");
        
        // Extension DLL one-time initialization
        AfxInitExtensionModule(extensionDLL, hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("COMMONS.DLL Terminating!\n");
    }

    return 1; // ok
}

extern "C" HINSTANCE GetCurrentModuleHandle()
{
  return extensionDLL.hModule;
} 

// Exported DLL initialization is run in context of running application
extern "C" extern void InitCCommonsModuleDLL()
{                                  
  WNDCLASS  wndClass;
                                                                            
  // create a new CDynLinkLibrary for this app
  new CDynLinkLibrary(extensionDLL);

  if(::GetClassInfo(GetCurrentModuleHandle(),"VBControl",&wndClass) == 0 ) 
	{
    ::GetClassInfo(AfxGetInstanceHandle(),"VBControl",&wndClass);
    wndClass.hInstance = GetCurrentModuleHandle();
    ::RegisterClass(&wndClass);
  }

  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views.


	if (AfxGetApp() == NULL)
		return;

	if(!AfxGetApp()->IsKindOf(RUNTIME_CLASS(CAppWithBrowse)))
		return;

  CAppWithBrowse* app = (CAppWithBrowse*) AfxGetApp();

	// TODO: register browse and details calls here
}                      
