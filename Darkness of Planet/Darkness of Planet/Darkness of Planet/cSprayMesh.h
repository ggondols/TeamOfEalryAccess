#pragma once

//��ǥ
//���콺��ġ�� �޽��� �����ǰ� Ű �Է��� ���� ������ ���� ����������
//Ŭ���� ���� �� ��ġ�� ������ ������ ���� ������Ʈ�� ����
//������ �����͸� �������� ������ ����ȭ�� ����ü�� �־�ΰ�
//������ ��� ���� ���̺긦 ���� �����͸� ����

class cModel;

struct sSIMPLEOBJ
{
	D3DXVECTOR3				m_position;
	D3DXQUATERNION			m_orientation;
	D3DXVECTOR3				m_scalling;
	D3DXMATRIX				m_matScale;
	D3DXMATRIX				m_matTrans;
	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matRotation;
	

	cModel*				m_pSelectModel;

	sSIMPLEOBJ() {};
	sSIMPLEOBJ(D3DXVECTOR3 pos, D3DXQUATERNION quter, D3DXVECTOR3 sacle, cModel* model)
	: m_position (pos)
		, m_orientation(quter)
		, m_scalling(sacle)
		, m_pSelectModel(model)
	{	
		D3DXMatrixScaling(&m_matScale, m_scalling.x, m_scalling.y ,m_scalling.z);
		D3DXMatrixRotationQuaternion(&m_matRotation, &m_orientation);
		D3DXMatrixTranslation(&m_matTrans, m_position.x, m_position.y, m_position.z);
		
		m_matWorld = m_matScale * m_matRotation*m_matTrans;
	}
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
	float			m_fAngle;
	float			m_fCustomHigh;

	LPD3DXMESH				m_pSelectMesh;
	vector<LPD3DXMESH>		m_pMeshList;

	cModel*					m_pSelectModel;
	vector<cModel*>			m_pModelList;

	list<sSIMPLEOBJ>	m_ObjSimpleList;

	vector<D3DXVECTOR3> m_vecNode;
	
	
	void EntryModel(cModel * pMesh);

	void EntryMesh(LPD3DXMESH pMesh);

	void Update();

	void SaveObjList(void);

	void SetTargetNode(vector<D3DXVECTOR3>& vec);

	void Render();

	void Release();

	cSprayMesh();
	~cSprayMesh();
	void MeshLoadFromScript(const std::string & fullPath);
	void CreateMesh(ifstream & file);


};

