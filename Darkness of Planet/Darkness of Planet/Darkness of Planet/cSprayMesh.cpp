#include "stdafx.h"
#include "cSprayMesh.h"


void cSprayMesh::EntryMesh(LPD3DXMESH pMesh)
{
	m_pMeshList.push_back(pMesh);
}
void cSprayMesh::Update()
{
	static int currentIndex;
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (currentIndex < m_pMeshList.size())
			m_pSelectMesh = m_pMeshList[++currentIndex];
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (currentIndex > 0)
			m_pSelectMesh = m_pMeshList[--currentIndex];
	}

	if (KEYMANAGER->isOnceKeyDown('-'))
	{
		if (m_scalling.x > 0.1f) {
			m_scalling.x -= 0.1;
			m_scalling.y -= 0.1;
			m_scalling.z -= 0.1;
		}
		else {
			m_scalling.x -= 0.01;
			m_scalling.y -= 0.01;
			m_scalling.z -= 0.01;
		}
	}
	else if (KEYMANAGER->isOnceKeyDown('+'))
	{
		if (m_scalling.x >= 0.1f) {
			m_scalling.x += 0.1;
			m_scalling.y += 0.1;
			m_scalling.z += 0.1;
		}
		else {
			m_scalling.x += 0.01;
			m_scalling.y += 0.01;
			m_scalling.z += 0.01;
		}
	}

	

}
void cSprayMesh::Render()
{
	for each(auto p in m_pMeshList)
	{
		p->DrawSubset(0);
	}
	
}

cSprayMesh::cSprayMesh()
	:m_position(0,0,0)
	,m_orientation(0,0,0,0)
	,m_scalling(0.01,0.01,0.01)
{
	D3DXMatrixIdentity(&m_matScale);
}


cSprayMesh::~cSprayMesh()
{
}
