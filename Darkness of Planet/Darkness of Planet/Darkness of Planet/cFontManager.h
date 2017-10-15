#pragma once
#include "cSingletonBase.h"

class cFontManager : public cSingletonBase<cFontManager>
{
public:
	enum eFontType
	{
		E_NORMAL,
		E_QUEST,
		E_TEI,
		E_CONSOLE,
		E_LODING,
		E_END,
		E_ENDING,
		E_INFO
	};

private:
	map<eFontType, LPD3DXFONT> m_mapFont;

public:
	cFontManager();
	~cFontManager();

	LPD3DXFONT GetFont(eFontType e);
	void Destroy();
};

