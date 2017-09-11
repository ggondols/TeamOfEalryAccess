#include "stdafx.h"
#include "TeicCollisionMove.h"




void TeicCollisionMove::Start()
{
}

void TeicCollisionMove::Update()
{
}

TeicCollisionMove::TeicCollisionMove()
{
	m_fPassedActionTime = 0.0f;
	m_fActionTime = 0.0f;
	m_fStarttime = 0.0f;

	m_pTarget = NULL;
	m_pDelegate = NULL;
	m_pSkinnedTarget = NULL;


	m_From = D3DXVECTOR3(0, 0, 0);
	m_To = D3DXVECTOR3(0, 0, 0);
	m_previous = D3DXVECTOR3(0, 0, 0);
	m_present = D3DXVECTOR3(0, 0, 0);

	m_Start = false;

	m_fDistance = 0.0f;
	m_Speed = 0.1f;

	m_Callback = NULL;
}


TeicCollisionMove::~TeicCollisionMove()
{
	SAFE_DELETE(m_pTarget);
	SAFE_DELETE(m_pDelegate);
	SAFE_DELETE(m_pSkinnedTarget);
	
}

//
//void TeicCollisionMove::SetCallback(CallbackBindFunction func)
//{
//	m_Callback = std::move(func);
//}
//
//
//
//
//void TeicCollisionMove::CalRotation()
//{
//
//	D3DXVECTOR3 vDirection = m_previous - m_present;
//
//	if (D3DXVec3LengthSq(&vDirection) <= D3DX_16F_EPSILON)
//		return;
//
//	D3DXVec3Normalize(&vDirection, &vDirection);
//
//	D3DXMATRIX matR;
//	D3DXMatrixLookAtLH(&matR,
//		&D3DXVECTOR3(0, 0, 0),
//		&vDirection,
//		&D3DXVECTOR3(0, 1, 0));
//	D3DXMatrixTranspose(&matR, &matR);
//
//
//	m_RotationMat = matR;
//}
//
//void TeicCollisionMove::Start()
//{
//	m_Start = true;
//	m_fStarttime = TIMEMANAGER->getWorldTime();
//	m_fPresenttime = TIMEMANAGER->getWorldTime();
//	m_fDistance = D3DXVec3Length(&(m_To - m_From));
//
//	m_fTime = m_fDistance / m_Speed;
//	TIMEMANAGER->getElapsedTime();
//}
//
//void TeicCollisionMove::Stop()
//{
//	m_Start = false;
//}
//
//void TeicCollisionMove::Update()
//{
//	if (!m_Start)return;
//
//
//
//
//	m_fPresenttime = TIMEMANAGER->getWorldTime();
//	float t = (m_fPresenttime - m_fStarttime) / m_fTime;
//	D3DXVec3Lerp(&m_present, &m_From, &m_To, t);
//
//
//
//
//	if (t >= 1)
//	{
//		m_present = m_To;
//		m_Start = false;
//		if (m_Callback)
//		{
//			m_Callback();
//		}
//
//	}
//	CalRotation();
//	m_previous = m_present;
//}
