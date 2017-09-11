#include "stdafx.h"
#include "TeicCollisionMove.h"
#include "TeicSkinnedMesh.h"



TeicCollisionMove::TeicCollisionMove()
{
	m_fPassedActionTime = 0.0f;
	m_fActionTime = 0.0f;
	

	m_pTarget = NULL;
	m_pDelegate = NULL;
	m_pSkinnedTarget = NULL;


	m_From = D3DXVECTOR3(0, 0, 0);
	m_To = D3DXVECTOR3(0, 0, 0);
	m_previous = D3DXVECTOR3(0, 0, 0);
	m_present = D3DXVECTOR3(0, 0, 0);

	m_Start = false;

	m_fTotalDistance = 0.0f;
	m_fElapsedDistance = 0.0f;
	m_fNowDistance = 0.0f;
	m_Speed = 1.0;

	m_Callback = NULL;
}


TeicCollisionMove::~TeicCollisionMove()
{
	SAFE_DELETE(m_pTarget);
	SAFE_DELETE(m_pDelegate);
	SAFE_DELETE(m_pSkinnedTarget);
	
}


void TeicCollisionMove::SetCallback(CallbackBindFunction func)
{
	m_Callback = std::move(func);
}




void TeicCollisionMove::CalRotation()
{

	D3DXVECTOR3 vDirection = m_previous - m_present;

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
	}
	
}

void TeicCollisionMove::Start()
{
	m_Start = true;

	m_fTotalDistance = D3DXVec3Length(&(m_To - m_From));
	m_fElapsedDistance = m_Speed / 60.0f;
	m_fNowDistance = 0.0f;
	
}

void TeicCollisionMove::Stop()
{
	m_Start = false;
}

void TeicCollisionMove::Update()
{
	if (!m_Start)return;
	if (TIMEMANAGER->getFrame() < 50)return;

	m_fElapsedDistance = m_Speed / TIMEMANAGER->getFrame();
	m_fNowDistance += m_fElapsedDistance;
	float t = m_fNowDistance / m_fTotalDistance;
	D3DXVec3Lerp(&m_present, &m_From, &m_To, t);
	
	


	
	if (t >= 1)
	{
		m_present = m_To;
		m_Start = false;
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
