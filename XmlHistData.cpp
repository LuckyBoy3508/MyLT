
#include "stdafx.h"
#include "MyLT.h"
#include "XmlHistData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXmlHistData

CXmlHistData::CXmlHistData()
{
}

CXmlHistData::~CXmlHistData()
{
}

BOOL CXmlHistData::RWHistData(LT_FILE_INFO *pFileDataInfo,
							BOOL bIsRead/* = TRUE*/,
							BOOL bOnlyHeader/* = FALSE*/)
{
	BOOL bRet = FALSE;
	
	if (pFileDataInfo && strlen(pFileDataInfo->szName) > 0)
	{
		CString strPathName;
		strPathName.Format("%s\\%s", theApp.m_strHistDataPath, pFileDataInfo->szName);

		CDvXML dvXml;
		DVXML_NODE *pRootNode = dvXml.GetRoot();
		if (bIsRead)
		{
			if (CDvDir::IsFileExist(strPathName) && dvXml.Load(strPathName))
				bRet = Node2FileInfo(pRootNode, pFileDataInfo, bOnlyHeader);
		}
		else
		{			
			CDvDir::DeleteFile(strPathName);
			if (FileInfo2Node(pFileDataInfo, pRootNode, bOnlyHeader))
			{
				pRootNode->strName = "FileInfo";
				bRet = dvXml.Save(strPathName);
			}
		}
	}

	return bRet;
}

BOOL CXmlHistData::Node2FileInfo(DVXML_NODE *pRootNode, LT_FILE_INFO *pFileDataInfo, 						
									BOOL bOnlyHeader)
{
	BOOL bRet = FALSE;

	if (pRootNode && pFileDataInfo)
	{
		bRet = TRUE;
		
		DVXML_NODE *pTmpNode = NULL;

		//1. get header info
		DVXML_NODE *pHeadNode = CDvXML::OperateNode(pRootNode, "HeaderInfo", CDvXML::CHECKONLY);
		if (pHeadNode)
		{
			CDvXML::GetAttrib(pHeadNode, "Description", pFileDataInfo->szDesp);
			CDvXML::GetAttrib(pHeadNode, "Version", pFileDataInfo->lVersion);
			CDvXML::GetAttrib(pHeadNode, "Province", pFileDataInfo->lProvince);
		}

		if (bOnlyHeader)
			return bRet;

		//2. get data info		
		DVXML_NODE *pDataNode = CDvXML::OperateNode(pRootNode, "DataInfo", CDvXML::CHECKONLY);
		if (pDataNode)
		{
			CDvXML::GetAttrib(pDataNode, "SelFrom", pFileDataInfo->lSelFrom);
			CDvXML::GetAttrib(pDataNode, "SelFor", pFileDataInfo->lSelFor);
			CDvXML::GetAttrib(pDataNode, "HaveSpec", pFileDataInfo->bHaveSpec);

			DVXML_NODE_LIST lstTmp;
			CDvXML::SelectNodes(pDataNode, "EveryPeriod", lstTmp);
			long lTotalCnt = lstTmp.GetCount();
			if (lTotalCnt > 0)
			{
				pFileDataInfo->lDataNum = lTotalCnt;
				pFileDataInfo->pDataInfo = new LT_DATA_INFO[lTotalCnt];
				memset(pFileDataInfo->pDataInfo, 0, sizeof(LT_DATA_INFO) * lTotalCnt);
				LT_DATA_INFO *pDataInfoTmp = pFileDataInfo->pDataInfo;

				long lTotalPeriod = 1;
				POSITION pos = lstTmp.GetHeadPosition();
				while (pos)
				{
					DVXML_NODE *pPeriodNode = lstTmp.GetNext(pos);

					pDataInfoTmp->lPeriodID = lTotalPeriod++;
					CDvXML::GetAttrib(pPeriodNode, "Period", pDataInfoTmp->szPeriod);
					CDvXML::GetAttrib(pPeriodNode, "Date", pDataInfoTmp->szDate);
					CDvXML::GetAttrib(pPeriodNode, "TotalDev", pDataInfoTmp->lTotalDev);

					DVXML_NODE *pNumberNode = CDvXML::OperateNode(pPeriodNode, "Numbers", CDvXML::CHECKONLY);
					if (pNumberNode)
					{
						long lCnt = 0;
						DVXML_NODE_LIST lstTmp1;
						CDvXML::SelectNodes(pNumberNode, "Number", lstTmp1);
						POSITION pos1 = lstTmp1.GetHeadPosition();
						while (pos1 && lCnt < LT_MAX_SELFOR)
						{
							pTmpNode = lstTmp1.GetNext(pos1);
							
							pDataInfoTmp->lData[lCnt] = 0;
							CString strVal;
							if (CDvXML::GetInnerText(pTmpNode, strVal))
								pDataInfoTmp->lData[lCnt]  = atoi(strVal);

							lCnt++;
						}
					}

					pDataInfoTmp++;
				}
			}
		}
	}

	return bRet;
}

