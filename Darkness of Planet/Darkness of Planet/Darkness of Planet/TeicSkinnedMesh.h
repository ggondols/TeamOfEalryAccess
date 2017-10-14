#pragma once
struct ST_BONE;
#include "HankcNode.h"
#include "TeicSkinnedMeshParent.h"
//###############################################
//#												#
//#		이동,위치,스케일,콜백이 적용된          #
//#		스킨드 메쉬 클래스 입니다.              #
//#		2017-09-11								#	
//#     SetPosition으로 위치를					#
//#		SetRotationAngle로 회전 매트릭스 셋팅   #
//#		SetCallbackfunction로 콜백 셋팅			#	
//#												#
//#					Made by 태영				#
//###############################################

class TeicSkinnedMesh:public TeicSkinnedMeshParent
{
	friend class cSkinnedMeshManager;

protected:
	
	//하나만 생성
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	D3DXMATRIX					m_Move;
	
	// 객체마다 생성
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(float, m_fAngle, Angle);

	bool						m_bBlending;
	
	float						m_fWeightDivide;
	float						m_fWeight;
	int m_iCurrentAniNum;
	float m_Starttime;
	float m_Finishtime;
	float m_Middletime;
	D3DXVECTOR3 position;
	bool m_Damaging;
	CallbackBindFunction			m_callback;
	CallbackBindFunction        m_attackCallback;
	
	int	m_Tracknum;
	int	m_iNum;
	bool						m_bAnion;
	D3DXMATRIX					m_RotationMat;
	bool m_bCollision;
	bool  m_bShow;


public:

	D3DXMATRIX m_matBip01RHand;
	D3DXMATRIX m_matBip01LHand;

public:
	float	m_fAttacktiming;
	float m_fScaleSize;
	BoundingSquare			m_pBoundingSquare;
	BoundingSquare			m_pCopy;
	float m_fZealotdiffer;
	float m_fUpdateSpeed;
	void SetCallbackfunction(CallbackBindFunction function);
	void SetAttackCallbackfunction(CallbackBindFunction function);
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);
	void SetMove(D3DXMATRIX& move) { m_Move = move; }
	void SetRandomTrackPosition(); // 테스트용
	void SetRotationAngle(float angle); 
	void SetRotationMatrix(D3DXMATRIX rotation) { m_RotationMat = rotation; }
	void SetUpdateSpeed(float t) { m_fUpdateSpeed = t; }
	D3DXVECTOR3* GetPositionPointer() { return  &m_vPosition; }
	
	void SetAnimation(int num);
	void ChangeAnimation(int num);

	void DotWorldSpace();

	void SetPlane();

	void CheckRender(D3DXVECTOR3 center, float radius);

	bool CheckShow(D3DXVECTOR3 center, float radius);

	float FIndMax(float x, float y, float z);






	
	void SetNextAni();
	int GetAninum() { return m_iNum; }
	void SetAnimationPlay(bool on) { m_bAnion = on; }
protected:
	
	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
	void Blending();

	void getAnotherMatrix(LPD3DXFRAME pFrame, D3DMATRIX* pParent);



public:
	void MeshRender(ST_BONE * pBone, LPD3DXEFFECT effect);
	void SetHP(int n) { m_iHp = n; }
	void SetAttack(int n) { m_iAttack = n; }
	int	 GetHP() { return m_iHp; }
	int	 GetAttack() { return m_iAttack; }
	void ShaderMeshRender(LPD3DXEFFECT effect);
public:
	TeicSkinnedMesh();
	TeicSkinnedMesh(char* szFolder, char* szFilename);
	~TeicSkinnedMesh();
	
	
};

