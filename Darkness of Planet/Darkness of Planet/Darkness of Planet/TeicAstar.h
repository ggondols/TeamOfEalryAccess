#pragma once
class HankcGrid;
class HankcNode;

//###############################################
//#												#
//#		이동 경로 탐색하는						#
//#		Astar 클래스 입니다.			        #
//#		2017-09-15								#	
//#	    setup 후 Findway로 찾습니다 			#
//#					Made by 태영				#
//###############################################


//
//	열: Col,  Z,  i
//	행: Row,  X,  j
//
class TeicAstar
{
private:

	HankcGrid* m_Node;
	/*vector<vector<bool>>		m_vecClose;
	vector<vector<float>>		m_vecF;
	vector<vector<float>>		m_vecG;
	vector<vector<float>>		m_vecH;
	vector<vector<HankcNode*>>	m_vecParent;
	vector<HankcNode*>			m_vecSelected;*/




	vector<vector<float>>		m_vecF;
	vector<vector<float>>		m_vecH;
	vector<vector<HankcNode*>>	m_vecParent;
	vector<D3DXVECTOR3>  m_Way;
	float			     m_distance;
	HankcNode* FindLow(vector<HankcNode*>& selected, vector<vector<bool>>& close,
		vector<vector<float>>& F);
public:
	void Setup(HankcGrid* Node);
	vector<D3DXVECTOR3> FindWay(int StartX, int StartZ, int LastX, int LastZ);
	vector<D3DXVECTOR3> FindWay(D3DXVECTOR3 start, D3DXVECTOR3 Last);
	vector<D3DXVECTOR3> SetWay(int Start, int Last);


public:
	TeicAstar();
	~TeicAstar();
};

