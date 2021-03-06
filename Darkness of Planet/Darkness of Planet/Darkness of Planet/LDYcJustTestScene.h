#pragma once

#include "cGameNode.h"
#include "LDYCamera.h"
#include "HankcGrid.h"

#include "TeicEnemy.h"
#include "TeicCollisionMove.h"
#include "TeicMoveSequence.h"
#include "HankcNode.h"
#include "TeicAstar.h"
#include "TeicAstarShort.h"
#include "TeicBresenham.h"

#define MOVESTYLE_DEFAULT 0


struct MotionBlurInfo
{
	D3DXVECTOR3 g_vWorldPos;
	D3DXMATRIXA16 g_mWorld;
	D3DXMATRIXA16 g_mWorldLast;
	LPD3DXMESH g_pMesh;
	LPDIRECT3DTEXTURE9 g_pMeshTexture;
};


class iMap;
class LDYCharacter;
class cSkyBoxCube;
class cSkyDome;
class cSkyCloud;
class cShadowMapping;
class LDYMotionBlur;

class LDYcJustTestScene :public cGameNode
{

private:

	LDYCamera*						m_pCamera;
	cSkyBoxCube*					m_pSkyBox;
	cSkyDome*						m_pSkyDome;
	cSkyCloud*						m_pSkyCloud;
	TeicBresenham*					m_pBresenham;
	Hank::cGrid*					m_pGrid;
	iMap*							m_pMap;


	float							m_fTime;   /// 쓰레드 시작
	float							m_fTime2;  ///  쓰레드 완료
	float							m_fTime3;

	int								m_iBodyUpgrade;
public:

	HankcGrid*						m_pNode;
	LDYCharacter*					m_pCharacter;
	TeicAstar*						m_pAstar;
	TeicAstarShort*					m_pAstarShort;
	bool							m_bThread;
	bool							m_bAstarThread;
	bool							m_bAttackOn;
	vector<TeicEnemy*>				m_vecEnemy;
	vector< vector<D3DXVECTOR3>>	m_vecEnemyWay;
	vector<TeicMoveSequence*>		m_vecEnemyCollisionMove;
	vector<POINT>					m_vecBresnhamNode;
	vector<bool>					m_vecAttackSlot;
	POINT							m_pAttackNode;
	D3DXVECTOR3						m_EnemyTarget;

	cShadowMapping*					m_pShadow;



	//케릭터 그림자용
	LPD3DXEFFECT			m_pCreateShadow;
	LPD3DXEFFECT			m_pApplyShadow;
	LPD3DXMESH				m_pHeightMapmesh;

	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;

	//create fx용핸들
	D3DXHANDLE				m_hCmatWorld;
	D3DXHANDLE				m_hCmatLightView;
	D3DXHANDLE				m_hCmatLightProjection;
	D3DXHANDLE				m_hCTechnic;

	//apply fx용 핸들
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

	LPD3DXMESH				m_pMesh;
	DWORD					m_dNum;
	LPD3DXEFFECT			m_pBloomEffect;
	LPDIRECT3DTEXTURE9		m_pBloomRenderTarget;
	LPDIRECT3DSURFACE9		m_pBloomDepthStencil;

	vector<ST_RHWT_VERTEX>	m_vecVertex;


public:

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();

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
	void AfterImage();
	float ComputeGaussianValue(float x, float mean, float std_deviation);

	LPD3DXEFFECT LoadEffect(const char* szFileName);
	LPD3DXEFFECT LoadEffectHpp(const char* szFileName);
public:
	LDYcJustTestScene();
	~LDYcJustTestScene();
};

