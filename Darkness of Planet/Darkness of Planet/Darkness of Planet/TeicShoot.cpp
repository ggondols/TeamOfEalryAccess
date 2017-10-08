#include "stdafx.h"
#include "TeicShoot.h"


void TeicShoot::Setup(HankcGrid *node, LDYCamera *camera, LDYCharacter* character)
{
	m_pNode = node;
	m_pCamera = camera;
	m_fShootDistance = 200.0f;
	m_fShootRange = 1.0f;
	m_vShootDir = D3DXVECTOR3(0, 0, 0);
	m_vShootPosition = D3DXVECTOR3(0, 0, 0);
	m_pCharacter = character;
	D3DXCreateLine(GETDEVICE, m_pLine);
	m_stBulletSquare.m_fSizeY = 0.1;
	m_stBulletSquare.m_fSizeX = 0.1;
	m_stBulletSquare.m_fSizeZ = 0.1;
	m_stBulletSquare.m_vCenterPos = D3DXVECTOR3(0, 0, 0);
	m_stBulletSquare.m_vXdir = D3DXVECTOR3(1, 0, 0);
	m_stBulletSquare.m_vYdir = D3DXVECTOR3(0, 1, 0);
	m_stBulletSquare.m_vZdir = D3DXVECTOR3(0, 0, 1);
	m_pObbcollision = new TeicObbCollision;
	m_pBresenham = new TeicBresenham;
}

