#pragma once

#include "cNode.h"

class cMainGame
{
private:
	enum
	{
		E_LEFT	= 1 << 0,	// 00000000 00000000 00000000 00000001
		E_RIGHT = 1 << 1,	// 00000000 00000000 00000000 00000010
		E_UP	= 1 << 2,	// 00000000 00000000 00000000 00000100
		E_DOWN	= 1 << 3,	// 00000000 00000000 00000000 00001000
	};

	typedef int eDirection;

	vector<cNode*>			m_vecNode;
	set<cNode*>				m_setOpenList;
	vector<eDirection>		m_vecDirection;
	map<eDirection, float>	m_mapDistance;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Render(HDC hdc);
	void FindPath();
	cNode* FindNode(cNode::eNodeType e);
	void InsertOpenList(cNode* pNode);
	cNode* MinFNodeAtOpenList();
	void MoveToCloseList(cNode* pNode);
	void Extend(cNode* pExtNode, cNode* pDestNode);
	cNode* GetAdjNode(cNode* pExtNode, eDirection e); // pExtNode의 e방향에 확장 대상 노드가 없는경우 NULL 리턴
	float CalcHeuristic(cNode* pCurrNode, cNode* pDestNode);
	void MarkNodeType(cNode* pDestNode);
};

