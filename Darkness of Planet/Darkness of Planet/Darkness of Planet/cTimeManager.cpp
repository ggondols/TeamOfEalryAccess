#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
{
}


cTimeManager::~cTimeManager()
{

}

HRESULT cTimeManager::Setup(void)
{
	_timer = new cTimer;
	_timer->Setup();

	D3DXFONT_DESC desc;
#pragma warning(disable : 4996)
	ZeroMemory(&desc, sizeof(desc));
	desc.CharSet = DEFAULT_CHARSET;
	strcpy(desc.FaceName, "Consolas");
	desc.Height = 20;
	desc.Width = 10;
	desc.Weight = FW_BOLD;
	desc.Quality = DEFAULT_QUALITY;
	desc.MipLevels = 1;
	desc.Italic = 0;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.PitchAndFamily = FF_DONTCARE;

	D3DXCreateFontIndirect(GETDEVICE, &desc, &m_pFont);


	

	return S_OK;


}