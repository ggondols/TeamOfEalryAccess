#include "stdafx.h"
#include "TeicWayManager.h"
#include "TeicAstar.h"
#include "HankcNode.h"
#include "HankcGrid.h"

void TeicWayManager::AddWay(const char * keyname, HankcGrid * Node, int StartX, int StartZ, int LastX, int LastZ)
{
	m_iterWay = m_mapWay.find(keyname);
	
	if (m_iterWay != m_mapWay.end())
	{
	}
	else
	{
		TeicAstar	temp;
		temp.Setup(Node);
		vector<D3DXVECTOR3> way = temp.FindWay(StartX, StartZ, LastX, LastZ);
		m_mapWay.insert(make_pair(keyname, way));
	}
	
}

void TeicWayManager::AddWay2(const char * keyname, HankcGrid * Node, D3DXVECTOR3 start, D3DXVECTOR3 last)
{
	m_iterWay = m_mapWay.find(keyname);

	if (m_iterWay != m_mapWay.end())
	{
	}
	else
	{
		TeicAstar	temp;
		temp.Setup(Node);
		vector<D3DXVECTOR3> way = temp.FindWay(start,last);
		m_mapWay.insert(make_pair(keyname, way));
	}
}

vector<D3DXVECTOR3> TeicWayManager::GetWay(char * keyname)
{
	m_iterWay = m_mapWay.find(keyname);

	//키 값 찾으면....
	if (m_iterWay != m_mapWay.end())
	{
		return m_iterWay->second;
	}


	return vector<D3DXVECTOR3>();
}

void TeicWayManager::Setup()
{
}

void TeicWayManager::Destroy()
{
}


TeicWayManager::TeicWayManager()
{
}


TeicWayManager::~TeicWayManager()
{
}
