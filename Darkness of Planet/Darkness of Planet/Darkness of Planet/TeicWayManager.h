#pragma once
class TeicAstar;
class HankcGrid;
class HankcNode;
#include "cSingletonBase.h"

//###############################################
//#												#
//#		Astar 미리 찾아놓고	가져오는			#
//#		에이스타 매니저 입니다.					#
//#		2017-09-21								#	
//#	    AddWay 후 GetWay로 얻습니다	            #
//#		Key값 작성방법:							#
//#		SX숫자SZ숫자LX숫자LZ숫자				#
//#		S(숫자,숫자,숫자)L(숫자,숫자,숫자)		#
//#					Made by 태영				#
//###############################################

class TeicWayManager: public cSingletonBase<TeicWayManager>
{
private:
	map<string, vector<D3DXVECTOR3>> m_mapWay;
	map<string, vector<D3DXVECTOR3>>::iterator	m_iterWay;

	
public:
	void AddWay(const char* keyname, HankcGrid* Node, int StartX, int StartZ, int LastX, int LastZ);
	void AddWay2(const char* keyname, HankcGrid* Node, D3DXVECTOR3 start, D3DXVECTOR3 last);
	vector<D3DXVECTOR3>	GetWay(char* keyname);
	void Setup();
	void Destroy();
	TeicWayManager();
	~TeicWayManager();
};

