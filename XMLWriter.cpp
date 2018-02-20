// Class for export XML-Data


#include "StdAfx.h"
#include "xmlwriter.h"
#include "UTF8Codec.h"

CXMLException::CXMLException(LPCTSTR szErrText)
{
}

CXMLException::~CXMLException(void)
{
}

//-------------------------------------------------------------------

CXMLAttribute::CXMLAttribute(void)
{
	m_strAttributeName = _T("");
	m_strAttributeValue = _T("");
}

CXMLAttribute::~CXMLAttribute(void)
{
}

//-------------------------------------------------------------------

CXMLWriter::CXMLWriter(
	BOOL createFile, float fVersion/*=1.0*/, LPCTSTR szEncoding/*=_T("utf-8")*/)
{
	m_fVersion		= fVersion;
	m_strEncoding = szEncoding;
	m_nTagCounter = 0;
	m_bStandalone = TRUE;
	m_bOpen = FALSE;

	if(createFile)
	{
		// Ask for the file name
		static char BASED_CODE szFilter[] = "Ficheros xml (*.xml)|*.xml|Todos los ficheros (*.*)|*.*||";
		CFileDialog dlgFile(FALSE, "xml", NULL, 
			OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | 0x800000, szFilter);
  
		CString lvFileName;
		if(dlgFile.DoModal() == IDOK)
		{
			lvFileName = dlgFile.GetPathName();
			OpenFile(lvFileName);
		}
	}
}

CXMLWriter::~CXMLWriter(void)
{
  POSITION    pos = m_objTagList.GetHeadPosition();
  HXMLTAG     hTag;
  while (pos)
  {
    hTag = m_objTagList.GetNext(pos);        
    if(hTag) 
			delete hTag;
  }

	CTypedPtrList<CObList, CXMLTag*> *pParent;
	
	for (POSITION IdxPos = sParentMap.GetStartPosition(); IdxPos!=NULL;) 
	{
		void *dummy, *dummy2;
		sParentMap.GetNextAssoc(IdxPos, dummy, dummy2);
		pParent = (CTypedPtrList<CObList, CXMLTag*> *) dummy2;

		delete pParent;
	}
	
	sParentMap.RemoveAll();


	if(IsOpen())
		CloseFile();
}

void CXMLWriter::OpenFile(CString& lvFileName)
{
	if(IsOpen() == FALSE)
	{
		if(m_File.Open(lvFileName, CFile::modeCreate|CFile::modeWrite) == 0) 
			throw CXMLException(XMLEXCEPTION_CREATEFILE);
		m_bOpen = TRUE;
	}
}

void CXMLWriter::CloseFile()
{
	if(IsOpen())
	{
		m_File.Close();
		m_bOpen = FALSE;
	}
}

LRESULT CXMLWriter::WriteHeader(CFile &file)
{
  CString	strBuffer;
    
  strBuffer.Format("<?xml version = \"%.1f\" encoding = \"%s\" standalone = \"%s\"?>\r\n",
    m_fVersion, 
    m_strEncoding,
    m_strDTD == _T("") ? _T("yes") : _T("no"));

  file.Write(strBuffer,strBuffer.GetLength());
  
  if (m_strDTD != _T(""))
  {
		strBuffer.Format ("<!DOCTYPE %s SYSTEM \"%s\">\r\n",
			m_strDocType, m_strDTD);
		file.Write(strBuffer, strBuffer.GetLength());
  }

	return S_OK;
}

LRESULT CXMLWriter::WriteHeader()
{
	if(IsOpen())
		return WriteHeader(m_File);
	return E_FAIL;
}

CString CXMLWriter::WriteToString()
{
	CMemFile memfile(32768);

	Write(memfile);

  DWORD szOutBuff = memfile.GetLength();
  char *outBuffer = (char*)malloc(szOutBuff);

  memfile.SeekToBegin();
  memfile.Read(outBuffer, szOutBuff);

  CString outStream(outBuffer);

	if(IsOpen())
		m_File.Write(outStream, outStream.GetLength());

	delete outBuffer;

	return outStream;
}

LRESULT CXMLWriter::Write(CFile &file)
{
  HXMLTAG   hTag;
  CString   strBuffer;
  int       nTabCounter = 0;
  POSITION	pos = m_objTagList.GetHeadPosition();

  if(pos) 
  {
    hTag = m_objTagList.GetNext(pos);
    POSITION pos = m_objTagList.GetHeadPosition();
    while (pos)
    {
      hTag = m_objTagList.GetNext(pos);
      int ret = hTag->Format(strBuffer);
      if (!hTag->m_bWritten) 
      {
        FileWrite(file,strBuffer,nTabCounter);
        if (ret == XMLTAGOPEN)
        {
          nTabCounter++;
          FindAndWriteChildTags(hTag,file,nTabCounter);
          strBuffer.Format("</%s>",hTag->m_strTagName);
          nTabCounter--;
          FileWrite(file,strBuffer,nTabCounter);
        }

        hTag->m_bWritten = TRUE;
      }
    }
  }

	return S_OK;
}

