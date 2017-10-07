#include "stdafx.h"
#include "cSprayMesh.h"
#include <fstream>
#include <sstream>
#include "cModel.h"
#include "cRayCaster.h"


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

void cSprayMesh::SetTargetNode(vector<D3DXVECTOR3>& vec)
{
	m_vecNode = vec;
}

void cSprayMesh::Render()
{
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);

	cRayCaster r; 
	D3DXVECTOR3 MouseVec = r.GetMousePtFromNode(g_ptMouse.x, g_ptMouse.y, m_vecNode);

	D3DXMatrixTranslation(&I, MouseVec.x, MouseVec.y, MouseVec.z);

	GETDEVICE->SetTransform(D3DTS_WORLD, &I);


	for each(auto p in m_pMeshList)
	{
		p->DrawSubset(0);
	}
	
}

void cSprayMesh::Release()
{
	for each(auto p in m_pMeshList)
	{
		SAFE_RELEASE(p);
	}
}

cSprayMesh::cSprayMesh()
	:m_position(0, 0, 0)
	, m_orientation(0, 0, 0, 0)
	, m_scalling(0.01, 0.01, 0.01)
	, m_vecNode(vector<D3DXVECTOR3>())
{
	D3DXMatrixIdentity(&m_matScale);
}


cSprayMesh::~cSprayMesh()
{
}


void cSprayMesh::MeshLoadFromScript(const std::string& fullPath)
{
	ifstream file(fullPath);

	string buffer, token;

	if (file.good() == false)
	{
		return;	
	}
	buffer.reserve(1024);
	
	while (getline(file, buffer))
	{
		istringstream iss(buffer);
		iss >> token;

		if (token == "[MeshLoad]")
		{
			CreateMesh(file);
			
			continue;
		}
		else if (token == "[Finish]")
		{
			break;
		}
		
	}
}

void cSprayMesh::CreateMesh(ifstream & file)
{
	string buffer, token;

	m_meshId.push_back(m_meshId.size() + 1);

	while (getline(file, buffer))
	{
		istringstream iss(buffer);
		iss >> token;

		if (token == "path")
		{
			string path;
			iss >> path;
			m_meshPath.push_back(path);
		}
		else if (token == "filename")
		{
			string name;
			iss >> name;
			m_meshFileName.push_back(name);
		}
		else if (token == "[end]")
		{
			int index = m_meshId.size() - 1;
			string fullPath = m_meshPath[index];
			string fileName = m_meshFileName[index];
			MODELMANAGER->LoadModel(fullPath, fileName);
			m_pMeshList.push_back(MODELMANAGER->GetModel(fileName)->GetMesh());

			return;
		}

	}
}