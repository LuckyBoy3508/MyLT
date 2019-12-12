
#ifndef _LTDEF_H
#define _LTDEF_H

#include "Dvlib\DvInc.h"

//1. Global const value define.

//Register info: define this is for register version.
#define LT_REGISTERED_VERSION	2005051201

//Production info.
#define LT_PRODUCTION_NAME		"蓝精灵－国内乐透型彩票通用版"
#define LT_MAINPROG_NAME		"MyLT.exe"
#define LT_PRODUTTION_VER		320

//App's Direction & Files
#define LT_HISTDATA_DIR			"HistData"
#define LT_TEXTDATA_DIR			"TextData"
#define LT_USERDATA_DIR			"UserData"
#define LT_LOGFILE_DIR			"Log"
#define LT_CFGFILE_NAME			"CfgFile.xml"

//net relate
#define LT_DOWNLOAD_DIR			"DownLoad"
#define LT_FILELIST_DIR			"download/production/mylt/histdata"
#define LT_FILELIST_NAME		"FileList.xml"
#define LT_NET_HOSTADDR			"http://dfliu.go.nease.net"
//#define LT_NET_HOSTADDR			"http://2kapgroup15"
//#define LT_NET_HOSTADDR			"localhost"
#define LT_NET_SOFTWARE			"http://dfliu.go.nease.net/download/production/mylt/allkindver/v3.11/MyLT.rar"
#define LT_NET_PROD_REG			"http://www.softreg.com.cn/shareware_view.asp?id=/FD192653-E67D-47AE-B738-4D051E20436E/"

//Data info.
#define LT_MAX_FILENAMELEN		DV_LEN_SMALL
#define LT_MAX_DESCRIPTIONLEN	DV_LEN_MIDDLE
#define LT_PERIOD_LEN			DV_LEN_SMALL
#define LT_DATE_LEN				DV_LEN_SMALL

#define LT_MAX_BONUS			8

#define LT_MIN_SELFROM			15
#define LT_MAX_SELFROM			37
#define LT_MIN_SELFOR			5
#define LT_MAX_SELFOR			8
#define LT_DEF_SELFROM			35
#define LT_DEF_SELFOR			8
#define LT_DEF_HAVESPECNO		TRUE

//2. Global data struct define.
struct LT_DATA_INFO
{
	long lPeriodID;
	char szPeriod[LT_PERIOD_LEN + 1];
	char szDate[LT_DATE_LEN + 1];
	long lTotalDev;
	long lData[LT_MAX_SELFOR];
};

struct LT_FILE_INFO
{	
	//header info
	char szName[LT_MAX_FILENAMELEN + 1];
	char szDesp[LT_MAX_DESCRIPTIONLEN + 1];
	long lVersion;
	long lProvince;

	//data info
	LT_DATA_INFO *pDataInfo;
	long lDataNum;
	long lSelFrom;
	long lSelFor;
	BOOL bHaveSpec;
};
typedef CList<LT_FILE_INFO *, LT_FILE_INFO *> FILELIST;

struct LT_SYSCFG_INFO
{
	BOOL bIsAutoLoad;
	char szFileName[LT_MAX_FILENAMELEN  +1];
	long lDefProvince;
};

#endif