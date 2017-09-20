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
	strcpy_s(fd.FaceName, "�ü�ü");	//�۲� ��Ÿ��
	D3DXCreateFontIndirect(GETDEVICE, &fd, &m_pFont);


	////////////////////////////////////////
	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursor.png", &m_cursortex);

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
	sprintf_s(str2, "������� 1��\n ������ 2�� \n ������ 3�� \n �� 4��");
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
		if (m_bCursor)m_bCursor = false;
		else if (!m_bCursor)m_bCursor = true;
	}
	if (m_bCursor)
	{
		surfcursor = LoadBMP("UI/cursor/cursor.png", surfcursor);
		m_cursortex->GetSurfaceLevel(0, &surfcursor);
		GETDEVICE->SetCursorProperties(0, 0, surfcursor);
	}
	else if(!m_bCursor)
	{
		if (surfcursor);
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
