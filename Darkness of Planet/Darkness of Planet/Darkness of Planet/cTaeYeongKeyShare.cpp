#include "stdafx.h"
#include "cTaeYeongKeyShare.h"


void cTaeYeongKeyShare::Update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//b_LbuttonDown = 1;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		/*if (b_LbuttonDown == 1)
		{
			b_LbuttonDown = 3;
		}
		else if (b_LbuttonDown != 1)
		{
			b_LbuttonDown = 2;
		}*/
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		//b_LbuttonDown = 0;
	}
}

cTaeYeongKeyShare::cTaeYeongKeyShare()
{
}


cTaeYeongKeyShare::~cTaeYeongKeyShare()
{
}
