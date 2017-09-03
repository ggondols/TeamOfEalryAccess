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

void cMaterialManager::SetCustomMaterial(OUT D3DMATERIAL9 & mat, D3DCOLORVALUE Ambient, D3DCOLORVALUE Diffuse, D3DCOLORVALUE Specular, D3DCOLORVALUE Emissive, float power)
{
	////////// 외부 클래스에서 머티리얼 만들어서 사용
	//////////혹시 몰라서 만들어놈(별로 쓸모 없을듯)
	mat.Ambient = Ambient;
	mat.Diffuse = Diffuse;
	mat.Specular = Specular;
	mat.Emissive = Emissive;
	mat.Power = power;

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