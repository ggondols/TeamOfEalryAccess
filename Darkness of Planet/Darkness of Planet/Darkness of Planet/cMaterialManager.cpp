#include "stdafx.h"
#include "cMaterialManager.h"


cMaterialManager::cMaterialManager()
{
}


cMaterialManager::~cMaterialManager()
{
}

void cMaterialManager::SetRed(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(1.0, 0, 0, 1.0f);
	mat.Diffuse = D3DXCOLOR(1.0, 0, 0, 1.0f);
	mat.Specular = D3DXCOLOR(1.0, 0, 0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialManager::SetGreen(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0, 1.0, 0, 1.0f);
	mat.Diffuse = D3DXCOLOR(0, 1.0, 0, 1.0f);
	mat.Specular = D3DXCOLOR(0, 1.0, 0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialManager::SetBlue(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0, 0, 1.0, 1.0f);
	mat.Diffuse = D3DXCOLOR(0, 0, 1.0, 1.0f);
	mat.Specular = D3DXCOLOR(0, 0, 1.0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialManager::SetWhite(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	mat.Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	mat.Specular = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialManager::SetBlack(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Diffuse = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Specular = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialManager::SetGray(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0.3, 0.3, 0.3, 1.0f);
	mat.Diffuse = D3DXCOLOR(0.3, 0.3, 0.3, 1.0f);
	mat.Specular = D3DXCOLOR(0.3, 0.3, 0.3, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialManager::SetGrayWhite(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0.7, 0.7, 0.7, 1.0f);
	mat.Diffuse = D3DXCOLOR(0.7, 0.7, 0.7, 1.0f);
	mat.Specular = D3DXCOLOR(0.7, 0.7, 0.7, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}


void cMaterialManager::Setup()
{
	SetRed(m_mRedmaterial);
	SetRed(m_mGreenmaterial);
	SetRed(m_mBluematerial);
	SetRed(m_mWhitematerial);
	SetRed(m_mBlackmaterial);
	SetRed(m_mGraymaterial);
	SetRed(m_mGrayWhitematerial);
	



}

void cMaterialManager::Release()
{
	///////필요 없는듯
}