#pragma once
struct ST_BONE;

class TeicSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	//하나만 생성
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	D3DXMATRIX					m_Move;
	D3DXMATRIX					m_Rotation;
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
	int m_iHp;
	int m_iShield;
	int m_iAttack;
	int			m_Tracknum;
	int	m_iNum;
public:
	float m_fZealotdiffer;
	
	void SetCallbackfunction(CallbackBindFunction function);
	void SetAttackCallbackfunction(CallbackBindFunction function);
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);
	void SetMove(D3DXMATRIX& move) { m_Move = move; }
	void SetRandomTrackPosition(); // 테스트용
	void SetRotationMatrix(D3DXMATRIX ro) { m_Rotation = ro; }
	D3DXVECTOR3* GetPositionPointer() { return  &m_vPosition; }
	void SetAnimation(int num);
	void ChangeAnimation(int num);

	void SetHP(int n) { m_iHp = n; }
	void SetShield(int n) { m_iShield = n; }
	void SetAttack(int n) { m_iAttack = n; }
	void SetNextAni();
	int GetAninum() { return m_iNum; }
private:
	
	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
	void Blending();

public:
	TeicSkinnedMesh();
	TeicSkinnedMesh(char* szFolder, char* szFilename);
	~TeicSkinnedMesh();
	
	
};

