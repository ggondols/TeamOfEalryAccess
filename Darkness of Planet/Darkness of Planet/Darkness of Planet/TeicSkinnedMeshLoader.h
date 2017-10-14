#pragma once
#include "cSingletonBase.h"
class LDYSkinnedMesh;
class LDYSkinnedMesh_Head;
class LDYSkinnedMesh_Weapon;
class TeicSkinnedMesh;
//###############################################
//#												#
//#		ĳ�� ���� �޽� ��������					#
//#		�޽� �δ� �Դϴ�.						#
//#		2017-09-21								#	
//#	    GetSkinnedMesh,	AddSkinnedMesh			#
//#	    GetSkinnedMeshHead,	AddSkinnedMeshHead	#
//#	    GetSkinnedMeshWeapon,	AddSkinnedWeapon#
//#     �� �̿��մϴ�							#
//#     ĳ���� Ŭ�������� �޽� delete���ּ���	#
//#					Made by �¿�				#
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

	//��Ų�� �޽� get statiac Mesh
	LDYSkinnedMesh*	GetStaticMesh(char* keyname);

	void Setup();
	void Destroy();
	TeicSkinnedMeshLoader();
	~TeicSkinnedMeshLoader();
};

