#pragma once
//###############################################
//#												#
//#		최소값 뽑아내는 						#
//#		MinHeap 클래스 입니다.					#
//#		Meap정렬 Min 버전입니다					#
//#												#
//#		2017-09-15								#	
//#	    addheap,getminheap을 사용합니다			#
//#					Made by 태영				#
//###############################################
#include "stdafx.h"
#include "HankcNode.h"
template<typename T>

/////////////자리가 이동됨
class TeicMinHeap
{
private:
	vector<T> m_vecHeap;
public:
	void AddHeap(T temp)
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
			if (m_vecHeap[parent] > m_vecHeap[i])
			{
				Node* temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				i = parent;
			}
			else
			{
				break;
			}

		}
	}
	T GetminHeap()
	{
		T temp = m_vecHeap[0];
		m_vecHeap[0] = m_vecHeap[m_vecHeap.size() - 1];
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
				if (m_vecHeap[i * 2 + 1] < m_vecHeap[i])
				{
					T sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;

					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else if (m_vecHeap[i * 2 + 1] <= m_vecHeap[i * 2 + 2])
			{
				if (m_vecHeap[i * 2 + 1] < m_vecHeap[i])
				{
					T sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;

					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (m_vecHeap[i * 2 + 2] < m_vecHeap[i])
				{
					T sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 2];
					m_vecHeap[i * 2 + 2] = sample;

					i = i * 2 + 2;
				}
				else
				{
					break;
				}
			}
		}


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
			if (m_vecHeap[parent] > m_vecHeap[i])
			{
				T temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;

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
	TeicMinHeap() {}
	~TeicMinHeap() {}
};

template<>
class TeicMinHeap <HankcNode*>
{
private:
	vector<HankcNode*> m_vecHeap;
public:
	int GetSize() { return m_vecHeap.size(); }
	void AddHeap(HankcNode* temp)
	{
		m_vecHeap.push_back(temp);
		int i = m_vecHeap.size() - 1;
		temp->m_pAstarNode->m_iIndex = i;
		while (1)
		{
			int parent = (i - 1) / 2;
			if (i - 1 < 0)
			{
				break;
			}
			if (m_vecHeap[parent]->m_pAstarNode->m_fF > m_vecHeap[i]->m_pAstarNode->m_fF)
			{
				HankcNode* temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				m_vecHeap[parent]->m_pAstarNode->m_iIndex = parent;
				m_vecHeap[i]->m_pAstarNode->m_iIndex = i;
				i = parent;
			}
			else
			{
				break;
			}

		}
	}
	HankcNode* GetminHeap()
	{
		HankcNode* temp = m_vecHeap[0];
		m_vecHeap[0] = m_vecHeap[m_vecHeap.size() - 1];
		m_vecHeap[0]->m_pAstarNode->m_iIndex = 0;
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
				if (m_vecHeap[i * 2 + 1]->m_pAstarNode->m_fF < m_vecHeap[i]->m_pAstarNode->m_fF)
				{
					HankcNode* sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;
					m_vecHeap[i]->m_pAstarNode->m_iIndex = i;
					m_vecHeap[i * 2 + 1]->m_pAstarNode->m_iIndex = i * 2 + 1;
					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else if (m_vecHeap[i * 2 + 1]->m_pAstarNode->m_fF <= m_vecHeap[i * 2 + 2]->m_pAstarNode->m_fF)
			{
				if (m_vecHeap[i * 2 + 1]->m_pAstarNode->m_fF < m_vecHeap[i]->m_pAstarNode->m_fF)
				{
					HankcNode* sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;
					m_vecHeap[i]->m_pAstarNode->m_iIndex = i;
					m_vecHeap[i * 2 + 1]->m_pAstarNode->m_iIndex = i * 2 + 1;
					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (m_vecHeap[i * 2 + 2]->m_pAstarNode->m_fF < m_vecHeap[i]->m_pAstarNode->m_fF)
				{
					HankcNode* sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 2];
					m_vecHeap[i * 2 + 2] = sample;
					m_vecHeap[i]->m_pAstarNode->m_iIndex = i;
					m_vecHeap[i * 2 + 2]->m_pAstarNode->m_iIndex = i * 2 + 2;
					i = i * 2 + 2;
				}
				else
				{
					break;
				}
			}
		}

		temp->m_pAstarNode->m_iIndex = -1;
		temp->m_pAstarNode->m_bClose = true;
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
			if (m_vecHeap[parent]->m_pAstarNode->m_fF > m_vecHeap[i]->m_pAstarNode->m_fF)
			{
				HankcNode* temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				m_vecHeap[parent]->m_pAstarNode->m_iIndex = parent;
				m_vecHeap[i]->m_pAstarNode->m_iIndex = i;
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

	bool Check(int x, int y)
	{
		int i = 0;
		while (1)
		{
			if (m_vecHeap[i]->m_nFrameX == x && m_vecHeap[i]->m_nFrameZ == y)
			{
				return true;
			}
			i++;
			if (i > m_vecHeap.size() - 1)
				break;
		}
		return false;
	}
	HankcNode* FindNode(int x, int y)
	{
		HankcNode* temp;
		temp = new HankcNode;
		temp->m_pAstarNode = new HankcAstarNode;
		temp->m_pAstarNode->m_iIndex = -1000;
		temp->m_pAstarNode->m_fG = INF;
		int i = 0;
		while (1)
		{
			if (m_vecHeap[i]->m_nFrameX == x && m_vecHeap[i]->m_nFrameZ == y)
			{
				temp = m_vecHeap[i];
				return temp;
			}
			i++;
			if (i > m_vecHeap.size() - 1)
				break;
		}
		return temp;

	}
	TeicMinHeap() {}
	~TeicMinHeap() {}
};




template<>
class TeicMinHeap <HankcNode>
{
private:
	vector<HankcNode> m_vecHeap;
public:
	int GetSize() { return m_vecHeap.size(); }
	void AddHeap(HankcNode temp)
	{
		int i = m_vecHeap.size();
		temp.m_pAstarNode->m_iIndex = i;
		m_vecHeap.push_back(temp);
		

		while (1)
		{
			int parent = (i - 1) / 2;
			if (i - 1 < 0)
			{
				break;
			}
			if (m_vecHeap[parent].m_pAstarNode->m_fF > m_vecHeap[i].m_pAstarNode->m_fF)
			{
				HankcNode temp;
				
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				m_vecHeap[parent].m_pAstarNode->m_iIndex = parent;
				m_vecHeap[i].m_pAstarNode->m_iIndex = i;
				i = parent;
			}
			else
			{
				break;
			}

		}
	}
	HankcNode GetminHeap()
	{
		if (m_vecHeap.size() == 0)
		{
			HankcNode temp;
			temp.m_pAstarNode->m_iIndex = -1000;
			return temp;
		}
		HankcNode temp = m_vecHeap[0];
		m_vecHeap[0] = m_vecHeap[m_vecHeap.size() - 1];
		m_vecHeap[0].m_pAstarNode->m_iIndex = 0;
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
				if (m_vecHeap[i * 2 + 1].m_pAstarNode->m_fF < m_vecHeap[i].m_pAstarNode->m_fF)
				{
					HankcNode sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;
					m_vecHeap[i].m_pAstarNode->m_iIndex = i;
					m_vecHeap[i * 2 + 1].m_pAstarNode->m_iIndex = i * 2 + 1;
					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else if (m_vecHeap[i * 2 + 1].m_pAstarNode->m_fF <= m_vecHeap[i * 2 + 2].m_pAstarNode->m_fF)
			{
				if (m_vecHeap[i * 2 + 1].m_pAstarNode->m_fF < m_vecHeap[i].m_pAstarNode->m_fF)
				{
					HankcNode sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 1];
					m_vecHeap[i * 2 + 1] = sample;
					m_vecHeap[i].m_pAstarNode->m_iIndex = i;
					m_vecHeap[i * 2 + 1].m_pAstarNode->m_iIndex = i * 2 + 1;
					i = i * 2 + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (m_vecHeap[i * 2 + 2].m_pAstarNode->m_fF < m_vecHeap[i].m_pAstarNode->m_fF)
				{
					HankcNode sample;
					sample = m_vecHeap[i];
					m_vecHeap[i] = m_vecHeap[i * 2 + 2];
					m_vecHeap[i * 2 + 2] = sample;
					m_vecHeap[i].m_pAstarNode->m_iIndex = i;
					m_vecHeap[i * 2 + 2].m_pAstarNode->m_iIndex = i * 2 + 2;
					i = i * 2 + 2;
				}
				else
				{
					break;
				}
			}
		}

		temp.m_pAstarNode->m_iIndex = -1;
		temp.m_pAstarNode->m_bClose = true;
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
			if (m_vecHeap[parent].m_pAstarNode->m_fF > m_vecHeap[i].m_pAstarNode->m_fF)
			{
				HankcNode temp;
				temp = m_vecHeap[parent];
				m_vecHeap[parent] = m_vecHeap[i];
				m_vecHeap[i] = temp;
				m_vecHeap[parent].m_pAstarNode->m_iIndex = parent;
				m_vecHeap[i].m_pAstarNode->m_iIndex = i;
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

	
	TeicMinHeap() {}
	~TeicMinHeap() {}
};

