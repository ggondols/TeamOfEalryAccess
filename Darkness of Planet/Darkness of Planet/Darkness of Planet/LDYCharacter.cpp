#include "stdafx.h"
#include "LDYCharacter.h"
#include "TeicPhysicsCrtCtrl.h"


LDYCharacter::LDYCharacter()
	: m_pWeapon_AA12(NULL)
	, m_pWeapon_AR6(NULL)
	, m_pWeapon_M4(NULL)
	, m_pWeapon_MP5(NULL)
	, m_iHeadLv(0)
	, m_iBodyLv(0)
	, m_eWpType(Wp_AA12)
	, m_eStType(ST_Idle)
	, m_bRun(false)

{
	m_Callback = NULL;
	m_pCtrl = NULL;

	for (int i = 0; i < 2; ++i)
	{
		m_pHeroHead[i] = NULL;
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pHeroBody_MP5[i] = NULL;
		m_pHeroBody_Melee[i] = NULL;
		m_pHeroBody_Base[i] = NULL;
		m_pHeroBody_IdleBreak[i] = NULL;
		m_pHeroBody_SMG[i] = NULL;
	}
	m_pPreviousPoint = PointMake(0, 0);
}

LDYCharacter::~LDYCharacter()
{
	SAFE_DELETE(m_pCtrl);

	for (int i = 0; i < 4; ++i)
	{
		SAFE_DELETE(m_pHeroBody_MP5[i]);
		SAFE_DELETE(m_pHeroBody_Melee[i]);
		SAFE_DELETE(m_pHeroBody_Base[i]);
		SAFE_DELETE(m_pHeroBody_IdleBreak[i]);
		SAFE_DELETE(m_pHeroBody_SMG[i]);
	}

	for (int i = 0; i < 2; ++i)
	{
		SAFE_DELETE(m_pHeroHead[i]);
	}

	SAFE_DELETE(m_pWeapon_AA12);
	SAFE_DELETE(m_pWeapon_AR6);
	SAFE_DELETE(m_pWeapon_M4);
	SAFE_DELETE(m_pWeapon_MP5);
}

