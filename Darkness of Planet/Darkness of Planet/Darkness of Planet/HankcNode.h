#pragma once
class HankcNode;
class TeicEnemy;
class TeicSkinnedMeshParent;
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
//	열: Col,  Z,  i
//	행: Row,  X,  j
//
enum Boundingtype
{
	Bounding_Enemy,
	Bounding_Object,
	Bounding_Character

};

struct BoundingSquare
{
	float m_fSizeX;
	float m_fSizeY;
	float m_fSizeZ;
	float m_fControlX;
	float m_fControlY;
	float m_fControlZ;
	D3DXVECTOR3 m_vXdir;
	D3DXVECTOR3 m_vYdir;
	D3DXVECTOR3 m_vZdir;
	D3DXVECTOR3 m_vCenterPos;
	Boundingtype st_Type;
	TeicSkinnedMeshParent*  m_pSkinnedObject;               //////////object 정보
	bool		m_bIsPicked;
	 
	


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
	nPositionInfo();
	nPositionInfo(int fx, int fy, int fz, int size);
	~nPositionInfo();
};

//노드가 가지고 있을 충돌체 정보 리스트
class nNodeBoundInfo
{
public:
	vector<BoundingSquare*> m_vecBounding;       /////////////list에서 벡터로바꿈
};

class HankcAstarNode
{
public:
	bool				m_bClose;
	float				m_fF;
	float				m_fG;
	float				m_fH;
	HankcNode*			m_pParent;
	int					m_iIndex;

public:
	void Set(bool Close, float F, float G, float H, HankcNode* parent)
	{
		m_bClose = Close;
		m_fF = F;
		m_fG = G;
		m_fH = H;
		m_pParent = parent;
	
	}
	HankcAstarNode();
	~HankcAstarNode();
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
	HankcAstarNode *m_pAstarNode;

public:
	
	HankcNode();	
	bool InitPosition(int size);
	void InitFrame(int fx, int fz);
	~HankcNode();

	virtual INTERFACETYPE GetInterType(void) {return E_INTER_END;};
};


//열정보
class HankcNodeCol
{
public:
	vector<HankcNode> m_vCol;

public:
	HankcNodeCol();
	~HankcNodeCol();

	HankcNode& operator[] (int row)
	{
		return m_vCol[row];
	}
};

//행열
class HankcGrid
{
public:
	vector<HankcNodeCol> m_vRow;

public:
	HankcGrid();
	~HankcGrid();

	HankcNodeCol& operator[] (int row)
	{
		return m_vRow[row];
	}
};