BOOL CXmlHistData::FileInfo2Node(LT_FILE_INFO *pFileDataInfo, DVXML_NODE *pRootNode, 						
									BOOL bOnlyHeader)
{
	BOOL bRet = FALSE;

	if (pFileDataInfo && pRootNode)
	{
		bRet = TRUE;
		
		DVXML_NODE *pTmpNode = NULL;

		//1. write header info
		DVXML_NODE *pHeadNode = CDvXML::OperateNode(pRootNode, "HeaderInfo", CDvXML::DELETECREATE);
		if (pHeadNode)
		{
			CDvXML::SetAttrib(pHeadNode, "Description", pFileDataInfo->szDesp);
			CDvXML::SetAttrib(pHeadNode, "Version", pFileDataInfo->lVersion);
			CDvXML::SetAttrib(pHeadNode, "Province", pFileDataInfo->lProvince);
		}

		//2. write data info		
		DVXML_NODE *pDataNode = CDvXML::OperateNode(pRootNode, "DataInfo", CDvXML::DELETECREATE);
		if (pDataNode)
		{
			CDvXML::SetAttrib(pDataNode, "SelFrom", pFileDataInfo->lSelFrom);
			CDvXML::SetAttrib(pDataNode, "SelFor", pFileDataInfo->lSelFor);
			CDvXML::SetAttrib(pDataNode, "HaveSpec", pFileDataInfo->bHaveSpec);
			
			LT_DATA_INFO *pDataInfoTmp = pFileDataInfo->pDataInfo;
			for (long a = 0; a < pFileDataInfo->lDataNum; a++, pDataInfoTmp++)
			{
				DVXML_NODE *pPeriodNode = CDvXML::OperateNode(pDataNode, "EveryPeriod", CDvXML::CREATEONLY);

				if (pPeriodNode)
				{
					CDvXML::SetAttrib(pPeriodNode, "Period", pDataInfoTmp->szPeriod);
					CDvXML::SetAttrib(pPeriodNode, "Date", pDataInfoTmp->szDate);
					CDvXML::SetAttrib(pPeriodNode, "TotalDev", pDataInfoTmp->lTotalDev);

					DVXML_NODE *pNumberNode = CDvXML::OperateNode(pPeriodNode, "Numbers", CDvXML::CREATEONLY);
					if (pNumberNode)
					{
						for (long b = 0; b < LT_MAX_SELFOR; b++)
						{
							pTmpNode = CDvXML::OperateNode(pNumberNode, "Number", CDvXML::CREATEONLY);

							CString strVal;
							strVal.Format("%d", pDataInfoTmp->lData[b]);
							CDvXML::SetInnerText(pTmpNode, strVal);							
						}						
					}
				}
			}
		}
	}

	return bRet;
}
