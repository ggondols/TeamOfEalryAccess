#include "stdafx.h"
#include "TeicMoveSequence.h"
#include "TeicCollisionMove.h"
#include "TeicSkinnedMesh.h"
TeicMoveSequence::TeicMoveSequence()
{
	m_Callback = NULL;
	m_nCurrActionIndex = 0;

	m_To = D3DXVECTOR3(0, 0, 0);
	m_fTime = 0.0f;
	m_Speed == 0.0f;
	
	m_bStart = false;

}


TeicMoveSequence::~TeicMoveSequence()
{
	for (int i = 0; i < m_vecAction.size(); i++)
	{
		SAFE_RELEASE(m_vecAction[i]);
	}
}



void TeicMoveSequence::AddAction(TeicCollisionMove * pAction)
{
	if (pAction)
	{
		pAction->SetCallback(bind(&TeicMoveSequence::MoveOnceCallback, this));
		m_vecAction.push_back(pAction);
	}
}

void TeicMoveSequence::Start()
{
	m_nCurrActionIndex = 0;

	if (m_vecAction.empty()) return;

	m_vecAction[0]->Start();
	m_bStart = true;
}

void TeicMoveSequence::Update()
{
	if (!m_bStart)return;
	
	if (m_nCurrActionIndex >= m_vecAction.size()) return;
	m_vecAction[m_nCurrActionIndex]->Update();
	
}

void TeicMoveSequence::SetCallback(CallbackBindFunction func)
{
	m_Callback = std::move(func);
}

void TeicMoveSequence::OnFinish()
{
	m_nCurrActionIndex++;
	if (m_nCurrActionIndex < m_vecAction.size())
	{
		m_vecAction[m_nCurrActionIndex]->Start();
		
	}
	else
	{
		
		m_vecAction.clear();
		m_bStart = false;
		if (m_Callback)
		{
			m_Callback();
		}
	}
}



void TeicMoveSequence::SetClear()
{
	m_bStart = false;
	for (int i = 0; i < m_vecAction.size(); i++)
	{
		SAFE_RELEASE(m_vecAction[i]);
	}
	 m_vecAction.clear(); 
}

void TeicMoveSequence::MoveOnceCallback()
{
	OnFinish();

}