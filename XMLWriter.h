#pragma once

#include <afxtempl.h>

#define XMLEXCEPTION_ADDTAG         _T("can't add tag to list")
#define XMLEXCEPTION_ADDATTRIBUTE   _T("can't add attribute to list")
#define XMLEXCEPTION_CREATEFILE     _T("can't create file")

#define XMLTAGREADY     0
#define XMLTAGOPEN      1

class CXMLException
{
    public:
        CXMLException(LPCTSTR szErrText);
        ~CXMLException(void);

        CString m_strErrorText;
};

//----------------------------------------------------------------

class CXMLAttribute : public CObject
{
    public:
        CXMLAttribute(void);
        ~CXMLAttribute(void);

    CString m_strAttributeName;
    CString m_strAttributeValue;

};
typedef CXMLAttribute * HXMLATTRIBUTE;

//----------------------------------------------------------------

class CXMLTag : public CObject
{
    public:
        CXMLTag(void);
        ~CXMLTag(void);
        int Format(CString &string);
        void SetTagValue (LPCTSTR szValue);
    
        CString     m_strTagName;
        int         m_nParentID;
        CTypedPtrList<CObList,CXMLAttribute*> m_objAttributeList;
        BOOL        m_bValueDefined;
        BOOL        m_bWritten;
         int         m_nTagID;
   private:
        CString     m_strTagValue;
};
typedef CXMLTag * HXMLTAG;

//----------------------------------------------------------------
// class to generate xml file

class CXMLWriter
{
public:
	CXMLWriter(BOOL createFile = FALSE, float fVersion = 1.0, LPCTSTR szEncoding = _T("utf-8"));
	~CXMLWriter(void);

	POSITION	AddTag(POSITION posParentTag,LPCTSTR szTagName,LPCTSTR szTagValue=NULL);
	POSITION	AddTagIfNotEmpty(POSITION posParentTag,LPCTSTR szTagName,LPCTSTR szTagValue=NULL);
	LRESULT		AddAttribute (POSITION posTag,LPCTSTR szAttributeName,LPCTSTR szAttributeValue);
	LRESULT		AddAttribute (POSITION posTag,LPCTSTR szAttributeName,double AttributeValue);
	LRESULT		AddAttribute (POSITION posTag,LPCTSTR szAttributeName,long AttributeValue);
	POSITION	AddTagIfNotEmptyWithTrim(POSITION posParentTag,LPCTSTR szTagName,LPCTSTR szTagValue=NULL);

	LRESULT Write(CFile &file, POSITION refpos, BOOL bClose);
	LRESULT Write(CFile &file);
	LRESULT Write();

	LRESULT	WriteHeader(CFile &file);
	LRESULT	WriteHeader();
	
	CString WriteToString();

	void OpenFile(CString &);
	void CloseFile();

  void FindAndWriteChildTags(HXMLTAG hTag,CFile &file, int &nTabCounter);
  void FileWrite(CFile &file, CString &string, int &nTabCounter);
  void SetDTD (LPCTSTR szDocType, LPCTSTR szDTDFile);

	BOOL IsOpen() { return m_bOpen; }

private:
	CFile		m_File;
	BOOL		m_bOpen;
	float   m_fVersion;
	CString m_strEncoding;
	CString m_strDTD;
	BOOL    m_bStandalone;
	CTypedPtrList<CObList, CXMLTag*> m_objTagList;
	int     m_nTagCounter;
	CString m_strDocType;

	CMapPtrToPtr sParentMap;
};

//----------------------------------------------------------------
