#include "stdafx.h"
#include "TeicCollisionMove.h"
#include "TeicSkinnedMesh.h"



TeicCollisionMove::TeicCollisionMove()
{
	
	m_pSkinnedTarget = NULL;


	m_From = D3DXVECTOR3(0, 0, 0);
	m_To = D3DXVECTOR3(0, 0, 0);
	m_previous = D3DXVECTOR3(0, 0, 0);
	m_present = D3DXVECTOR3(0, 0, 0);

	m_bStart = false;


	m_fTotalDistance = 0.0f;
	m_fElapsedDistance = 0.0f;
	m_fNowDistance = 0.0f;
	m_Speed = 1.0;

	m_Callback = NULL;
	m_fRotationAngle = 0.0f;
}


TeicCollisionMove::~TeicCollisionMove()
{

	m_pSkinnedTarget = NULL;
	
}


void TeicCollisionMove::SetCallback(CallbackBindFunction func)
{
	m_Callback = std::move(func);
}




void TeicCollisionMove::CalRotation()
{
	
	/*D3DXVECTOR3 vDirection = m_previous - m_present;

	if (D3DXVec3LengthSq(&vDirection) <= 0.00001)
		return;

	D3DXVec3Normalize(&vDirection, &vDirection);

	D3DXMATRIX matR;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);
	D3DXMATRIX	skinnedRot;
	D3DXMatrixRotationY(&skinnedRot, D3DX_PI / 2);
	matR = matR* skinnedRot;
	if (m_pSkinnedTarget)
	{
		m_pSkinnedTarget->SetRotationMatrix(matR);
	}*/

	D3DXVECTOR3 vDirection = m_present -m_previous;
	if (D3DXVec3LengthSq(&vDirection) <= 0.00001)
		return;
	D3DXVec3Normalize(&vDirection, &vDirection);

	D3DXVECTOR3 base = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross,&base, &vDirection);
	float targetangle =acosf( D3DXVec3Dot(&base, &vDirection));

	if (Cross.y < 0) targetangle = D3DX_PI*2 - targetangle;

	

	if (targetangle > m_fRotationAngle)
	{
		if (targetangle - m_fRotationAngle > D3DX_PI)
		{
			m_fRotationAngle -= 0.05;
			if (m_fRotationAngle < 0)
			{
				m_fRotationAngle = D3DX_PI * 2 - m_fRotationAngle;
			}
		}
		else
		{
			m_fRotationAngle += 0.05;
			if (m_fRotationAngle > targetangle) m_fRotationAngle = targetangle;
		}
		
	}
	else
	{
		if (m_fRotationAngle - targetangle > D3DX_PI)
		{
			m_fRotationAngle += 0.05;
			if (m_fRotationAngle >D3DX_PI*2 )
			{
				m_fRotationAngle = m_fRotationAngle -D3DX_PI * 2  ;
			}
		}
		else
		{
			m_fRotationAngle -= 0.05;
			if (m_fRotationAngle < targetangle) m_fRotationAngle = targetangle;
		}
	}
	
	
	
	if (m_pSkinnedTarget)
	{
		m_pSkinnedTarget->SetRotationAngle(m_fRotationAngle);
	}
}

void TeicCollisionMove::Start()
{
	m_bStart = true;

	m_fTotalDistance = D3DXVec3Length(&(m_To - m_From));
	m_fElapsedDistance = m_Speed / 60.0f;
	m_fNowDistance = 0.0f;
	
}

void TeicCollisionMove::Stop()
{
	m_bStart = false;
}

void TeicCollisionMove::SetFrom(D3DXVECTOR3 from)
{
	m_From = from;
	m_From.y = 0;
	m_previous = m_From;
}

void TeicCollisionMove::SetTo(D3DXVECTOR3 to)
{
	m_To = to;
	m_To.y = 0;
}


void TeicCollisionMove::SetFrom2(D3DXVECTOR3 from)
{
	m_From = from;
	
	m_previous = m_From;
}

void TeicCollisionMove::SetTo2(D3DXVECTOR3 to)
{
	m_To = to;

}


void TeicCollisionMove::Update()
{
	if (!m_bStart)return;
	if (m_pSkinnedTarget)
	{
		if (m_pSkinnedTarget->m_bCollision)return;
	}
	
	if (TIMEMANAGER->getFrame() < 40)return;

	m_fElapsedDistance = m_Speed / TIMEMANAGER->getFrame();
	m_fNowDistance += m_fElapsedDistance;
	float t = m_fNowDistance / m_fTotalDistance;
	D3DXVec3Lerp(&m_present, &m_From, &m_To, t);
	
	


	
	if (t >= 1)
	{
		m_present = m_To;
		m_bStart = false;
		if (m_pSkinnedTarget)
		{
			m_pSkinnedTarget->SetPosition(m_present);
		}
		CalRotation();
		if (m_Callback)
		{
			m_Callback();
		}
		return;

	}
	if (m_pSkinnedTarget)
	{
		m_pSkinnedTarget->SetPosition(m_present);
	}
	CalRotation();
	m_previous = m_present;
}
