#include "stdafx.h"
#include "cSprayMesh.h"
#include <fstream>
#include <sstream>
#include "cModel.h"
#include "cRayCaster.h"
#include "iMap.h"
#include "cHeightMap.h"


void cSprayMesh::EntryModel(cModel* pMesh)
{
	m_pModelList.push_back(pMesh);
}

void cSprayMesh::EntryMesh(LPD3DXMESH pMesh)
{
	m_pMeshList.push_back(pMesh);
}
void cSprayMesh::Update()
{
	static int currentIndex;

	//맵 좌표 피킹 Y값 헤이트맵 보정 오차있음 
	cRayCaster r;
	m_position = r.GetMousePtFromNode(g_ptMouse.x, g_ptMouse.y, m_vecNode);
	iMap* m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");
	m_pMap->GetHeight(m_position.x, m_position.y, m_position.z);

	m_position.y += m_fCustomHigh;

	if (!m_pSelectModel)
		if (!m_pModelList.empty())
			m_pSelectModel = m_pModelList[currentIndex];


	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (currentIndex < m_pModelList.size() - 1)
			m_pSelectModel = m_pModelList[++currentIndex];
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (currentIndex > 0)
			m_pSelectModel = m_pModelList[--currentIndex];
	}


	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		m_scalling.x -= 0.1;
		m_scalling.y -= 0.1;
		m_scalling.z -= 0.1;

		if (m_scalling.x < 0.1f) {
			m_scalling.x += 0.09;
			m_scalling.y += 0.09;
			m_scalling.z += 0.09;
		}
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		m_scalling.x += 0.1;
		m_scalling.y += 0.1;
		m_scalling.z += 0.1;

		if (m_scalling.x < 0.2f) {
			m_scalling.x -= 0.09;
			m_scalling.y -= 0.09;
			m_scalling.z -= 0.09;
		}
	}
	else if (KEYMANAGER->isOnceKeyDown('P')) //print
	{
  		if (m_pSelectModel) {
			sSIMPLEOBJ obj(m_position, m_orientation, m_scalling, m_pSelectModel);
			m_ObjSimpleList.push_back(obj);
		}
	}
	else if (KEYMANAGER->isOnceKeyDown('O')) // save
	{
		if (!m_ObjSimpleList.empty())
		{
			SaveObjList();
		}
	}
	if (KEYMANAGER->isOnceKeyDown('L')) //undo remove
	{
		if (!m_ObjSimpleList.empty())
			m_ObjSimpleList.pop_back();
	}


	if (KEYMANAGER->isStayKeyDown('I')) //roatate Y
	{
		m_fAngleY -= 0.1;
	}
	else if (KEYMANAGER->isStayKeyDown('K')) //rotate Y
	{
		m_fAngleY += 0.1;
	}
	if (KEYMANAGER->isStayKeyDown('U')) //roatate Z
	{
		m_fAngleZ -= 0.1;
	}
	else if (KEYMANAGER->isStayKeyDown('J')) //roatate Z
	{
		m_fAngleZ += 0.1;
	}
	if (KEYMANAGER->isStayKeyDown('Y')) //roatate X
	{
		m_fAngleX -= 0.1;
	}
	else if (KEYMANAGER->isStayKeyDown('H')) //roatate X
	{
		m_fAngleX += 0.1;
	}

	if (KEYMANAGER->isStayKeyDown('T'))
	{
		m_fCustomHigh -= 0.1;
	}
	else if (KEYMANAGER->isStayKeyDown('G'))
	{
		m_fCustomHigh += 0.1;
	}
	
	
}