void TeicShoot::Shoot(WeaponType type)
{
	m_vecPoint.clear();
	m_vecTargetNode.clear();
	m_vShootPosition = m_pCamera->getEye();
	m_vShootDir = m_pCamera->getShootTarget() - m_vShootPosition;
	D3DXVec3Normalize(&m_vShootDir, &m_vShootDir);
	m_vFinish = m_vShootPosition + m_vShootDir* m_fShootDistance;

	m_stBulletSquare.m_vCenterPos = (m_vShootPosition + m_vFinish) / 2;
	m_stBulletSquare.m_fSizeZ = m_fShootDistance/2;
	CalRotation();
	m_vecPoint = m_pBresenham->FindNodeAccuracy(m_vShootPosition.x, m_vShootPosition.z,
		m_vFinish.x, m_vFinish.z);
	/*m_vecDeletePoint= m_pBresenham->FindNode(m_vShootPosition.x, m_vShootPosition.z,
		m_pCharacter->GetPositionYZero().x, m_pCharacter->GetPositionYZero().z);*/
	for (int i = 0; i < m_vecPoint.size(); i++)
	{
		if (m_vecPoint[i].x <0 || m_vecPoint[i].y <0 ||
			m_vecPoint[i].x >m_pNode->m_vRow.size() - 1 || m_vecPoint[i].y >m_pNode->m_vRow.size() - 1)break;
		/*int a = 0;
		for (int j = 0; j < m_vecDeletePoint.size(); j++)
		{
			if (m_vecPoint[i].x == m_vecDeletePoint[j].x &&
				m_vecPoint[i].y == m_vecDeletePoint[j].y)
			{
				a = 1;
			}
		}
		if(a==0)*/
		m_vecTargetNode.push_back(&m_pNode->m_vRow[m_vecPoint[i].y].m_vCol[m_vecPoint[i].x] );
	}
	D3DXVECTOR3 position;
	for (int i = 0; i < m_vecTargetNode.size(); i++)
	{
		if (m_vecTargetNode[i]->m_pBoundInfo != NULL)
		{
			for (int j = 0; j < m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding.size(); j++)
			{
				if (m_pObbcollision->CheckCollision(m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j], &m_stBulletSquare) == true)
				{
					switch (type)
					{
					case Wp_Melee:
						break;
					case Wp_AA12:
						


						
						break;
					case Wp_AR6:
						position = GetPosition(m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_vecVertex, m_vShootPosition, m_vShootDir);
						SKILLEFFECTMANAGER->play("MChill", position, D3DXVECTOR3(0, 0, 0));
						m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_iHp -= DATABASE->GetItemValue("AR6");
						m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_bSlow = true;
						m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_fSlowTime = TIMEMANAGER->getWorldTime();
						break;
					case Wp_M4:
						position = GetPosition(m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_vecVertex, m_vShootPosition, m_vShootDir);
						SKILLEFFECTMANAGER->play("MBlood", position, D3DXVECTOR3(0, 0, 0));
						m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_iHp -= DATABASE->GetItemValue("M4");
						break;
					case Wp_MP5:
						break;
					case WP_FireGun:
						break;
					default:
						break;
					}
					
					/*if (m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_bHit == true) continue;
					m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_bHit = true;*/
					return;
				}
			}
		}
	}
}
void TeicShoot::CalRotation()
{
	

	D3DXMatrixLookAtLH(&matR,
		&m_vShootPosition,
		&m_vFinish,
		&D3DXVECTOR3(0, 1, 0));
	matR._41 = matR._42 = matR._43 = 0.0f;
	D3DXMatrixTranspose(&matR, &matR);
	
	D3DXVec3TransformNormal(&m_stBulletSquare.m_vXdir, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXVec3Normalize(&m_stBulletSquare.m_vXdir, &m_stBulletSquare.m_vXdir);
	D3DXVec3TransformNormal(&m_stBulletSquare.m_vYdir, &D3DXVECTOR3(0, 1, 0), &matR);
	D3DXVec3Normalize(&m_stBulletSquare.m_vYdir, &m_stBulletSquare.m_vYdir);
	D3DXVec3TransformNormal(&m_stBulletSquare.m_vZdir, &D3DXVECTOR3(0, 0, 1), &matR);
	D3DXVec3Normalize(&m_stBulletSquare.m_vZdir, &m_stBulletSquare.m_vZdir);



}

D3DXVECTOR3 TeicShoot::GetPosition(vector<ST_PN_VERTEX>  info, D3DXVECTOR3 rayorigin, D3DXVECTOR3 raydir)
{
	//// 바닥에 레이저 쏴서 마우스가 바닥 어디 찍었는지 확인하는 함수
	float distance = 9999999;
	D3DXVECTOR3 temp = D3DXVECTOR3(0, 0, 0);
	float savedistance = 9999999;
	for (int i = 0; i < info.size(); i += 3)
	{

		D3DXIntersectTri(&info[i].p,
			&info[i + 1].p,
			&info[i + 2].p, &rayorigin, &raydir, NULL, NULL, &distance);

		if (distance < 1000)
		{
			if (distance < savedistance)
			{
				savedistance = distance;
			}
			
		}

	}
	raydir = savedistance * raydir;
	temp = rayorigin + raydir;
	return temp;

}

void TeicShoot::Render()
{
	D3DXVECTOR3 temp = m_pCharacter->GetPosition();
	temp.y = 10;
	ST_PC_VERTEX vertex[6];
	vertex[0].p = temp;
	vertex[0].c = D3DCOLOR_XRGB(255, 0, 0);
	vertex[1].p = temp + m_stBulletSquare.m_vXdir;
	vertex[1].c = D3DCOLOR_XRGB(255, 0, 0);

	vertex[2].p = temp;
	vertex[2].c = D3DCOLOR_XRGB(0, 255, 0);
	vertex[3].p = temp + m_stBulletSquare.m_vYdir;
	vertex[3].c = D3DCOLOR_XRGB(0, 255, 0);

	vertex[4].p = temp;
	vertex[4].c = D3DCOLOR_XRGB(0, 0, 255);
	vertex[5].p = temp + m_stBulletSquare.m_vZdir;
	vertex[5].c = D3DCOLOR_XRGB(0, 0, 255);




	//vertex[2].p = m_vShootPosition;
	//vertex[2].c = D3DCOLOR_XRGB(0, 255, 0);
	//vertex[3].p = D3DXVECTOR3(0, 0, 0);
	//vertex[3].c = D3DCOLOR_XRGB(0, 255, 0);
	//vertex[4].p = m_vFinish;
	//vertex[4].c = D3DCOLOR_XRGB(0, 0, 255);
	//vertex[5].p = D3DXVECTOR3(0, 0, 0);
	//vertex[5].c = D3DCOLOR_XRGB(0, 0, 255);


	/*vertex[3].p = D3DXVECTOR3(10, 0, 10);
	vertex[3].c = D3DCOLOR_XRGB(255, 0, 0);
	vertex[4].p = m_pCharacter->GetPosition();
	vertex[4].c = D3DCOLOR_XRGB(255, 0, 0);
	vertex[5].p = D3DXVECTOR3(0, 0, 0);;
	vertex[5].c = D3DCOLOR_XRGB(255, 0, 0);*/




	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	GETDEVICE->SetTexture(0, NULL);
	GETDEVICE->SetFVF(ST_PC_VERTEX::FVF);
	GETDEVICE->DrawPrimitiveUP(D3DPT_LINELIST,3, vertex, sizeof(ST_PC_VERTEX));
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);



	MakeBoundingBox();
	/*D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 0,0,0);
	m_stRect = ST_PN_Rectangle(m_stBulletSquare.m_fSizeX , m_stBulletSquare.m_fSizeY , m_stBulletSquare.m_fSizeZ);
	
	GETDEVICE->SetTransform(D3DTS_WORLD, &(matR*trans));
	GETDEVICE->SetFVF(ST_PN_VERTEX::FVF);
	GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &m_stRect.m_vecVertex[0], sizeof(ST_PN_VERTEX));
	*/
}


