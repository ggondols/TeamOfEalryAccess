#include "stdafx.h"
#include "LDYCamera.h"

LDYCamera::LDYCamera()
	: m_pvTarget(NULL)
	, m_fangleY(0.0f)
	, m_fangleX(0.0f)
	, m_isLButtonDown(false)
	, m_fDistance(8.0f)
	, m_vEye(0, 0, -m_fDistance)
	, getMousePos(false)
	, m_fBoundingX(0)
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

void LDYCamera::Update(D3DXVECTOR3 *pvTarget,int num)
{
	
	/*if (UIOBJECTMANAGER->CheckShowState("inventory"))
	{
		m_ptPrevMouse = g_ptMouse;
		m_fangleY = D3DX_PI;
		return;
	}*/
	//if (m_fBoundingX > 0.0001)
	//{
	//	m_fBoundingX -= 0.3;
	//	if (m_fBoundingX < 0)
	//		m_fBoundingX = 0;
	//}
	if (m_fBoundingX < 0.0001)
	{
		m_fBoundingX += 0.2;
		if (m_fBoundingX > 0)
			m_fBoundingX = 0;
	}
	if (pvTarget)
	{
		m_vTartget.x = pvTarget->x;
		m_vTartget.y = pvTarget->y + 8.0f;
		m_vTartget.z = pvTarget->z;

		
	}
	else
		m_vTartget = D3DXVECTOR3(m_vTartget.x, m_vTartget.y, m_vTartget.z);


	float deltaX = g_ptMouse.y - m_ptPrevMouse.y  ;
	float deltaY = g_ptMouse.x - m_ptPrevMouse.x;


	m_fangleX += deltaX / 100.0f;
	m_fangleY += deltaY / 100.0f;


	if (m_fangleX < -D3DX_PI / 4.0f + 0.0001f)
		m_fangleX = -D3DX_PI / 4.0f + 0.0001f;

	if (m_fangleX > D3DX_PI / 4.0f - 0.0001f)
		m_fangleX = D3DX_PI / 4.0f - 0.0001f;

	

	if (g_MouseWheel)
	{
		if (g_MouseWheel > 0)
		{
			m_fDistance -= 2.0f;
		}
		else if (g_MouseWheel < 0)
		{
			m_fDistance += 2.0f;
		}
		g_MouseWheel = 0;

		/*if (m_fDistance <= 6.0f)
		{
			m_fDistance = 6.0f;
		}
		else if (m_fDistance >= 12.0f)
		{
			m_fDistance = 12.0f;
		}*/
	}
	if (num == 1)
	{
		m_fDistance=50;
	}
	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);

	if (UIOBJECTMANAGER->CheckShowState("inventory"))
	{
		m_ptPrevMouse = g_ptMouse;
		m_fangleX = 0;
		m_fangleY = D3DX_PI;
		m_vEye = D3DXVECTOR3(0, 0, -8);

		if (pvTarget) 
		{
			m_vTartget.x = pvTarget->x + 2.0f;
			m_vTartget.y = pvTarget->y + 3.0f;
			m_vTartget.z = pvTarget->z;
		}
	}

	D3DXMATRIXA16 matRX;
	D3DXMATRIXA16 matRY;
	D3DXMatrixRotationX(&matRX, m_fangleX + m_fBoundingX/100.0);
	D3DXMatrixRotationY(&matRY, m_fangleY);

	D3DXMATRIXA16 matR = matRX * matRY;

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

	m_ptPrevMouse = g_ptMouse;

	DotWorldSpace();
	SetPlane();
}

void LDYCamera::rebound()
{
	m_fBoundingX -= RND->getFromFloatTo(1.0, 2.0);
}





void LDYCamera::DotWorldSpace()
{
	//				   5---------6
	//				  /|	   / |
	//               / |      /  |
	//				/  4-----/---7
	//				1-------2   
	//				|		|
	//				|		|
	//				0-------3

	m_Dot[0] = D3DXVECTOR3(-1, -1, 0);
	m_Dot[1] = D3DXVECTOR3(-1, 1, 0);
	m_Dot[2] = D3DXVECTOR3(1, 1, 0);
	m_Dot[3] = D3DXVECTOR3(1, -1, 0);

	m_Dot[4] = D3DXVECTOR3(-1, -1, 1);
	m_Dot[5] = D3DXVECTOR3(-1, 1, 1);
	m_Dot[6] = D3DXVECTOR3(1, 1, 1);
	m_Dot[7] = D3DXVECTOR3(1, -1, 1);

	D3DXMATRIX PorjectionInverse;
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &PorjectionInverse);
	D3DXMatrixInverse(&PorjectionInverse, NULL, &PorjectionInverse);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_Dot[i], &m_Dot[i], &PorjectionInverse);
	}
	D3DXMATRIX ViewSpace;
	GETDEVICE->GetTransform(D3DTS_VIEW, &ViewSpace);
	D3DXMatrixInverse(&ViewSpace, NULL, &ViewSpace);
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_Dot[i], &m_Dot[i], &ViewSpace);
	}
}

void LDYCamera::SetPlane()
{
	///앞쪽 평면    
	D3DXPlaneFromPoints(&g_Plane[0], &m_Dot[1], &m_Dot[2], &m_Dot[3]);
	//위쪽 평면
	D3DXPlaneFromPoints(&g_Plane[1], &m_Dot[1], &m_Dot[5], &m_Dot[6]);
	//오른쪽 평면
	D3DXPlaneFromPoints(&g_Plane[2], &m_Dot[2], &m_Dot[6], &m_Dot[7]);
	//아래쪽 평면
	D3DXPlaneFromPoints(&g_Plane[3], &m_Dot[0], &m_Dot[3], &m_Dot[7]);
	//왼쪽 평면
	D3DXPlaneFromPoints(&g_Plane[4], &m_Dot[5], &m_Dot[1], &m_Dot[0]);
	//뒤쪽 평면
	D3DXPlaneFromPoints(&g_Plane[5], &m_Dot[6], &m_Dot[5], &m_Dot[4]);
}
