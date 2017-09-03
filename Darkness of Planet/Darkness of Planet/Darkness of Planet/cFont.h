#pragma once
class cFont
{
private:
	D3DXMATRIX m_matWorld;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3* m_pTarget;
	float m_fStartTime;
public:
	cFont();
	~cFont();

	void Setup(D3DXVECTOR3* vTarget, D3DXVECTOR3 vPos);
	void Update();
	void Render();
};

