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

enum dir
{
	Up,
	Left,
	Right,
};
class TeicPhysicsCrtCtrl
{
private:
	dir						m_eDir;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vFuturePos;
	D3DXVECTOR3				m_vPastPos;
	
	float					m_fAcceleration;
	float					m_fAngle;
	bool					m_bMoving;

	bool					m_bRunning;
	D3DXMATRIX				m_matRotation;
	POINT					m_ptPrevMouse;
	bool					m_isLButtonDown;
	bool					m_bgetMousePos;
	float					m_fangleX;
	float					m_fangleY;

	
public:
	bool					m_bAttacking;
	float					m_fSpeed;
protected:
	SYNTHESIZE_PASS_BY_REF(float, m_fSpeedSetting, Speed);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);
	D3DXVECTOR3				m_vDir;
public:


	void Update(float angle);

	D3DXVECTOR3* GetPosition();
	D3DXVECTOR3* GetPositionFuture();
	D3DXVECTOR3* GetPositionPast();
	bool getMoving() { return m_bMoving; };
	bool getAttacking() { return m_bAttacking; }
	void setAttacking(bool on) { m_bAttacking = on; }
	void setMoving(bool on) { m_bMoving = on; }
	void SetPosition(D3DXVECTOR3 p) { m_vPos = p; }
	float getAngle() { return m_fAngle; }
	D3DXMATRIX getCrtCtrlMatrix() { return m_matWorld; }



public:
	TeicPhysicsCrtCtrl();
	~TeicPhysicsCrtCtrl();
};

