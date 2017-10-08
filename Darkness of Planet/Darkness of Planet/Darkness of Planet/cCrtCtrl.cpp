#include "stdafx.h"
#include "cCrtCtrl.h"
#include "iMap.h"

cCrtCtrl::cCrtCtrl()
	: m_vDir(0, 0, 1)
	, m_vPos(0, 0, 0)
	, m_fSpeed(0.1f)
	, m_fAngle(0.0f)
	, m_fRun(0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCrtCtrl::~cCrtCtrl()
{
}


void cCrtCtrl::Update(iMap* pMap/* = NULL*/)
{
	D3DXVECTOR3 vPos = m_vPos;

	
	if (GetKeyState('W') & 0x8000)
	{
		vPos = m_vPos + m_vDir * (m_fSpeed+m_fRun);
	}
	if (GetKeyState('S') & 0x8000)
	{
		vPos = m_vPos - m_vDir * (m_fSpeed+m_fRun);
	}

	if (GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
	}

	if (GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
	}

	if (pMap)
	{
		if (pMap->GetHeight(vPos.x, vPos.y, vPos.z))
		{
			m_vPos = vPos;
		}
	}
	else
	{
		m_vPos = vPos;
	}

	m_vDir = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);

	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matR * matT;
}

D3DXVECTOR3* cCrtCtrl::GetPosition()
{
	return &m_vPos;
}

void cCrtCtrl::UpdateByDir(D3DXVECTOR3 &pDir)
{
	D3DXVECTOR3 vPos = m_vPos;


	if (GetKeyState('W') & 0x8000)
	{
		m_vPos = m_vPos + -pDir * (m_fSpeed + m_fRun);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_vPos = m_vPos + pDir * (m_fSpeed + m_fRun);
	}
	if (KEYMANAGER->isStayKeyDown(VK_SPACE))
	{
		m_vPos.y += 0.5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LCONTROL))
	{
		m_vPos.y -= 0.5;
	}
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		D3DXVECTOR3 lDir, front;
		D3DXVECTOR3 up;
		D3DXMATRIX matR;
		float angle = D3DXToDegree(45);
		D3DXQUATERNION qr;
		//D3DXQuaternionRotationYawPitchRoll(&qr, 0, angle, 0);
		
		//D3DXMatrixRotationQuaternion(&matR, &qr);

		D3DXVec3TransformNormal(&lDir, &pDir, &matR);

	
		//D3DXVec3Cross(&lDir, &pDir, &up);
		//D3DXVec3Normalize(&lDir, &lDir);
		m_vPos = m_vPos - lDir * (m_fSpeed + m_fRun);
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{

	}


	m_vDir = pDir;

	D3DXMATRIX matT, matR;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);


	m_matWorld = matT;
}