void cSprayMesh::SaveObjList(void)
{
	ofstream openFile("Data/Script/LastestSave.txt");

	map<string, vector<string>> buffer;

	// 모델의 갯수 만큼 스트링 벡터의 벡터 버퍼 생성
	// 각각 스트링벡터는 줄단위의 데이터를 저장

	for each(auto p in m_ObjSimpleList)
	{
		string temp;

		float x, y, z, w;

		temp = "in\t";

		x = p.m_scalling.x;
		y = p.m_scalling.y;
		z = p.m_scalling.z;

		temp += toString(x) + "/" + toString(y) + "/" + toString(z) + "\t";

		x = p.m_orientation.x;
		y = p.m_orientation.y;
		z = p.m_orientation.z;
		w = p.m_orientation.w;

		temp += toString(x) + "/" + toString(y) + "/" + toString(z) + "/" + toString(w) + "\t";

		x = p.m_position.x;
		y = p.m_position.y;
		z = p.m_position.z;

		temp += toString(x) + "/" + toString(y) + "/" + toString(z);

		buffer[p.m_pSelectModel->m_name].push_back(temp); //이름이 있으면 추가하고 없으면 생성 하게 된다.
	}

	for each(auto p in buffer)
	{
		openFile << p.first << endl;
		for (int i = 0; i < p.second.size(); i++)
		{
			openFile << p.second[i] << endl;
		}
	}

	//openFile << temp << endl;
}


void cSprayMesh::SetTargetNode(vector<D3DXVECTOR3>& vec)
{
	m_vecNode = vec;
}

void cSprayMesh::Render()
{
	D3DXMATRIX matWorld, matT, matR, matS;
	D3DXMatrixIdentity(&matWorld);


	//크기 계산
	D3DXMatrixScaling(&matS, m_scalling.x, m_scalling.y, m_scalling.z);



	//쿼터니언 회전 계산
	//m_orientation = D3D::GetAngletoQuaternion(m_position.x, m_position.y, m_position.z, m_fAngle);
	D3DXQuaternionRotationYawPitchRoll(&m_orientation, m_fAngleX, m_fAngleY, m_fAngleZ);

	//이거 쓰면 헬리콥터됨
	/*D3DXQUATERNION rotQuat, rotResult;
	D3DXQuaternionRotationYawPitchRoll(&rotQuat, m_fAngle, 0,0 );

	D3DXQuaternionMultiply(&rotResult, &m_orientation, &rotQuat);
	m_orientation = rotResult;*/

	D3DXMatrixRotationQuaternion(&matR, &m_orientation);

	//이동위치 계산
	D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);

	//합체
	matWorld = matS * matR * matT;

	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);


	if (m_pSelectModel)
		m_pSelectModel->Render(GETDEVICE);

	for each(auto p in m_ObjSimpleList)
	{
		GETDEVICE->SetTransform(D3DTS_WORLD, &p.m_matWorld);
		p.m_pSelectModel->Render(GETDEVICE);
	}

}

void cSprayMesh::Release()
{
	for each(auto p in m_pMeshList)
	{
		//	SAFE_RELEASE(p);
	}
}

cSprayMesh::cSprayMesh()
	: m_position(0, 0, 0)
	, m_orientation(0, 0, 0, 1)
	, m_scalling(0.01, 0.01, 0.01)
	, m_vecNode(vector<D3DXVECTOR3>())
	, m_fAngleX(0)
	, m_fCustomHigh(0)
{
	D3DXMatrixIdentity(&m_matScale);
	D3D::GetAngletoQuaternion(m_position.x, m_position.y, m_position.z, m_fAngleX);
}


cSprayMesh::~cSprayMesh()
{
}


void cSprayMesh::MeshLoadFromScript(const std::string& fullPath)
{
	ifstream file(fullPath);

	string buffer, token;

	if (file.good() == false)
	{
		return;
	}
	buffer.reserve(1024);

	while (getline(file, buffer))
	{
		istringstream iss(buffer);
		iss >> token;

		if (token == "[MeshLoad]")
		{
			CreateMesh(file);

			continue;
		}
		else if (token == "[Finish]")
		{
			break;
		}

	}
}

void cSprayMesh::CreateMesh(ifstream & file)
{
	string buffer, token;

	m_meshId.push_back(m_meshId.size() + 1);

	while (getline(file, buffer))
	{
		istringstream iss(buffer);
		iss >> token;

		if (token == "path")
		{
			string path;
			iss >> path;
			m_meshPath.push_back(path);
		}
		else if (token == "filename")
		{
			string name;
			iss >> name;
			m_meshFileName.push_back(name);
		}
		else if (token == "[end]")
		{
			int index = m_meshId.size() - 1;
			string fullPath = m_meshPath[index];
			string fileName = m_meshFileName[index];
			MODELMANAGER->LoadModel(fullPath, fileName);
			m_pModelList.push_back(MODELMANAGER->GetModel(fileName));

			return;
		}

		

	}
}