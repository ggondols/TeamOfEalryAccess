#include "stdafx.h"
#include "TeicAstar.h"
#include "HankcNode.h"

TeicAstar::TeicAstar()
{
}


TeicAstar::~TeicAstar()
{
}
//void TeicAstar::Setup(HankcGrid* Node)
//{
//
//	m_Node = Node;
//	
//}
//
//vector<D3DXVECTOR3> TeicAstar::FindWay(int StartX, int StartZ, int LastX, int LastZ)
//{
//	m_Way.clear();
//	m_Selected.clear();
//	///////[col ==i][row==j]
//	for (int i = 0; i < m_Node->m_vCol.size(); i++)
//	{
//		for (int j = 0; j < m_Node->m_vCol[i].m_vRow.size(); j++)
//		{
//			(*m_Node)[i][j].m_vecClose = false;
//			(*m_Node)[i][j].m_vecF = INF;
//			(*m_Node)[i][j].m_vecG = INF;
//			(*m_Node)[i][j].m_vecH = INF;
//			(*m_Node)[i][j].st_parent = NULL;
//		}
//
//	}
//	(*m_Node)[StartZ][StartX].m_vecClose = true;
//	for (int i = 0; i < (*m_Node)[StartZ][StartX].st_neighbor.size(); i++)
//	{
//		if ((*m_Node)[StartZ][StartX].st_neighbor[i]->st_type != WALL)
//		{
//
//			float G = D3DXVec3Length(&((*m_Node)[StartZ][StartX].st_position - (*m_Node)[StartZ][StartX].st_neighbor[i]->st_position));
//
//			////////////벽에 대한 정확도 떨어질수도
//			////////// 근처에 벽이 있으면 대각선 전부 막아버림 (완벽하지 않다.)
//			if (G > m_distance + 0.00001)
//			{
//				for (int j = 0; j < (*m_Node)[StartZ][StartX].st_neighbor.size(); j++)
//				{
//					if ((*m_Node)[StartZ][StartX].st_neighbor[j]->st_type == WALL)
//					{
//						continue;
//					}
//				}
//			}
//
//			(*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecG = G;
//			(*m_Node)[StartZ][StartX].st_neighbor[i]->st_parent = &(*m_Node)[StartZ][StartX];
//			(*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecH =
//				D3DXVec3Length(&((*m_Node)[LastZ][LastX].st_position
//					- (*m_Node)[StartZ][StartX].st_neighbor[i]->st_position));
//			(*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecF = (*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecG
//				+ (*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecH;
//			m_Selected.push_back((*m_Node)[StartZ][StartX].st_neighbor[i]);
//		}
//	}
//	while (1)
//	{
//		Node* now = FindLow(m_Selected);
//		if (!now)
//			break;
//		now->m_vecClose = true;
//		if (now->m_vecH <0.00001)
//		{
//			break;
//		}
//		for (int i = 0; i < now->st_neighbor.size(); i++)
//		{
//			if (now->st_neighbor[i]->m_vecClose)continue;
//			if (now->st_neighbor[i]->st_type == WALL)
//			{
//				continue;
//			}
//			float G = D3DXVec3Length(&(now->st_position - now->st_neighbor[i]->st_position));
//
//			////////////벽에 대한 정확도 떨어질수도
//			////////// 근처에 벽이 있으면 대각선 전부 막아버림 (완벽하지 않다.)
//			if (G > m_distance + 0.00001)
//			{
//				for (int j = 0; j < now->st_neighbor.size(); j++)
//				{
//					if (now->st_neighbor[j]->st_type == WALL)
//					{
//						continue;
//					}
//				}
//			}
//			if (now->st_neighbor[i]->m_vecG > G + now->m_vecG)
//			{
//				now->st_neighbor[i]->m_vecG = G + now->m_vecG;
//				now->st_neighbor[i]->st_parent = now;
//
//
//			}
//			now->st_neighbor[i]->m_vecH =
//				D3DXVec3Length(&((*m_Node)[LastZ][LastX].st_position
//					- now->st_neighbor[i]->st_position));
//			now->st_neighbor[i]->m_vecF = now->st_neighbor[i]->m_vecG
//				+ now->st_neighbor[i]->m_vecH;
//
//			m_Selected.push_back(now->st_neighbor[i]);
//		}
//
//
//	}
//
//	vector<D3DXVECTOR3> opposite;
//	Node* now = &(*m_Node)[LastZ][LastX];
//	while (1)
//	{
//
//		opposite.push_back(now->st_position);
//		now = now->st_parent;
//		if (now == NULL)
//			break;
//	}
//	int count = 0;
//	m_Way.resize(opposite.size());
//	for (int i = opposite.size() - 1; i >= 0; i--)
//	{
//		m_Way[count] = opposite[i];
//		count++;
//	}
//
//
//
//
//
//
//	return m_Way;
//}
//
//vector<D3DXVECTOR3> TeicAstar::SetWay(int Start, int Last)
//{
//
//	vector<D3DXVECTOR3> temp;
//	bool Put = false;
//	if (Start >= Last)
//	{
//		temp.push_back(m_Way[Start]);
//		m_Way = temp;
//		return m_Way;
//	}
//	for (int i = 0; i < m_Way.size(); i++)
//	{
//		if (i == Start)
//		{
//			Put = true;
//		}
//		if (Put)
//		{
//			temp.push_back(m_Way[i]);
//		}
//		if (i == Last)
//		{
//			Put = false;
//		}
//	}
//	m_Way = temp;
//	return m_Way;
//
//}
//
//HankcGrid * TeicAstar::FindLow(vector<HankcGrid*>& selected)
//{
//	if (selected.size() <= 0)return NULL;
//	float Min = INF;
//	Node* temp = NULL;
//	for (int i = 0; i < selected.size(); i++)
//	{
//		if (selected[i]->m_vecClose)
//		{
//			selected.erase(selected.begin() + i);
//		}
//		else if (selected[i]->m_vecF < Min)
//		{
//			Min = selected[i]->m_vecF;
//			temp = selected[i];
//		}
//	}
//
//	return temp;
//}
//
//
//
//
//
