#pragma once
//###############################################
//#												#
//#		�ӵ� ���ӵ� ������ ����� ��������      #
//#		��Ʈ�� Ŭ���� �Դϴ�.                   #
//#		2017-09-10								#	
//#     getposition���� ��ġ��					#
//#		getAngle�� ȸ�� ������ ���� �� �ֽ��ϴ� #
//#												#	
//#												#
//#					Made by �¿�				#
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

