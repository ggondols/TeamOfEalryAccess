#include "stdafx.h"
#include "HankcNode.h"

HankcNode::HankcNode()
	: m_pBoundInfo(NULL)
	, m_vPosList(NULL)
	, m_nFrameX(0)
	, m_nFrameZ(0)
	, m_nIndex(-1)
{
}


bool HankcNode::InitPosition(int size)
{
	m_vPosList = new nPositionInfo(m_nFrameX, 0, m_nFrameZ, size);
	
	if (m_vPosList)
		return true;
	else
		return false;
}

void HankcNode::InitFrame(int fz, int fx)
{
	m_nFrameX = fx;
	m_nFrameZ = fz;



}

HankcNode::~HankcNode()
{
//	SAFE_DELETE(m_vPosList);
//	SAFE_DELETE(m_pInterface);
	SAFE_DELETE(m_pBoundInfo);
}

nPositionInfo::nPositionInfo()
{
}

nPositionInfo::nPositionInfo(int fx,int fy, int fz, int size)
{
	vector<D3DXVECTOR3> nodeVertex;

	// 0---1
	// | / |
	// 2---3
	// 012 - 213
	// imap 역수로 전환 해야함
	nodeVertex.push_back(D3DXVECTOR3(0, 0, 0));
	nodeVertex.push_back(D3DXVECTOR3(1 * size, 0, 0));
	nodeVertex.push_back(D3DXVECTOR3(0, 0, -1 * size));
	nodeVertex.push_back(D3DXVECTOR3(1, 0, -1 * size));

	m_vPos.x = fx * size;
	m_vPos.y = 0;
	m_vPos.z = -fz * size;

	m_vCenterPos.x = fx * size + (size / 2);
	m_vCenterPos.y = 0;
	m_vCenterPos.z = -fz * size + (size / 2);

	m_vecVertex.push_back(nodeVertex[0]);
	m_vecVertex.push_back(nodeVertex[1]);
	m_vecVertex.push_back(nodeVertex[2]);
	m_vecVertex.push_back(nodeVertex[2]);
	m_vecVertex.push_back(nodeVertex[1]);
	m_vecVertex.push_back(nodeVertex[3]);
}

nPositionInfo::~nPositionInfo()
{
	
}


HankcGrid::HankcGrid()
{
}

HankcGrid::~HankcGrid()
{
}

HankcNodeCol::HankcNodeCol()
{
}

HankcNodeCol::~HankcNodeCol()
{
}

HankcAstarNode::HankcAstarNode()
{
}

HankcAstarNode::~HankcAstarNode()
{
}
