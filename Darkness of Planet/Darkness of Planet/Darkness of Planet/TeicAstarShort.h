#pragma once
class HankcGrid;
class HankcNode;


//###############################################
//#												#
//#		�̵� ��� Ž���ϴ�						#
//#		Astar Ŭ���� �Դϴ�.					#
//#		���� Astar���� ����ӵ��� ����		#
//#     �����Դϴ�								#
//#		2017-09-15								#	
//#	    setup �� Findway�� ã���ϴ� 			#
//#					Made by �¿�				#
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




