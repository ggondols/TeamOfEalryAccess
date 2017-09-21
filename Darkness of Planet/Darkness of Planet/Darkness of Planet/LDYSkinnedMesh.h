#pragma once
struct ST_BONE;
//###############################################
//#												#
//#		태영이꺼스킨드메쉬가져와서 수정하는중인       #
//#		스킨드 메쉬 클래스 입니다.              #
//#		2017-09-15								#	
//#												#
//#											  #
//#												#	
//#												#
//#											#
//###############################################

class LDYSkinnedMesh
{
	friend class cSkinnedMeshManager;

protected:
	//하나만 생성
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	D3DXMATRIX					m_Move;
	float						m_fScale;
	// 객체마다 생성
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(float, m_fAngle, Angle);
	bool						m_bBlending;
	float						m_fWeightDivide;
	float						m_fWeight;
	int							m_iCurrentAniNum;
	float						m_Starttime;
	float						m_Finishtime;
	float						m_Middletime;
	D3DXVECTOR3					position;
	bool						m_Damaging;
	CallbackBindFunction		m_callback;
	CallbackBindFunction        m_attackCallback;
	int							m_iHp;
	int							m_iShield;
	int							m_iAttack;
	int							m_Tracknum;
	int							m_iNum;

	D3DXMATRIX					m_RotationMat;
	bool						m_bCollision;


public:

	D3DXMATRIX					m_matHead;
	D3DXMATRIX					m_matWeapon;

public:
	float m_fZealotdiffer;

	void SetCallbackfunction(CallbackBindFunction function);
	void SetAttackCallbackfunction(CallbackBindFunction function);
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);
	void SetMove(D3DXMATRIX& move) { m_Move = move; }
	void SetRandomTrackPosition(); // 테스트용
	void SetRotationAngle(float angle);
	void SetRotationMatrix(D3DXMATRIX rotation) { m_RotationMat = rotation; }
	D3DXVECTOR3* GetPositionPointer() { return  &m_vPosition; }

	void SetAnimation(int num);
	void ChangeAnimation(int num);

	void SetHP(int n) { m_iHp = n; }
	void SetShield(int n) { m_iShield = n; }
	void SetAttack(int n) { m_iAttack = n; }
	void SetNextAni();
	int GetAninum() { return m_iNum; }
	void SetScale(float f) { m_fScale =f;}
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
	LDYSkinnedMesh();
	LDYSkinnedMesh(char* szFolder, char* szFilename);
	~LDYSkinnedMesh();


};

