
#ifndef __DVMEMDC_H
#define __DVMEMDC_H

#include "DvDef.h"

class AFX_CLASS_EXPORT CDvMemDC : public CDC  
{
public:
	CDvMemDC();
	~CDvMemDC();

public:
	BOOL CreateMemDC(CDC *pDC, COLORREF clrBk = RGB(0, 0, 0), int nWidth = 0, int nHeight = 0);
	void ClearBkGround();	
	CSize m_sizeBitmap;

private:	
	CBitmap *m_pOldBitmap;
	CBitmap m_theBitmap;
	COLORREF m_colorBk;
};

#endif
