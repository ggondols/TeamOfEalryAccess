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
	vector<POINT> FindNodeAccuracy(float startX, float startY, float endX, float endY);
	vector<POINT> FindNodeAccuracyHeightMap(float startX, float startY, float endX, float endY, D3DXVECTOR3 start, D3DXVECTOR3 end);
	vector<POINT> FindNodeAccuracy2(float startX, float startY, float endX, float endY);
	TeicBresenham();
	~TeicBresenham();
};