void LDYCharacter::Setup(char* Foldername, char* Filename)
{
	//ctrl생성
	m_pCtrl = new TeicPhysicsCrtCtrl;


	//body 상황별 메쉬 셋업 

	//MP5Body
	char MP5buffFolder[1024];
	char* MP5Folder = "MP5_Aim_Anims/";
	sprintf_s(MP5buffFolder, "%s%s", Foldername, MP5Folder);

	char MP5buffFile[1024];
	char* MP5File = "_MP5.X";

	for (int i = 0; i < 4; ++i)
	{
		sprintf_s(MP5buffFile, "%s%s%d%s", MP5Folder, Filename, i + 1, MP5File);
		m_pHeroBody_MP5[i] = MESHLOADER->GetSkinnedMesh(MP5buffFile);
		m_pHeroBody_MP5[i]->SetPosition(D3DXVECTOR3(0, 0, 0));
		m_pHeroBody_MP5[i]->SetRandomTrackPosition();
		m_pHeroBody_MP5[i]->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));
	}


	//MeleeBody
	char Melee5buff[1024];
	char* MeleeFolder = "Player_Melee/";
	sprintf_s(Melee5buff, "%s%s", Foldername, MeleeFolder);

	char Melee5buffFile[1024];
	char* MeleeFile = "_Player_Melee.X";

	for (int i = 0; i < 4; ++i)
	{
		sprintf_s(Melee5buffFile, "%s%s%d%s", MeleeFolder, Filename, i + 1, MeleeFile);

		m_pHeroBody_Melee[i] = MESHLOADER->GetSkinnedMesh(Melee5buffFile);
		m_pHeroBody_Melee[i]->SetPosition(D3DXVECTOR3(0, 0, 0));
		m_pHeroBody_Melee[i]->SetRandomTrackPosition();
		m_pHeroBody_Melee[i]->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));
	}

	//BaseBody
	char Basebuff[1024];
	char* BaseFolder = "PR_Base_Anims/";
	sprintf_s(Basebuff, "%s%s", Foldername, BaseFolder);

	char BasebuffFile[1024];
	char* BaseFile = "_Base.X";

	for (int i = 0; i < 4; ++i)
	{
		sprintf_s(BasebuffFile, "%s%s%d%s", BaseFolder, Filename, i + 1, BaseFile);

		m_pHeroBody_Base[i] = MESHLOADER->GetSkinnedMesh(BasebuffFile);
		m_pHeroBody_Base[i]->SetPosition(D3DXVECTOR3(0, 0, 0));
		m_pHeroBody_Base[i]->SetRandomTrackPosition();
		m_pHeroBody_Base[i]->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));
	}

	//IdleBody
	char Idlebuff[1024];
	char* IdleBreakFolder = "PR_IdleBreak_Anims/";
	sprintf_s(Idlebuff, "%s%s", Foldername, IdleBreakFolder);

	char IdlebuffFile[1024];
	char* IdleFile = "_IdleBreak.X";

	for (int i = 0; i < 4; ++i)
	{
		sprintf_s(IdlebuffFile, "%s%s%d%s", IdleBreakFolder, Filename, i + 1, IdleFile);

		m_pHeroBody_IdleBreak[i] = MESHLOADER->GetSkinnedMesh(IdlebuffFile);
		m_pHeroBody_IdleBreak[i]->SetPosition(D3DXVECTOR3(0, 0, 0));
		m_pHeroBody_IdleBreak[i]->SetRandomTrackPosition();
		m_pHeroBody_IdleBreak[i]->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));
	}

	//SMG Body
	char SMGbuff[1024];
	char* SMGFolder = "SMG_Aim_Anims/";
	sprintf_s(SMGbuff, "%s%s", Foldername, SMGFolder);

	char SMGbuffFile[1024];
	char* SMGFile = "_SMG.X";

	for (int i = 0; i < 4; ++i)
	{
		sprintf_s(SMGbuffFile, "%s%s%d%s", SMGFolder, Filename, i + 1, SMGFile);

		m_pHeroBody_SMG[i] = MESHLOADER->GetSkinnedMesh(SMGbuffFile);
		m_pHeroBody_SMG[i]->SetPosition(D3DXVECTOR3(0, 0, 0));
		m_pHeroBody_SMG[i]->SetRandomTrackPosition();
		m_pHeroBody_SMG[i]->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));
	}



	char* XfileExtension = ".X";


	//head 레벨별 셋업
	char HeadbuffFolder[1024];
	char* HeadFolder = "HeroHead/";
	sprintf_s(HeadbuffFolder, "%s%s", Foldername, HeadFolder);

	char HeadbuffFile[1024];
	char* HeadFile = "HeroHeadLv";

	for (int i = 0; i < 2; ++i) {
		sprintf_s(HeadbuffFile, "%s%s%d%s", HeadFolder,HeadFile, i + 1, XfileExtension);
		m_pHeroHead[i] = MESHLOADER->GetSkinnedMeshHead(HeadbuffFile); 

	}

	//무기 메쉬 세팅  mp5
	char WP_MP5Folder[1024];
	char* MP5wpFolder = "Weapon/";
	
	char* MP5Filename = "Wp_MP5.X";
	sprintf_s(WP_MP5Folder, "%s%s", MP5wpFolder, MP5Filename);

	m_pWeapon_MP5 = MESHLOADER->GetSkinnedMeshWeapon(WP_MP5Folder);

	//aa12
	char WP_AA12Folder[1024];
	char* AA12wpFolder = "Weapon/";
	char* AA12Filename = "Wp_AA12.X";
	sprintf_s(WP_AA12Folder, "%s%s", AA12wpFolder, AA12Filename);


	m_pWeapon_AA12 = MESHLOADER->GetSkinnedMeshWeapon(WP_AA12Folder);
	

	//ar6
	char WP_AR6Folder[1024];
	char* AR6wpFolder = "Weapon/";
	char* AR6Filename = "Wp_AR6.X";
	sprintf_s(WP_AR6Folder, "%s%s", AR6wpFolder, AR6Filename);

	
	m_pWeapon_AR6 = MESHLOADER->GetSkinnedMeshWeapon(WP_AR6Folder);
	

	//m4
	char WP_M4Folder[1024];
	char* M4wpFolder = "Weapon/";
	char* M4Filename = "Wp_M4.X";
	sprintf_s(WP_M4Folder, "%s%s", M4wpFolder, M4Filename);

	
	m_pWeapon_M4 = MESHLOADER->GetSkinnedMeshWeapon(WP_M4Folder);



}

