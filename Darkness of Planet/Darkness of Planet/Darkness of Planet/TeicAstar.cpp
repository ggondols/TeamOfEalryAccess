#include "stdafx.h"
#include "TeicAstar.h"
#include "HankcNode.h"
//테스트용
TeicAstar::TeicAstar()
{
}


TeicAstar::~TeicAstar()
{
}
void TeicAstar::Setup(HankcGrid* Node)
{

	m_Node = Node;

	m_vecF.resize(m_Node->m_vRow.size());
	m_vecH.resize(m_Node->m_vRow.size());
	m_vecParent.resize(m_Node->m_vRow.size());

	for (int i = 0; i < m_vecF.size(); i++)
	{
		m_vecF[i].resize(m_Node->m_vRow[i].m_vCol.size(), INF);
		m_vecH[i].resize(m_Node->m_vRow[i].m_vCol.size(), INF);
		m_vecParent[i].resize(m_Node->m_vRow[i].m_vCol.size(), NULL);

	}
}

vector<D3DXVECTOR3> TeicAstar::FindWay(int StartX, int StartZ, int LastX, int LastZ)
{

	m_Way.clear();

	vector<vector<bool>>		m_vecClose;
	vector<vector<float>>		m_vecG;
	vector<HankcNode*>			m_vecSelected;

	m_vecClose.resize(m_Node->m_vRow.size());
	m_vecG.resize(m_Node->m_vRow.size());

	for (int i = 0; i < m_vecClose.size(); i++)
	{
		m_vecClose[i].resize(m_Node->m_vRow[i].m_vCol.size(), false);
		m_vecG[i].resize(m_Node->m_vRow[i].m_vCol.size(), INF);

	}

	//m_vecSelected.clear();


	////for (int i = 0; i <m_vecClose.size(); i++)
	////{
	////	for (int j = 0; j <m_vecClose[i].size(); j++)
	////	{
	////		m_vecClose[i][j] = false;
	////		//		m_vecF			[i][j]		= INF;
	////		m_vecG[i][j] = INF;
	////		//		m_vecH			[i][j]		= INF;
	////		m_vecParent[i][j] = NULL;
	////	}

	////}
	//for (int i = 0; i < m_vecClose.size(); i++)
	//{
	//	for (int j = 0; j < m_vecClose[i].size(); j++)
	//	{
	//		m_vecClose[i][j] = false;
	//		m_vecG[i][j] = INF;
	//	}
	//}


	if (StartX == LastX && StartZ == LastZ)
	{
		return m_Way;
	}



	m_vecClose[StartZ][StartX] = true;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{


			if (StartZ + i < 0 || StartZ + i >= m_vecClose.size() ||
				StartX + j < 0 || StartX + j >= m_vecClose[0].size()) continue;
			if ((*m_Node)[StartZ + i][StartX + j].m_pBoundInfo != NULL &&
				(*m_Node)[StartZ + i][StartX + j].m_pBoundInfo->m_vecBounding.size() != 0)continue;
			if (m_vecClose[StartZ + i][StartX + j] == true) continue;
			if (StartZ - 1 >= 0)
			{
				if ((*m_Node)[StartZ - 1][StartX].m_pBoundInfo != NULL &&
					(*m_Node)[StartZ - 1][StartX].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					if (i == -1)continue;
				}
			}
			if (StartZ + 1 < m_vecClose[0].size())
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
			if (StartX + 1 < m_vecClose[0].size())
			{
				if ((*m_Node)[StartZ][StartX + 1].m_pBoundInfo != NULL &&
					(*m_Node)[StartZ][StartX + 1].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					if (j == 1)continue;
				}
			}

			float G = D3DXVec3Length(&((*m_Node)[StartZ + i][StartX + j].m_vPosList->m_vCenterPos - (*m_Node)[StartZ][StartX].m_vPosList->m_vCenterPos));


			m_vecG[StartZ + i][StartX + j] = G;
			m_vecParent[StartZ + i][StartX + j] = &m_Node->m_vRow[StartZ].m_vCol[StartX];
			m_vecH[StartZ + i][StartX + j] = D3DXVec3Length(&((*m_Node)[LastZ][LastX].m_vPosList->m_vCenterPos
				- (*m_Node)[StartZ + i][StartX + j].m_vPosList->m_vCenterPos));
			m_vecF[StartZ + i][StartX + j] = m_vecG[StartZ + i][StartX + j] + m_vecH[StartZ + i][StartX + j];
			m_vecSelected.push_back(&m_Node->m_vRow[StartZ + i].m_vCol[StartX + j]);



		}
	}

	while (1)
	{
		HankcNode* now = FindLow(m_vecSelected, m_vecClose, m_vecF);
		if (!now)
			break;
		m_vecClose[now->m_nFrameZ][now->m_nFrameX] = true;

		if (m_vecH[now->m_nFrameZ][now->m_nFrameX] < 0.00001)
		{
			break;
		}
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{

				if (now->m_nFrameZ + i < 0 || now->m_nFrameZ + i >= m_vecClose.size() ||
					now->m_nFrameX + j < 0 || now->m_nFrameX + j >= m_vecClose[0].size()) continue;
				if ((*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_pBoundInfo != NULL &&
					(*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_pBoundInfo->m_vecBounding.size() != 0)continue;
				if (m_vecClose[now->m_nFrameZ + i][now->m_nFrameX + j] == true) continue;
				if (now->m_nFrameZ - 1 >= 0)
				{
					if ((*m_Node)[now->m_nFrameZ - 1][now->m_nFrameX].m_pBoundInfo != NULL &&
						(*m_Node)[now->m_nFrameZ - 1][now->m_nFrameX].m_pBoundInfo->m_vecBounding.size() != 0)
					{
						if (i == -1)continue;
					}
				}
				if (now->m_nFrameZ + 1 < m_vecClose[0].size())
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
				if (now->m_nFrameX + 1 < m_vecClose[0].size())
				{
					if ((*m_Node)[now->m_nFrameZ][now->m_nFrameX + 1].m_pBoundInfo != NULL &&
						(*m_Node)[now->m_nFrameZ][now->m_nFrameX + 1].m_pBoundInfo->m_vecBounding.size() != 0)
					{
						if (j == 1)continue;
					}
				}


				float G = D3DXVec3Length(&((*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_vPosList->m_vCenterPos - (*m_Node)[now->m_nFrameZ][now->m_nFrameX].m_vPosList->m_vCenterPos));

				if (m_vecG[now->m_nFrameZ + i][now->m_nFrameX + j] > G + m_vecG[now->m_nFrameZ][now->m_nFrameX])
				{
					m_vecG[now->m_nFrameZ + i][now->m_nFrameX + j] = G + m_vecG[now->m_nFrameZ][now->m_nFrameX];
					m_vecParent[now->m_nFrameZ + i][now->m_nFrameX + j] = &m_Node->m_vRow[now->m_nFrameZ].m_vCol[now->m_nFrameX];
					m_vecH[now->m_nFrameZ + i][now->m_nFrameX + j] = D3DXVec3Length(&((*m_Node)[LastZ][LastX].m_vPosList->m_vCenterPos
						- (*m_Node)[now->m_nFrameZ + i][now->m_nFrameX + j].m_vPosList->m_vCenterPos));
					m_vecF[now->m_nFrameZ + i][now->m_nFrameX + j] = m_vecG[now->m_nFrameZ + i][now->m_nFrameX + j] + m_vecH[now->m_nFrameZ + i][now->m_nFrameX + j];
					m_vecSelected.push_back(&m_Node->m_vRow[now->m_nFrameZ + i].m_vCol[now->m_nFrameX + j]);

				}






			}
		}

	}

	vector<D3DXVECTOR3> opposite;
	HankcNode* now = &(*m_Node)[LastZ][LastX];
	while (1)
	{

		opposite.push_back(now->m_vPosList->m_vCenterPos);
		if (now->m_nFrameZ == StartZ && now->m_nFrameX == StartX)
			break;
		now = m_vecParent[now->m_nFrameZ][now->m_nFrameX];
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

vector<D3DXVECTOR3> TeicAstar::SetWay(int Start, int Last)
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


HankcNode * TeicAstar::FindLow(vector<HankcNode*>& selected, vector<vector<bool>>& close,
	vector<vector<float>>& F)
{
	if (selected.size() <= 0)return NULL;
	float Min = INF;
	HankcNode* temp = NULL;
	for (int i = 0; i < selected.size(); i++)
	{
		if (close[selected[i]->m_nFrameZ][selected[i]->m_nFrameX])
		{
			selected.erase(selected.begin() + i);
		}
		else if (F[selected[i]->m_nFrameZ][selected[i]->m_nFrameX] < Min)
		{
			Min = F[selected[i]->m_nFrameZ][selected[i]->m_nFrameX];
			temp = selected[i];
		}
	}

	return temp;
}





