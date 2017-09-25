#pragma once
class cUIObject : public cObject
{
protected:
	vector<cUIObject*>	m_vecChild;
	D3DXVECTOR3			m_vPosition; // 상대위치
	cUIObject*			m_pParent;
	D3DXMATRIX			m_matWorld;
	SYNTHESIZE(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(int, m_nTag, Tag);
	SYNTHESIZE(DWORD, m_dwAlpha, Alpha);

public:
	cUIObject(void);
	virtual ~cUIObject(void);

	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void SetPosition(float x, float y);
	virtual void SetSizeWidth(float width) { m_stSize.fWidth = width; }
	virtual void SetSizeHeight(float height) { m_stSize.fHeight = height; }
	virtual void GetRect(RECT* pRect);
	virtual cUIObject* GetChildByTag(int nTag);
	virtual cUIObject* GetChildByPosition(POINT ptMouse);
	virtual cUIObject* GetChildByPosition(POINT ptMouse, cUIObject* elseObject);
	virtual POINT GetPointPosition();

	virtual int GetChildSize() { return m_vecChild.size(); }
};

