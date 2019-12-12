
#ifndef __DVXML_H
#define __DVXML_H

#include "DvDef.h"

struct DVXML_ATTR
{
	CString strName;
	CString strVal;
};
typedef CList<DVXML_ATTR *, DVXML_ATTR *> DVXML_ATTR_LIST;

struct DVXML_NODE
{
	//NODE_ATTRIBUTE					attribute 
	//NODE_CDATA_SECTION				cdatasection 
	//NODE_COMMENT						comment 
	//NODE_DOCUMENT						document 
	//NODE_DOCUMENT_FRAGMENT			documentfragment 
	//NODE_DOCUMENT_TYPE				documenttype 
	//NODE_ELEMENT						element 
	//NODE_ENTITY						entity 
	//NODE_ENTITY_REFERENCE				entityreference 
	//NODE_NOTATION						notation 
	//NODE_PROCESSING_INSTRUCTION		processinginstruction 
	//NODE_TEXT							text 	
	CString strType;
	CString strName;	
	CString strVal;

	DVXML_ATTR_LIST lstAttr;
	CList<DVXML_NODE *, DVXML_NODE *> lstNode;
};
typedef CList<DVXML_NODE *, DVXML_NODE *> DVXML_NODE_LIST;


class AFX_CLASS_EXPORT CDvXML
{
public:
	CDvXML();
	~CDvXML();
	
public:
	//file oper
	BOOL Load(LPCTSTR lpszPathName, LPCTSTR lpszDefRootName = NULL);
	BOOL Save(LPCTSTR lpszPathName = NULL);
	LPCTSTR GetError() { return m_strError; }

	//node oper
	enum DVXML_OPERFLAG
	{
		CHECKONLY,
		CHECKCREATE,
		CREATEONLY,
		DELETEONLY,
		DELETECREATE,
	};

	DVXML_NODE *GetRoot();
	static DVXML_NODE *OperateNode(DVXML_NODE *pParent, LPCTSTR lpszNodeName, 
									DVXML_OPERFLAG eFlag = CHECKCREATE);

	static LPCTSTR GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, CString &strReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, LPTSTR lpszReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, long &lReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned long &lReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, int &nReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned int &nReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, char &cReturn);
	static BOOL GetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned char &cReturn);

	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, LPCTSTR lpszVal);
	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, long lVal);
	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned long lVal);
	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, int nVal);
	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned int nVal);
	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, char cVal);
	static BOOL SetAttrib(DVXML_NODE *pNode, LPCTSTR lpszAttrName, unsigned char cVal);

	static BOOL GetInnerText(DVXML_NODE *pNode, CString &strInnerText);
	static BOOL SetInnerText(DVXML_NODE *pNode, LPCTSTR lpszInnerText);
	
	static BOOL SelectNodes(DVXML_NODE *pParent, LPCTSTR lpszNodeName, DVXML_NODE_LIST &lstNode);

	//helper fuction.
	static void FreeNodeMem(DVXML_NODE *pNode);
	static void FreeListNodeMem(DVXML_NODE_LIST &lstNode);
	static void CopyNode(DVXML_NODE *pDest, DVXML_NODE *pSrc);

private:
	CString m_strPathName;
	CString m_strError;
	DVXML_NODE m_RootNode;

	IXMLDOMDocument *GetDOM();	
	BOOL ReadXMLNode(IXMLDOMNode *pIXmlNode, DVXML_NODE *pParent);
	BOOL XmlGetNodeName(IXMLDOMNode *pIXmlNode, CString &strName);
	BOOL XmlGetNodeVal(IXMLDOMNode *pIXmlNode, CString &strVal);
	BOOL WriteXMLNode(IXMLDOMDocument *pIXMLDoc, IXMLDOMNode *pIXmlNode, 								
								DVXML_NODE *pParent);
	IXMLDOMNode *XmlCreateNode(IXMLDOMDocument *pIXMLDoc, IXMLDOMNode *pIXmlNodeParent,
								CString &strType, CString &strName, CString &strVal);
	BOOL XmlSetNodeAttr(IXMLDOMDocument *pIXMLDoc, IXMLDOMNode *pIXmlNode, 								
								CString &strName, CString &strVal);	
};


#endif
