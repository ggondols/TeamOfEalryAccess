#include "stdafx.h"
#include "TeicSkinnedMeshLoader.h"
#include "LDYSkinnedMesh.h"
#include "LDYSkinnedMesh_Weapon.h"
#include "LDYSkinnedMesh_Head.h"



void TeicSkinnedMeshLoader::Destroy()
{
	//////이 포인트를 받아온 곳에서 지운다.
}

TeicSkinnedMeshLoader::TeicSkinnedMeshLoader()
{
}


TeicSkinnedMeshLoader::~TeicSkinnedMeshLoader()
{
}

void TeicSkinnedMeshLoader::AddSkinnedMesh(const char * keyname,  char * foldername,  char * filename)
{

	m_iterSkinnedMesh = m_mapSkinnedMesh.find(keyname);

	if (m_iterSkinnedMesh != m_mapSkinnedMesh.end())
	{
	}
	else
	{
		LDYSkinnedMesh* temp =new LDYSkinnedMesh(foldername, filename);
		m_mapSkinnedMesh.insert(make_pair(keyname, temp));
		
	}
}

void TeicSkinnedMeshLoader::AddSkinnedMeshHead(const char * keyname, char * foldername, char * filename)
{
	m_iterSkinnedMesh_Head = m_mapSkinnedMesh_Head.find(keyname);

	if (m_iterSkinnedMesh_Head != m_mapSkinnedMesh_Head.end())
	{
	}
	else
	{
		LDYSkinnedMesh_Head* temp = new LDYSkinnedMesh_Head(foldername, filename);
		m_mapSkinnedMesh_Head.insert(make_pair(keyname, temp));

	}
}

void TeicSkinnedMeshLoader::AddSkinnedWeapon(const char * keyname, char * foldername, char * filename)
{
	m_iterSkinnedMesh_Weapon = m_mapSkinnedMesh_Weapon.find(keyname);

	if (m_iterSkinnedMesh_Weapon != m_mapSkinnedMesh_Weapon.end())
	{
	}
	else
	{
		LDYSkinnedMesh_Weapon* temp = new LDYSkinnedMesh_Weapon(foldername, filename);
		m_mapSkinnedMesh_Weapon.insert(make_pair(keyname, temp));

	}
}

LDYSkinnedMesh * TeicSkinnedMeshLoader::GetSkinnedMesh(char * keyname)
{
	m_iterSkinnedMesh = m_mapSkinnedMesh.find(keyname);

	//키 값 찾으면....
	if (m_iterSkinnedMesh != m_mapSkinnedMesh.end())
	{
		return m_iterSkinnedMesh->second;
	}


	return NULL;
}

LDYSkinnedMesh_Head * TeicSkinnedMeshLoader::GetSkinnedMeshHead(char * keyname)
{
	m_iterSkinnedMesh_Head = m_mapSkinnedMesh_Head.find(keyname);

	//키 값 찾으면....
	if (m_iterSkinnedMesh_Head != m_mapSkinnedMesh_Head.end())
	{
		return m_iterSkinnedMesh_Head->second;
	}


	return NULL;
}

LDYSkinnedMesh_Weapon * TeicSkinnedMeshLoader::GetSkinnedMeshWeapon(char * keyname)
{
	m_iterSkinnedMesh_Weapon = m_mapSkinnedMesh_Weapon.find(keyname);

	//키 값 찾으면....
	if (m_iterSkinnedMesh_Weapon != m_mapSkinnedMesh_Weapon.end())
	{
		return m_iterSkinnedMesh_Weapon->second;
	}


	return NULL;
}

LDYSkinnedMesh * TeicSkinnedMeshLoader::GetStaticMesh(char * keyname)
{
	return nullptr;
}

void TeicSkinnedMeshLoader::Setup()
{
}
