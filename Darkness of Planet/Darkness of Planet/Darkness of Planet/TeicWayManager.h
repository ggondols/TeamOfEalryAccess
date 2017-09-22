#pragma once
class TeicAstar;
class HankcGrid;
class HankcNode;
#include "cSingletonBase.h"

//###############################################
//#												#
//#		Astar �̸� ã�Ƴ���	��������			#
//#		���̽�Ÿ �Ŵ��� �Դϴ�.					#
//#		2017-09-21								#	
//#	    AddWay �� GetWay�� ����ϴ�	            #
//#		Key�� �ۼ����:							#
//#		SX����SZ����LX����LZ����				#
//#		S(����,����,����)L(����,����,����)		#
//#					Made by �¿�				#
//###############################################

class TeicWayManager: public cSingletonBase<TeicWayManager>
{
private:
	map<string, vector<D3DXVECTOR3>> m_mapWay;
	map<string, vector<D3DXVECTOR3>>::iterator	m_iterWay;

	
public:
	void AddWay(const char* keyname, HankcGrid* Node, int StartX, int StartZ, int LastX, int LastZ);
	void AddWay2(const char* keyname, HankcGrid* Node, D3DXVECTOR3 start, D3DXVECTOR3 last);
	vector<D3DXVECTOR3>	GetWay(char* keyname);
	void Setup();
	void Destroy();
	TeicWayManager();
	~TeicWayManager();
};

