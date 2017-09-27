#pragma once

class cUITextView : public cUIObject
{
protected:
	SYNTHESIZE(string, m_sText, Text);
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(DWORD, m_dwDT, DrawTextFormat);
	SYNTHESIZE(D3DCOLOR, m_dwColor, Color);

	float*	m_pfTime;

public:
	cUITextView(void);
	virtual ~cUITextView(void);

	virtual void SetTimeAddress(float* pTime) { m_pfTime = pTime; }

	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

