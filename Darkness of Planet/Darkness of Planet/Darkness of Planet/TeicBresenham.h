#pragma once
//###############################################
//#												#
//#		시작점과 끝점을 이용해 지나가는 		#
//#		노드를 찾아내는 브레전햄 클래스입니다	#
//#	  정수값만 이용하므로 연산 속도가 빠릅니다	#
//#												#
//#		2017-09-16								#	
//#	    addheap,getminheap을 사용합니다			#
//#					Made by 태영				#
//###############################################
//#2017.09.21 브레젠험 알고리즘 오차율 제로로 만듦
class TeicBresenham
{
private:
	vector<POINT>	m_vecNodePoint;
public:
	vector<POINT> FindNode(float startX,float startY,float endX,float endY);
	TeicBresenham();
	~TeicBresenham();
};