LRESULT CXMLWriter::Write(CFile &file, POSITION refpos, BOOL bClose)
{
  HXMLTAG   hTag;
  CString   strBuffer;
  int       nTabCounter = 0;
  POSITION	pos = m_objTagList.GetHeadPosition();
	POSITION	prev = NULL;

	if(bClose)
	{
		while (pos)
		{
			prev = pos;
			hTag = m_objTagList.GetNext(pos);
			if(prev == refpos)
			{
				pos = prev;
				break;
			}
		}
	}

  while(pos)
  {
		POSITION	aux = pos;
    hTag = m_objTagList.GetNext(pos);
    int ret = hTag->Format(strBuffer);
    if (!hTag->m_bWritten) 
    {
			if(!bClose)
			{
				FileWrite(file,strBuffer,nTabCounter);
				if(aux == refpos)
					break;
			}
			else
			{
				if(ret == XMLTAGOPEN)
				{
					nTabCounter++;
					FindAndWriteChildTags(hTag,file,nTabCounter);
					strBuffer.Format("</%s>",hTag->m_strTagName);
					nTabCounter--;
					FileWrite(file,strBuffer,nTabCounter);
				}

	      hTag->m_bWritten = TRUE;
			}
    }
  } // end while

	return S_OK;
}

LRESULT CXMLWriter::Write()
{
	if(IsOpen())
		return Write(m_File);
	return E_FAIL;
}


void CXMLWriter::FindAndWriteChildTags(HXMLTAG hTag,CFile &file, int &nTabCounter)
{
	CTypedPtrList<CObList, CXMLTag*> *objTagList;
	
	void *ptrlist;
	if (!sParentMap.Lookup((void *) (hTag->m_nTagID), ptrlist))
	{
		return;  // nothing to do
	}
	else
	{		
		objTagList = (CTypedPtrList<CObList, CXMLTag*> *) ptrlist;
	}

	POSITION    posChild = objTagList->GetHeadPosition();
	HXMLTAG     hTagChild;
	CString     strBuffer;

	while (posChild)
	{
		hTagChild = objTagList->GetNext(posChild);
		if (hTagChild->m_nParentID == -1) continue;
		if (!hTag->m_bWritten && hTag->m_nTagID == hTagChild->m_nParentID)
		{
			int ret = hTagChild->Format(strBuffer);
			FileWrite(file,strBuffer,nTabCounter);
			if (ret == XMLTAGOPEN)
			{
				nTabCounter++;
				FindAndWriteChildTags(hTagChild,file,nTabCounter);
				strBuffer.Format("</%s>",hTagChild->m_strTagName);
				nTabCounter--;
				FileWrite(file,strBuffer,nTabCounter);
			}

			hTagChild->m_bWritten = TRUE;
		}
	}
}

void CXMLWriter::FileWrite(CFile &file, CString &string, int &nTabCounter)
{
	int ilx;

	CString sAux;

	for (ilx = 0; ilx < nTabCounter; ilx++) 
	{
		sAux += "\t";
	}

	sAux = sAux + string + "\r\n";	

	file.Write(sAux, sAux.GetLength()); 
}

void CXMLWriter::SetDTD(LPCTSTR szDocType, LPCTSTR szDTDFile)
{
  m_strDTD = szDTDFile;
  m_strDocType = szDocType;
  m_bStandalone = FALSE;
}

LRESULT CXMLWriter::AddAttribute (POSITION posTag,LPCTSTR szAttributeName,LPCTSTR szAttributeValue)
{
	ASSERT(posTag != NULL);
	HXMLTAG pTag = m_objTagList.GetAt(posTag);
	HXMLATTRIBUTE pNewAttribute = new (CXMLAttribute);
	pNewAttribute->m_strAttributeName = szAttributeName;
	pNewAttribute->m_strAttributeValue = szAttributeValue;

	POSITION    pPos = pTag->m_objAttributeList.AddTail(pNewAttribute);

	if (!pPos) 
			throw CXMLException(XMLEXCEPTION_ADDATTRIBUTE);

	return S_OK;
}

LRESULT CXMLWriter::AddAttribute (POSITION posTag,LPCTSTR szAttributeName,double AttributeValue)
{
	CString strValue;
	strValue.Format ("%f",AttributeValue);
	return AddAttribute(posTag,szAttributeName,strValue);
}

