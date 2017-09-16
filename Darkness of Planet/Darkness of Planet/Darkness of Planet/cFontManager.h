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
		E_END,
	};

private:
	map<eFontType, LPD3DXFONT> m_mapFont;

public:
	cFontManager();
	~cFontManager();

	LPD3DXFONT GetFont(eFontType e);
	void Destroy();
};

