
#ifndef __DRAWRES_H
#define __DRAWRES_H

class CDrawRes
{
public:
	CDrawRes();
	~CDrawRes();

public:
	void RWCfg(BOOL bIsRead);
	void SetUp();

public:
	//Color
	enum CLR_STYLE
	{
		//stock.
		CLR_BLACK = 0,
		CLR_WHITE,
		CLR_RED,
		CLR_GREEN,
		CLR_BLUE,
		CLR_YELLOW,
		CLR_PINK,
		CLR_CYAN,
		CLR_GRAY,

		//custom
		CLR_BKGROUND, //9
		CLR_FRAME,
		CLR_CELL,

		CLR_RANDOM = 99,
	};	

	//Font
	//0~2:Chinese big middle small
	//3~5:English big middle small
	enum FONT_STYLE
	{
		FONT_CH_BIG = 0,
		FONT_CH_MIDDLE,
		FONT_CH_SMALL,

		FONT_EN_BIG,
		FONT_EN_MIDDLE,
		FONT_EN_SMALL,
	};

	//Pen
	//0~1: Frame
	//2~3: Cell
	//4~6: Gray
	enum PEN_STYLE
	{
		PEN_FRAME_BOLD = 0,	//red default
		PEN_FRAME_THIN,

		PEN_CELL_BOLD,		//blue default
		PEN_CELL_THIN,

		PEN_GRAY_BOLD,
		PEN_GRAY_THIN,
	};	
	
	//Brush
	//0:Background 1:Red 2:Blue 3:Gray
	enum BRS_STYLE
	{		
		BRS_BKGROUND,
		BRS_FRAME,
		BRS_CELL,
		BRS_GRAY,
	};	

	COLORREF GetColor(CLR_STYLE eColor);
	CFont *GetFont(FONT_STYLE eFont);
	CPen *GetPen(PEN_STYLE ePen);
	CBrush *GetBrush(BRS_STYLE eBrush);

private:
	void GetDefalultVal();
	void DeleteDrawObject();
	void CreateDrawObject();
	LOGFONT m_LogFont[6];

	COLORREF m_clrStock[12];
	CFont m_StockFont[6];
	CPen m_StockPen[6];
	CBrush m_StockBrush[5];
};

#endif