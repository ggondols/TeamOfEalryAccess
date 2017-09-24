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
//###############################################
class TeicObbCollision
{
public:
	bool CheckCollision(BoundingSquare* A, BoundingSquare* B);
public:
	TeicObbCollision();
	~TeicObbCollision();
};

