#include "stdafx.h"
#include "TeicPhysicsCrtCtrl.h"


TeicPhysicsCrtCtrl::TeicPhysicsCrtCtrl()
	: m_vDir(0, 0, 1)
	, m_vPos(0, 0, 0)
	, m_fSpeed(0.0f)
	, m_fAngle(0.0f)
	, m_bMoving(false)
	, m_fAcceleration(0.0f)
	, m_bAttacking(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}


TeicPhysicsCrtCtrl::~TeicPhysicsCrtCtrl()
{
}

void TeicPhysicsCrtCtrl::Update()
{

	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		m_bAttacking = true;
		m_fSpeed = 0;
	}

	if (!m_bAttacking)
	{
		if (KEYMANAGER->isStayKeyDown('W'))
		{

			m_fAcceleration += 0.001f;
			m_bMoving = true;


		}
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			m_fAcceleration -= 0.001f;
			//m_vPos = m_vPos + m_vDir * m_fSpeed;

		}
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			m_fAngle -= 0.1f;


		}

		if (KEYMANAGER->isStayKeyDown('D'))
		{
			m_fAngle += 0.1f;


		}
	}


	m_fAcceleration -= 0.00025f;
	if (m_fAcceleration > 0.001) m_fAcceleration = 0.001f;
	m_fSpeed += m_fAcceleration;
	if (m_fSpeed > 0.1)m_fSpeed = 0.1;
	if (m_fSpeed < 0)
	{
		m_fSpeed = 0;
		m_bMoving = false;
		m_fAcceleration = 0;
	}
	//m_vPos = m_vPos + m_vDir * m_fSpeed;
	m_vPos.x += cosf(m_fAngle) * m_fSpeed;
	m_vPos.z -= sinf(m_fAngle) * m_fSpeed;


	m_vDir = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);


	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);





	m_matWorld =/* matR **/ matT;

}


D3DXVECTOR3* TeicPhysicsCrtCtrl::GetPosition()
{
	return &m_vPos;
}

D3DXVECTOR3 * TeicPhysicsCrtCtrl::GetPositionFuture()
{
	return &m_vFuturePos;
}

D3DXVECTOR3 * TeicPhysicsCrtCtrl::GetPositionPast()
{
	return &m_vPastPos;
}
