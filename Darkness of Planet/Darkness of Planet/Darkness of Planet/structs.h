#pragma once
#include <d3dx9.h>

//--------------------------------------------------------------//
// Light structures 
//--------------------------------------------------------------//

struct LightOmni_s
{
	LightOmni_s()
		: m_colour(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)),
		m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
		m_fMaxRange(0.0f),
		m_bActive(false),
		m_bCastsShadows(false),
		m_bVisible(false) {}

	LightOmni_s(D3DXVECTOR4& colour, D3DXVECTOR3& position, float maxrange, bool bActive = true, bool bCastsShadows = true)
		: m_colour(colour),
		m_position(position),
		m_fMaxRange(maxrange),
		m_bActive(bActive),
		m_bCastsShadows(bCastsShadows),
		m_bVisible(false) {}

	~LightOmni_s() {}

	D3DXVECTOR4 m_colour;
	D3DXVECTOR3 m_position;
	float		m_fMaxRange;
	bool		m_bActive;
	bool		m_bCastsShadows;

	// house keeping
	bool		m_bVisible;
};

struct LightDirectional_s
{
	LightDirectional_s()
		: m_colour(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)),
		m_fPitch(-90.0f),
		m_fYaw(0.0f),
		m_bActive(false),
		m_bCastsShadows(false),
		m_bVisible(false) {}

	LightDirectional_s(D3DXVECTOR4& colour, float fPitch, float fYaw, bool bActive = true, bool bCastsShadows = true)
		: m_colour(colour),
		m_fPitch(fPitch),
		m_fYaw(fYaw),
		m_bActive(bActive),
		m_bCastsShadows(bCastsShadows),
		m_bVisible(false) {}

	~LightDirectional_s() {}

	D3DXVECTOR4 m_colour;
	float		m_fPitch;		// RADIANS
	float		m_fYaw;			// RADIANS
	bool		m_bActive;
	bool		m_bCastsShadows;

	// house keeping
	bool		m_bVisible;
};

struct LightSpot_s
{
	LightSpot_s()
		: m_colour(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)),
		m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
		m_fPitch(0.0f),
		m_fYaw(0.0f),
		m_fMaxRange(8.0f),
		m_fInnerAngle(0.0f),
		m_fOuterAngle(0.0f),
		m_fFalloff(1.0f),
		m_bActive(false),
		m_bCastsShadows(false),
		m_bVisible(false),
		m_pConeMesh(NULL) {}

	LightSpot_s(D3DXVECTOR4& colour, D3DXVECTOR3& position, float fPitch, float fYaw, float fMaxRange,
		float fInnerAngle, float fOuterAngle, float fFalloff, bool bActive = true, bool bCastsShadows = true)

		: m_colour(colour),
		m_position(position),
		m_fPitch(fPitch),
		m_fYaw(fYaw),
		m_fMaxRange(fMaxRange),
		m_fInnerAngle(fInnerAngle),
		m_fOuterAngle(fOuterAngle),
		m_fFalloff(fFalloff),
		m_bActive(bActive),
		m_bCastsShadows(bCastsShadows),
		m_bVisible(false),
		m_pConeMesh(NULL) { }

	~LightSpot_s() {}

	D3DXVECTOR4 m_colour;
	D3DXVECTOR3 m_position;
	float		m_fPitch;		// RADIANS
	float		m_fYaw;			// RADIANS
	float		m_fMaxRange;
	float		m_fInnerAngle;	// Full angle RADIANS -- not the half angle.  Remember to mult by 0.5 before doing cone calcs in shader
	float		m_fOuterAngle;	// Full angle RADIANS -- not the half angle.  Remember to mult by 0.5 before doing cone calcs in shader	
	float		m_fFalloff;		// 1.0f = standard
	bool		m_bActive;
	bool		m_bCastsShadows;

	ID3DXMesh*	m_pConeMesh;

	// house keeping
	bool		m_bVisible;
};