LRESULT CXMLWriter::AddAttribute (POSITION posTag,LPCTSTR szAttributeName,long AttributeValue)
{
	CString strValue;
	strValue.Format ("%d",AttributeValue);
	return AddAttribute(posTag,szAttributeName,strValue);
}

POSITION CXMLWriter::AddTag(POSITION posParentTag, LPCTSTR szTagName, LPCTSTR szTagValue/*=NULL*/) 
{
	HXMLTAG pNewTag = new (CXMLTag);
	pNewTag->m_strTagName = szTagName;
	if (szTagValue)
	{
		CString sTagValue(szTagValue);
		std::string str((LPCTSTR)sTagValue);
		UTF8Codec::Encode(str);
		sTagValue = CString(str.c_str());
		szTagValue = (LPCTSTR)sTagValue;
	}
	pNewTag->SetTagValue (szTagValue);
	if (posParentTag)
	{
			pNewTag->m_nParentID = m_objTagList.GetAt(posParentTag)->m_nTagID;
			
			void *ptrlist;
			if (!sParentMap.Lookup((void *)pNewTag->m_nParentID, ptrlist))
			{				
				CTypedPtrList<CObList, CXMLTag*> *pParent;
				
				pParent = new CTypedPtrList<CObList, CXMLTag*>;
				pParent->AddTail(pNewTag);
				ptrlist = (void *) pParent;
				sParentMap.SetAt((void *)pNewTag->m_nParentID, ptrlist);
			}
			else
			{
				CTypedPtrList<CObList, CXMLTag*> *objTagList = (CTypedPtrList<CObList, CXMLTag*> *) ptrlist;
				objTagList->AddTail(pNewTag);
			}
	}

	pNewTag->m_nTagID = m_nTagCounter++;
	POSITION    pPos = m_objTagList.AddTail(pNewTag);

	if (!pPos) 
			throw CXMLException(XMLEXCEPTION_ADDTAG);

	return (pPos);
}

// TAKE CARE!!!! It may return NULL
POSITION	CXMLWriter::AddTagIfNotEmpty(POSITION posParentTag, LPCTSTR szTagName, LPCTSTR szTagValue/*=NULL*/)
{
	CString sAux;
	sAux = szTagValue;
	if (sAux.IsEmpty())
	{
		return NULL;			
	}
	else
	{
		return (AddTag(posParentTag, szTagName, szTagValue));
	}

}

POSITION CXMLWriter::AddTagIfNotEmptyWithTrim(POSITION posParentTag, LPCTSTR szTagName, LPCTSTR szTagValue/*=NULL*/)
{
	CString sAux;
	sAux = szTagValue;
	sAux.TrimRight();
	sAux.TrimLeft();
	if(sAux.IsEmpty())
	{
		return NULL;			
	}
	else
	{
		return (AddTag(posParentTag, szTagName, szTagValue));
	}
}


//-------------------------------------------------------------------

CXMLTag::CXMLTag(void)
{
	m_strTagName = _T("");
	m_strTagValue = _T("");
	m_bValueDefined = FALSE;
	m_bWritten = FALSE;
	m_nTagID = -1;
	m_nParentID = -1;
}

CXMLTag::~CXMLTag(void)
{
	POSITION        pos = m_objAttributeList.GetHeadPosition();
	HXMLATTRIBUTE   hAttribute;
	while (pos)
	{
			hAttribute = m_objAttributeList.GetNext(pos);        
			if (hAttribute) delete hAttribute;
	}
}

void CXMLTag::SetTagValue(LPCTSTR szValue)
{
	m_strTagValue = szValue;
	if (szValue) 
			m_bValueDefined = TRUE;
}

int CXMLTag::Format(CString &string)
{
	HXMLATTRIBUTE   hAttribute;
	CString         strBuffer;
	string.Format ("<%s",m_strTagName);
	int             retval = XMLTAGOPEN;

	POSITION pos = m_objAttributeList.GetHeadPosition();
	while (pos)
	{
			hAttribute = m_objAttributeList.GetNext(pos);
			strBuffer.Format (" %s=\"%s\"",
					hAttribute->m_strAttributeName, hAttribute->m_strAttributeValue);
			string += strBuffer;
	}

	if (m_bValueDefined) 
	{
			if (m_strTagValue == _T(""))
					string += "/>";
			else
			{
					strBuffer.Format (">%s</%s>",m_strTagValue,m_strTagName);
					string += strBuffer;
			}
			retval = XMLTAGREADY;
	}
	else
			string += ">";

	return retval;
}
