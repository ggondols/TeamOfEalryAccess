#pragma once

#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"
#include "TeicEnemy.h"
#include "TeicCollisionMove.h"
#include "TeicMoveSequence.h"
#include "HankcNode.h"
#include "TeicAstar.h"
#include "TeicAstarShort.h"
#include "TeicBresenham.h"
#include "cSphere.h"
#include "LDYCharacter.h"
#include "cSkyBoxCube.h"
#include "LDYCamera.h"
#include "TeicObbCollision.h"
#include "TeicShoot.h"
#include "TeicIceExplosion.h"
#include "TeicBoss.h"

//��� ���
#include "cModel.h"
#include "cObjectNode.h"
#include "cStaticMeshLoader.h"
#include "cConsole.h"


class iMap;

class FieldItem;
class cSkyDome;
class cSkyCloud;
class Inventory;


class DarknessofPlanetMainScene:public cGameNode
{

private:

	cSkyDome*						m_pSkyDome;
	cSkyCloud*						m_pSkyCloud;
	TeicShoot*				m_pShoot;
	TeicObbCollision*		m_pCollision;
	//	LDYCamera*				m_pCamera;

	TeicBresenham*			m_pBresenham;
	Hank::cGrid*			m_pGrid;
	iMap*					m_pMap;
	LPD3DXFONT				m_pFont;

	float					m_fTime;   /// ������ ����
	float					m_fTime2;  ///  ������ �Ϸ�
	float					m_fTime3;   ////  Ÿ�� ��� �ð�
	float					m_fTime4; /// ����� �ڼ����� ���ƿ��� �ð�
	float					m_fTime5; /// �� ���� �ӵ� �ð�
	float					m_fTime6; //// �Ϲ� ��忡�� ĳ���� �Ѿƿ�  10�ʰ���
	float					m_fTime7; //// �ʵ� ���� ���� �ð� (20�� ����)
	float					m_fBossTime;
	int						m_iBodyUpgrade;

	// �κ��丮 �߰�
	Inventory*				m_pInventory;
	vector<FieldItem*>		m_vecItem;

private:
	//Map Object Control
	std::list<cObjectNode*>	m_ObjNodes;
	cStaticMeshLoader		m_meshList;
	cConsole*				m_pConsole;
	

public:

	HankcGrid*				m_pNode;
	LDYCharacter*			m_pCharacter;
	TeicAstar*				m_pAstar;
	TeicAstarShort*		    m_pAstarShort;
	bool					m_bThread;
	bool					m_bAstarThread;
	bool					m_bAstarThread2;

	vector<TeicEnemy*>				m_vecEnemy;
	vector<TeicEnemy*>				m_vecMakingEnemy;
	vector< vector<D3DXVECTOR3>>	m_vecEnemyWay;
	vector<TeicMoveSequence*>		m_vecEnemyCollisionMove;
	vector<POINT>					m_vecBresnhamNode;
	vector<bool>					m_vecAttackSlot;
	POINT							m_pAttackNode;
	D3DXVECTOR3						m_EnemyTarget;
	TeicCollisionMove*				m_pBossMove;

	//����
private:

	//�ɸ��� �׸��ڿ�
	LPD3DXEFFECT			m_pCreateShadow;
	LPD3DXEFFECT			m_pApplyShadow;
	LPD3DXMESH				m_pHeightMapmesh;

	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;

	//create fx���ڵ�
	D3DXHANDLE				m_hCmatWorld;
	D3DXHANDLE				m_hCmatLightView;
	D3DXHANDLE				m_hCmatLightProjection;
	D3DXHANDLE				m_hCTechnic;

	//apply fx�� �ڵ�
	D3DXHANDLE				m_hApplyTexture;
	D3DXHANDLE				m_hAmatWorld;
	D3DXHANDLE				m_hAmatLightView;
	D3DXHANDLE				m_hAmatLightProjection;
	D3DXHANDLE				m_hAm_vec4LightPosition;
	D3DXHANDLE				m_hAmatViewProjection;
	D3DXHANDLE				m_hAgObjectColor;


	D3DXVECTOR4				m_vec4LightPosition;
	D3DXMATRIXA16			matS;
	D3DXMATRIXA16			matLightView;
	D3DXMATRIXA16			matLightProjection;


	deque<D3DXVECTOR3>		m_vecAfterImageMuzzle;
	deque<D3DXVECTOR3>		m_vecAfterImageWeapon;
	float					m_fStartTime;
	float					m_fEndTime;
	float					m_fCurrentTime;


	DWORD					m_dNum;
	LPD3DXEFFECT			m_pBloomEffect;
	LPDIRECT3DTEXTURE9		m_pBloomRenderTarget;
	LPDIRECT3DSURFACE9		m_pBloomDepthStencil;

	vector<ST_RHWT_VERTEX>	m_vecVertex;


public:
	int			m_iCameranum;
	TeicEnemy*  m_pTempEnemy;
	cSphere*	m_pTempSPhere;
	TeicBoss*	m_pBoss;
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	void MakingEnemy();
	void CallbackOn(int number);
	bool CollisionCheck(TeicEnemy* A, TeicEnemy* B);
	void Push2(TeicEnemy* A, TeicEnemy* B);
	void TotalPushCheck2();
	void ChangeGridInfo();
	void TargetOn();
	bool CheckStraight(TeicEnemy* A);
	float EnemyEnemyDistance(TeicEnemy* A, TeicEnemy* B);
	float EnemyPlayerDistance(TeicEnemy* ene);
	bool TotalCollisionCheck();
	bool CheckSlot();
	bool ChangeCheckPoint();
	void AngleChange(TeicEnemy* A);
	void WayUpdate();
	void SlowUpdate();
	void CleanHit();
	bool SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B);  //A�� B�� ������?
	void CheckDie();
	float GetFireRate();
	float GetCallbackTime();
	void SetRushAttack();
	void BossAttack();
	//����
	void AfterImage();
	float ComputeGaussianValue(float x, float mean, float std_deviation);
	LPD3DXEFFECT LoadEffect(const char* szFileName);
	LPD3DXEFFECT LoadEffectHpp(const char* szFileName);
	void MakingFieldEnemy();
	D3DXVECTOR3 GetWhere(int n);

public:
	DarknessofPlanetMainScene();
	~DarknessofPlanetMainScene();
};

