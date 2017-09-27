#pragma once
#include "TeicParticle.h"
class TeicParticleSystem
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	vector<TeicParticle*>		m_vecParticle;
	string					m_text;
public:

	void Setup(D3DXVECTOR3 center, float Maxparticle, float lifespan, float lifespanVariance,
		float startsize, float startsizeVariance, float finishsize, float finishsizevariance,
		float emitterangle, float anglevariance, float startrot, float startrotvar, float endrot,
		float endrotvar, float speed, float speedvar, D3DXVECTOR3 startcolor, int startcolorvar,
		D3DXVECTOR3 endcolor, int endcolorvar,
		char* texture, float dotsize, float radius, bool loop);
	void Update();
	void Render();
	void Release();
	void Start();
	void End();

public:
	TeicParticleSystem();
	~TeicParticleSystem();
};

