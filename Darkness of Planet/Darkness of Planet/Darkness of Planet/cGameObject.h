#pragma once
#include "cAction.h"
// 생성일 : 2017/09/03
// 작성자 : 여현구
// 최종 수정일 : 2017/09/03
// 최종 수정자 : 여현구

// 기능 : 오브젝트를 상속 받아 게임 오브젝트(캐릭터, AI몬스터)등을 만드는데 사용

class cGameObject : public cObject
{
protected:
	D3DXVECTOR3	m_vPrevPosition; //지난 프레임에서의 위치 방향의 설정과 플레이어가 이동중인지 판단에 사용
	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldMatrix);
	SYNTHESIZE(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vDir, Direction);
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(bool, m_isDirection, IsDirection);

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update();
};

