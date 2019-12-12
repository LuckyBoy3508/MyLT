
#include "stdafx.h"
#include "DvXML.h"
#include "DvDir.h"

#import <msxml3.dll>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDvXML

CDvXML::CDvXML()
{
	m_strPathName = "";
	m_strError = "";
}

CDvXML::~CDvXML()
{
	FreeNodeMem(&m_RootNode);
}

void CDvXML::FreeNodeMem(DVXML_NODE *pNode)
{
	if (pNode)
	{
		POSITION pos = NULL;

		//1. property
		pNode->strType.Empty();
		pNode->strName.Empty();		
		pNode->strVal.Empty();

		//2. free attribute.
		pos = pNode->lstAttr.GetHeadPosition();
		while (pos)
		{
			DVXML_ATTR *pTmp = pNode->lstAttr.GetNext(pos);
			delete pTmp;
		}
		pNode->lstAttr.RemoveAll();

		//3. free children.
		FreeListNodeMem(pNode->lstNode);
	}
}

void CDvXML::FreeListNodeMem(DVXML_NODE_LIST &lstNode)
{
	POSITION pos = lstNode.GetHeadPosition();
	while (pos)
	{
		DVXML_NODE *pTmp = lstNode.GetNext(pos);
		FreeNodeMem(pTmp);

		delete pTmp;
	}
	lstNode.RemoveAll();
}

void CDvXML::CopyNode(DVXML_NODE *pDest, DVXML_NODE *pSrc)
{
	if (pDest && pSrc)
	{
		POSITION pos = NULL;

		//1. property
		pDest->strType = pSrc->strType;
		pDest->strName = pSrc->strName;
		pDest->strVal = pSrc->strVal;		

		//2. free attribute.
		pos = pSrc->lstAttr.GetHeadPosition();
		while (pos)
		{
			DVXML_ATTR *pTmp = pSrc->lstAttr.GetNext(pos);
			
			DVXML_ATTR *pNew = new DVXML_ATTR;
			pDest->lstAttr.AddTail(pNew);

			pNew->strName = pTmp->strName;
			pNew->strVal = pTmp->strVal;
		}

		//3. free children.		
		pos = pSrc->lstNode.GetHeadPosition();
		while (pos)
		{
			DVXML_NODE *pTmp = pSrc->lstNode.GetNext(pos);

			DVXML_NODE *pNew = new DVXML_NODE;
			pDest->lstNode.AddTail(pNew);
			CopyNode(pNew, pTmp);
		}
	}
}

BOOL CDvXML::Load(LPCTSTR lpszPathName, LPCTSTR lpszDefRootName/* = NULL*/)
{
	BOOL bRet = FALSE;

	FreeNodeMem(&m_RootNode);
	m_strPathName.Empty();

	if (lpszPathName)
	{		
		m_strPathName = lpszPathName;

		CDvDir dvDir;
		if (dvDir.IsFileExist(lpszPathName))
		{
			CoInitialize(NULL);

			IXMLDOMDocument *pIXMLDoc = GetDOM();
			if (pIXMLDoc)
			{
				VARIANT_BOOL bIsSucc = VARIANT_FALSE;
				HRESULT hResult = pIXMLDoc->load(_variant_t(lpszPathName), &bIsSucc);		
				if (SUCCEEDED(hResult) && bIsSucc == VARIANT_TRUE)
				{
					IXMLDOMElement *pDocElement = NULL;
					hResult = pIXMLDoc->get_documentElement(&pDocElement);
					if (SUCCEEDED(hResult) && pDocElement)
					{
						m_RootNode.strType = "element";
						bRet = ReadXMLNode(pDocElement, &m_RootNode);
						
						pDocElement->Release();
						pDocElement = NULL;
					}
				}
				else
				{
					IXMLDOMParseError *pIParseError = NULL;

					hResult = pIXMLDoc->get_parseError(&pIParseError);
					if (SUCCEEDED(hResult) && pIParseError)
					{
						long lLine, lChar;
						lLine = lChar = 0;
						BSTR bstrReason, bstrSrc;

						pIParseError->get_line(&lLine);
						pIParseError->get_linepos(&lChar);
						pIParseError->get_reason(&bstrReason);
						pIParseError->get_srcText(&bstrSrc);						

						_bstr_t btstrReason(bstrReason);
						_bstr_t btstrSrc(bstrSrc);

						CString strReason = (LPCTSTR)btstrReason;
						CString strSrc = (LPCTSTR)btstrSrc;

						m_strError.Format(L"Load xml file failed!\nFile:<%s>\nLine<%d> Char<%d>\nReason:<%s>\nSrc:<%s>", 
											m_strPathName, lLine, lChar, strReason, strSrc);
					}
				}

				pIXMLDoc->Release();
				pIXMLDoc = NULL;
			}

			CoUninitialize();
		}
		else
		{
			if (lpszDefRootName)
			{
				bRet = TRUE;
				m_RootNode.strName = lpszDefRootName;
			}			
		}
	}

	return bRet;	
}

