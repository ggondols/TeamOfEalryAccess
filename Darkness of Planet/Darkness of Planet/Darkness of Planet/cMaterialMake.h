#pragma once
#include "cSingletonBase.h"
/////////////////////////////////////////////////////
////////////2017.09.03 ��Ƽ���� ������ִ� Ŭ����///////
//////////// ���¿�///////////////////////////////////
/////// ��Ƽ������ ���� ������ �ٲ��ش�////////////////
/////////////////////////////////////////////////////
class cMaterialMake:public cSingletonBase<cMaterialMake>
{
public:
	void Setup();
	void Release();
	cMaterialMake();
	~cMaterialMake();
	void SetRed(D3DMATERIAL9& mat);
	void SetGreen(D3DMATERIAL9& mat);
	void SetBlue(D3DMATERIAL9& mat);
	void SetWhite(D3DMATERIAL9& mat);
	void SetBlack(D3DMATERIAL9& mat);
	void SetGray(D3DMATERIAL9& mat);
	void SetGrayWhite(D3DMATERIAL9 & mat);

};

