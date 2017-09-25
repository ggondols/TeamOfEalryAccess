#pragma once
#include "iMap.h"
//###############################################
//#												#
//#		맵 클래스 파일입니다			 		#
//#		정현이가 만들었고						#
//#	    제가 쿼드트리 형식으로 (4*9)36개로 나눠	#
//#		출력하는 방식으로 구성하였습니다		#
//#		2017-09-25								#	
//#					Edit by 태영				#
//###############################################

class cHeightMap : public iMap
{
private:
	LPD3DXMESH			m_pMesh[4];
	string				m_sTexture;
	D3DMATERIAL9		m_stMtl;
	int					m_nTile;
	vector<D3DXVECTOR3>	m_vecVertex;
	vector<ST_PNT_VERTEX>	m_vecVertexCopy;
public:
	cHeightMap(void);
	virtual ~cHeightMap(void);

	virtual void Load(string szFolder,
		string szFile,
		string szTexture,
		DWORD dwBytesPerPixel = 1);

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
	virtual void Render(D3DXVECTOR3 Characterposition) override;
	void MeshRender(int num);
	LPD3DXMESH getMesh() override { return m_pMesh[0]; }
};


