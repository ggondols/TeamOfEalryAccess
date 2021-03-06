#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"
#include "cModel.h"
#include "cObjectNode.h"
#include "cStaticMeshLoader.h"
#include "HankcNode.h"
#include "LDYCamera.h"
#include "cConsole.h"
#include "cSprayMesh.h"
class iMap;
class cCrtCtrl;
class HankcDefferedRenderTest : public cGameNode
{
private:
	HankcGrid*				m_pNode;
	iMap*					m_pMap;
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	TeicCharacter*			m_pCharacter;

	cModel*					m_Tree;
	cModel*					pNewModel;

	std::list<cObjectNode*>	m_ObjNodes;

	cStaticMeshLoader		m_meshList;

	TeicPhysicsCrtCtrl*		m_pPhysicsCrtCtrl;
	cCrtCtrl*				m_DummyCtrl;

	D3DXVECTOR3				m_ViewCamera;

	cConsole*				m_pConsole;

	cSprayMesh*				m_pMeshSpray;

	int						m_mna;
		
public:

	HankcDefferedRenderTest();
	~HankcDefferedRenderTest();

	void CallbackOn(int num);

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};
