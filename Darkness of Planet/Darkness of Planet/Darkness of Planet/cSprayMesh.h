#pragma once

//��ǥ
//���콺��ġ�� �޽��� �����ǰ� Ű �Է��� ���� ������ ���� ����������
//Ŭ���� ���� �� ��ġ�� ������ ������ ������ ������Ʈ�� ����
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

