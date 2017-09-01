#include "StdAfx.h"
#include "cMainGame.h"


//졸라 짱쎈 투명 드래곤이 포요 했다 크와아아아아아아
//그러자 흑발의 적안인 그가 말했다.
//오른손의 흑염룡이 날뛰려 하는 군 흐흐흐흐흐흐

cMainGame::cMainGame(void)
{
}


cMainGame::~cMainGame(void)
{
}

void cMainGame::Setup()
{
	map<char, cNode::eNodeType> mapNodeType;
	mapNodeType['E'] = cNode::E_EMPTY;
	mapNodeType['B'] = cNode::E_WALL;
	mapNodeType['S'] = cNode::E_START;
	mapNodeType['D'] = cNode::E_DEST;

	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);	// 00000101
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);

	m_mapDistance[E_LEFT] = 1.0f;
	m_mapDistance[E_RIGHT] = 1.0f;
	m_mapDistance[E_UP] = 1.0f;
	m_mapDistance[E_DOWN] = 1.0f;
	m_mapDistance[E_LEFT | E_UP	] = sqrt(2.0f);
	m_mapDistance[E_LEFT | E_DOWN] = sqrt(2.0f);
	m_mapDistance[E_RIGHT | E_UP] = sqrt(2.0f);
	m_mapDistance[E_RIGHT | E_DOWN] = sqrt(2.0f);

	FILE* fp = 0;
	fopen_s(&fp, "astar.txt", "r");
	while(!feof(fp))
	{
		char c = fgetc(fp);
		if(c < 33) continue;

		cNode* pNode = new cNode(m_vecNode.size(), mapNodeType[c]);
		m_vecNode.push_back(pNode);
	}
	fclose(fp);

	FindPath();
}

void cMainGame::Render(HDC hdc)
{
	for each(auto p in m_vecNode)
	{
		p->Render(hdc);
	}
}

void cMainGame::FindPath()
{
	cNode* pStartNode = FindNode(cNode::E_START);
	cNode* pDestNode = FindNode(cNode::E_DEST);

	pStartNode->G = 0.0f;
	pStartNode->H = CalcHeuristic(pStartNode, pDestNode);
	pStartNode->F = pStartNode->G + pStartNode->H;

	InsertOpenList(pStartNode);

	while(true)
	{
		cNode* pMinFNode = MinFNodeAtOpenList();
		if (pMinFNode == NULL)
		{
			// 길없음
			MarkNodeType(pDestNode);
			break;
		}

		MoveToCloseList(pMinFNode); 

		Extend(pMinFNode, pDestNode);

		if (pMinFNode == pDestNode)
		{
			//길찾음
			MarkNodeType(pDestNode);
			break;
		}
	}
}

cNode* cMainGame::FindNode( cNode::eNodeType e )
{
	for each(auto p in m_vecNode)
	{
		if(p->m_eNodeType == e) return p;
	}
	return NULL;
}

void cMainGame::InsertOpenList( cNode* pNode )
{
	pNode->m_isOpen = true;
	m_setOpenList.insert(pNode);
}

cNode* cMainGame::MinFNodeAtOpenList()
{
	cNode* pMinFNode = NULL;
	float fMinF = FLT_MAX;
	for each(auto p in m_setOpenList)
	{
		if (p->F < fMinF)
		{
			pMinFNode = p;
			fMinF = p->F;
		}
	}
	return pMinFNode;
}

void cMainGame::MoveToCloseList( cNode* pNode )
{
	m_setOpenList.erase(pNode);
	pNode->m_isOpen = false;
	pNode->m_isClose = true;
}

void cMainGame::Extend( cNode* pExtNode, cNode* pDestNode )
{
	for each(eDirection e in m_vecDirection)
	{
		cNode* pAdjNode = GetAdjNode(pExtNode, e);
	
		if (pAdjNode == NULL) continue;

		if (pAdjNode->m_isOpen)
		{
			if (pAdjNode->G > pExtNode->G + m_mapDistance[e])
			{
				pAdjNode->G = pExtNode->G + m_mapDistance[e];
				pAdjNode->F = pAdjNode->G + pAdjNode->H;
				pAdjNode->m_pParent = pExtNode;
			}
		}
		else
		{
			pAdjNode->G = pExtNode->G + m_mapDistance[e];
			pAdjNode->H = CalcHeuristic(pAdjNode, pDestNode);
			pAdjNode->F = pAdjNode->G + pAdjNode->H;
			pAdjNode->m_pParent = pExtNode;
			InsertOpenList(pAdjNode);
		}
	}
}

cNode* cMainGame::GetAdjNode( cNode* pExtNode, eDirection e )
{
	int nAdjRow = pExtNode->m_nRow;
	int nAdjCol = pExtNode->m_nCol;
	if (e & E_LEFT)
	{
		if (pExtNode->m_nCol == 0) return NULL;
		int nIndex = pExtNode->m_nRow * TILE_N + pExtNode->m_nCol - 1;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjCol -= 1;
	}
	else if (e & E_RIGHT)
	{
		if (pExtNode->m_nCol == TILE_N - 1) return NULL;
		int nIndex = pExtNode->m_nRow * TILE_N + pExtNode->m_nCol + 1;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjCol += 1;
	}

	if (e & E_UP)
	{
		if (pExtNode->m_nRow == 0) return NULL;
		int nIndex = (pExtNode->m_nRow - 1) * TILE_N + pExtNode->m_nCol;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjRow -= 1;
	}
	else if (e & E_DOWN)
	{
		if (pExtNode->m_nRow == TILE_N - 1) return NULL;
		int nIndex = (pExtNode->m_nRow + 1) * TILE_N + pExtNode->m_nCol;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjRow += 1;
	}

	int nIndex = nAdjRow * TILE_N + nAdjCol;
	if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
	if (m_vecNode[nIndex]->m_isClose) return NULL;

	return m_vecNode[nIndex];
}

float cMainGame::CalcHeuristic( cNode* pCurrNode, cNode* pDestNode )
{
	return fabs((float)pCurrNode->m_nRow - pDestNode->m_nRow) +
		fabs((float)pCurrNode->m_nCol - pDestNode->m_nCol);
}

void cMainGame::MarkNodeType( cNode* pDestNode )
{
	for each(auto p in m_vecNode)
	{
		if(p->m_isOpen) p->m_eNodeType = cNode::E_OPEN;
		if(p->m_isClose) p->m_eNodeType = cNode::E_CLOSE;
	}
	cNode* pTemp = pDestNode;
	while(pTemp)
	{
		pTemp->m_eNodeType = cNode::E_PATH;
		pTemp = pTemp->m_pParent;
	}
}
