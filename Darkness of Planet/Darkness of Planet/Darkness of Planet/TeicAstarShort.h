#pragma once
class HankcGrid;
class HankcNode;


//###############################################
//#												#
//#		이동 경로 탐색하는						#
//#		Astar 클래스 입니다.					#
//#		기존 Astar에서 연산속도가 향상된		#
//#     버전입니다								#
//#		2017-09-15								#	
//#	    setup 후 Findway로 찾습니다 			#
//#					Made by 태영				#
//###############################################

#include "TeicMinHeap.h"
class TeicAstarShort
{
private:
	int			m_iLimit;
	HankcGrid* m_Node;

	vector<HankcNode*>		    m_vecCloselist;
	TeicMinHeap<HankcNode*>		m_vecOpenlist;

	vector<D3DXVECTOR3>  m_Way;
	float			     m_distance;


public:
	void Setup(HankcGrid* Node);
	vector<D3DXVECTOR3> FindWay(int StartX, int StartZ, int LastX, int LastZ);
	vector<D3DXVECTOR3> SetWay(int Start, int Last);
	bool Check(int x, int y);
	HankcNode* GetNode(int x , int y);
public:
	TeicAstarShort();
	~TeicAstarShort();
};




