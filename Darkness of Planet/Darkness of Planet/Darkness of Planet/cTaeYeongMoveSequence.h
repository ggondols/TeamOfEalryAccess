#pragma once
class cTaeYoengMoveOnce;
class cTaeYeongMoveSequence
{

private:

	int					m_nCurrActionIndex;

	D3DXVECTOR3			m_To;
	float				m_fTime;
	float				m_Speed;
	CallbackBindFunction	m_Callback;
	D3DXVECTOR3			m_present;
	D3DXMATRIX			m_Matrix;

public:
	bool				m_bSet;
	vector<cTaeYoengMoveOnce*>	m_vecAction;
	void AddAction(cTaeYoengMoveOnce* pAction);
	void Start();
	void Update();
	void SetCallback(CallbackBindFunction func);
	void OnFinish();

	D3DXVECTOR3 GetPresent();
	D3DXMATRIX  GetRoationMat();
	void SetPresent(D3DXVECTOR3 vec) { m_present = vec; }
	void SetTo(D3DXVECTOR3 to) { m_To = to; };
	void SetTime(float time) { m_fTime = time; };
	void SetSpeed(float speed) { m_Speed = speed; };
	void SetClear() { m_vecAction.clear(); };
	void MoveOnceCallback();

public:
	cTaeYeongMoveSequence();
	~cTaeYeongMoveSequence();
};

