#pragma once

//��ǥ
//���콺��ġ�� �޽��� �����ǰ� Ű �Է��� ���� ������ ���� ����������
//Ŭ���� ���� �� ��ġ�� ������ ������ ���� ������Ʈ�� ����
//������ �����͸� �������� ������ ����ȭ�� ����ü�� �־�ΰ�
//������ ��� ���� ���̺긦 ���� �����͸� ����


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
	vector<string>	m_meshFileName;
	vector<string>	m_meshPath;
	vector<int>		m_meshId;

public:
	D3DXVECTOR3		m_position;
	D3DXQUATERNION	m_orientation;
	D3DXVECTOR3		m_scalling;
	D3DXMATRIX		m_matScale;

	LPD3DXMESH				m_pSelectMesh;
	vector<LPD3DXMESH>		m_pMeshList;

	list<sSIMPLEOBJ>	m_ObjSimpleList;

	vector<D3DXVECTOR3>& m_vecNode;
	
	
	void EntryMesh(LPD3DXMESH pMesh);

	void Update();

	void SetTargetNode(vector<D3DXVECTOR3>& vec);

	void Render();

	void Release();



	cSprayMesh();
	~cSprayMesh();
	void MeshLoadFromScript(const std::string & fullPath);
	void CreateMesh(ifstream & file);
};

