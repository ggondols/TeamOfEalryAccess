#pragma once
#include "HankcNode.h"
#include "cSingletonBase.h"

//###############################################
//#												#
//#		��� ���� ��������					#
//#		��� �Ŵ��� �Դϴ�.						#
//#		2017-09-21								#	
//#	    GetNode()�� �����ɴϴ�					#
//#     ������ ��� Ŭ�������� delete���������� #
//#					Made by �¿�				#
//###############################################
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
