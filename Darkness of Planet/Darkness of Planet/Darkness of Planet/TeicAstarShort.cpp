#include "stdafx.h"
#include "TeicAstarShort.h"
#include "HankcNode.h"

TeicAstarShort::TeicAstarShort()
{
}


TeicAstarShort::~TeicAstarShort()
{
}


void TeicAstarShort::Setup(HankcGrid* Node)
{
	m_Node = Node;
}

vector<D3DXVECTOR3> TeicAstarShort::FindWay(int StartX, int StartZ, int LastX, int LastZ)
{
	m_iLimit = 0;
	m_Way.clear();
	m_vecCloselist.clear();
	m_vecOpenlist.Clear();

	if (StartX == LastX && StartZ == LastZ)
	{
		return m_Way;
	}

	HankcNode* temp = new HankcNode;


	
	temp->m_vPosList = new nPositionInfo();
	temp->m_pBoundInfo = new nNodeBoundInfo;
	temp->m_pAstarNode = new HankcAstarNode;
	temp = &m_Node->m_vRow[StartZ].m_vCol[StartX];
	m_vecCloselist.push_back(temp);

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{


			if (StartZ + i < 0 || StartZ + i >= (*m_Node).m_vRow.size() ||
				StartX + j < 0 || StartX + j >= (*m_Node).m_vRow.size()) continue;
			if ((*m_Node)[StartZ + i][StartX + j].m_pBoundInfo != NULL &&
				(*m_Node)[StartZ + i][StartX + j].m_pBoundInfo->m_vecBounding.size() != 0)continue;
			if (Check(StartX + j, StartZ + i))continue;
			if (StartZ - 1 >= 0)
			{
				if ((*m_Node)[StartZ - 1][StartX].m_pBoundInfo != NULL &&
					(*m_Node)[StartZ - 1][StartX].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					if (i == -1)continue;
				}
			}
			if (StartZ + 1 <  (*m_Node).m_vRow.size())
			{
				if ((*m_Node)[StartZ + 1][StartX].m_pBoundInfo != NULL &&
					(*m_Node)[StartZ + 1][StartX].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					if (i == 1)continue;
				}
			}
			if (StartX - 1 >= 0)
			{
				if ((*m_Node)[StartZ][StartX - 1].m_pBoundInfo != NULL &&
					(*m_Node)[StartZ][StartX - 1].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					if (j == -1)continue;
				}
			}
			if (StartX + 1 <  (*m_Node).m_vRow.size())
			{
				if ((*m_Node)[StartZ][StartX + 1].m_pBoundInfo != NULL &&
					(*m_Node)[StartZ][StartX + 1].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					if (j == 1)continue;
				}
			}

			float G = D3DXVec3Length(&((*m_Node)[StartZ + i][StartX + j].m_vPosList->m_vCenterPos - (*m_Node)[StartZ][StartX].m_vPosList->m_vCenterPos));
			temp = &(*m_Node)[StartZ + i][StartX + j];
			temp->m_pAstarNode->m_fG = G;
			temp->m_pAstarNode->m_pParent = &m_Node->m_vRow[StartZ].m_vCol[StartX];
			temp->m_pAstarNode->m_fH = D3DXVec3Length(&((*m_Node)[LastZ][LastX].m_vPosList->m_vCenterPos
				- (*m_Node)[StartZ + i][StartX + j].m_vPosList->m_vCenterPos));
			temp->m_pAstarNode->m_fF = G + temp->m_pAstarNode->m_fH;
			
			m_vecOpenlist.AddHeap(temp);
			

		}
	}

	while (1)
	{
		m_iLimit++;
		if (m_iLimit > 100)
		{

			m_Way.clear();
			return m_Way;
		}
		HankcNode* now = m_vecOpenlist.GetminHeap();
		if (now->m_pAstarNode->m_iIndex == -1000)
		{
			break;

		}
		m_vecCloselist.push_back(now);


		if (now->m_nFrameX == LastX &&now->m_nFrameZ == LastZ)
		{
			break;
		}
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{

				if (now->m_nFrameZ + i < 0 || now->m_nFrameZ + i >= (*m_Node).m_vRow.size() ||
					now->m_nFrameX + j < 0 || now->m_nFrameX + j >= (*m_Node).m_vRow.size()) continue;
				if ((*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_pBoundInfo != NULL &&
					(*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_pBoundInfo->m_vecBounding.size() != 0)continue;
				
				if (Check(now->m_nFrameX + j, now->m_nFrameZ + i) )continue;
				if (now->m_nFrameZ - 1 >= 0)
				{
					if ((*m_Node)[now->m_nFrameZ - 1][now->m_nFrameX].m_pBoundInfo != NULL &&
						(*m_Node)[now->m_nFrameZ - 1][now->m_nFrameX].m_pBoundInfo->m_vecBounding.size() != 0)
					{
						if (i == -1)continue;
					}
				}
				if (now->m_nFrameZ + 1 <(*m_Node).m_vRow.size())
				{
					if ((*m_Node)[now->m_nFrameZ + 1][now->m_nFrameX].m_pBoundInfo != NULL &&
						(*m_Node)[now->m_nFrameZ + 1][now->m_nFrameX].m_pBoundInfo->m_vecBounding.size() != 0)
					{
						if (i == 1)continue;
					}
				}
				if (now->m_nFrameX - 1 - 1 >= 0)
				{
					if ((*m_Node)[now->m_nFrameZ][now->m_nFrameX - 1].m_pBoundInfo != NULL &&
						(*m_Node)[now->m_nFrameZ][now->m_nFrameX - 1].m_pBoundInfo->m_vecBounding.size() != 0)
					{
						if (j == -1)continue;
					}
				}
				if (now->m_nFrameX + 1 < (*m_Node).m_vRow.size())
				{
					if ((*m_Node)[now->m_nFrameZ][now->m_nFrameX + 1].m_pBoundInfo != NULL &&
						(*m_Node)[now->m_nFrameZ][now->m_nFrameX + 1].m_pBoundInfo->m_vecBounding.size() != 0)
					{
						if (j == 1)continue;
					}
				}


				float G = D3DXVec3Length(&((*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_vPosList->m_vCenterPos
					- (*m_Node)[now->m_nFrameZ][now->m_nFrameX].m_vPosList->m_vCenterPos));

				HankcNode*  CalNode = new HankcNode;
				CalNode->m_vPosList = new nPositionInfo();
				CalNode->m_pBoundInfo = new nNodeBoundInfo;
				CalNode->m_pAstarNode = new HankcAstarNode;

				CalNode=m_vecOpenlist.FindNode(now->m_nFrameX + j, now->m_nFrameZ + i);
				
				float NowG = CalNode->m_pAstarNode->m_fG;
				float ParentG = now->m_pAstarNode->m_fG;
				if (NowG > G + ParentG)
				{
					if (CalNode->m_pAstarNode->m_iIndex == -1000)
					{
						CalNode = &(*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j];
						CalNode->m_pAstarNode->m_fG= G + ParentG;
						CalNode->m_pAstarNode->m_pParent = now;
						CalNode->m_pAstarNode->m_fH = D3DXVec3Length(&((*m_Node)[LastZ][LastX].m_vPosList->m_vCenterPos
							- (*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_vPosList->m_vCenterPos));
						CalNode->m_pAstarNode->m_fF = CalNode->m_pAstarNode->m_fG + CalNode->m_pAstarNode->m_fH;
						
						
						m_vecOpenlist.AddHeap(CalNode);
					}
					else
					{
						CalNode->m_pAstarNode->m_fG = G + ParentG;
						CalNode->m_pAstarNode->m_pParent = now;
						CalNode->m_pAstarNode->m_fH = D3DXVec3Length(&((*m_Node)[LastZ][LastX].m_vPosList->m_vCenterPos
							- (*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_vPosList->m_vCenterPos));
						CalNode->m_pAstarNode->m_fF = CalNode->m_pAstarNode->m_fG + CalNode->m_pAstarNode->m_fH;
						m_vecOpenlist.Setting(CalNode->m_pAstarNode->m_iIndex);



					}

				}


			}
		}

	}

	vector<D3DXVECTOR3> opposite;
	HankcNode* now = GetNode(LastX, LastZ);
	while (1)
	{

		opposite.push_back(now->m_vPosList->m_vCenterPos);
		if (now->m_nFrameZ == StartZ && now->m_nFrameX == StartX)
			break;

		now = now->m_pAstarNode->m_pParent;

	}
	int count = 0;
	m_Way.resize(opposite.size());
	for (int i = opposite.size() - 1; i >= 0; i--)
	{
		m_Way[count] = opposite[i];
		count++;
	}






	return m_Way;
}

vector<D3DXVECTOR3> TeicAstarShort::SetWay(int Start, int Last)
{

	vector<D3DXVECTOR3> temp;
	bool Put = false;
	if (Start >= Last)
	{
		temp.push_back(m_Way[Start]);
		m_Way = temp;
		return m_Way;
	}
	for (int i = 0; i < m_Way.size(); i++)
	{
		if (i == Start)
		{
			Put = true;
		}
		if (Put)
		{
			temp.push_back(m_Way[i]);
		}
		if (i == Last)
		{
			Put = false;
		}
	}
	m_Way = temp;
	return m_Way;

}

bool TeicAstarShort::Check(int x, int y)
{
	for (int i = 0; i < m_vecCloselist.size(); i++)
	{
		if (m_vecCloselist[i]->m_nFrameX == x &&
			m_vecCloselist[i]->m_nFrameZ == y)
		{
			return true;
		}
	}
	return false;
}

HankcNode * TeicAstarShort::GetNode(int x, int y)
{
	for (int i = 0; i < m_vecCloselist.size(); i++)
	{
		if (m_vecCloselist[i]->m_nFrameX == x &&
			m_vecCloselist[i]->m_nFrameZ == y)
		{
			return m_vecCloselist[i];
		}
	}
	return NULL;
}



