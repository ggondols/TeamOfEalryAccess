#pragma once
#include "stdafx.h"
template<typename T>
class cTaeYeongMinHeap
{
public:
	cTaeYeongMinHeap() {}
	~cTaeYeongMinHeap() {}
};

template<>
class cTaeYeongMinHeap <Node*>
{
private:
	vector<Node*> m_vecHeap;
public:
	void AddHeap(Node* temp)
	{
		m_vecHeap.push_back(temp);
		int i = m_vecHeap.size() - 1;
		temp->m_iIndex = i;
		while (1)
		{
			int parent = (i - 1) / 2;
			if (i - 1 < 0)
			{
				break;
			}
			if (m_vecHeap[parent]->m_vecF > m_vecHeap[i]->m_vecF)
			{
				Node* temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				m_vecHeap[parent]->m_iIndex = parent;
				m_vecHeap[i]->m_iIndex = i;
				i = parent;
			}
			else
			{
				break;
			}

		}
	}
	Node* GetminHeap()
	{
		Node* temp = m_vecHeap[0];
		m_vecHeap[0] = m_vecHeap[m_vecHeap.size() - 1];
		m_vecHeap[0]->m_iIndex = 0;
		m_vecHeap.erase(m_vecHeap.begin() + (m_vecHeap.size() - 1));
		int i = 0;
		while (1)
		{
			if (i * 2 + 1 > m_vecHeap.size() - 1)
			{
				break;
			}
			if (i * 2 + 1 == m_vecHeap.size() - 1)
			{
				if (m_vecHeap[i * 2 + 1]->m_vecF < m_vecHeap[i]->m_vecF)
				{
					Node* sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;
					m_vecHeap[i]->m_iIndex = i;
					m_vecHeap[i * 2 + 1]->m_iIndex = i * 2 + 1;
					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else if (m_vecHeap[i * 2 + 1]->m_vecF <= m_vecHeap[i * 2 + 2]->m_vecF)
			{
				if (m_vecHeap[i * 2 + 1]->m_vecF < m_vecHeap[i]->m_vecF)
				{
					Node* sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;
					m_vecHeap[i]->m_iIndex = i;
					m_vecHeap[i * 2 + 1]->m_iIndex = i * 2 + 1;
					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (m_vecHeap[i * 2 + 2]->m_vecF < m_vecHeap[i]->m_vecF)
				{
					Node* sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 2];
					m_vecHeap[i * 2 + 2] = sample;
					m_vecHeap[i]->m_iIndex = i;
					m_vecHeap[i * 2 + 2]->m_iIndex = i * 2 + 2;
					i = i * 2 + 2;
				}
				else
				{
					break;
				}
			}
		}

		temp->m_iIndex = -1;
		temp->m_vecClose = true;
		return temp;
	}


	void Setting(int num)
	{

		int i = num;
		while (1)
		{
			int parent = (i - 1) / 2;
			if (parent < 0)
			{
				break;
			}
			if (m_vecHeap[parent]->m_vecF > m_vecHeap[i]->m_vecF)
			{
				Node* temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				m_vecHeap[parent]->m_iIndex = parent;
				m_vecHeap[i]->m_iIndex = i;
				i = parent;
			}
			else
			{
				break;
			}

		}
	}
	void Clear()
	{
		m_vecHeap.clear();
	}
	cTaeYeongMinHeap() {}
	~cTaeYeongMinHeap() {}
};

