#pragma once

// 생성일 : 2017/09/03
// 작성자 : 여현구
// 최종 수정일 : 2017/09/03
// 최종 수정자 : 여현구
// 기능 : 항상 캐릭터를 바라보는 폰트를 생성함

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

