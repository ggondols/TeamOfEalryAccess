#include "StdAfx.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"
#include "TeicSkinnedMesh.h"
cSkinnedMeshManager::cSkinnedMeshManager(void)
{
}


cSkinnedMeshManager::~cSkinnedMeshManager(void)
{
}

void cSkinnedMeshManager::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh( char* szFolder, char* szFilename )
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if(m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapSkinnedMesh[sFullPath];
}

TeicSkinnedMesh * cSkinnedMeshManager::GetTeiSkinnedMesh(char * szFolder, char * szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapTeiSkinnedMesh.find(sFullPath) == m_mapTeiSkinnedMesh.end())
	{
		TeicSkinnedMesh* pSkinnedMesh = new TeicSkinnedMesh();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapTeiSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapTeiSkinnedMesh[sFullPath];
}
