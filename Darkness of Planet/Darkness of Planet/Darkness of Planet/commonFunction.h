#pragma once
#include "stdafx.h"
namespace D3D {

	//레이 계산
	struct Ray
	{
		D3DXVECTOR3 _origin;
		D3DXVECTOR3 _direction;
	};

	inline Ray CalcPickingRay(int x, int y)
	{
		float px = 0.0f;
		float py = 0.0f;

		D3DVIEWPORT9 vp;
		GETDEVICE->GetViewport(&vp);

		D3DXMATRIX proj;
		GETDEVICE->GetTransform(D3DTS_PROJECTION, &proj);

		px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
		py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

		D3D::Ray ray;
		ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ray._direction = D3DXVECTOR3(px, py, 1.0f);

		return ray;

	}

	inline void TransformRay(D3D::Ray* ray, D3DXMATRIX* T)
	{
		//광선의 원점을 변환한다. w = 1;
		D3DXVec3TransformCoord(&ray->_origin, &ray->_origin, T);
		//광선의 방향을 변환한다. w = 0;
		D3DXVec3TransformNormal(&ray->_direction, &ray->_direction, T);

		//방향을 정규화 한다.
		D3DXVec3Normalize(&ray->_direction, &ray->_direction);
	}

	struct BoundingSphere
	{
		//BoundingSphere() {};


		D3DXVECTOR3 _center;
		float		_radius;
	};
	//스피어 픽킹
	inline bool ChractorChoose(D3D::Ray* ray, BoundingSphere *sphere)
	{

		D3DXVECTOR3 v = ray->_origin - sphere->_center;

		float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
		float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);

		// find the discriminant  판별식을 찾는다.
		float discriminant = (b*b) - (4.0f*c);

		//test for imaginary number 가상의 수에 대한 테스트
		if (discriminant < 0.0f)
			return false;

		discriminant = sqrtf(discriminant);

		float s0 = (-b + discriminant) / 2.0f;
		float s1 = (-b - discriminant) / 2.0f;

		// 해가 >= 0일 경우 구체를 교차하는 것이다.
		// if a solution is >= 0, then we intersected the sphere
		if (s0 >= 0.0f || s1 >= 0.0f)
			return true;

		return false;
	}

	//메테리얼 자동 생성
	inline D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = a;
		mtrl.Diffuse = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power = p;

		return mtrl;
	}

	D3DMATERIAL9 WHITE_MTRL = InitMtrl(D3DXCOLOR(1, 1, 1, 1.0f), D3DXCOLOR(1, 1, 1, 1.0f), D3DXCOLOR(1, 1, 1, 1.0f), D3DXCOLOR(0, 0, 0, 1.0f), 8.0f);
	D3DMATERIAL9 RED_MTRL = InitMtrl(D3DXCOLOR(1, 0, 0, 1.0f), D3DXCOLOR(1, 0, 0, 1.0f), D3DXCOLOR(1, 0, 0, 1.0f), D3DXCOLOR(0, 0, 0, 1.0f), 8.0f);
	D3DMATERIAL9 GREEN_MTRL =InitMtrl(D3DXCOLOR(0, 1, 0, 1.0f), D3DXCOLOR(0, 1, 0, 1.0f), D3DXCOLOR(0, 1, 0, 1.0f), D3DXCOLOR(0, 0, 0, 1.0f), 8.0f);
	D3DMATERIAL9 BLUE_MTRL = InitMtrl(D3DXCOLOR(0, 0, 1, 1.0f), D3DXCOLOR(0, 0, 1, 1.0f), D3DXCOLOR(0, 0, 1, 1.0f), D3DXCOLOR(0, 0, 0, 1.0f), 8.0f);
	D3DMATERIAL9 YELLOW_MTRL = InitMtrl(D3DXCOLOR(1, 0, 1, 1.0f), D3DXCOLOR(1, 0, 1, 1.0f), D3DXCOLOR(1, 0, 1, 1.0f), D3DXCOLOR(0, 0, 0, 1.0f), 8.0f);



	//다이렉션 라이트 생성
	inline 	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3 &direction, D3DXCOLOR &color)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = color * 0.4f;
		light.Diffuse = color;
		light.Specular = color*0.6f;
		light.Direction = direction;

		return light;
	}

	inline void SetLight()
	{
		D3DLIGHT9 stLight;
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));
		stLight.Type = D3DLIGHT_DIRECTIONAL;
		D3DXVECTOR3 vDir(1, -1, 1);
		D3DXVec3Normalize(&vDir, &vDir);
		stLight.Direction = vDir;
		stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		GETDEVICE->SetLight(0, &stLight);
		GETDEVICE->LightEnable(0, true);
	}

	inline void SetFont(OUT LPD3DXFONT& pFont)
	{
		D3DXFONT_DESC desc;
#pragma warning(disable : 4996)
		ZeroMemory(&desc, sizeof(desc));
		desc.CharSet = DEFAULT_CHARSET;
		strcpy(desc.FaceName, "Consolas");
		desc.Height = 20;
		desc.Width = 10;
		desc.Weight = FW_BOLD;
		desc.Quality = DEFAULT_QUALITY;
		desc.MipLevels = 1;
		desc.Italic = 0;
		desc.OutputPrecision = OUT_DEFAULT_PRECIS;
		desc.PitchAndFamily = FF_DONTCARE;

		D3DXCreateFontIndirect(GETDEVICE, &desc, &pFont);
	}

	inline float GetDistanceVec3(D3DXVECTOR3 a, D3DXVECTOR3 b)
	{
		return sqrt(((a.x - b.x)*(a.x - b.x)) +
			((a.y - b.y)* (a.y - b.y)) +
			((a.z - b.z) *(a.z - b.z)));
	}

	inline float Lerp(float a, float b, float t)
	{
		return a - (a*t) + (b*t);
	}


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

	

	//노말 벡터 계산 삼각형의 방향 구해서 크로스 해줌 in in in out
	inline void ComputeNormal(IN D3DXVECTOR3 & p0, IN D3DXVECTOR3 & p1, IN D3DXVECTOR3 & p2, OUT D3DXVECTOR3 & out)
	{
		D3DXVECTOR3 u = p1 - p0;
		D3DXVECTOR3 v = p2 - p0;

		D3DXVec3Cross(&out, &u, &v);
		D3DXVec3Normalize(&out, &out);
	}

}