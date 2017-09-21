#pragma once
class cHeightMap;
#include "cSingletonBase.h"


//###############################################
//#												#
//#		하이트맵 가져오는						#
//#		하이트맵 매니저 입니다.					#
//#		2017-09-21								#	
//#	    AddHeightMap 후 GetHeightMap로 얻습니다	#
//#					Made by 태영				#
//###############################################


class TeicHeightMapManager :
	public cSingletonBase<TeicHeightMapManager>
{
private:
	map<string, cHeightMap*>			m_mapHeightmap;
	map<string, cHeightMap*>::iterator  m_iterHeightmap;
public:
	void AddHeightMap(string Keyname, string szFolder, string szFile, string szTexture, DWORD dwBytesPerPixel/* = 1*/);
	cHeightMap* GetHeightMap(char* Keyname);
public:
	void Setup();
	void Destroy();
public:
	TeicHeightMapManager();
	~TeicHeightMapManager();
};

