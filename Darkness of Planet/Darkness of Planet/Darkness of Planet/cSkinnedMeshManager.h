#pragma once

//############# 20170904 ############
// cSkinnedMesh, cSkinnedMeshManager, cAllocateHierarchy 추가
// 대원's
//###################################

#define SKINMANAGER cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;

class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;

public:
	SINGLETON(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFilename);
	
	void Destroy();
};

