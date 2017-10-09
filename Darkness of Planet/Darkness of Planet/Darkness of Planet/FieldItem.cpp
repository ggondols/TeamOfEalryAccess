#include "stdafx.h"
#include "FieldItem.h"
#include "LDYSkinnedMesh_Weapon.h"

FieldItem::FieldItem()
	: m_pMesh(NULL)
	, m_fAngleY(0.0f)
	, m_eType(ITEMTYPE_NONE)
	, m_fNullTime(0.0f)
	, m_isNull(false)
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
}

void FieldItem::Setup(int itemNum)
{
	m_eType = ITEMTYPE_PART;
	m_pMesh = LoadModel("object/xFile/Item/", "ItemPart.X");

	SetNameNumber(itemNum);
}

void FieldItem::Update()
{
	m_fAngleY += 0.05f;
	if (m_isNull)
	{
		m_fNullTime += TIMEMANAGER->getElapsedTime();
	}
	else
	{
		m_fNullTime = 0.0f;
	}
}

void FieldItem::Render()
{
	if (m_isNull) return;

	D3DXMATRIX matS, matRY, matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationY(&matRY, m_fAngleY);
	D3DXMatrixScaling(&matS, 0.07f, 0.07f, 0.07f);

	m_matWorld = matS * matRY * matT;
	GETDEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture("object/xFile/Item/T_Pickup_Packs.tga"));

	m_pMesh->DrawSubset(0);
}

void FieldItem::SetNameNumber(int nNum)
{
	switch (nNum)
	{
	case 0:
		m_sName = "FullArmorPart1";
		break;
	case 1:
		m_sName = "FullArmorPart2";
		break;
	case 2:
		m_sName = "FullArmorPart3";
		break;
	case 3:
		m_sName = "HelmetPart1";
		break;
	case 4:
		m_sName = "HelmetPart2";
		break;
	}
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
