#pragma once
#include "cAction.h"
class cGameObject : public cObject
{
protected:
	D3DXVECTOR3	m_vPrevPosition;
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

