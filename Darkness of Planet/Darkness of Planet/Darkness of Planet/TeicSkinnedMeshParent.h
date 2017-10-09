#pragma once
class TeicSkinnedMeshParent
{
public:
	bool	m_bSlotOn;
	bool	m_bHit;
	bool	m_bSlow;
	bool	m_bFire;
	float	m_fFireTime;
	float	m_fSlowTime;
	int m_iHp;
	int m_iAttack;
	vector<ST_PN_VERTEX>	m_vecVertex;
	
public:
	TeicSkinnedMeshParent();
	~TeicSkinnedMeshParent();
};

