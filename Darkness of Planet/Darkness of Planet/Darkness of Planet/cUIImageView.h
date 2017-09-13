#pragma once

class cUIImageView : public cUIObject
{
private:
	string	m_sTexture;
	bool	m_isCenter;

public:
	cUIImageView(void);
	virtual ~cUIImageView(void);

	virtual void SetIsCenter(bool isCenter);
	virtual void SetTexture(string sFullPath);
	virtual void SetScale(float x, float y);

	// cUIObject override
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