void LDYCharacter::CallbackOn(int n)
{
	if (m_Callback)
	{
		m_Callback();
	}

}

void LDYCharacter::SetCallbackfunction(CallbackBindFunction function)
{
	m_Callback = std::move(function);
}

void LDYCharacter::SetAttackCallbackfunction(CallbackBindFunction function)
{
	m_AttackCallback = std::move(function);
}

void LDYCharacter::UpdateAndRender()
{
	D3DXMATRIX matRX, matRY, matRZ;
	D3DXMATRIX matHead, matWeapon;
	int degreeX, degreeY, degreeZ;

	if (m_eStType == ST_Attack)
	{
		degreeX = 4;
		degreeY = 0;
		degreeZ = 2;
	}
	else
	{
		degreeX = 33;
		degreeY = 37;
		degreeZ = 39;
	}

	D3DXMatrixRotationX(&matRX, D3DXToDegree(degreeX));
	D3DXMatrixRotationY(&matRY, D3DXToDegree(degreeY));
	D3DXMatrixRotationZ(&matRZ, D3DXToDegree(degreeZ));

	D3DXMatrixTranslation(&matHead, 0.0f, -4.5f, 0.0f);
	D3DXMatrixTranslation(&matWeapon, -0.0f, 0.0f, 0.0f);
	matWeapon = matRX*matRY*matRZ;


	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_Melee[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_MP5[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_MP5[m_iBodyLv]->m_matWeapon;
			m_pWeapon_AA12->m_matWeapon = matWeapon;
			m_pWeapon_AA12->UpdateAndRender();

		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_SMG[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();
			 
			matWeapon *= m_pHeroBody_SMG[m_iBodyLv]->m_matWeapon;
			m_pWeapon_AR6->m_matWeapon = matWeapon;
			m_pWeapon_AR6->UpdateAndRender();
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_SMG[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_SMG[m_iBodyLv]->m_matWeapon;
			m_pWeapon_M4->m_matWeapon = matWeapon;
			m_pWeapon_M4->UpdateAndRender();
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_MP5[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_MP5[m_iBodyLv]->m_matWeapon;
			m_pWeapon_MP5->m_matWeapon = matWeapon;
			m_pWeapon_MP5->UpdateAndRender();
		}
		break;

		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_Base[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_Base[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_Base[m_iBodyLv]->m_matWeapon;
			m_pWeapon_AA12->m_matWeapon = matWeapon;
			m_pWeapon_AA12->UpdateAndRender();
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_Base[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_Base[m_iBodyLv]->m_matWeapon;
			m_pWeapon_AR6->m_matWeapon = matWeapon;
			m_pWeapon_AR6->UpdateAndRender();
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_Base[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_Base[m_iBodyLv]->m_matWeapon;
			m_pWeapon_M4->m_matWeapon = matWeapon;
			m_pWeapon_M4->UpdateAndRender();
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_Base[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_Base[m_iBodyLv]->m_matWeapon;
			m_pWeapon_MP5->m_matWeapon = matWeapon;
			m_pWeapon_MP5->UpdateAndRender();
		}
		break;
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matWeapon;
			m_pWeapon_AA12->m_matWeapon = matWeapon;
			m_pWeapon_AA12->UpdateAndRender();
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matWeapon;
			m_pWeapon_AR6->m_matWeapon = matWeapon;
			m_pWeapon_AR6->UpdateAndRender();
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matWeapon;
			m_pWeapon_M4->m_matWeapon = matWeapon;
			m_pWeapon_M4->UpdateAndRender();
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->UpdateAndRender();

			matHead *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matHead;
			m_pHeroHead[m_iHeadLv]->m_matHead = matHead;
			m_pHeroHead[m_iHeadLv]->UpdateAndRender();

			matWeapon *= m_pHeroBody_IdleBreak[m_iBodyLv]->m_matWeapon;
			m_pWeapon_MP5->m_matWeapon = matWeapon;
			m_pWeapon_MP5->UpdateAndRender();
		}
		break;
		}
	}
	break;
	}

}

void LDYCharacter::SetAnimationIndex(int nIndex)
{
	//if (m_pHeroBody_Base)m_pHeroBody_Base->SetAnimationIndex(nIndex);
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->SetAnimationIndex(nIndex);

		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimationIndex(nIndex);
		}
		break;
		}
	}
	break;
	}
}

