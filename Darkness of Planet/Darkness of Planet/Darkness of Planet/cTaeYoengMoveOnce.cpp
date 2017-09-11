#include "stdafx.h"
#include "cTaeYoengMoveOnce.h"


cTaeYoengMoveOnce::cTaeYoengMoveOnce()
{
	m_From = D3DXVECTOR3(0, 0, 0);
	m_To = D3DXVECTOR3(0, 0, 0);
	m_previous = D3DXVECTOR3(0, 0, 0);
	m_present = D3DXVECTOR3(0, 0, 0);

	m_fTime = 0.0f;
	m_Speed = 0.0f;
	m_Start = false;
	m_fStarttime = 0.0f;
	m_fDistance = 0.0f;

	m_Callback = NULL;

	m_fPresenttime = 0.0f;
	D3DXMatrixIdentity(&m_RotationMat);
}


cTaeYoengMoveOnce::~cTaeYoengMoveOnce()
{
}


void cTaeYoengMoveOnce::SetTo(D3DXVECTOR3 to)
{
	m_To = to; 
	m_To.y = 0;
}

void cTaeYoengMoveOnce::SetFrom(D3DXVECTOR3 from)
{
	 m_From = from;
	 m_From.y = 0;
}

void cTaeYoengMoveOnce::SetCallback(CallbackBindFunction func)
{
	m_Callback = std::move(func);
}

void cTaeYoengMoveOnce::CalRotation()
{

	D3DXVECTOR3 vDirection = m_previous - m_present;

	if (D3DXVec3LengthSq(&vDirection) <= D3DX_16F_EPSILON)
		return;

	D3DXVec3Normalize(&vDirection, &vDirection);

	D3DXMATRIX matR;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);


	m_RotationMat = matR;
}

void cTaeYoengMoveOnce::Start()
{
	m_Start = true;
	m_fStarttime = TIMEMANAGER->getWorldTime();
	m_fPresenttime = TIMEMANAGER->getWorldTime();
	m_fDistance = D3DXVec3Length(&(m_To - m_From));

	m_fTime = m_fDistance / m_Speed;
	TIMEMANAGER->getElapsedTime();
}

void cTaeYoengMoveOnce::Stop()
{
	m_Start = false;
}

void cTaeYoengMoveOnce::Update()
{
	if (!m_Start)return;




	m_fPresenttime = TIMEMANAGER->getWorldTime();
	float t = (m_fPresenttime - m_fStarttime) / m_fTime;
	D3DXVec3Lerp(&m_present, &m_From, &m_To, t);




	if (t >= 1)
	{
		m_present = m_To;
		m_Start = false;
		if (m_Callback)
		{
			m_Callback();
		}

	}
	CalRotation();
	m_previous = m_present;
}
