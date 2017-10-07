#pragma once
#include "LDYCamera.h"
#include "HankcNode.h"
#include "LDYCharacter.h"
#include "cSphere.h"
#include "TeicObbCollision.h"
#include "TeicBresenham.h"
//###############################################
//#												#
//#		총을 쏘고 맞추는						#
//#		shoot 매니저 입니다.					#
//#		2017-09-24								#	
//#					Made by 태영				#
//#     Setup이후에 Shoot함수를 쓰면 됩니다		#
//#		Render를하면 총알이 나가는 궤적을		#
//#     볼 수 있습니다.							#
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