void LDYCharacter::SetMove(D3DXMATRIX & move)
{
	//if (m_pHeroBody_Base)m_pHeroBody_Base->SetMove(move);
	/*switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->SetMove(move);

		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_Pistol:
		{
			m_pHeroBody_Pistol[m_iBodyLv]->SetMove(move);
		}
		break;

		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_Pistol:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetMove(move);
		}
		break;
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetMove(move);
		}
		break;
		case Wp_Pistol:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetMove(move);
		}
		break;
		}
	}
	break;
	}*/
}

void LDYCharacter::SetRotationAngle(float angle)
{
	//if (m_pHeroBody_Base)m_pHeroBody_Base->SetRotationAngle(angle);
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->SetRotationAngle(angle);

		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;

		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationAngle(angle);
		}
		break;
		}
	}
	break;
	}
}

void LDYCharacter::SetRotationMatrix(D3DXMATRIX rotation)
{
	//if (m_pHeroBody_Base)m_pHeroBody_Base->SetRotationMatrix(rotation);
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->SetRotationMatrix(rotation);

		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
	
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetRotationMatrix(rotation);
		}
		break;
	
		}
	}
	break;
	}
}

D3DXVECTOR3 * LDYCharacter::GetPositionPointer()
{
	/*if (m_pHeroBody_Base)
	{
	return m_pHeroBody_Base->GetPositionPointer();
	}*/
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_Melee[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_MP5[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_SMG[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_SMG[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_MP5[m_iBodyLv]->GetPositionPointer();
		}
		break;
		
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPositionPointer();
		}
		break;
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPositionPointer();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPositionPointer();
		}
		break;
	
		}
	}
	break;
	}
	return NULL;
}

D3DXVECTOR3 LDYCharacter::GetPosition()
{
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_Melee[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_MP5[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_SMG[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_SMG[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_MP5[m_iBodyLv]->GetPosition();
		}
		break;
	
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetPosition();
		}
		break;
	
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
		}
		break;
	
		}
	}
	break;
	}
	return D3DXVECTOR3();
}

void LDYCharacter::SetPosition(D3DXVECTOR3 position)
{
	//if (m_pHeroBody_Base)m_pHeroBody_Base->SetPosition(position);
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->SetPosition(position);

		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetPosition(position);
		}
		break;
	
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetPosition(position);
		}
		break;
	
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetPosition(position);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetPosition(position);
		}
		break;
	
		}
	}
	break;
	}
}

void LDYCharacter::SetAnimation(int num)
{
	//if (m_pHeroBody_Base)m_pHeroBody_Base->SetAnimation(num);
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Melee[m_iBodyLv]->SetAnimation(num);

		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_SMG[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_MP5[m_iBodyLv]->SetAnimation(num);
		}
		break;
	
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_Base[m_iBodyLv]->SetAnimation(num);
		}
		break;
	
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_AA12:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_AR6:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_M4:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(num);
		}
		break;
		case Wp_MP5:
		{
			m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(num);
		}
		break;
		
		}
	}
	break;
	}
}

int LDYCharacter::GetAninum()
{
	/*if (m_pHeroBody_Base)
	{
	return m_pHeroBody_Base->GetAninum();
	}*/
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_Melee[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_MP5[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_SMG[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_SMG[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_MP5[m_iBodyLv]->GetAninum();
		}
		break;
	
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_Base[m_iBodyLv]->GetAninum();
		}
		break;
		
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_AA12:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_AR6:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_M4:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum();
		}
		break;
		case Wp_MP5:
		{
			return m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum();
		}
		break;
		
		}
	}
	break;
	}

	return 0;
}

