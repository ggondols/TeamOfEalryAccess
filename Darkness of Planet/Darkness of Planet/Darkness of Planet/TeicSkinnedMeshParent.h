#pragma once
class TeicSkinnedMeshParent
{
public:
	bool	m_bSlotOn;
	bool	m_bHit;
	int m_iHp;
	int m_iAttack;
	vector<ST_PN_VERTEX>	m_vecVertex;
	
public:
	TeicSkinnedMeshParent();
	~TeicSkinnedMeshParent();
};

