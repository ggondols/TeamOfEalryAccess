#include "stdafx.h"
#include "cUIImageView.h"

cUIImageView::cUIImageView(void)
{
}

cUIImageView::~cUIImageView(void)
{
}

void cUIImageView::SetTexture(string sFullPath)
{
	m_sTexture = sFullPath;

	D3DXIMAGE_INFO stImageInfo;
	TEXTUREMANAGER->GetTextureEx(m_sTexture, &stImageInfo);
	
	m_stSize.fWidth = stImageInfo.Width;
	m_stSize.fHeight = stImageInfo.Height;
}

void cUIImageView::SetScale(float x, float y)
{
	m_matWorld._11 *= x;
	m_matWorld._22 *= y;
}

void cUIImageView::Render(LPD3DXSPRITE pSprite)
{
	D3DXIMAGE_INFO stImageInfo;
	LPDIRECT3DTEXTURE9 pTexture = TEXTUREMANAGER->GetTextureEx(m_sTexture, &stImageInfo);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.fWidth, m_stSize.fHeight);
	
	pSprite->Draw(pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(m_dwAlpha, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}