void LDYCharacter::Update(float angle)
{
	m_pCtrl->Update(angle);

	if (KEYMANAGER->isOnceKeyDown('H'))
	{
		m_iHeadLv++;

		if (m_iHeadLv > 1) {
			m_iHeadLv = 0;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		m_iBodyLv++;

		if (m_iBodyLv > 3) {
			m_iBodyLv = 0;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('N'))
	{
		switch (m_eWpType)
		{
		
		case Wp_AA12:
		{
			m_eWpType = Wp_AR6;
		}
		break;
		case Wp_AR6:
		{
			m_eWpType = Wp_M4;
		}
		break;
		case Wp_M4:
		{
			m_eWpType = Wp_MP5;
		}
		break;
		case Wp_MP5:
		{
			m_eWpType = Wp_AA12;
		}
		break;
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SHIFT))
	{
		m_bRun = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_SHIFT))
	{
		m_bRun = false;
	}

	if (m_bRun)
	{
		m_pCtrl->SetSpeed(1.5f);
	}
	else {
		m_pCtrl->SetSpeed(0.5f);
	}

	if (m_pCtrl->getMoving()) {
		m_eStType = ST_Move;
	}
	else if (m_pCtrl->getAttacking()) {
		m_eStType = ST_Attack;
	}
	else
	{
		m_eStType = ST_Idle;
	}
	if (m_pCtrl != NULL)
	{
		SetPosition(*m_pCtrl->GetPosition());
		SetRotationAngle(m_pCtrl->getAngle());

		switch (m_eStType)
		{
		case ST_Attack:
		{
			switch (m_eWpType)
			{
			//공격 상태이고 무기가없을때
			case Wp_Melee:
			{
				if (m_pHeroBody_Melee[m_iBodyLv]->GetAninum() != 3) 
				{
					m_pHeroBody_Melee[m_iBodyLv]->SetAnimation(3);
				}
			}
			break;
			//공격상태 무기AA12
			case Wp_AA12:
			{
				if (m_pHeroBody_MP5[m_iBodyLv]->GetAninum() != 9)
				{
					m_pHeroBody_MP5[m_iBodyLv]->SetAnimation(9);
				}
			}
			break;
			//공격상태 무기AR6
			case Wp_AR6:
			{
				if (m_pHeroBody_SMG[m_iBodyLv]->GetAninum() != 9)
				{
					m_pHeroBody_SMG[m_iBodyLv]->SetAnimation(9);
				}
			}
			break;
			//공격상태 무기 M4
			case Wp_M4:
			{
				if (m_pHeroBody_SMG[m_iBodyLv]->GetAninum() != 9)
				{
					m_pHeroBody_SMG[m_iBodyLv]->SetAnimation(9);
				}
			}
			break;
			//공격상태 무기MP5
			case Wp_MP5:
			{
				if (m_pHeroBody_MP5[m_iBodyLv]->GetAninum() != 9)
				{
					m_pHeroBody_MP5[m_iBodyLv]->SetAnimation(9);
				}
			}
			break;
			}
		}
		break;
		case ST_Move:
		{
			switch (m_eWpType)
			{
			//움직이는중 맨손
			case Wp_Melee:
			{
				if (!m_bRun)
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 6)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(6);
					}
				}
				else
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 32)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(32);
					}
				}
			}
			break;
			//움직이는중 AA12
			case Wp_AA12:
			{
				if (!m_bRun)
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 6)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(6);
					}
				}
				else
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 32)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(32);
					}
				}
			}
			break;
			//움직이는중 AR6
			case Wp_AR6:
			{
				if (!m_bRun)
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 6)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(6);
					}
				}
				else
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 32)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(32);
					}
				}
			}
			break;
			//움직이는중 M4
			case Wp_M4:
			{
				if (!m_bRun)
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 6)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(6);
					}
				}
				else
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 32)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(32);
					}
				}
			}
			break;
			//움직이는중 MP5
			case Wp_MP5:
			{
				if (!m_bRun)
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 6)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(6);
					}
				}
				else
				{
					if (m_pHeroBody_Base[m_iBodyLv]->GetAninum() != 32)
					{
						m_pHeroBody_Base[m_iBodyLv]->SetAnimation(32);
					}
				}
			}
			break;

			}
		}
		break;
		case ST_Idle:
		{
			switch (m_eWpType)
			{
			//대기상태 맨손
			case Wp_Melee:
			{
			if (m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum() != 9)
				{
					m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(9);
				}
			}
			break;
			//대기상태 AA12
			case Wp_AA12:
			{
				if (m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum() != 6)
				{
					m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(6);
				}
			}
			break;
			//대기상태 AR6
			case Wp_AR6:
			{
				if (m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum() != 5)
				{
					m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(5);
				}
			}
			break;
			//대기상태 M4
			case Wp_M4:
			{
				if (m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum() != 0)
				{
					m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(0);
				}
			}
			break;
			//대기상태 MP5
			case Wp_MP5:
			{
				if (m_pHeroBody_IdleBreak[m_iBodyLv]->GetAninum() != 9)
				{
					m_pHeroBody_IdleBreak[m_iBodyLv]->SetAnimation(9);
				}
			}
			break;

			}
		}
		break;
		}

	}
}

