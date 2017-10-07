#pragma once
#include "LDYCamera.h"
#include "HankcNode.h"
#include "LDYCharacter.h"
#include "cSphere.h"
#include "TeicObbCollision.h"
#include "TeicBresenham.h"
//###############################################
//#												#
//#		���� ��� ���ߴ�						#
//#		shoot �Ŵ��� �Դϴ�.					#
//#		2017-09-24								#	
//#					Made by �¿�				#
//#     Setup���Ŀ� Shoot�Լ��� ���� �˴ϴ�		#
//#		Render���ϸ� �Ѿ��� ������ ������		#
//#     �� �� �ֽ��ϴ�.							#
//###############################################
class TeicShoot
{
private:
	TeicObbCollision*		m_pObbcollision;
	TeicBresenham*			m_pBresenham;
	SYNTHESIZE(float, m_fShootDistance, Distance);
	SYNTHESIZE(float, m_fShootRange, Range);
	SYNTHESIZE(D3DXVECTOR3, m_vShootDir, Dir);
	SYNTHESIZE(D3DXVECTOR3, m_vShootPosition, StartPosition);
	D3DXVECTOR3				m_vFinish;
	LDYCamera*				m_pCamera;
	HankcGrid*				m_pNode;
	LDYCharacter*			m_pCharacter;
	LPD3DXLINE*				m_pLine;
	BoundingSquare			m_stBulletSquare;
	ST_PN_Rectangle			m_stRect;
	D3DXMATRIX matR;
	vector<POINT>			m_vecPoint;
	vector<POINT>			m_vecDeletePoint;
	vector<HankcNode*>		m_vecTargetNode;
public:
	void Setup(HankcGrid* node, LDYCamera* camera, LDYCharacter* character);
	void Shoot(WeaponType type);
	void Render();
	void CalRotation();
	D3DXVECTOR3 GetPosition(vector<ST_PN_VERTEX> info, D3DXVECTOR3 rayorigin, D3DXVECTOR3 raydir);
public:
	TeicShoot();
	~TeicShoot();
	void MakeBoundingBox();
};

