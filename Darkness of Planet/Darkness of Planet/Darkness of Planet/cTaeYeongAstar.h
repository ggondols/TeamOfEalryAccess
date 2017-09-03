#pragma once
#include "cTaeYeongMinHeap.h"
class cTaeYeongAstar
{
private:

	cTaeYeongMinHeap<Node*>		m_Selected;
	vector<vector<Node>>* m_Node;
	//	vector<Node*>		 m_Selected;
	vector<D3DXVECTOR3>  m_Way;
	float			     m_distance;
public:
	void Setup(vector<vector<Node>>* Node);
	vector<D3DXVECTOR3> FindWay(int StartX, int StartZ, int LastX, int LastZ);
	vector<D3DXVECTOR3> SetWay(int Start, int Last);
	Node* FindLow(vector<Node*>& selected);

public:
	cTaeYeongAstar();
	~cTaeYeongAstar();
};