TeicShoot::TeicShoot()
{
	m_pNode = NULL;
	m_pCamera = NULL;
	m_pCharacter = NULL;
	m_pLine = NULL;
	m_pObbcollision = NULL;
	m_pBresenham = NULL;
}


TeicShoot::~TeicShoot()
{
	/*SAFE_DELETE(m_pNode);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharacter);*/
	SAFE_DELETE(m_pLine);
	SAFE_DELETE(m_pObbcollision);
	SAFE_DELETE(m_pBresenham);
}




void TeicShoot::MakeBoundingBox()
{
	vector<ST_PN_VERTEX>	m_vecVertex;
	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	float fCubeSizeX = m_stBulletSquare.m_fSizeX;
	float fCubeSizeY = m_stBulletSquare.m_fSizeY;
	float fCubeSizeZ = m_stBulletSquare.m_fSizeZ;
	D3DXVECTOR3	center = m_stBulletSquare.m_vCenterPos;

	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y - fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y + fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y + fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y - fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y - fCubeSizeY, center.z + fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y + fCubeSizeY, center.z + fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y + fCubeSizeY, center.z + fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y - fCubeSizeY, center.z + fCubeSizeZ));



	vector<D3DXVECTOR3> vecNormal;
	vecNormal.push_back(D3DXVECTOR3(0, 0, -1));
	vecNormal.push_back(D3DXVECTOR3(0, 0, 1));
	vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(0, 1, 0));
	vecNormal.push_back(D3DXVECTOR3(0, -1, 0));

	//앞
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	//뒤
	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);
	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);
	//좌
	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);
	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);
	//우
	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);
	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);
	//상
	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);
	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);
	//하
	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);
	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVECTOR3 n = vecNormal[i / 6];
		m_vecVertex.push_back(ST_PN_VERTEX(p, n));
	}
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, -center.x, -center.y, -center.z);
	
	matWorld = trans*matR;
	D3DXMatrixTranslation(&trans, center.x, center.y, center.z);
	matWorld = matWorld* trans;
	//D3DXMatrixRotationY(&matWorld, m_pSkinnedMesh->GetAngle());
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	GETDEVICE->SetFVF(ST_PN_VERTEX::FVF);
	GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &m_vecVertex[0], sizeof(ST_PN_VERTEX));

}
