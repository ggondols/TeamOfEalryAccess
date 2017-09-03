#include "stdafx.h"
#include "cTaeYeongAstar.h"


cTaeYeongAstar::cTaeYeongAstar()
{
}


cTaeYeongAstar::~cTaeYeongAstar()
{
}
void cTaeYeongAstar::Setup(vector<vector<Node>>* Node)
{

	m_Node = Node;
	m_distance = D3DXVec3Length(&((*m_Node)[0][0].st_position - (*m_Node)[0][1].st_position));
}

vector<D3DXVECTOR3> cTaeYeongAstar::FindWay(int StartX, int StartZ, int LastX, int LastZ)
{
	m_Way.clear();
	m_Selected.Clear();


	for (int i = 0; i < (*m_Node).size(); i++)
	{
		for (int j = 0; j < (*m_Node)[i].size(); j++)
		{
			(*m_Node)[i][j].m_vecClose = false;
			(*m_Node)[i][j].m_vecF = INF;
			(*m_Node)[i][j].m_vecG = INF;
			(*m_Node)[i][j].m_vecH = INF;
			(*m_Node)[i][j].st_parent = NULL;
			(*m_Node)[i][j].m_iIndex = -1;
		}

	}
	(*m_Node)[StartZ][StartX].m_vecClose = true;
	for (int i = 0; i < (*m_Node)[StartZ][StartX].st_neighbor.size(); i++)
	{
		if ((*m_Node)[StartZ][StartX].st_neighbor[i]->st_type != WALL)
		{

			float G = D3DXVec3Length(&((*m_Node)[StartZ][StartX].st_position - (*m_Node)[StartZ][StartX].st_neighbor[i]->st_position));

			////////////���� ���� ��Ȯ�� ����������
			////////// ��ó�� ���� ������ �밢�� ���� ���ƹ��� (�Ϻ����� �ʴ�.)
			if (G > m_distance + 0.00001)
			{
				for (int j = 0; j < (*m_Node)[StartZ][StartX].st_neighbor.size(); j++)
				{
					if ((*m_Node)[StartZ][StartX].st_neighbor[j]->st_type == WALL)
					{
						continue;
					}
				}
			}

			(*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecG = G;
			(*m_Node)[StartZ][StartX].st_neighbor[i]->st_parent = &(*m_Node)[StartZ][StartX];
			(*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecH =
				D3DXVec3Length(&((*m_Node)[LastZ][LastX].st_position
					- (*m_Node)[StartZ][StartX].st_neighbor[i]->st_position));
			(*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecF = (*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecG
				+ (*m_Node)[StartZ][StartX].st_neighbor[i]->m_vecH;
			m_Selected.AddHeap((*m_Node)[StartZ][StartX].st_neighbor[i]);
		}
	}
	while (1)
	{
		Node* now = m_Selected.GetminHeap();
		if (!now)
			break;
		now->m_vecClose = true;
		if (now->m_vecH < 0.00001)
		{
			break;
		}
		for (int i = 0; i < now->st_neighbor.size(); i++)
		{
			if (now->st_neighbor[i]->m_vecClose)continue;
			if (now->st_neighbor[i]->st_type == WALL)
			{
				continue;
			}
			float G = D3DXVec3Length(&(now->st_position - now->st_neighbor[i]->st_position));

			////////////���� ���� ��Ȯ�� ����������
			////////// ��ó�� ���� ������ �밢�� ���� ���ƹ��� (�Ϻ����� �ʴ�.)
			if (G > m_distance + 0.00001)
			{

				int a = 0;
				for (int j = 0; j < now->st_neighbor.size(); j++)
				{
					if (now->st_neighbor[j]->st_type == WALL)
					{
						a++;
					}
				}
				if (a > 0)
					continue;
			}
			if (now->st_neighbor[i]->m_vecG > G + now->m_vecG)
			{
				now->st_neighbor[i]->m_vecG = G + now->m_vecG;
				now->st_neighbor[i]->st_parent = now;
				if (now->st_neighbor[i]->m_iIndex != -1)
				{
					m_Selected.Setting(now->st_neighbor[i]->m_iIndex);
				}

			}


			now->st_neighbor[i]->m_vecH =
				D3DXVec3Length(&((*m_Node)[LastZ][LastX].st_position
					- now->st_neighbor[i]->st_position));
			now->st_neighbor[i]->m_vecF = now->st_neighbor[i]->m_vecG
				+ now->st_neighbor[i]->m_vecH;
			if (now->st_neighbor[i]->m_iIndex == -1)
			{
				m_Selected.AddHeap(now->st_neighbor[i]);
			}
		}


	}

	vector<D3DXVECTOR3> opposite;
	Node* now = &(*m_Node)[LastZ][LastX];
	while (1)
	{

		opposite.push_back(now->st_position);
		now = now->st_parent;
		if (now == NULL)
			break;
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

vector<D3DXVECTOR3> cTaeYeongAstar::SetWay(int Start, int Last)
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

Node * cTaeYeongAstar::FindLow(vector<Node*>& selected)
{
	if (selected.size() <= 0)return NULL;
	float Min = INF;
	Node* temp = NULL;
	for (int i = 0; i < selected.size(); i++)
	{
		if (selected[i]->m_vecClose)
		{
			selected.erase(selected.begin() + i);
		}
		else if (selected[i]->m_vecF < Min)
		{
			Min = selected[i]->m_vecF;
			temp = selected[i];
		}
	}

	return temp;
}




