#pragma once
//###############################################
//#												#
//#		�������� ������ �̿��� �������� 		#
//#		��带 ã�Ƴ��� �극���� Ŭ�����Դϴ�	#
//#	  �������� �̿��ϹǷ� ���� �ӵ��� �����ϴ�	#
//#												#
//#		2017-09-16								#	
//#	    addheap,getminheap�� ����մϴ�			#
//#					Made by �¿�				#
//###############################################
//#2017.09.21 �극���� �˰��� ������ ���η� ����
class TeicBresenham
{
private:
	vector<POINT>	m_vecNodePoint;
public:
	vector<POINT> FindNode(float startX,float startY,float endX,float endY);
	TeicBresenham();
	~TeicBresenham();
};

