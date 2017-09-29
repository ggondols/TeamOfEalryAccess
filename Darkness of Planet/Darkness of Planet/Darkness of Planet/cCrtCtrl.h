#pragma once

class iMap;

class cCrtCtrl
{
	D3DXVECTOR3				m_vDir;
	D3DXVECTOR3				m_vPos;
	float					m_fSpeed;
	float					m_fAngle;
	float					m_fRun;

protected:
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);

public:
	cCrtCtrl(void);
	~cCrtCtrl(void);

	
	void Update(iMap* pMap = NULL);
	D3DXVECTOR3* GetPosition();
	void UpdateByDir(D3DXVECTOR3 & pDir);
	void setPosY(float y) { m_vPos.y = y; }
	void setSpeed(float speed) { m_fSpeed = speed; }
	void setRunSpped(float fRun) { m_fRun = fRun; }
};