BOOL CDvXML::Save(LPCTSTR lpszPathName/* = NULL*/)
{
	BOOL bRet = FALSE;

	//1. get file name
	if (lpszPathName == NULL && m_strPathName.GetLength() > 0)
		lpszPathName = m_strPathName;

	//2. write data to xml file
	if (lpszPathName && !m_RootNode.strName.IsEmpty())
	{
		CoInitialize(NULL);
				
		IXMLDOMDocument *pIXMLDoc = GetDOM();		
		if (pIXMLDoc)
		{
			//1. create xml doc node tree.			
			IXMLDOMElement *pDocElement = (IXMLDOMElement *)XmlCreateNode(pIXMLDoc, NULL, 
									m_RootNode.strType, m_RootNode.strName, m_RootNode.strVal);
			if (pDocElement != NULL)
			{
				bRet = WriteXMLNode(pIXMLDoc, pDocElement, &m_RootNode);

				pDocElement->Release();
				pDocElement = NULL;
			}
			
			//2. write to xml file
			if (bRet)
			{
				CDvDir dvDir;
				dvDir.DeleteFile(lpszPathName);
				pIXMLDoc->save(_variant_t(lpszPathName));
			}

			pIXMLDoc->Release();
			pIXMLDoc = NULL;
		}

		CoUninitialize();
	}

	return bRet;
}

DVXML_NODE *CDvXML::GetRoot()
{
	return &m_RootNode;
}

DVXML_NODE *CDvXML::OperateNode(DVXML_NODE *pParent, LPCTSTR lpszNodeName, 
								DVXML_OPERFLAG eFlag/* = CHECKCREATE*/)
{
	DVXML_NODE *pRetNode = NULL;

	if (pParent && lpszNodeName)
	{
		DVXML_NODE *pIXmlNodeTmp = NULL;

		//1. find child node.
		POSITION pos, pos1;
        pos = NULL;
        pos1 = NULL;
		pos = pParent->lstNode.GetHeadPosition();
		while (pos)
		{
			pos1 = pos;

			pIXmlNodeTmp = pParent->lstNode.GetNext(pos);
			if (pIXmlNodeTmp->strName.CompareNoCase(lpszNodeName) == 0)
			{
				pRetNode = pIXmlNodeTmp;
				break;
			}
		}

		//2. do operate
		switch (eFlag)
		{
		case CHECKONLY:
			break;
		case CHECKCREATE:
			{
				if (pRetNode == NULL)
				{
					pRetNode = new DVXML_NODE;
					pParent->lstNode.AddTail(pRetNode);

					pRetNode->strType = "element";
					pRetNode->strName = lpszNodeName;
				}
			}
			break;
		case CREATEONLY:
			{
				pRetNode = new DVXML_NODE;
				pParent->lstNode.AddTail(pRetNode);

				pRetNode->strType = "element";
				pRetNode->strName = lpszNodeName;
			}
			break;
		case DELETEONLY:
			{
				if (pRetNode != NULL)
				{
					pParent->lstNode.RemoveAt(pos1);
					FreeNodeMem(pRetNode);
					delete pRetNode;
					pRetNode = NULL;
				}					
			}
			break;
		case DELETECREATE:
			{
				if (pRetNode != NULL)
				{
					pParent->lstNode.RemoveAt(pos1);
					FreeNodeMem(pRetNode);
					delete pRetNode;
					pRetNode = NULL;
				}
				
				pRetNode = new DVXML_NODE;
				pParent->lstNode.AddTail(pRetNode);

				pRetNode->strType = "element";
				pRetNode->strName = lpszNodeName;
			}
			break;
		default:
			break;
		}
	}

	return pRetNode;
}

