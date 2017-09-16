#include "stdafx.h"
#include "TeicBresenham.h"



TeicBresenham::TeicBresenham()
{
}


TeicBresenham::~TeicBresenham()
{
}



vector<POINT> TeicBresenham::FindNode(float startX, float startY, float endX, float endY)
{
	float AnotherStartY = -startY;
	float AnotherEndY = -endY;
	m_vecNodePoint.clear();

	if ((int)startX == (int)endX && (int)AnotherStartY == (int)AnotherEndY)
	{
		return m_vecNodePoint;
	}

	float dx = abs(endX - startX); // 시작 점과 끝 점의 각 x 좌표의 거리
	float dy = abs(AnotherEndY - AnotherStartY); // 시작 점과 끝 점의 각 y 좌표의 거리



	if (dy <= dx)
	{

		float p = 2 * (dy - dx);
		int y = AnotherStartY;
		int inc_x = 1;

		if (endX< startX)
		{
			inc_x = -1;
		}
		int inc_y = 1;
		if (AnotherEndY < AnotherStartY)
		{
			inc_y = -1;
		}

		for (int x = startX; ((int)startX <= (int)endX ? x <= (int)endX : x >= (int)endX); x += inc_x)
		{
			if (x == (int)endX)break;
			if (0 >= p)
			{
				p += 2 * dy;
			}
			else
			{
				p += 2 * (dy - dx);
				y += inc_y;
			}
			m_vecNodePoint.push_back(PointMake(x, y));
		}
	}
	else
	{
		float p = 2 * (dx - dy);
		int x = startX;
		int inc_x = 1;

		if (endX < startX)
		{
			inc_x = -1;
		}
		int inc_y = 1;

		if (AnotherEndY < AnotherStartY)
		{
			inc_y = -1;
		}

		for (int y = AnotherStartY; ((int)AnotherStartY <= (int)AnotherEndY ? y <= (int)AnotherEndY : y >= (int)AnotherEndY); y += inc_y)
		{
			if (y == (int)AnotherEndY)break;
			if (0 >= p)
			{
				p += 2 * dx;
			}
			else
			{
				p += 2 * (dx - dy);
				x += inc_x;
			}
			m_vecNodePoint.push_back(PointMake(x, y));
		}
	}



	return m_vecNodePoint;
}