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
	float					m_fNullTime;

	LPD3DXMESH LoadModel(char* szFolder, char* szFilename);

	SYNTHESIZE(bool, m_isNull, IsNull);

public:
	FieldItem();
	~FieldItem();

	virtual void Setup(string name, ITEM_TYPE type);
	virtual void Setup(int itemNum);
	virtual void Update();
	virtual void Render();

	void SetNameNumber(int nNum);

	string GetName() { return m_sName; }
	ITEM_TYPE GetItemType() { return m_eType; }
	float GetNullTime() { return m_fNullTime; }
};

