#pragma once
class TeicCollisionMove;
class TeicSkinnedMesh;
class TeicMoveSequence
{
private:
	
	int					m_nCurrActionIndex;

	D3DXVECTOR3			m_To;
	float				m_fTime;
	float				m_Speed;
	CallbackBindFunction	m_Callback;



public:
	bool				m_bStart;
	vector<TeicCollisionMove*>	m_vecAction;
	void AddAction(TeicCollisionMove* pAction);
	void Start();
	void Update();
	void SetCallback(CallbackBindFunction func);
	void OnFinish();


	
	void SetClear(); 
	void MoveOnceCallback();
public:
	TeicMoveSequence();
	~TeicMoveSequence();
};

