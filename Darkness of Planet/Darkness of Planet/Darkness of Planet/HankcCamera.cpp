#include "stdafx.h"
#include "HankcCamera.h"

namespace Hank
{

	cCamera::cCamera()
		: m_pvTarget(NULL)
		, m_fangleY(0.0f)
		, m_fangleX(0.0f)
		, m_isLButtonDown(false)
		, m_fDistance(5)
		, m_vEye(0, 0, -m_fDistance)
		, getMousePos(false)
	{
	}

	cCamera::~cCamera()
	{
	}

	void cCamera::Setup(D3DXVECTOR3 * pvTarget /*=NULL*/)
	{
		m_bTwist = false;
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

	void cCamera::Update()
	{

		//static float time;
		//time += TIMEMANAGER->getElapsedTime();
		//if (2 < time)
		//{
		//	time = 0;
		//
		//	RECT rc, rcFrom;
		//	POINT tt;
		//	GetWindowRect(g_hWnd, &rcFrom);
		//	
		//	ScreenToClient(g_hWnd, &tt);
		//	GetClientRect(g_hWnd, &rc);
		//	
		//	SetCursorPos(rcFrom.left + rc.right / 2+20, rcFrom.top +  rc.bottom/2+20);
		//	g_ptMouse.x = rc.right / 2;
		//	g_ptMouse.y = rc.bottom / 2;
		//	//m_ptPrevMouse = g_ptMouse;
		//}

		//방향키 이동 for debug
		/*if (GetKeyState(VK_UP) & 0x8000)
		{
		m_vEye.y += 0.1f;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
		m_vEye.y -= 0.1f;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
		m_vEye.x += 0.1f;
		}
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
		m_vEye.x -= 0.1f;
		}*/


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
			float deltaX = g_ptMouse.y - m_ptPrevMouse.y;
			float deltaY = g_ptMouse.x - m_ptPrevMouse.x;
			m_fangleX += deltaX / 50.0f;
			m_fangleY += deltaY / 50.0f;

			if (m_fangleX < -D3DX_PI / 2.0f + 0.0001f)
				m_fangleX = -D3DX_PI / 2.0f + 0.0001f;

			if (m_fangleX > D3DX_PI / 2.0f - 0.0001f)
				m_fangleX = D3DX_PI / 2.0f - 0.0001f;

			m_ptPrevMouse = g_ptMouse;	

		}

		if (g_MouseWheel)
		{
			if (g_MouseWheel > 0)
			{
				m_fDistance -= 1;
			}
			else if (g_MouseWheel < 0)
			{
				m_fDistance += 1;
			}
			g_MouseWheel = 0;

			if (m_fDistance <= 3)
				m_fDistance = 3;
		}

		D3DXMATRIXA16 matRX;
		D3DXMATRIXA16 matRY;
		D3DXMatrixRotationX(&matRX, m_fangleX);
		D3DXMatrixRotationY(&matRY, m_fangleY);

		D3DXMATRIXA16 matR = matRX * matRY;

		m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);
		D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

		D3DXVECTOR3 vTarget = D3DXVECTOR3(0, 0, 0);
		if (m_pvTarget)
		{
			vTarget = *m_pvTarget;
		}

		
		if (m_bTwist)
		{
			vTarget += D3DXVECTOR3(RND->getFromFloatTo(-0.25, 0.25), RND->getFromFloatTo(-0.25, 0.25), 0);
			m_vEye = m_vEye + vTarget ;
		}
		else
		{
			m_vEye = m_vEye + vTarget ;

		}

		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView,
			&m_vEye,
			m_pvTarget,
			&D3DXVECTOR3(0, 1, 0));
		GETDEVICE->SetTransform(D3DTS_VIEW, &matView);


		
	}


}