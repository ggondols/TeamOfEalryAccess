#pragma once
#include "cSingletonBase.h"
class LDYSkinnedMesh;
class LDYSkinnedMesh_Head;
class LDYSkinnedMesh_Weapon;
class TeicSkinnedMesh;
//###############################################
//#												#
//#		캐릭 관련 메쉬 가져오는					#
//#		메쉬 로더 입니다.						#
//#		2017-09-21								#	
//#	    GetSkinnedMesh,	AddSkinnedMesh			#
//#	    GetSkinnedMeshHead,	AddSkinnedMeshHead	#
//#	    GetSkinnedMeshWeapon,	AddSkinnedWeapon#
//#     를 이용합니다							#
//#     캐릭터 클래스에서 메쉬 delete해주세요	#
//#					Made by 태영				#
//###############################################
class TeicSkinnedMeshLoader:public cSingletonBase<TeicSkinnedMeshLoader>
{
private:
	map<string, LDYSkinnedMesh*>					m_mapSkinnedMesh;
	map<string, LDYSkinnedMesh*>::iterator			m_iterSkinnedMesh;
	map<string, TeicSkinnedMesh*>					m_mapSkinnedMeshTei;
	map<string, TeicSkinnedMesh*>::iterator			m_iterSkinnedMeshTei;
	map<string, LDYSkinnedMesh_Head*>				m_mapSkinnedMesh_Head;
	map<string, LDYSkinnedMesh_Head*>::iterator		m_iterSkinnedMesh_Head;
	map<string, LDYSkinnedMesh_Weapon*>				m_mapSkinnedMesh_Weapon;
	map<string, LDYSkinnedMesh_Weapon*>::iterator	m_iterSkinnedMesh_Weapon;
	map<string, LDYSkinnedMesh*>					m_mapStaticMesh;
	map<string, LDYSkinnedMesh*>::iterator			m_iterSmapStaticMesh;
public:
	void AddSkinnedMesh(const char* keyname,  char* foldername,  char* filename);
	void AddSkinnedMeshTei(const char * keyname, char * foldername, char * filename);
	void AddSkinnedMeshHead(const char* keyname,  char* foldername,  char* filename);
	void AddSkinnedWeapon(const char* keyname,  char* foldername,  char* filename);
	LDYSkinnedMesh*	GetSkinnedMesh(char* keyname);
	TeicSkinnedMesh * GetSkinnedMeshTei(char * keyname);
	LDYSkinnedMesh_Head*	GetSkinnedMeshHead(char* keyname);
	LDYSkinnedMesh_Weapon*	GetSkinnedMeshWeapon(char* keyname);

	//스킨드 메쉬 get statiac Mesh
	LDYSkinnedMesh*	GetStaticMesh(char* keyname);

	void Setup();
	void Destroy();
	TeicSkinnedMeshLoader();
	~TeicSkinnedMeshLoader();
};

