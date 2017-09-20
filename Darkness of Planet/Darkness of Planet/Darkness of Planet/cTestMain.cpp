#include "stdafx.h"
#include "cTestMain.h"



cTestMain::cTestMain()
	:m_bCursor(false)
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

	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursor.png", &m_cursortex);
	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursorHit.png", &m_cursortex2);
	m_cursortex->GetSurfaceLevel(0, &surfcursor);
	GETDEVICE->SetCursorProperties(0, 0, surfcursor);

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
	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		if (g_bCursor)g_bCursor = false;
		else if (!g_bCursor)g_bCursor = true;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD1))
	{
		m_cursortex2->GetSurfaceLevel(0, &surfcursor);
		GETDEVICE->SetCursorProperties(0, 0, surfcursor);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD2))
	{
		m_cursortex->GetSurfaceLevel(0, &surfcursor);
		GETDEVICE->SetCursorProperties(0, 0, surfcursor);
	}
}


IDirect3DSurface9 * cTestMain::LoadBMP(std::string filename, IDirect3DSurface9 * surface)
{
	D3DXIMAGE_INFO imageinfo;
	HRESULT hResult = D3DXGetImageInfoFromFile(filename.c_str(), &imageinfo);

	hResult = GETDEVICE->CreateOffscreenPlainSurface(imageinfo.Width,
		imageinfo.Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename.c_str(),
		NULL, D3DX_DEFAULT, 0, NULL);

	return surface;

}
