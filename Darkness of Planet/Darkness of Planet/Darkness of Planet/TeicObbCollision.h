#pragma once
#include "HankcNode.h"
//###############################################
//#												#
//#		Obb�浹�� üũ�ϴ�				        #
//#		Ŭ�����Դϴ�.					        #
//#		2017-09-24								#	
//#     Checkcollision�Լ��� 					#
//#		�浹 ���θ� �˻��մϴ�				    #
//#					Made by �¿�				#
//#    obb, ray �浹�� �ð� ����� �˾ƺ���		#
//#    ���߽��ϴ�								#
//###############################################

class TeicObbCollision
{
public:
	bool CheckCollision(BoundingSquare* A, BoundingSquare* B);
	//bool ObbRayCollision();
public:
	
	TeicObbCollision();
	~TeicObbCollision();
};

