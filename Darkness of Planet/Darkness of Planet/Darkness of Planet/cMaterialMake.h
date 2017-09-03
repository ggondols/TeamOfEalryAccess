#pragma once
#include "cSingletonBase.h"
/////////////////////////////////////////////////////
////////////2017.09.03 머티리얼 만들어주는 클래스///////
//////////// 김태영///////////////////////////////////
/////// 머티리얼을 집어 넣으면 바꿔준다////////////////
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

