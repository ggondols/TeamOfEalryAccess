#pragma once

//목표
//마우스위치에 메쉬가 생성되고 키 입력을 통한 사이즈 조절 방향조절후
//클릭을 통해 그 위치에 변형된 정보를 보함한 오브젝트를 생성
//생성된 데이터를 기준으로 별도의 간소화된 구조체에 넣어두고
//랜더로 출력 이후 세이브를 통해 데이터를 저장


struct sSIMPLEOBJ
{
	D3DXVECTOR3				m_position;
	D3DXQUATERNION			m_orientation;
	D3DXVECTOR3				m_scalling;
	D3DXMATRIX				m_matScale;

	LPD3DXMESH				m_pSelectMesh;
};

class cSprayMesh
{
private:

public:
	D3DXVECTOR3		m_position;
	D3DXQUATERNION	m_orientation;
	D3DXVECTOR3		m_scalling;
	D3DXMATRIX		m_matScale;

	LPD3DXMESH				m_pSelectMesh;
	vector<LPD3DXMESH>		m_pMeshList;

	list<sSIMPLEOBJ>	m_ObjSimpleList;
	
	
	void EntryMesh(LPD3DXMESH pMesh);

	void Update();

	void Render();



	cSprayMesh();
	~cSprayMesh();
};

