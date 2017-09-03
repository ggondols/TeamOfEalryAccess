#include "stdafx.h"
#include "cTaeYeongMoveSequence.h"
#include "cTaeYoengMoveOnce.h"

cTaeYeongMoveSequence::cTaeYeongMoveSequence()
{
	m_Callback = NULL;
	m_nCurrActionIndex = 0;

	m_To = D3DXVECTOR3(0, 0, 0);
	m_fTime = 0.0f;
	m_Speed == 0.0f;
	m_present = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixIdentity(&m_Matrix);
	m_bSet = false;
}


cTaeYeongMoveSequence::~cTaeYeongMoveSequence()
{
}



void cTaeYeongMoveSequence::AddAction(cTaeYoengMoveOnce * pAction)
{
	if (pAction)
	{

		pAction->SetSpeed(m_Speed);
		pAction->SetCallback(bind(&cTaeYeongMoveSequence::MoveOnceCallback, this));
		m_vecAction.push_back(pAction);
	}
}

void cTaeYeongMoveSequence::Start()
{
	m_nCurrActionIndex = 0;

	if (m_vecAction.empty()) return;

	m_vecAction[0]->Start();
	m_bSet = true;
}

void cTaeYeongMoveSequence::Update()
{
	if (m_nCurrActionIndex >= m_vecAction.size()) return;
	m_vecAction[m_nCurrActionIndex]->Update();
	if (m_bSet)
	{
		m_present = m_vecAction[m_nCurrActionIndex]->GetPresent();
		m_Matrix = m_vecAction[m_nCurrActionIndex]->GetRotation();
	}
}

void cTaeYeongMoveSequence::SetCallback(CallbackBindFunction func)
{
	m_Callback = std::move(func);
}

void cTaeYeongMoveSequence::OnFinish()
{
	m_nCurrActionIndex++;
	if (m_nCurrActionIndex < m_vecAction.size())
	{
		m_vecAction[m_nCurrActionIndex]->Start();
		m_vecAction[m_nCurrActionIndex]->SetRotation(m_vecAction[m_nCurrActionIndex - 1]->GetRotation());

	}
	else
	{
		m_bSet = false;
		m_vecAction.clear();
		if (m_Callback)
		{
			m_Callback();
		}
	}
}

D3DXVECTOR3 cTaeYeongMoveSequence::GetPresent()
{
	return m_present;

}

D3DXMATRIX cTaeYeongMoveSequence::GetRoationMat()
{


	return m_Matrix;
}


void cTaeYeongMoveSequence::MoveOnceCallback()
{
	OnFinish();

}