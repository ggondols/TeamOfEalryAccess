#pragma once
class cHeightMap;
#include "cSingletonBase.h"
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

