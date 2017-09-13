#pragma once
class HankcGrid;
class HankcNode;



//
//	За: Col,  Z,  i
//	ї­: Row,  X,  j
//
class TeicAstar
{
private:

	HankcGrid* m_Node;
	vector<vector<bool>>		m_vecClose;
//	vector<vector<float>>		m_vecClose;
//	vector<vector<float>>		m_vecClose;
//	vector<vector<float>>		m_vecClose;
//	vector<vector<HankcNode*>>	m_vecParent;

		
	
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

