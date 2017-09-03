#pragma once
class cTaeYoengMoveOnce
{
private:
	D3DXVECTOR3 m_From;
	D3DXVECTOR3 m_To;
	D3DXVECTOR3 m_previous;
	D3DXVECTOR3 m_present;
	D3DXMATRIX  m_RotationMat;
	float		m_fTime;
	float		m_Speed;
	bool		m_Start;
	float		m_fStarttime;
	float		m_fDistance;

	CallbackBindFunction	m_Callback;
public:
	float m_fPresenttime;


	void SetTo(D3DXVECTOR3 to) { m_To = to; };
	void SetFrom(D3DXVECTOR3 from) { m_From = from; };
	void SetTime(float time) { m_fTime = time; };
	void SetSpeed(float speed) { m_Speed = speed; };

	void SetCallback(CallbackBindFunction func);
	D3DXVECTOR3 GetPresent() { return m_present; }
	void		SetPresent(D3DXVECTOR3 vec) { m_present = vec; }
	D3DXMATRIX  GetRotation() { return m_RotationMat; }
	void		SetRotation(D3DXMATRIX mat) { m_RotationMat = mat; }
	void CalRotation();
	void Start();
	void Stop();
	void Update();

public:
	cTaeYoengMoveOnce();
	~cTaeYoengMoveOnce();
};

