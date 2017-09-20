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

	//Ű �� ã����....
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
		//���� �� ������ �ݺ��� ���� x
		if (m_iterHeightmap->second != NULL)
		{
			delete m_iterHeightmap->second;
			m_iterHeightmap = m_mapHeightmap.erase(m_iterHeightmap);
		}
		else //�ݺ��� ����!
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
