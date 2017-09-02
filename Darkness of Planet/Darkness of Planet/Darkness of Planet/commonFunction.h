#pragma once
//##############################
//#####    2017/09/02    #######
//#####  comman funtion  #######
//##############################

namespace D3D {
	struct Ray
	{
		D3DXVECTOR3 _origin;
		D3DXVECTOR3 _direction;
	};

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	extern D3DMATERIAL9 WHITE_MTRL;
	extern D3DMATERIAL9 RED_MTRL;
	extern D3DMATERIAL9 GREEN_MTRL;
	extern D3DMATERIAL9 BLUE_MTRL;
	extern D3DMATERIAL9 YELLOW_MTRL;


	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3 &direction, D3DXCOLOR &color);
	/*extern D3DXCOLOR WHITE;
	extern D3DXCOLOR RED;
	extern D3DXCOLOR GREEN;
	extern D3DXCOLOR BLUE;
	extern D3DXCOLOR BLACK;*/

	const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

	const D3DXCOLOR BEACH_SAND(D3DCOLOR_XRGB(255, 249, 157));
	const D3DXCOLOR DESERT_SAND(D3DCOLOR_XRGB(250, 205, 135));

	const D3DXCOLOR LIGHTGREEN(D3DCOLOR_XRGB(60, 184, 120));
	const D3DXCOLOR  PUREGREEN(D3DCOLOR_XRGB(0, 166, 81));
	const D3DXCOLOR  DARKGREEN(D3DCOLOR_XRGB(0, 114, 54));

	const D3DXCOLOR LIGHT_YELLOW_GREEN(D3DCOLOR_XRGB(124, 197, 118));
	const D3DXCOLOR  PURE_YELLOW_GREEN(D3DCOLOR_XRGB(57, 181, 74));
	const D3DXCOLOR  DARK_YELLOW_GREEN(D3DCOLOR_XRGB(25, 123, 48));

	const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
	const D3DXCOLOR DARKBROWN(D3DCOLOR_XRGB(115, 100, 87));

	void ComputeNormal(IN D3DXVECTOR3& p0, IN D3DXVECTOR3& p1, IN D3DXVECTOR3& p2, OUT D3DXVECTOR3& out);
	void TransformRay(D3D::Ray* ray, D3DXMATRIX* T);
	Ray CalcPickingRay(int x, int y);

	void SetLight();
	void SetFont(OUT LPD3DXFONT& pFont);

	float GetDistanceVec3(D3DXVECTOR3 a, D3DXVECTOR3 b);

	float Lerp(float a, float b, float t);
}

//RectMakeCenter
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };
	return rc;
}

//RectMake
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}
