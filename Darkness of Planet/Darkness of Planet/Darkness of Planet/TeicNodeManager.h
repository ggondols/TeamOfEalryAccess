#pragma once
#include "HankcNode.h"
#include "cSingletonBase.h"

//###############################################
//#												#
//#		노드 만들어서 가져오는					#
//#		노드 매니저 입니다.						#
//#		2017-09-21								#	
//#	    GetNode()로 가져옵니다					#
//#     가져온 노드 클래스에서 delete하지마세요 #
//#					Made by 태영				#
//###############################################
class TeicNodeManager:public cSingletonBase<TeicNodeManager>
{
	HankcGrid*				m_pNode;
	HankcNode*				m_pPickingNode;
public:
	void Setup(int size);
	void Destroy();
	HankcGrid*	GetNode();
	HankcNode*  GetPickingNode();
	TeicNodeManager();
	~TeicNodeManager();
};

