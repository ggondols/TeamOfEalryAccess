#include "stdafx.h"
#include "FieldItem.h"
#include "LDYSkinnedMesh_Weapon.h"

FieldItem::FieldItem()
	: m_pMesh(NULL)
	, m_pMeshTest(NULL)
{
}

FieldItem::~FieldItem()
{
	SAFE_DELETE(m_pMesh);
	//SAFE_DELETE(m_pMeshTest);
}

void FieldItem::Setup(string name)
{
	m_sName = name;
	if (m_sName == "ArmorArm") m_pMeshTest = LoadModel("object/xFile/Item/", "ArmorArm.X");
	else if (m_sName == "ArmorBody") m_pMeshTest = LoadModel("object/xFile/Item/", "ArmorBody.X");
	else if (m_sName == "ArmorLeg") m_pMeshTest = LoadModel("object/xFile/Item/", "ArmorLeg.X");
}

void FieldItem::Update()
{
}

void FieldItem::Render()
{
	m_pMeshTest->DrawSubset(0);
	//m_pMesh->UpdateAndRender();
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
