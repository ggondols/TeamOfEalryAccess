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
//
//	행: Col,  Z,  i
//	열: Row,  X,  j
//
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
	D3DXVECTOR3 m_vCenterPos;
	vector<D3DXVECTOR3> m_vecVertex;
public:
	nPositionInfo(int fx, int fy, int fz, int size);
	~nPositionInfo();
};

//노드가 가지고 있을 충돌체 정보 리스트
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
	bool InitPosition(int size);
	void InitFrame(int fx, int fz);
	~HankcNode();

	virtual INTERFACETYPE GetInterType(void) {return E_INTER_END;};
};


//열정보
class HankcNodeRow
{
public:
	vector<HankcNode> m_vRow;

public:
	HankcNodeRow();
	~HankcNodeRow();

	HankcNode& operator[] (int row)
	{
		return m_vRow[row];
	}
};

//행열
class HankcGrid
{
public:
	vector<HankcNodeRow> m_vCol;

public:
	HankcGrid();
	~HankcGrid();

	HankcNodeRow& operator[] (int col)
	{
		return m_vCol[col];
	}
};