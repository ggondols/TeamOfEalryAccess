#pragma once

//###############################################
//#												#
//#		Total Node Initialize with Astar		#	
//#		09 - 09 - 2017							#	
//#		each Node information contains			#	
//#		Collision bounders, Node edge position	#	
//#		Astar Information						#	
//#												#
//#					Made by Hank				#
//###############################################

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

class nAstarInfo
{
public:
	bool m_isOpen;
	bool m_isClose;
};

//��尡 ������ ���� �浹ü ���� ����Ʈ
class nNodeBoundInfo
{
public:
	list<BoundingSquare*> m_vecBounding;
};


//��� �ٽ� ����
class HankcNode : nInterface
{
public:
	int m_nFrameX;
	int m_nFrameZ;
	int m_nIndex;
	
public:
	//link function // �������̽� Ÿ��, GRID�� �簢�� ��, ������� �ִ� �浹ü�� ���� // astar�� 
	nInterface *m_pInterface;
	nPositionInfo *m_vPosList;	
	nNodeBoundInfo *m_pBoundInfo;

public:
	HankcNode();
	~HankcNode();
};

//������
class HankcNodeRow
{
public:

	vector<HankcNode> m_vRow;

public:
	HankcNode* operator [] (int row)
	{
		return &m_vRow[row];
	}
};

//�࿭
class HankcGrid
{
public:
	vector<HankcNodeRow> m_vCol;
};