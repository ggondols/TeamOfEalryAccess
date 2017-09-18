#pragma once
class LDYCamera
{
private:
	float m_fangleX;
	float m_fangleY;
	POINT m_ptPrevMouse;
	bool m_isLButtonDown;
	D3DXVECTOR3 *m_pvTarget;
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3	m_vTartget;
	float m_fDistance;

	bool getMousePos;


public:
	LDYCamera();
	~LDYCamera();

	void Setup(D3DXVECTOR3 *pvTarget = NULL);
	void Update(D3DXVECTOR3 pvTarget);
	D3DXVECTOR3 getEye(void) { return m_vEye; }
	D3DXVECTOR3 getLook(void) { return *m_pvTarget; }

};