LPCTSTR CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName)
{
	LPCTSTR lpszRet = NULL;

	if (pNode && lpszAttrName)
	{
		POSITION pos = pNode->lstAttr.GetHeadPosition();
		while (pos)
		{
			DVXML_ATTR *pAttr = pNode->lstAttr.GetNext(pos);
			if (pAttr->strName.CompareNoCase(lpszAttrName) == 0)
			{
				lpszRet = pAttr->strVal;
				break;
			}
		}
	}	

	return lpszRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, CString &strReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr)
	{
		strReturn = lpszAttr;
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, LPTSTR lpszReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr)
	{        
		wcscpy_s(lpszReturn, wcslen(lpszAttr), lpszAttr);
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, bool &bReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
        if (wcscmp(lpszAttr, L"false") == 0)
            bReturn = false;
        else
            bReturn = true;

		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, long &lReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
		lReturn = (long)(_wtoi(lpszAttr));
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned long &lReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
		lReturn = (unsigned long)(_wtoi(lpszAttr));
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, int &nReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
		nReturn = (int)(_wtoi(lpszAttr));
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned int &nReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
		nReturn = (unsigned int)(_wtoi(lpszAttr));
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, char &cReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
		cReturn = (char)(lpszAttr[0]);
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned char &cReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
		cReturn = (unsigned char)(_wtoi(lpszAttr));
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, double &dReturn)
{
	BOOL bRet = FALSE;
	
	LPCTSTR lpszAttr = GetAttrib(pNode, lpszAttrName);
	if (lpszAttr && wcslen(lpszAttr) > 0)
	{
        dReturn = (double)(_wtof(lpszAttr));
		bRet = TRUE;
	}	

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, LPCTSTR lpszAttrVal)
{
	BOOL bRet = FALSE;

	if (pNode && lpszAttrName && lpszAttrVal)
	{
		DVXML_ATTR *pRetAttr = NULL;

		//1. check exist
		POSITION pos = pNode->lstAttr.GetHeadPosition();
		while (pos)
		{
			DVXML_ATTR *pAttr = pNode->lstAttr.GetNext(pos);
			if (pAttr->strName.CompareNoCase(lpszAttrName) == 0)
			{
				pRetAttr = pAttr;
				break;
			}
		}

		//2. set attr
		if (!pRetAttr)
		{
			pRetAttr = new DVXML_ATTR;
			pNode->lstAttr.AddTail(pRetAttr);

			pRetAttr->strName = lpszAttrName;			
		}

		pRetAttr->strVal = lpszAttrVal;

		bRet = TRUE;
	}	
	
	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, bool bVal)
{
	BOOL bRet = FALSE;

    CString strVal = bVal ? L"true" : L"false";
	bRet = SetAttrib(pNode, lpszAttrName, strVal);
	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, long lVal)
{
	BOOL bRet = FALSE;

	CString strVal;
	strVal.Format(L"%d", lVal);

	bRet = SetAttrib(pNode, lpszAttrName, strVal);

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned long lVal)
{
	BOOL bRet = FALSE;

	CString strVal;
	strVal.Format(L"%d", lVal);

	bRet = SetAttrib(pNode, lpszAttrName, strVal);

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, int nVal)
{
	BOOL bRet = FALSE;

	CString strVal;
	strVal.Format(L"%d", nVal);

	bRet = SetAttrib(pNode, lpszAttrName, strVal);

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned int nVal)
{
	BOOL bRet = FALSE;

	CString strVal;
	strVal.Format(L"%d", nVal);

	bRet = SetAttrib(pNode, lpszAttrName, strVal);

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, char cVal)
{
	BOOL bRet = FALSE;

	CString strVal(cVal);
	bRet = SetAttrib(pNode, lpszAttrName, strVal);

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned char cVal)
{
	BOOL bRet = FALSE;

	CString strVal;
	strVal.Format(L"%d", cVal);
	bRet = SetAttrib(pNode, lpszAttrName, strVal);

	return bRet;
}

