
#include "stdafx.h"
#include "DvMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDvMemDC

CDvMemDC::CDvMemDC()
{
	m_pOldBitmap=NULL;
}

CDvMemDC::~CDvMemDC()
{
	if(GetSafeHdc())
	{
		SelectObject(m_pOldBitmap);
		DeleteDC();
	}
}

BOOL CDvMemDC::CreateMemDC(CDC *pDC, COLORREF clrBk /*= RGB(0, 0, 0)*/, 
							int nWidth /*= 0*/, int nHeight /*= 0*/)
{
	BOOL bRet = FALSE;

	if(!GetSafeHdc())
	{
		if(nWidth <= 0) 
			nWidth = ::GetSystemMetrics(SM_CXSCREEN);

		if(nHeight <= 0) 
			nHeight = ::GetSystemMetrics(SM_CYSCREEN);

		if(m_theBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight) && 
			CreateCompatibleDC(pDC))		   
		{
			m_pOldBitmap = SelectObject(&m_theBitmap);
			m_sizeBitmap.cx = nWidth;
			m_sizeBitmap.cy = nHeight;
			m_colorBk = clrBk;

			ClearBkGround();

			bRet = TRUE;
		}	
	}

	return bRet;
}

void CDvMemDC::ClearBkGround()
{
	if(GetSafeHdc())
		FillSolidRect(0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy, m_colorBk);
}
