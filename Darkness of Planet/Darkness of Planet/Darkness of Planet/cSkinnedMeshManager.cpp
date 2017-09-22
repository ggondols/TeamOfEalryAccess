#include "StdAfx.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"
#include "TeicSkinnedMesh.h"
#include "LDYSkinnedMesh.h"
#include "LDYSkinnedMesh_Head.h"
#include "LDYSkinnedMesh_Weapon.h"

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

BoundingSquare  cSkinnedMeshManager::GetTeiBoundingSquare(char * szFolder, char * szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapTeiSkinnedMesh.find(sFullPath) == m_mapTeiSkinnedMesh.end())
	{
		
	}
	return m_mapTeiSkinnedMesh[sFullPath]->m_pBoundingSquare;
}

LDYSkinnedMesh * cSkinnedMeshManager::GetLDYSkinnedMesh(char * szFolder, char * szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapLDYSkinnedMesh.find(sFullPath) == m_mapLDYSkinnedMesh.end())
	{
		LDYSkinnedMesh* pSkinnedMesh = new LDYSkinnedMesh();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapLDYSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapLDYSkinnedMesh[sFullPath];
}

LDYSkinnedMesh_Weapon * cSkinnedMeshManager::GetLDYSkinnedMesh_Weapon(char * szFolder, char * szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapLDYSkinnedMesh_Weapon.find(sFullPath) == m_mapLDYSkinnedMesh_Weapon.end())
	{
		LDYSkinnedMesh_Weapon* pSkinnedMesh = new LDYSkinnedMesh_Weapon();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapLDYSkinnedMesh_Weapon[sFullPath] = pSkinnedMesh;
	}
	return m_mapLDYSkinnedMesh_Weapon[sFullPath];
}

LDYSkinnedMesh_Head * cSkinnedMeshManager::GetLDYSkinnedMesh_Head(char * szFolder, char * szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapLDYSkinnedMesh_Head.find(sFullPath) == m_mapLDYSkinnedMesh_Head.end())
	{
		LDYSkinnedMesh_Head* pSkinnedMesh = new LDYSkinnedMesh_Head();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapLDYSkinnedMesh_Head[sFullPath] = pSkinnedMesh;
	}
	return m_mapLDYSkinnedMesh_Head[sFullPath];
}
