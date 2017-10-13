#pragma once

class HankcGrid;
#include "cModel.h"

class cSpatial
{
public:
	cSpatial();
	virtual ~cSpatial();
	// Rotates the object around the x, y and z axes by the amount of DEGREES specified for each
	void RotateBy(float fX, float fY, float fZ);

	// Rotates by a quaternion.  New orientation = old quat * rot
	void RotateBy(const D3DXQUATERNION& rot);

	// Sets orientation to new pitch, roll & yaw.  DEGREES.
	void SetOrientation(float pitch, float roll, float yaw);

	// Returns the object's world matrix in both the pOut parameter and the return value
	D3DXMATRIX* GetWorldMatrix(D3DXMATRIX* pOut);

	// Object's Update function.  Call once per frame
	virtual void Update(float fTimeDelta);

	// INLINES ///////////////////////////////////////////////////////////////////////

	// Move BY displacement delta vector specified by parameter
	void MoveBy(const D3DXVECTOR3& delta) { m_position += delta; }
	// Move by x, y z displacement deltas
	void MoveBy(const float x, const float y, const float z) { m_position.x += x; m_position.y += y; m_position.z += z; }
	// Set the position to this explicit position vector
	void SetPosition(const D3DXVECTOR3& position) { m_position = position; }
	// Set the orientation to this explicit rotation quaternion 
	void SetOrientation(const D3DXQUATERNION& orientation) { m_orientation = orientation; }
	// Set the Scalling to this Mesh
	void SetSclling(float x, float y, float z);

	// Return position as a vector
	const D3DXVECTOR3&		GetPosition()		const { return m_position; }

	// Return orientation as a quaternion
	const D3DXQUATERNION&	GetOrientation()	const { return m_orientation; }

protected:
	D3DXVECTOR3		m_position;
	D3DXQUATERNION	m_orientation;
	D3DXVECTOR3		m_scalling;
	D3DXMATRIX		m_matScale;
};

class cObjectNode : public cSpatial
{
private:
public:

	cModel*			m_pModel;
	//cBoundingSphere m_BoundingSphere;
	//CAABB			m_BoundingBox;

	bool			m_bVisible;
	UINT			m_uiIlluminationFlags;
	UINT			m_uiShadowFlags;
	D3DXVECTOR3     m_vCenterInfo;
public:
	cObjectNode()
		: m_pModel(NULL),
		m_bVisible(false),
		m_uiIlluminationFlags(0),
		m_uiShadowFlags(0)
	{
		//m_BoundingSphere.Empty();
	}

	cObjectNode(cModel* pModel)
		: m_pModel(pModel),
		m_bVisible(false),
		m_uiIlluminationFlags(0),
		m_uiShadowFlags(0)
	{

	}
	
	~cObjectNode();

	// Object's update function.  Call once per frame
	void Update(float fTimeDelta);

	// INLINES /////////////////////////////////////////////////////

	// Returns a pointer to the model used by this node
	cModel*	GetModel() const { return m_pModel; }

	// Sets the model used by the node and updates the node's bounding sphere
	void SetModel(cModel* pModel)
	{
		m_pModel = pModel;
	}

	void PutBoundBoxtoNodeByPosition(OUT HankcGrid* node)
	{
		/*m_pNode->m_vRow[-(pos.z / NodeLength)].m_vCol[pos.x / NodeLength].m_pBoundInfo = new nNodeBoundInfo;
		m_pNode->m_vRow[0].m_vCol[0].m_pBoundInfo->m_vecBounding.push_back();*/

		BoundingSquare* temp = new BoundingSquare;
		
		D3DXMATRIX rot;
		D3DXVECTOR3 vDir(1,0,0);
		D3DXMatrixRotationQuaternion(&rot, &m_orientation);
		D3DXVec3TransformNormal(&vDir, &vDir, &rot);
		temp->m_vXdir = vDir;

		vDir = D3DXVECTOR3(0, 1, 0);		
		D3DXVec3TransformNormal(&vDir, &vDir, &rot);
		temp->m_vYdir = vDir;;

		vDir =  D3DXVECTOR3(0, 0, 1);
		D3DXVec3TransformNormal(&vDir, &vDir, &rot);
		temp->m_vZdir = vDir;;

		temp->m_fSizeX = m_pModel->m_fSizeX * m_scalling.x;
		temp->m_fSizeY = m_pModel->m_fSizeY * m_scalling.y;
		temp->m_fSizeZ = m_pModel->m_fSizeY * m_scalling.y;

		D3DXVECTOR3 scalledMeshCenter;
		scalledMeshCenter.x = m_pModel->m_centerPos.x * m_scalling.x;
		scalledMeshCenter.y = m_pModel->m_centerPos.y * m_scalling.y;
		scalledMeshCenter.z = m_pModel->m_centerPos.z * m_scalling.z;

		temp->m_vCenterPos = m_position + scalledMeshCenter;
		m_vCenterInfo = temp->m_vCenterPos;
		temp->st_Type = Bounding_Object;

		node->m_vRow[-(m_vCenterInfo.z / NodeLength)].m_vCol[m_vCenterInfo.x / NodeLength].m_pBoundInfo = new nNodeBoundInfo;
		node->m_vRow[-(m_vCenterInfo.z / NodeLength)].m_vCol[m_vCenterInfo.x / NodeLength].m_pBoundInfo->m_vecBounding.push_back(temp);

	}

};

