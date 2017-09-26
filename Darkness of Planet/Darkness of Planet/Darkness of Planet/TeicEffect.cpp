#include "stdafx.h"
#include "TeicEffect.h"




TeicEffect::TeicEffect()
{
}


TeicEffect::~TeicEffect()
{
	SAFE_RELEASE(m_pMesh);
}


void TeicEffect::Setup(char * Textfilename, float size, int Xnum, int Ynum)
{
	m_fSize = size;
	m_iSpriteXnum = Xnum;
	m_iSpriteYnum = Ynum;



}

void TeicEffect::Update()
{
}

void TeicEffect::Render()
{
}