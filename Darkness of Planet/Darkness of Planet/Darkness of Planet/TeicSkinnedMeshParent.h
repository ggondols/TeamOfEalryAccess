#pragma once
class TeicSkinnedMeshParent
{
public:
	bool	m_bSlotOn;
	bool	m_bHit;
	bool	m_bSlow;
	float	m_fSlowTime;
	int m_iHp;
	int m_iAttack;
	vector<ST_PN_VERTEX>	m_vecVertex;
	
public:
	TeicSkinnedMeshParent();
	~TeicSkinnedMeshParent();
};

