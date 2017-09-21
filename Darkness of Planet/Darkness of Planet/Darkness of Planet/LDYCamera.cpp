#include "stdafx.h"
#include "LDYCamera.h"

LDYCamera::LDYCamera()
	: m_pvTarget(NULL)
	, m_fangleY(0.0f)
	, m_fangleX(0.0f)
	, m_isLButtonDown(false)
	, m_fDistance(9.0f)
	, m_vEye(0, 0, -m_fDistance)
	, getMousePos(false)
{
}

LDYCamera::~LDYCamera()
{
}

void LDYCamera::Setup(D3DXVECTOR3 * pvTarget /*=NULL*/)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	if (pvTarget)
		m_pvTarget = pvTarget;
	else
		m_pvTarget = new D3DXVECTOR3(0, 0, 0);


	D3DXMATRIXA16 world;
	D3DXMatrixTranslation(&world, m_vEye.x, m_vEye.y, m_vEye.z);
	GETDEVICE->SetTransform(D3DTS_WORLD, &world);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		m_pvTarget,
		&D3DXVECTOR3(0, 1, 0));
	GETDEVICE->SetTransform(D3DTS_VIEW, &matView);

}

void LDYCamera::Update(D3DXVECTOR3 pvTarget)
{
	if (pvTarget)
	{
		m_vTartget.x = pvTarget.x;
		m_vTartget.y = pvTarget.y + 8.0f;
		m_vTartget.z = pvTarget.z;
	}
	else
		m_vTartget = D3DXVECTOR3(0, 0, 0);


	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		m_ptPrevMouse = g_ptMouse;
		getMousePos = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		getMousePos = false;
	}

	if (getMousePos)
	{
		float deltaY = g_ptMouse.x - m_ptPrevMouse.x;

		m_fangleY += deltaY / 50.0f;

		m_ptPrevMouse = g_ptMouse;
	}

	if (g_MouseWheel)
	{
		if (g_MouseWheel > 0)
		{
			m_fDistance -= 0.5f;
		}
		else if (g_MouseWheel < 0)
		{
			m_fDistance += 0.5f;
		}
		g_MouseWheel = 0;

		if (m_fDistance <= 6.0f)
		{
			m_fDistance = 6.0f;
		}
		else if (m_fDistance >= 12.0f)
		{
			m_fDistance = 12.0f;
		}
	}

	D3DXMATRIXA16 matRX;
	D3DXMATRIXA16 matRY;
	D3DXMatrixRotationX(&matRX, m_fangleX);
	D3DXMatrixRotationY(&matRY, m_fangleY);

	D3DXMATRIXA16 matR = matRX * matRY;

	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	D3DXVECTOR3 vTarget = D3DXVECTOR3(0, 0, 0);
	
	vTarget = m_vTartget;
	

	m_vEye = m_vEye + vTarget;


	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		&m_vTartget,
		&D3DXVECTOR3(0, 1, 0));
	GETDEVICE->SetTransform(D3DTS_VIEW, &matView);

}