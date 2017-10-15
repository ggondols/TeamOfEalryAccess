#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "TeicEnemy.h"
#include "TeicMoveSequence.h"
class Loading;

class LoadingScene : public cGameNode
{
private:
	TeicEnemy*      m_pEnemy[10];
	LPD3DXMESH      m_pMesh;
	LPD3DXEFFECT    m_pEffect;
	LPDIRECT3DTEXTURE9 m_pSpace;
	Hank::cCamera*	m_pCamera;
	bool			m_bCursor;
	float			m_fMovetime;
	LPDIRECT3DSURFACE9 surfcursor;
	LPDIRECT3DTEXTURE9 m_cursortex;
	LPDIRECT3DTEXTURE9 m_cursortex2;

	TeicMoveSequence*	m_vecEnemyCollisionMove[10];

	std::vector< D3DXVECTOR3 >		m_BoundingBoxVertices;

	std::vector< D3DMATERIAL9 >			m_mtrls;
	std::vector< LPDIRECT3DTEXTURE9 >	m_pTextures;
	std::vector< LPDIRECT3DTEXTURE9 >	m_pNormalTextures;
	D3DXVECTOR3  m_vcenter;
	//////////

	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;

	bool							m_bHasNormals;
	bool							m_bHasEmissive;
	std::string						m_name;

	void CallbacOn(int n);
public:
	//Model infomation for bounding box
	D3DXVECTOR3 m_vMin, m_vMax;
	D3DXVECTOR3 m_centerPos;
	float		m_fSizeX, m_fSizeY, m_fSizeZ;

	float		m_fAttacktime;

	int			m_iNum;
	ST_PT_VERTEX			m_stVertex[6];
	/////////
public:
	
	Loading* m_pLoading;
	HANDLE thread1;
	DWORD dwThread1;
	bool			m_bIsthreading;
	CRITICAL_SECTION cs;
public:
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();

	HRESULT LoadModel(const string & path, const string & name, LPDIRECT3DDEVICE9 pDevice, bool bSkybox);

	LPD3DXEFFECT LoadEffect(const char * szFileName);

	virtual void Update();

	LoadingScene();
	~LoadingScene();
};

