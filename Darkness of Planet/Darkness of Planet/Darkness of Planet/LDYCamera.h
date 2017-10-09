#pragma once
#include "cSingletonBase.h"
class LDYCamera:public cSingletonBase<LDYCamera>
{
private:
	float m_fangleX;
	float m_fBoundingX;
	float m_fangleY;
	POINT m_ptPrevMouse;
	bool m_isLButtonDown;
	D3DXVECTOR3 *m_pvTarget;
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3	m_vTartget;
	

	bool getMousePos;


public:
	float m_fDistance;
	LDYCamera();
	~LDYCamera();

	void Setup(D3DXVECTOR3 *pvTarget = NULL);
	void Update(D3DXVECTOR3 * pvTarget, int num);
	//void Update(D3DXVECTOR3 *pvTarget);
	D3DXVECTOR3 getEye(void) { return m_vEye; }
	D3DXVECTOR3 getLook(void) { return *m_pvTarget; }
	D3DXVECTOR3 getShootTarget() { return m_vTartget; }
	float getAngleY(void) { return m_fangleY; }
	void rebound();
	void SetTarget(D3DXVECTOR3 * target) { m_pvTarget  = target;}
	
};

