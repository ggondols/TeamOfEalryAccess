#pragma once
class HankcGrid;
class HankcNode;



//
//	ї­: Col,  Z,  i
//	За: Row,  X,  j
//
class TeicAstar
{
private:

	HankcGrid* m_Node;
	vector<vector<bool>>		m_vecClose;
	vector<vector<float>>		m_vecF;
	vector<vector<float>>		m_vecG;
	vector<vector<float>>		m_vecH;
	vector<vector<HankcNode*>>	m_vecParent;
	vector<HankcNode*>		 m_Selected;

	vector<D3DXVECTOR3>  m_Way;
	float			     m_distance;
public:
	void Setup(HankcGrid* Node);
	vector<D3DXVECTOR3> FindWay(int StartX, int StartZ, int LastX, int LastZ);
	vector<D3DXVECTOR3> SetWay(int Start, int Last);
	HankcGrid* FindLow(vector<HankcGrid*>& selected);

public:
	TeicAstar();
	~TeicAstar();
};

