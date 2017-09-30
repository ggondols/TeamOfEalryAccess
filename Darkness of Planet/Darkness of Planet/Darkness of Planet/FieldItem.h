#pragma once
#include "cGameObject.h"

class TeicSkinnedMesh;

class FieldItem : public cGameObject
{
private:
	string					m_sName;
	LPD3DXMESH				m_pMesh;
	ITEM_TYPE				m_eType;

	float					m_fAngleY;

	LPD3DXMESH LoadModel(char* szFolder, char* szFilename);

public:
	FieldItem();
	~FieldItem();

	virtual void Setup(string name, ITEM_TYPE type);
	virtual void Update();
	virtual void Render();

	string GetName() { return m_sName; }
	ITEM_TYPE GetItemType() { return m_eType; }
};

