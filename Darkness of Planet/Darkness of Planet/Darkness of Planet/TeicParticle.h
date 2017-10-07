#pragma once
class TeicParticle
{
private:

	float			m_fLifeSpan;
	float			m_fStartSize;
	float			m_fFinishSize;
	float			m_fEmitterAngle;
	float			m_fStartRot;
	float			m_fEndRot;
	float			m_fSpeed;
	D3DXVECTOR3		m_dwStartColor;
	D3DXVECTOR3		m_dwFinishColor;
	D3DXVECTOR3		m_dwNowcolor;
	bool			m_Start;
	bool			m_loop;
	float			m_fTime;

	float			m_delta;
	float			m_distance;
	float			m_deltadistance;


	D3DXVECTOR3		m_vLastPosition;
	float			m_fCurveAngleX;
	float			m_fCurveAngleY;
	float			m_fCurveAngleZ;
	float			m_fUpDown;
	
public:
	float			m_alpha;
	ST_PC_VERTEX*	m_pVertex;
	ST_PC_VERTEX	m_pVertexSample;
public:

	void Setup(ST_PC_VERTEX* vertex, float liftspan, float startsize, float finishsize, float emitterangle,
		float startrot, float endrot, float speed, D3DXVECTOR3 startcolor, D3DXVECTOR3 finishcolor, bool loop);
	void Setup2(ST_PC_VERTEX* vertex, float liftspan, D3DXVECTOR3 endposition,float angleX, float angleY, float angleZ,
		float speed, D3DXVECTOR3 startcolor, D3DXVECTOR3 finishcolor, bool loop);
	void SetPosition(ST_PC_VERTEX* vertex, D3DXVECTOR3 endposition);
	void Update();
	void Update2();
	void Update3();
	void Update4();
	void Update5();
	void Start();
	void Start2();
	void End();

public:
	TeicParticle();
	~TeicParticle();
};