BOOL CDvXML::SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, double dVal)
{
	BOOL bRet = FALSE;

    CString strVal;
    strVal.Format(L"%8.3f", dVal);
	bRet = SetAttrib(pNode, lpszAttrName, strVal);
	return bRet;
}

BOOL CDvXML::GetInnerText(DVXML_NODE *pNode, CString &strInnerText)
{
	strInnerText = "";

	if (pNode)
	{
		if (!pNode->strVal.IsEmpty())
			strInnerText = pNode->strVal;
		else
		{
			POSITION pos = pNode->lstNode.GetHeadPosition();
			while (pos)
			{
				DVXML_NODE *pChild = pNode->lstNode.GetNext(pos);

				CString strTmp;
				if (GetInnerText(pChild, strTmp))
					strInnerText += strTmp;
			}
		}
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CDvXML::SetInnerText(DVXML_NODE *pNode, LPCTSTR lpszInnerText)
{
	if (pNode)
	{
		DVXML_NODE *pTextNode = new DVXML_NODE;
		pTextNode->strType = "text";
		pTextNode->strName = "#text";
		pTextNode->strVal = lpszInnerText;
		pNode->lstNode.AddTail(pTextNode);
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CDvXML::SelectNodes(DVXML_NODE *pParent, LPCTSTR lpszNodeName, DVXML_NODE_LIST &lstNode)
{
	BOOL bRet = FALSE;

	lstNode.RemoveAll();
	if (pParent && lpszNodeName)
	{
		bRet = TRUE;

		POSITION pos = pParent->lstNode.GetHeadPosition();
		while (pos)
		{
			DVXML_NODE *pNode = pParent->lstNode.GetNext(pos);
			if (pNode->strName.CompareNoCase(lpszNodeName) == 0)
			{
				lstNode.AddTail(pNode);
			}
		}
	}

	return bRet;
}

BOOL CDvXML::ReadXMLNode(IXMLDOMNode *pIXmlNode, DVXML_NODE *pParent)
{
	HRESULT hResult;

	//1. get node-name/inner-text
	XmlGetNodeName(pIXmlNode, pParent->strName);
	XmlGetNodeVal(pIXmlNode, pParent->strVal);

	if (pParent->strType != "element")
		return TRUE;

	//2. get node attrib
	IXMLDOMNamedNodeMap *pIXMLNodeMap = NULL;
	hResult = pIXmlNode->get_attributes(&pIXMLNodeMap);
	if (SUCCEEDED(hResult) && pIXMLNodeMap)
	{
		long lChildNum = 0;
		pIXMLNodeMap->get_length(&lChildNum);
		for (long a = 0; a < lChildNum; a++)
		{
			IXMLDOMNode *pIXMLAttrNode = NULL;
			hResult = pIXMLNodeMap->get_item(a, &pIXMLAttrNode);
			if (SUCCEEDED(hResult) && pIXMLAttrNode)
			{
				DVXML_ATTR *pAttr = new DVXML_ATTR;
				pParent->lstAttr.AddTail(pAttr);

				XmlGetNodeName(pIXMLAttrNode, pAttr->strName);
				XmlGetNodeVal(pIXMLAttrNode, pAttr->strVal);

				pIXMLAttrNode->Release();
				pIXMLAttrNode = NULL;
			}
			else
			{
				pIXMLNodeMap->Release();
				pIXMLNodeMap = NULL;

				return FALSE;
			}
		}

		pIXMLNodeMap->Release();
		pIXMLNodeMap = NULL;
	}
	else
		return FALSE;

	//3. get children	
	IXMLDOMNodeList *pIXMLNodeList = NULL;
	hResult = pIXmlNode->get_childNodes(&pIXMLNodeList);
	if (SUCCEEDED(hResult) && pIXMLNodeList)
	{
		long lChildNum = 0;
		pIXMLNodeList->get_length(&lChildNum);
		for (long a = 0; a < lChildNum; a++)
		{
			BOOL bRet = FALSE;

			IXMLDOMNode *pIXMLChildNode = NULL;
			hResult = pIXMLNodeList->nextNode(&pIXMLChildNode);
			if (SUCCEEDED(hResult) && pIXMLChildNode)
			{
				DVXML_NODE *pNew = new DVXML_NODE;
				pParent->lstNode.AddTail(pNew);

				DOMNodeType eNodeType;
				pIXMLChildNode->get_nodeType(&eNodeType);
				if (eNodeType == NODE_ELEMENT)
					pNew->strType = "element";
				else if (eNodeType == NODE_TEXT)
					pNew->strType = "text";
				else if (eNodeType == NODE_CDATA_SECTION)
					pNew->strType = "cdatasection";

				bRet = ReadXMLNode(pIXMLChildNode, pNew);

				pIXMLChildNode->Release();
				pIXMLChildNode = NULL;
			}

			if (!bRet)
			{
				pIXMLNodeList->Release();
				pIXMLNodeList = NULL;

				return FALSE;
			}
		}

		pIXMLNodeList->Release();
		pIXMLNodeList = NULL;
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CDvXML::XmlGetNodeName(IXMLDOMNode *pIXmlNode, CString &strName)
{
	BOOL bRet = FALSE;

	if (pIXmlNode)
	{
		BSTR bstrName;
		HRESULT hResult = pIXmlNode->get_nodeName(&bstrName);
		if (SUCCEEDED(hResult))
		{
			_bstr_t btNodeName(bstrName);
			if (btNodeName.length() > 0)
			{				
				strName = (LPCTSTR)btNodeName;
				bRet = TRUE;
			}
		}
	}

	return bRet;
}

BOOL CDvXML::XmlGetNodeVal(IXMLDOMNode *pIXmlNode, CString &strVal)
{
	BOOL bRet = FALSE;

	if (pIXmlNode)
	{
		COleVariant varVal;
		HRESULT hResult = pIXmlNode->get_nodeValue(&varVal);
		if (SUCCEEDED(hResult) && varVal.bstrVal)
		{
			_bstr_t btNodeVal(varVal.bstrVal);
			if (btNodeVal.length() > 0)
			{
				strVal = (LPCTSTR)btNodeVal;
				bRet = TRUE;
			}
		}
	}

	return bRet;
}

BOOL CDvXML::WriteXMLNode(IXMLDOMDocument *pIXMLDoc, IXMLDOMNode *pIXmlNode, 							
							DVXML_NODE *pParent)
{
	POSITION pos = NULL;
	IXMLDOMNode *pChildNode = NULL;
	
	//1. write attribute
	pos = pParent->lstAttr.GetHeadPosition();
	while (pos)
	{
		DVXML_ATTR *pAttr = pParent->lstAttr.GetNext(pos);
		if (!XmlSetNodeAttr(pIXMLDoc, pIXmlNode, pAttr->strName, pAttr->strVal))
			return FALSE;
	}

	//2. wirte children
	pos = pParent->lstNode.GetHeadPosition();
	while (pos)
	{
		DVXML_NODE *pChild = pParent->lstNode.GetNext(pos);
		
		pChildNode = XmlCreateNode(pIXMLDoc, pIXmlNode, pChild->strType, 
									pChild->strName, pChild->strVal);
		if (pChildNode)
		{
			BOOL bRet = WriteXMLNode(pIXMLDoc, pChildNode, pChild);

			pChildNode->Release();
			pChildNode = NULL;

			if (!bRet)
				return FALSE;
		}
		else
			return FALSE;
	}
	
	return TRUE;
}

BOOL CDvXML::XmlSetNodeAttr(IXMLDOMDocument *pIXMLDoc, IXMLDOMNode *pIXmlNode, 								
								CString &strName, CString &strVal)
{
	BOOL bRet = FALSE;

	if (pIXmlNode != NULL && !strName.IsEmpty())
	{
		HRESULT hResult;
		IXMLDOMNamedNodeMap *pNodeMap = NULL;
		hResult = pIXmlNode->get_attributes(&pNodeMap);
		if (SUCCEEDED(hResult) && pNodeMap)
		{
			IXMLDOMAttribute *pAttrTmp = NULL;
			hResult = pIXMLDoc->createAttribute(_bstr_t(strName), &pAttrTmp);
			if (SUCCEEDED(hResult) && pAttrTmp)
			{
				pAttrTmp->put_nodeValue(_variant_t(strVal));

				IXMLDOMNode *pAttrNode = NULL;
				hResult = pNodeMap->setNamedItem(pAttrTmp, &pAttrNode);				
				if (SUCCEEDED(hResult) && pAttrNode)
				{
					pAttrNode->Release();
					pAttrNode = NULL;

					bRet = TRUE;
				}

				pAttrTmp->Release();
				pAttrTmp = NULL;
			}

			pNodeMap->Release();
			pNodeMap = NULL;
		}
	}

	return bRet;
}

IXMLDOMNode *CDvXML::XmlCreateNode(IXMLDOMDocument *pIXMLDoc, IXMLDOMNode *pIXmlNodeParent,
									CString &strType, CString &strName, CString &strVal)
{
	IXMLDOMNode *pIXmlNodeRet = NULL;

	HRESULT hResult;
	if (pIXmlNodeParent != NULL)
	{//create child node		
		IXMLDOMNode *pIXmlNodeTmp = NULL;

		if (strType == "element")
		{
			IXMLDOMElement *pIXmlElement = NULL;

			hResult = pIXMLDoc->createElement(_bstr_t(strName), &pIXmlElement);
			pIXmlNodeTmp = pIXmlElement;
		}
		else if (strType == "text")
		{
			IXMLDOMText *pIXmlText = NULL;

			hResult = pIXMLDoc->createTextNode(_bstr_t(strVal), &pIXmlText);
			pIXmlNodeTmp = pIXmlText;
		}
		else if (strType == "cdatasection")
		{
			IXMLDOMCDATASection *pIXmlCDataSection = NULL;

			hResult = pIXMLDoc->createCDATASection(_bstr_t(strVal), &pIXmlCDataSection);
			pIXmlNodeTmp = pIXmlCDataSection;
		}

		if (SUCCEEDED(hResult) && pIXmlNodeTmp)
		{
			pIXmlNodeParent->appendChild(pIXmlNodeTmp, &pIXmlNodeRet);

			pIXmlNodeTmp->Release();
			pIXmlNodeTmp = NULL;
		}
	}
	else
	{//create doc-element		
		CString strXml;
		strXml.Format(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><%s></%s>", strName, strName);
		_bstr_t bt((LPCTSTR)strXml);

		VARIANT_BOOL bIsSucc = VARIANT_FALSE;
		hResult = pIXMLDoc->loadXML(bt.copy(), &bIsSucc);
		if (SUCCEEDED(hResult) && bIsSucc == VARIANT_TRUE)
		{
			IXMLDOMElement *pIXmlElement = NULL;

			hResult = pIXMLDoc->get_documentElement(&pIXmlElement);
			if (SUCCEEDED(hResult) && pIXmlElement)
			{
				pIXmlNodeRet = pIXmlElement;
			}
		}
	}

	return pIXmlNodeRet;
}

IXMLDOMDocument *CDvXML::GetDOM()
{
	IXMLDOMDocument *pIXMLDoc = NULL;

	HRESULT hResult = CoCreateInstance(__uuidof(MSXML2::DOMDocument30),
										NULL,
										CLSCTX_INPROC_SERVER,
										__uuidof(IXMLDOMDocument),
										(void**)&pIXMLDoc);

	return pIXMLDoc;
}
