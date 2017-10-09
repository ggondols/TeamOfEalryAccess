#include "stdafx.h"
#include "TeicPhysicsCrtCtrl.h"


TeicPhysicsCrtCtrl::TeicPhysicsCrtCtrl()
	: m_vDir(0, 0, 1)
	, m_vPos(20, 0, -20)
	, m_fSpeed(0.0f)
	, m_fAngle(0.0f)
	, m_bMoving(false)
	, m_fAcceleration(0.0f)
	, m_bAttacking(false)
	, m_fSpeedSetting(1.0f)
	, m_bRunning(false)
	, m_bgetMousePos(false)
	, m_fangleX(0.0f)
	, m_eDir(Up)

{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotation);
}


TeicPhysicsCrtCtrl::~TeicPhysicsCrtCtrl()
{
}

void TeicPhysicsCrtCtrl::Update(float angle)
{
	switch (m_eDir)
	{
	case Up:
		m_fAngle = angle - D3DX_PI / 2;
		break;
	case Left:
		m_fAngle = angle - D3DX_PI;
		break;
	case Right:
		m_fAngle = angle;
		break;
	default:
		break;
	}
	


	//if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	//{
	//	m_bAttacking = true;
	//	m_fSpeed = 0;
	//}
	//if (KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
	//{
	//	m_bAttacking = false;
	//	
	//}

	if (KEYMANAGER->isOnceKeyDown(VK_LSHIFT))
	{
		m_bRunning = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LSHIFT))
	{
		m_bRunning = false;
	}


	if (!m_bAttacking)
	{
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			m_fAcceleration += 0.001f*m_fSpeedSetting;
			m_fAngle = angle - D3DX_PI / 2;
			m_eDir = Up;
			m_bMoving = true;
		}
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			m_fAcceleration -= 0.001f*m_fSpeedSetting;
			m_fAngle = angle - D3DX_PI / 2;
			m_bMoving = true;
		}
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			m_fAcceleration += 0.001f*m_fSpeedSetting;
			m_eDir = Left;
			m_fAngle = angle - D3DX_PI;
			m_bMoving = true;
		}

		if (KEYMANAGER->isStayKeyDown('D'))
		{
			m_fAcceleration += 0.001f*m_fSpeedSetting;
			m_eDir = Right;
			m_fAngle = angle;
			m_bMoving = true;
		}
	}


	if (m_bRunning&&m_bMoving)
	{
		m_fSpeedSetting = 5.0f;
	}
	else if(!m_bRunning&&m_bMoving)
	{
		m_fSpeedSetting = 2.0f;
	}

	
	m_fAcceleration -= 0.00025f*m_fSpeedSetting;
	
	if (m_fAcceleration > 0.001*m_fSpeedSetting) m_fAcceleration = 0.001f*m_fSpeedSetting;
	if (m_fAcceleration < -0.001*m_fSpeedSetting) m_fAcceleration = -0.001f*m_fSpeedSetting;
	m_fSpeed += m_fAcceleration;
	if (m_fSpeed > 0.1*m_fSpeedSetting)m_fSpeed = 0.1*m_fSpeedSetting;
	if (m_fSpeed < -0.1*m_fSpeedSetting)m_fSpeed = -0.1*m_fSpeedSetting;


	if (m_fSpeed < 0)
	{
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			/*if(!m_bAttacking)
				m_fSpeed = -0.1*0.5;*/
		}
		else
		{
			m_fSpeed = 0;
			m_bMoving = false;
			m_fAcceleration = 0;
			m_eDir = Up;
		}
		
	}



	m_vPos.x += cosf(m_fAngle) * m_fSpeed;
	m_vPos.z -= sinf(m_fAngle) * m_fSpeed;



	m_vDir = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&m_matRotation, m_fangleX);
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);


	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matR * matT;

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