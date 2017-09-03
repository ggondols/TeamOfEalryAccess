#include "stdafx.h"
#include "cMaterialMake.h"




cMaterialMake::cMaterialMake()
{
}


cMaterialMake::~cMaterialMake()
{
}

void cMaterialMake::SetRed(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(1.0, 0, 0, 1.0f);
	mat.Diffuse = D3DXCOLOR(1.0, 0, 0, 1.0f);
	mat.Specular = D3DXCOLOR(1.0, 0, 0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialMake::SetGreen(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0, 1.0, 0, 1.0f);
	mat.Diffuse = D3DXCOLOR(0, 1.0, 0, 1.0f);
	mat.Specular = D3DXCOLOR(0, 1.0, 0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialMake::SetBlue(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0, 0, 1.0, 1.0f);
	mat.Diffuse = D3DXCOLOR(0, 0, 1.0, 1.0f);
	mat.Specular = D3DXCOLOR(0, 0, 1.0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialMake::SetWhite(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	mat.Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	mat.Specular = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialMake::SetBlack(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Diffuse = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Specular = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialMake::SetGray(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0.3, 0.3, 0.3, 1.0f);
	mat.Diffuse = D3DXCOLOR(0.3, 0.3, 0.3, 1.0f);
	mat.Specular = D3DXCOLOR(0.3, 0.3, 0.3, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}

void cMaterialMake::SetGrayWhite(D3DMATERIAL9 & mat)
{
	mat.Ambient = D3DXCOLOR(0.7, 0.7, 0.7, 1.0f);
	mat.Diffuse = D3DXCOLOR(0.7, 0.7, 0.7, 1.0f);
	mat.Specular = D3DXCOLOR(0.7, 0.7, 0.7, 1.0f);
	mat.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	mat.Power = 5.0f;
}


void cMaterialMake::Setup()
{
	///////////뭐 필요 없는듯
}

void cMaterialMake::Release()
{
	///////필요 없는듯
}