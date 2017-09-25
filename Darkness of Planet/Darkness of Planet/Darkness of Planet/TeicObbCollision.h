#pragma once
#include "HankcNode.h"
//###############################################
//#												#
//#		Obb충돌을 체크하는				        #
//#		클래스입니다.					        #
//#		2017-09-24								#	
//#     Checkcollision함수로 					#
//#		충돌 여부를 검사합니다				    #
//#					Made by 태영				#
//#    obb, ray 충돌은 시간 관계상 알아보지		#
//#    못했습니다								#
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

