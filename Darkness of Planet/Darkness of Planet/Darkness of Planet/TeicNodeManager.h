#pragma once
#include "HankcNode.h"
#include "cSingletonBase.h"
class TeicNodeManager:public cSingletonBase<TeicNodeManager>
{
	HankcGrid*				m_pNode;
public:
	void Setup(int size);
	void Destroy();
	HankcGrid*	GetNode();
	TeicNodeManager();
	~TeicNodeManager();
};