void LDYCharacter::SetMoving(bool on)
{
	m_pCtrl->setMoving(on);
}

bool LDYCharacter::GetMoving()
{
	return m_pCtrl->getMoving();
}

void LDYCharacter::SetAttacking(bool on)
{
	m_pCtrl->setAttacking(on);
}

bool LDYCharacter::GetAttacking()
{
	return m_pCtrl->getAttacking();
}

POINT LDYCharacter::GetNodeNum()
{
	POINT temp;
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			D3DXVECTOR3 pos = m_pHeroBody_Melee[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;

			
	
		}
		break;
		case Wp_AA12:
		{
			D3DXVECTOR3 pos = m_pHeroBody_MP5[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_AR6:
		{
			D3DXVECTOR3 pos = m_pHeroBody_SMG[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_M4:
		{
			D3DXVECTOR3 pos = m_pHeroBody_SMG[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_MP5:
		{
			D3DXVECTOR3 pos = m_pHeroBody_MP5[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			D3DXVECTOR3 pos = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_AA12:
		{
			D3DXVECTOR3 pos = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_AR6:
		{
			D3DXVECTOR3 pos = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_M4:
		{
			D3DXVECTOR3 pos = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_MP5:
		{
			D3DXVECTOR3 pos = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			D3DXVECTOR3 pos = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_AA12:
		{
			D3DXVECTOR3 pos = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_AR6:
		{
			D3DXVECTOR3 pos = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_M4:
		{
			D3DXVECTOR3 pos = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		case Wp_MP5:
		{
			D3DXVECTOR3 pos = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			pos.y = 0;
			temp.x = pos.x / NodeLength;
			temp.y = -(pos.z / NodeLength);
			return temp;
		}
		break;
		}
	}
	break;
	}
	
	temp.x = 0;
	temp.y = 0;
	return temp;
}

D3DXVECTOR3 LDYCharacter::GetPositionYZero()
{
	switch (m_eStType)
	{
	case ST_Attack:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{

			D3DXVECTOR3 temp = m_pHeroBody_Melee[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_AA12:
		{
			D3DXVECTOR3 temp = m_pHeroBody_MP5[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_AR6:
		{
			D3DXVECTOR3 temp = m_pHeroBody_SMG[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_M4:
		{
			D3DXVECTOR3 temp = m_pHeroBody_SMG[m_iBodyLv]->GetPosition();
		}
		break;
		case Wp_MP5:
		{
			D3DXVECTOR3 temp = m_pHeroBody_MP5[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;

		}
	}
	break;
	case ST_Move:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			D3DXVECTOR3 temp = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_AA12:
		{
			D3DXVECTOR3 temp = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_AR6:
		{
			D3DXVECTOR3 temp = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_M4:
		{
			D3DXVECTOR3 temp = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_MP5:
		{
			D3DXVECTOR3 temp = m_pHeroBody_Base[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;

		}
	}
	break;
	case ST_Idle:
	{
		switch (m_eWpType)
		{
		case Wp_Melee:
		{
			D3DXVECTOR3 temp = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_AA12:
		{
			D3DXVECTOR3 temp = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_AR6:
		{
			D3DXVECTOR3 temp = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_M4:
		{
			D3DXVECTOR3 temp = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;
		case Wp_MP5:
		{
			D3DXVECTOR3 temp = m_pHeroBody_IdleBreak[m_iBodyLv]->GetPosition();
			temp.y = 0;
			return temp;
		}
		break;

		}
	}
	break;
	}
	return D3DXVECTOR3();
}