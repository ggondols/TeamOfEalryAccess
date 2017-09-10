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
class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;
	std::map<std::string, TeicSkinnedMesh*> m_mapTeiSkinnedMesh; //�߰� ���¿� 2017.09.11
public:
	SINGLETON(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFilename);
	TeicSkinnedMesh* GetTeiSkinnedMesh(char* szFolder, char* szFilename); //�߰� ���¿� 2017.09.11
	void Destroy();
};

