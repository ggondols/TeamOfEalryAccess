#pragma once

//############# 20170904 ############
// cSkinnedMesh, cSkinnedMeshManager, cAllocateHierarchy 추가
// 대원's
// 2017.09.11  -김태영-
// TeicSkinnedMesh 관련된 m_mapTeiSkinnedMesh과 GetTeiSkinnedMesh 추가
//###################################

#define SKINMANAGER cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;
class TeicSkinnedMesh;
class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;
	std::map<std::string, TeicSkinnedMesh*> m_mapTeiSkinnedMesh; //추가 김태영 2017.09.11
public:
	SINGLETON(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFilename);
	TeicSkinnedMesh* GetTeiSkinnedMesh(char* szFolder, char* szFilename); //추가 김태영 2017.09.11
	void Destroy();
};

