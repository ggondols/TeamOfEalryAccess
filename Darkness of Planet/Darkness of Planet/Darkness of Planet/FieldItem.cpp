#include "stdafx.h"
#include "FieldItem.h"
#include "LDYSkinnedMesh_Weapon.h"

FieldItem::FieldItem()
	: m_pMesh(NULL)
	, m_fAngleY(0.0f)
	, m_eType(ITEMTYPE_NONE)
{
}

FieldItem::~FieldItem()
{
	SAFE_RELEASE(m_pMesh);
}

void FieldItem::Setup(string name, ITEM_TYPE type)
{
	m_sName = name;
	m_eType = type;

	if (m_sName == "ArmorArm") m_pMesh = LoadModel("object/xFile/Item/", "ArmorArm.X");
	else if (m_sName == "ArmorBody") m_pMesh = LoadModel("object/xFile/Item/", "ArmorBody.X");
	else if (m_sName == "ArmorLeg") m_pMesh = LoadModel("object/xFile/Item/", "ArmorLeg.X");
}

void FieldItem::Update()
{
	m_fAngleY += 0.05f;
}

void FieldItem::Render()
{
	D3DXMATRIX matS, matRY, matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationY(&matRY, m_fAngleY);
	if (m_sName == "ArmorArm") D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	else if (m_sName == "ArmorBody") D3DXMatrixScaling(&matS, 0.06f, 0.06f, 0.06f);
	else if (m_sName == "ArmorLeg") D3DXMatrixScaling(&matS, 0.07f, 0.07f, 0.07f);

	m_matWorld = matS * matRY * matT;
	GETDEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture("object/xFile/Item/T_GabeArmor_Diffuse.tga"));
	m_pMesh->DrawSubset(0);
}

LPD3DXMESH FieldItem::LoadModel(char* szFolder, char* szFilename)
{
	LPD3DXMESH ret = NULL;

	string fullPath = szFolder;
	fullPath += szFilename;
	if (FAILED(D3DXLoadMeshFromX(fullPath.c_str(), D3DXMESH_SYSTEMMEM, GETDEVICE, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패 ");
		OutputDebugString(fullPath.c_str());
		OutputDebugString("\n");
	}
	return ret;
}
