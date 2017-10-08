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
	m_eType = type;

	m_sName = name;
	m_pMesh = LoadModel("object/xFile/Item/", "ItemPart.X");

	/*if (name == "ArmorArm")
	{
		m_sName = "FullArmorPart1";
		m_pMesh = LoadModel("object/xFile/Item/", "ArmorArm.X");
	}
	else if (name == "ArmorBody")
	{
		m_sName = "FullArmorPart2";
		m_pMesh = LoadModel("object/xFile/Item/", "ArmorBody.X");
	}
	else if (name == "ArmorLeg")
	{
		m_sName = "FullArmorPart3";
		m_pMesh = LoadModel("object/xFile/Item/", "ArmorLeg.X");
	}
	else if (name == "HelmetPart1")
	{
		m_sName = "HelmetPart1";
		m_pMesh = LoadModel("object/xFile/Item/", "HelmetPart.X");
	}
	else if (name == "HelmetPart2")
	{
		m_sName = "HelmetPart2";
		m_pMesh = LoadModel("object/xFile/Item/", "HelmetPart.X");
	}*/
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
	D3DXMatrixScaling(&matS, 0.07f, 0.07f, 0.07f);

	/*if (m_sName == "FullArmorPart1") D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	else if (m_sName == "FullArmorPart2") D3DXMatrixScaling(&matS, 0.06f, 0.06f, 0.06f);
	else if (m_sName == "FullArmorPart3") D3DXMatrixScaling(&matS, 0.07f, 0.07f, 0.07f);
	else if (m_sName == "HelmetPart1") D3DXMatrixScaling(&matS, 0.07f, 0.07f, 0.07f);
	else if (m_sName == "HelmetPart2") D3DXMatrixScaling(&matS, 0.07f, 0.07f, 0.07f);*/

	m_matWorld = matS * matRY * matT;
	GETDEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture("object/xFile/Item/T_Pickup_Packs.tga"));

	/*if (m_sName == "FullArmorPart1" || m_sName == "FullArmorPart2" || m_sName == "FullArmorPart3")
	{
		GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture("object/xFile/Item/T_GabeArmor_Diffuse.tga"));
	}
	else if (m_sName == "HelmetPart1" || m_sName == "HelmetPart2")
	{
		GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture("object/xFile/Item/T_Pickup_Packs.tga"));
	}*/

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
