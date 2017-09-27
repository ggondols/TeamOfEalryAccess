#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView(void)
	: m_eFontType(cFontManager::E_NORMAL)
	, m_dwDT(DT_LEFT | DT_TOP | DT_NOCLIP)
	, m_dwColor(D3DCOLOR_XRGB(255, 255, 255))
	, m_pfTime(NULL)
{
}

cUITextView::~cUITextView(void)
{
}

void cUITextView::Update()
{
	if (m_pfTime)
	{
		char str[1024];
		int nTime = (int)(*m_pfTime * 1000.0f);
		int nMinute = nTime % 60;
		int nHour = (nTime / 60) % 12;
		sprintf_s(str, "%d : %d", nHour, nMinute);
		m_sText = str;
	}

	cUIObject::Update();
}

void cUITextView::Render(LPD3DXSPRITE pSprite)
{
	RECT rc;
	rc.left = m_matWorld._41;
	rc.top = m_matWorld._42;
	rc.right = m_matWorld._41 + m_stSize.fWidth;
	rc.bottom = m_matWorld._42 + m_stSize.fHeight;

	LPD3DXFONT pFont = FONTMANAGER->GetFont(m_eFontType);
	pFont->DrawTextA(NULL,
		m_sText.c_str(),
		m_sText.length(),
		&rc,
		m_dwDT,
		m_dwColor);
}
