#pragma once

struct BoundingSquare
{
	float m_fSizeX;
	float m_fSizeY;
	float m_fSizeZ;
	D3DXVECTOR3 m_vCenterPos;
};

enum INTERFACETYPE
{
	E_INTER_NODE,
	E_INTER_NODE_POS,
	E_INTER_END,
};

class nInterface
{
public:
	INTERFACETYPE m_InterType;
	virtual INTERFACETYPE GetInterType(void) PURE;
};

class nPositionInfo
{
public:
	D3DXVECTOR3 m_vPos;
	vector<D3DXVECTOR3> m_vecVertex;
};

//��尡 ������ ���� �浹ü ���� ����Ʈ
class nNodeBoundInfo
{
public:
	list<BoundingSquare*> m_vecBounding;
};

class HankcNode : nInterface
{
public:
	int m_nFrameX;
	int m_nFrameZ;
	int m_nIndex;
	
public:
	//link function
	nInterface *m_pInterface;
	nPositionInfo *m_vPosList;	
	nNodeBoundInfo *m_pBoundInfo;


public:

	HankcNode();
	~HankcNode();
};