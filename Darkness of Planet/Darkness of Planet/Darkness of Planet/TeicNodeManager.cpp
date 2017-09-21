#include "stdafx.h"
#include "TeicNodeManager.h"


void TeicNodeManager::Setup(int size)
{
	m_pNode = new HankcGrid;
	int sizeX = size;
	int sizeZ = size;
	m_pNode->m_vRow.resize(sizeZ);
	for (int i = 0; i < sizeZ; i++)
	{
		m_pNode->m_vRow[i].m_vCol.resize(sizeX);
		for (int j = 0; j < sizeX; j++)
		{
			m_pNode->m_vRow[i].m_vCol[j].InitFrame(i, j); // �Ϲ� ��常 �����մϴ�.
			m_pNode->m_vRow[i].m_vCol[j].InitPosition(NodeLength); // ������ ��带 ������� ���ؽ��� ���� �»������ �����մϴ�.
			m_pNode->m_vRow[i].m_vCol[j].m_pAstarNode = new HankcAstarNode;
		}
	}
}

void TeicNodeManager::Destroy()
{
	SAFE_DELETE(m_pNode);
}

HankcGrid * TeicNodeManager::GetNode()
{
	return m_pNode;
}

TeicNodeManager::TeicNodeManager()
{
	m_pNode = NULL;
}


TeicNodeManager::~TeicNodeManager()
{
}
