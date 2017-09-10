#pragma once
//###############################################
//#												#
//#		속도 가속도 개념이 적용된 물리적인      #
//#		컨트롤 클래스 입니다.                   #
//#		2017-09-10								#	
//#     getposition으로 위치를					#
//#		getAngle로 회전 각도를 얻을 수 있습니다 #
//#												#	
//#												#
//#					Made by 태영				#
//###############################################
class TeicPhysicsCrtCtrl
{
private:

	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vFuturePos;
	D3DXVECTOR3				m_vPastPos;
	float					m_fSpeed;
	float					m_fAcceleration;
	float					m_fAngle;
	bool					m_bMoving;
	bool					m_bAttacking;
public:

protected:
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);
	D3DXVECTOR3				m_vDir;
public:


	void Update();

	D3DXVECTOR3* GetPosition();
	D3DXVECTOR3* GetPositionFuture();
	D3DXVECTOR3* GetPositionPast();
	bool getMoving() { return m_bMoving; };
	bool getAttacking() { return m_bAttacking; }
	void setAttacking(bool on) { m_bAttacking = on; }
	void setMoving(bool on) { m_bMoving = on; }
	float getAngle() { return m_fAngle; }
public:
	TeicPhysicsCrtCtrl();
	~TeicPhysicsCrtCtrl();
};

