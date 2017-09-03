#pragma once

namespace Hank
{
	class cCamera
	{
	private:
		float m_fangleX;
		float m_fangleY;
		POINT m_ptPrevMouse;
		bool m_isLButtonDown;
		D3DXVECTOR3 *m_pvTarget;
		D3DXVECTOR3 m_vEye;
		float m_fDistance;

		bool getMousePos;


	public:
		cCamera();
		~cCamera();

		void Setup(D3DXVECTOR3 *pvTarget = NULL);
		void Update();
		D3DXVECTOR3 getEye(void) { return m_vEye; }
		D3DXVECTOR3 getLook(void) { return *m_pvTarget; }

	};
}