#pragma once
#include "cSingletonBase.h"
class LDYSkinnedMesh;
class LDYSkinnedMesh_Head;
class LDYSkinnedMesh_Weapon;

class TeicSkinnedMeshLoader:public cSingletonBase<TeicSkinnedMeshLoader>
{
private:
	map<string, LDYSkinnedMesh*>					m_mapSkinnedMesh;
	map<string, LDYSkinnedMesh*>::iterator			m_iterSkinnedMesh;
	map<string, LDYSkinnedMesh_Head*>				m_mapSkinnedMesh_Head;
	map<string, LDYSkinnedMesh_Head*>::iterator		m_iterSkinnedMesh_Head;
	map<string, LDYSkinnedMesh_Weapon*>				m_mapSkinnedMesh_Weapon;
	map<string, LDYSkinnedMesh_Weapon*>::iterator	m_iterSkinnedMesh_Weapon;
public:
	void AddSkinnedMesh(const char* keyname,  char* foldername,  char* filename);
	void AddSkinnedMeshHead(const char* keyname,  char* foldername,  char* filename);
	void AddSkinnedWeapon(const char* keyname,  char* foldername,  char* filename);
	LDYSkinnedMesh*	GetSkinnedMesh(char* keyname);
	LDYSkinnedMesh_Head*	GetSkinnedMeshHead(char* keyname);
	LDYSkinnedMesh_Weapon*	GetSkinnedMeshWeapon(char* keyname);
	void Setup();
	void Destroy();
	TeicSkinnedMeshLoader();
	~TeicSkinnedMeshLoader();
};

