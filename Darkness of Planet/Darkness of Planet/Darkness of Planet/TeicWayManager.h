#pragma once
class TeicAstar;
class HankcGrid;
class HankcNode;
#include "cSingletonBase.h"
class TeicWayManager: public cSingletonBase<TeicWayManager>
{
private:
	map<string, vector<D3DXVECTOR3>> m_mapWay;
	map<string, vector<D3DXVECTOR3>>::iterator	m_iterWay;

	
public:
	void AddWay(char* keyname, HankcGrid* Node, int StartX, int StartZ, int LastX, int LastZ);
	vector<D3DXVECTOR3>	GetWay(char* keyname);
	void Setup();
	void Destroy();
	TeicWayManager();
	~TeicWayManager();
};

