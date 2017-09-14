#include "stdafx.h"
#include "cTestMain.h"



cTestMain::cTestMain()
{
}


cTestMain::~cTestMain()
{
}
HRESULT cTestMain::Setup()
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 100;
	fd.Width = 100;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "궁서체");	//글꼴 스타일
	D3DXCreateFontIndirect(GETDEVICE, &fd, &m_pFont);
	return S_OK;
}

void cTestMain::Release()
{
}

void cTestMain::Render()
{
	RECT rc2;
	rc2 = RectMake(0, 100, 100, 100);
	char str2[256];
	sprintf_s(str2, "대원이형 1번\n 동윤이 2번 \n 정현이 3번 \n 나 4번");
	m_pFont->DrawText(NULL,
		str2,
		strlen(str2),
		&rc2,
		DT_LEFT | DT_TOP | DT_NOCLIP,
		D3DCOLOR_XRGB(255, 0, 255));
}

void cTestMain::Update()
{
}


