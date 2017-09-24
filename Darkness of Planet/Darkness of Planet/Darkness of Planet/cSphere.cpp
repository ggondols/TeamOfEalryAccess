#include "stdafx.h"
#include "cSphere.h"


cSphere::cSphere()
{
	Red = false;

	Whitematerial.Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Specular = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	Whitematerial.Power = 5.0f;


	Redmaterial.Ambient = D3DXCOLOR(1.0, 0, 0, 1.0f);
	Redmaterial.Diffuse = D3DXCOLOR(1.0, 0, 0, 1.0f);
	Redmaterial.Specular = D3DXCOLOR(1.0, 0, 0, 1.0f);
	Redmaterial.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	Redmaterial.Power = 5.0f;
}


cSphere::~cSphere()
{
}

void cSphere::Setup(float x, float y)
{
	m_center = D3DXVECTOR3(0, 0, 0);
	D3DXCreateSphere(GETDEVICE, 0.5, 50, 50, &m_pMesh, NULL);
	D3DXMatrixTranslation(&m_matworld, x + 0.5, 10, -y - 0.5);
	D3DXVec3TransformCoord(&m_center, &m_center, &m_matworld);
	tilex = x;
	tiley = y;








}

void cSphere::Setup(D3DXVECTOR3 center, float radius)
{
	m_center = center;
	D3DXCreateSphere(GETDEVICE, radius, 50, 50, &m_pMesh, NULL);
	D3DXMatrixTranslation(&m_matworld, m_center.x, m_center.y, m_center.z);
	
	
}

void cSphere::Render()
{

	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	GETDEVICE->SetTransform(D3DTS_WORLD, &m_matworld);
	GETDEVICE->SetTexture(0, NULL);
	if (Red)
	{
		GETDEVICE->SetMaterial(&Redmaterial);
	}
	else
	{
		GETDEVICE->SetMaterial(&Whitematerial);
	}
	m_pMesh->DrawSubset(0);
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);
}

void cSphere::Update()
{
}

void cSphere::setGo2(float x, float y)
{
	m_center = D3DXVECTOR3(0, 0, 0);
	m_matworld._41 = x;
	m_matworld._43 = y;
	D3DXVec3TransformCoord(&m_center, &m_center, &m_matworld);
}