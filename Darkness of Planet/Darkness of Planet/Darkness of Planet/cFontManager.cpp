#include "stdafx.h"
#include "cFontManager.h"

cFontManager::cFontManager(void)
{
}

cFontManager::~cFontManager(void)
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	if (m_mapFont.find(e) == m_mapFont.end())
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

		if (e == E_NORMAL)
		{
			fd.Height = 24;
			fd.Width = 12;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "궁서체");	//글꼴 스타일
		}
		else if (e == E_QUEST)
		{
			fd.Height = 50;
			fd.Width = 25;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "궁서체");	//글꼴 스타일
		}
		else if (e == E_TEI)
		{
			fd.Height = 5;
			fd.Width = 5;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "궁서체");	//글꼴 스타일
		}
		else if (e == E_CONSOLE)
		{
			fd.Height = 15;
			fd.Width = 7;
			fd.Weight = FW_BOLD;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "나눔고딕");	//글꼴 스타일
		}
		else if (e == E_LODING)
		{
			fd.Height = 32;
			fd.Width = 20;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "Algerian");	//글꼴 스타일
			
			//Algerian
			//Book Antiqua
			//Bradley Hand ITC
			//Castellar
			//Comic Sans MS
			//Copperplate Gothic Light
			//Forte
			//OCR A Extended
			//Rockwell
		}
		else if (e == E_ENDING)
		{
			fd.Height = 100;
			fd.Width = 100;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "Algerian");	//글꼴 스타일

												//Algerian
												//Book Antiqua
												//Bradley Hand ITC
												//Castellar
												//Comic Sans MS
												//Copperplate Gothic Light
												//Forte
												//OCR A Extended
												//Rockwell
		}
		else if (e == E_INFO)
		{
			fd.Height = 15;
			fd.Width = 20;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "Algerian");	//글꼴 스타일

												//Algerian
												//Book Antiqua
												//Bradley Hand ITC
												//Castellar
												//Comic Sans MS
												//Copperplate Gothic Light
												//Forte
												//OCR A Extended
												//Rockwell
		}


		
		D3DXCreateFontIndirect(GETDEVICE, &fd, &m_mapFont[e]);
	}

	return m_mapFont[e];
}

void cFontManager::Destroy()
{
	for each(auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}
}
