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
	m_stBulletSquare.m_fSizeY = 0.01f;
	m_stBulletSquare.m_fSizeX = 0.01f;
	m_stBulletSquare.m_fSizeZ = 0.01f;
	m_stBulletSquare.m_vCenterPos = D3DXVECTOR3(0, 0, 0);
	m_stBulletSquare.m_vXdir = D3DXVECTOR3(1, 0, 0);
	m_stBulletSquare.m_vYdir = D3DXVECTOR3(0, 1, 0);
	m_stBulletSquare.m_vZdir = D3DXVECTOR3(0, 0, 1);
	m_pObbcollision = new TeicObbCollision;
	m_pBresenham = new TeicBresenham;
}

void TeicShoot::Shoot()
{
	m_vShootPosition = m_pCamera->getEye();
	m_vShootDir = m_pCamera->getShootTarget() - m_vShootPosition;
	D3DXVec3Normalize(&m_vShootDir, &m_vShootDir);
	m_vFinish = m_vShootPosition + m_vShootDir* m_fShootDistance;

	m_stBulletSquare.m_vCenterPos = (m_vShootPosition + m_vFinish) / 2;
	m_stBulletSquare.m_fSizeZ = m_fShootDistance;
	CalRotation();
	m_vecPoint = m_pBresenham->FindNode(m_pCharacter->GetPositionYZero().x, m_pCharacter->GetPositionYZero().z,
		m_vFinish.x, m_vFinish.z);
	for (int i = 0; i < m_vecPoint.size(); i++)
	{
		if (m_vecPoint[i].x <0 || m_vecPoint[i].y <0 ||
			m_vecPoint[i].x >m_pNode->m_vRow.size() - 1 || m_vecPoint[i].y >m_pNode->m_vRow.size() - 1)break;
		m_vecTargetNode.push_back(&m_pNode->m_vRow[m_vecPoint[i].y].m_vCol[m_vecPoint[i].x] );
	}
	for (int i = 0; i < m_vecTargetNode.size(); i++)
	{
		if (m_vecTargetNode[i]->m_pBoundInfo != NULL)
		{
			for (int j = 0; j < m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding.size(); j++)
			{
				if (m_pObbcollision->CheckCollision(m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j], &m_stBulletSquare) == true)
				{
					if (m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_bHit == true) continue;
					m_vecTargetNode[i]->m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject->m_bHit = true;
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

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 0,0,0);
	m_stRect = ST_PN_Rectangle(m_stBulletSquare.m_fSizeX , m_stBulletSquare.m_fSizeY , m_stBulletSquare.m_fSizeZ);
	
	GETDEVICE->SetTransform(D3DTS_WORLD, &(matR*trans));
	GETDEVICE->SetFVF(ST_PN_VERTEX::FVF);
	GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &m_stRect.m_vecVertex[0], sizeof(ST_PN_VERTEX));
	
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
