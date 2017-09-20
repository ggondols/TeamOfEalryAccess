#include "stdafx.h"
#include "TeicHeightMapManager.h"
#include "cHeightMap.h"

void TeicHeightMapManager::AddHeightMap(string Keyname, string szFolder, string szFile, string szTexture, DWORD dwBytesPerPixel)
{
	m_iterHeightmap = m_mapHeightmap.find(Keyname);

	if (m_iterHeightmap != m_mapHeightmap.end())
	{
	}
	else
	{
		cHeightMap* temp = new cHeightMap;
		temp->Load(szFolder, szFile, szTexture, dwBytesPerPixel);
		m_mapHeightmap.insert(make_pair(Keyname, temp));
	}
}

cHeightMap * TeicHeightMapManager::GetHeightMap(char * Keyname)
{
	m_iterHeightmap = m_mapHeightmap.find(Keyname);

	//키 값 찾으면....
	if (m_iterHeightmap != m_mapHeightmap.end())
	{
		return m_iterHeightmap->second;
	}

	
	return NULL;
}

void TeicHeightMapManager::Setup()
{
}

void TeicHeightMapManager::Destroy()
{
	m_iterHeightmap = m_mapHeightmap.begin();

	for (; m_iterHeightmap != m_mapHeightmap.end();)
	{
		//지울 수 있으면 반복자 증가 x
		if (m_iterHeightmap->second != NULL)
		{
			delete m_iterHeightmap->second;
			m_iterHeightmap = m_mapHeightmap.erase(m_iterHeightmap);
		}
		else //반복자 증가!
		{
			++m_iterHeightmap;
		}
	}

	m_mapHeightmap.clear();
	
}

TeicHeightMapManager::TeicHeightMapManager()
{
}


TeicHeightMapManager::~TeicHeightMapManager()
{
}
