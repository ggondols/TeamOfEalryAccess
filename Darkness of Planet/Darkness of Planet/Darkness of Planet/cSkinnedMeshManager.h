#pragma once

//############# 20170904 ############
// cSkinnedMesh, cSkinnedMeshManager, cAllocateHierarchy �߰�
// ���'s
// 2017.09.11  -���¿�-
// TeicSkinnedMesh ���õ� m_mapTeiSkinnedMesh�� GetTeiSkinnedMesh �߰�
//###################################

#define SKINMANAGER cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;
class TeicSkinnedMesh;
class LDYSkinnedMesh;
class LDYSkinnedMesh_Weapon;
class LDYSkinnedMesh_Head;

class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;
	std::map<std::string, TeicSkinnedMesh*> m_mapTeiSkinnedMesh; //�߰� ���¿� 2017.09.11
	std::map<std::string, LDYSkinnedMesh*> m_mapLDYSkinnedMesh; //�߰� �̵��� 2017.09.15
	std::map<std::string, LDYSkinnedMesh_Weapon*> m_mapLDYSkinnedMesh_Weapon; //�߰� �̵��� 2017.09.15
	std::map<std::string, LDYSkinnedMesh_Head*> m_mapLDYSkinnedMesh_Head; //�߰� �̵��� 2017.09.15
public:
	SINGLETON(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFilename);
	TeicSkinnedMesh* GetTeiSkinnedMesh(char* szFolder, char* szFilename); //�߰� ���¿� 2017.09.11
	LDYSkinnedMesh* GetLDYSkinnedMesh(char* szFolder, char* szFilename); //�߰� �̵��� 2017.09.15
	LDYSkinnedMesh_Weapon* GetLDYSkinnedMesh_Weapon(char* szFolder, char* szFilename); //�߰� �̵��� 2017.09.15
	LDYSkinnedMesh_Head* GetLDYSkinnedMesh_Head(char* szFolder, char* szFilename); //�߰� �̵��� 2017.09.15
	void Destroy();
};

