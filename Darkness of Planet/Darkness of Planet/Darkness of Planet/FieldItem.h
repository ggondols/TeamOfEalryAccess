#pragma once
#include "cGameObject.h"

class TeicSkinnedMesh;

class FieldItem : public cGameObject
{
private:
	string					m_sName;
	LPD3DXMESH				m_pMesh;

	float					m_fAngleY;

	LPD3DXMESH LoadModel(char* szFolder, char* szFilename);

public:
	FieldItem();
	~FieldItem();

	virtual void Setup(string name);
	virtual void Update();
	virtual void Render();
};

