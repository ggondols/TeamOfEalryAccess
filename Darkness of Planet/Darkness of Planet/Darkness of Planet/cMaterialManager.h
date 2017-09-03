#pragma once
#include "cSingletonBase.h"
////////////////////////////////////////////////////////
///////////2017.09.03   ��Ƽ���� �Ŵ���  ////////////
////////////////���¿� ////////////////////////////
/////// ��Ƽ������ �����ϰ� �⺻ ��Ƽ���� �������� Ŭ����//////////
//////////////////////////////////////////////////////////

class cMaterialManager :
	public cSingletonBase<cMaterialManager>


{
private:
	D3DMATERIAL9	m_mRedmaterial;
	D3DMATERIAL9	m_mGreenmaterial;
	D3DMATERIAL9	m_mBluematerial;
	D3DMATERIAL9	m_mWhitematerial;
	D3DMATERIAL9	m_mBlackmaterial;
	D3DMATERIAL9	m_mGraymaterial;
	D3DMATERIAL9	m_mGrayWhitematerial;
public:
	void Setup();
	void Release();
	
	void SetRed(D3DMATERIAL9& mat);
	void SetGreen(D3DMATERIAL9& mat);
	void SetBlue(D3DMATERIAL9& mat);
	void SetWhite(D3DMATERIAL9& mat);
	void SetBlack(D3DMATERIAL9& mat);
	void SetGray(D3DMATERIAL9& mat);
	void SetGrayWhite(D3DMATERIAL9& mat);
	void SetCustomMaterial(OUT D3DMATERIAL9& mat, D3DCOLORVALUE Ambient, D3DCOLORVALUE Diffuse, D3DCOLORVALUE Specular, D3DCOLORVALUE Emissive,float power);
	D3DMATERIAL9 GetRed() { return m_mRedmaterial; }
	D3DMATERIAL9 GetGreen() { return m_mGreenmaterial; }
	D3DMATERIAL9 GetBlue() { return m_mBluematerial; }
	D3DMATERIAL9 GetWhite() { return m_mWhitematerial; }
	D3DMATERIAL9 GetBlack() { return m_mBlackmaterial; }
	D3DMATERIAL9 GetGray() { return m_mGraymaterial; }
	D3DMATERIAL9 GetGrayWhite() { return m_mGrayWhitematerial; }
	cMaterialManager();
	~cMaterialManager();
};

