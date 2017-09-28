#pragma once
#include "cGameObject.h"

class TeicSkinnedMesh;

class FieldItem : public cGameObject
{
private:
	string					m_sName;
	LDYSkinnedMesh_Weapon*		m_pMesh;
	LPD3DXMESH				m_pMeshTest;

public:
	FieldItem();
	~FieldItem();

	virtual void Setup(string name);
	virtual void Update();
	virtual void Render();

	LPD3DXMESH LoadModel(char* szFolder, char* szFilename);
};

