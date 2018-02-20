// NEW 32-bits header. See 'Export and Import Using AFX_EXT_CLASS' topic
// for explanation about this.

// Replace TEMPLATE for your proyect name and copy this
// file to your proyect directory.

#ifdef CCOMMONS_IMPL
        #define CCOMMONS_DATA __declspec(dllexport)
#else
        #define CCOMMONS_DATA __declspec(dllimport)
#endif

#undef AFX_DATA
#define AFX_DATA CCOMMONS_DATA
