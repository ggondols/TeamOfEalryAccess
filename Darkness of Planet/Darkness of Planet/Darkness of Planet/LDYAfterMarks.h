#pragma once
class TeicParticle;

class LDYAfterMarks
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	vector<TeicParticle*>	m_vecParticle;
	string					m_text;
	D3DXVECTOR3				m_vStartVar;
	D3DXVECTOR3				m_vEndVar;
	float					m_fDotSize;
	float					m_fDotSizevar;

public:
	LDYAfterMarks();
	~LDYAfterMarks();

	void Setup();
	void Update();
	void Render();
	void Release();
	void Start();
	void End();


};

