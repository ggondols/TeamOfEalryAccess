#include "stdafx.h"
#include "cStaticMeshLoader.h"
#include <fstream>
#include <sstream>
#include "cModel.h"
#include "cModelManager.h"
#include "cObjectNode.h"




cStaticMeshLoader::cStaticMeshLoader()
{
}


cStaticMeshLoader::~cStaticMeshLoader()
{
}

void cStaticMeshLoader::ScriptLoader(const string& fullPath, std::list<cObjectNode*>& cNodes)
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
			int a = 0;
			a = 1;
			continue;
		}
		else if (token == "[Placement]")
		{
			ObjPlacement(file, cNodes);
		}
		else if (token == "[Finish]")
		{
			break;
		}
	}


}
void cStaticMeshLoader::ObjPlacement(ifstream & file, std::list<cObjectNode*>& cNodes)
{
	string buffer, token;

	cObjectNode* pTempNode = NULL;
	cModel*		 pModel = NULL;

	while (getline(file, buffer))
	{
		istringstream iss(buffer);
		iss >> token;

		if (token == "MeshNumber")
		{
			int id;
			iss >> id;
			pModel = MODELMANAGER->GetModel(m_meshFileName[id]);
		}
		else if (token == "S_R_T")
		{
			if (pModel)
				ObjPlacementSRTin(file, *pModel, cNodes);
		}
		else if (token == "[end]")
		{
			break;
		}
	}
}

void cStaticMeshLoader::ObjPlacementSRTin(ifstream & file, cModel& pModel, std::list<cObjectNode*>& cNodes)
{

	string buffer, token;

	while (getline(file, buffer))
	{
		istringstream iss(buffer);
		iss >> token;

		if (token == "[end]")
		{
			break;
		}
		else if (token == "in")
		{

			cObjectNode* tempObject = new cObjectNode(&pModel);

			float sX, sY, sZ; //scale
			float tX, tY, tZ; //translate
			float rX, rY, rZ; //rotation
			char dummy; // '/' 받기용

			iss >> sX >> dummy >> sY >> dummy >> sZ;		
			iss >> rX >> dummy >> rY >> dummy >> rZ;		
			iss >> tX >> dummy >> tY >> dummy >> tZ;
			
			// initialise spartial information 공간 정보 정의
			tempObject->SetSclling(sX, sY, sZ);
			tempObject->SetOrientation(D3DXQUATERNION(rX, rY, rZ, 1));
			tempObject->SetPosition(D3DXVECTOR3(tX, tY, tZ));
			tempObject->m_bVisible = true;


			/*	float qx, qy, qz, qw;
				qx = x * sin(angle / 2);
				qy = y * sin(angle / 2);
				qz = z * sin(angle / 2);
				qw = cos(angle / 2);
				D3DXQUATERNION m_qQuaternion(qx, qy, qz, qw);
			*/
			cNodes.push_back(tempObject);

		}

	}

}

void cStaticMeshLoader::CreateMesh(ifstream & file)
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

			return;
		}

	}

}

