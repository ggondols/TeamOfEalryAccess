#include "stdafx.h"
#include "LDYCharacter.h"
#include "TeicPhysicsCrtCtrl.h"


LDYCharacter::LDYCharacter()
	:m_pHeroBody_MP5(NULL)
	, m_pHeroBody_Pistol(NULL)
	, m_pHeroBody_Melee(NULL)
	, m_pHeroBody_Base(NULL)
	, m_pHeroBody_IdleBreak(NULL)
	, m_pHeroBody_SMG(NULL)
	, m_pWeapon_AA12(NULL)
	, m_pWeapon_AR6(NULL)
	, m_pWeapon_M4(NULL)
	, m_pWeapon_MP5(NULL)
	, m_pWeapon_Pistol(NULL)
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;
	m_pCtrl = NULL;

	for (int i = 0; i < 2; ++i) {
		m_pHeroHead[i] = NULL;
	}

}

LDYCharacter::~LDYCharacter()
{
	SAFE_DELETE(m_pCtrl);
	SAFE_DELETE(m_pSkinnedMesh);

	SAFE_DELETE(m_pHeroBody_MP5);
	SAFE_DELETE(m_pHeroBody_Pistol);
	SAFE_DELETE(m_pHeroBody_Melee);
	SAFE_DELETE(m_pHeroBody_Base);
	SAFE_DELETE(m_pHeroBody_IdleBreak);
	SAFE_DELETE(m_pHeroBody_SMG);

	for (int i = 0; i < 2; ++i) {
		SAFE_DELETE(m_pHeroHead[i]);
	}

	SAFE_DELETE(m_pWeapon_AA12);
	SAFE_DELETE(m_pWeapon_AR6);
	SAFE_DELETE(m_pWeapon_M4);
	SAFE_DELETE(m_pWeapon_MP5);
	SAFE_DELETE(m_pWeapon_Pistol);
}

void LDYCharacter::Setup(char* Foldername, char* Filename)
{
	//ctrl생성
	m_pCtrl = new TeicPhysicsCrtCtrl;

	//body 상황별 메쉬 셋업 
	char MP5buffFolder[1024];
	char* MP5Folder = "MP5_Aim_Anims/";
	sprintf_s(MP5buffFolder, "%s%s", Foldername, MP5Folder);

	char MP5buffFile[1024];
	char* MP5File = "_MP5.X";
	sprintf_s(MP5buffFile, "%s%d%s", Filename,1, MP5File);

	m_pHeroBody_MP5 = new LDYSkinnedMesh(MP5buffFolder, MP5buffFile);
	m_pHeroBody_MP5->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_MP5->SetRandomTrackPosition();
	m_pHeroBody_MP5->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));


	char PistolbuffFolder[1024];
	char* PistolFolder = "Pistol_Aim_Anims/";
	sprintf_s(PistolbuffFolder, "%s%s", Foldername, PistolFolder);

	char PistolbuffFile[1024];
	char* PistolFile = "_Pistol.X";
	sprintf_s(PistolbuffFile, "%s%d%s", Filename, 1, PistolFile);

	m_pHeroBody_Pistol = new LDYSkinnedMesh(PistolbuffFolder, PistolbuffFile);
	m_pHeroBody_Pistol->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_Pistol->SetRandomTrackPosition();
	m_pHeroBody_Pistol->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));


	char Melee5buff[1024];
	char* MeleeFolder = "Player_Melee/";
	sprintf_s(Melee5buff, "%s%s", Foldername, MeleeFolder);

	char Melee5buffFile[1024];
	char* MeleeFile = "_Player_Melee.X";
	sprintf_s(Melee5buffFile, "%s%d%s", Filename, 1, MeleeFile);

	m_pHeroBody_Melee = new LDYSkinnedMesh(Melee5buff, Melee5buffFile);
	m_pHeroBody_Melee->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_Melee->SetRandomTrackPosition();
	m_pHeroBody_Melee->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));

	char Basebuff[1024];
	char* BaseFolder = "PR_Base_Anims/";
	sprintf_s(Basebuff, "%s%s", Foldername, BaseFolder);

	char BasebuffFile[1024];
	char* BaseFile = "_Base.X";
	sprintf_s(BasebuffFile, "%s%d%s", Filename, 1, BaseFile);

	m_pHeroBody_Base = new LDYSkinnedMesh(Basebuff, BasebuffFile);
	m_pHeroBody_Base->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_Base->SetRandomTrackPosition();
	m_pHeroBody_Base->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));


	char Idlebuff[1024];
	char* IdleBreakFolder = "PR_IdleBreak_Anims/";
	sprintf_s(Idlebuff, "%s%s", Foldername, IdleBreakFolder);

	char IdlebuffFile[1024];
	char* IdleFile = "_IdleBreak.X";
	sprintf_s(IdlebuffFile, "%s%d%s", Filename, 1, IdleFile);

	m_pHeroBody_IdleBreak = new LDYSkinnedMesh(Idlebuff, IdlebuffFile);
	m_pHeroBody_IdleBreak->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_IdleBreak->SetRandomTrackPosition();
	m_pHeroBody_IdleBreak->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));

	char SMGbuff[1024];
	char* SMGFolder = "SMG_Aim_Anims/";
	sprintf_s(SMGbuff, "%s%s", Foldername, SMGFolder);

	char SMGbuffFile[1024];
	char* SMGFile = "_SMG.X";
	sprintf_s(SMGbuffFile, "%s%d%s", Filename, 1, SMGFile);

	m_pHeroBody_SMG = new LDYSkinnedMesh(SMGbuff, SMGbuffFile);
	m_pHeroBody_SMG->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_SMG->SetRandomTrackPosition();
	m_pHeroBody_SMG->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));



	char* XfileExtension = ".X";


	//head 레벨별 셋업
	char HeadbuffFolder[1024];
	char* HeadFolder = "HeroHead/";
	sprintf_s(HeadbuffFolder, "%s%s", Foldername, HeadFolder);

	char HeadbuffFile[1024];
	char* HeadFile = "HeroHeadLv";

	for (int i = 0; i < 2; ++i) {
		sprintf_s(HeadbuffFile, "%s%d%s", HeadFile, 1, XfileExtension);
		m_pHeroHead[i] = new LDYSkinnedMesh_Head(HeadbuffFolder, HeadbuffFile);
		m_pHeroHead[i]->m_matHead = m_pHeroBody_Base->m_matHead;

	}
	
	//무기 메쉬 세팅  mp5
	char WP_MP5Folder[1024];
	char* MP5wpFolder = "Weapon/";
	sprintf_s(WP_MP5Folder, "%s%s", Foldername, MP5wpFolder);

	char* MP5Filename = "Wp_MP5.X";

	m_pWeapon_MP5 = new LDYSkinnedMesh_Weapon(WP_MP5Folder, MP5Filename);
	m_pWeapon_MP5->m_matWeapon = m_pHeroBody_Base->m_matWeapon;

	//aa12
	char WP_AA12Folder[1024];
	char* AA12wpFolder = "Weapon/";
	sprintf_s(WP_AA12Folder, "%s%s", Foldername, AA12wpFolder);

	char* AA12Filename = "Wp_AA12.X";

	m_pWeapon_AA12 = new LDYSkinnedMesh_Weapon(WP_AA12Folder, AA12Filename);
	m_pWeapon_AA12->m_matWeapon = m_pHeroBody_Base->m_matWeapon;

	//ar6
	char WP_AR6Folder[1024];
	char* AR6wpFolder = "Weapon/";
	sprintf_s(WP_AR6Folder, "%s%s", Foldername, AR6wpFolder);

	char* AR6Filename = "Wp_AR6.X";
	
	m_pWeapon_AR6 = new LDYSkinnedMesh_Weapon(WP_AR6Folder, AR6Filename);
	m_pWeapon_AR6->m_matWeapon = m_pHeroBody_Base->m_matWeapon;

	//m4
	char WP_M4Folder[1024];
	char* M4wpFolder = "Weapon/";
	sprintf_s(WP_M4Folder, "%s%s", Foldername, M4wpFolder);

	char* M4Filename = "Wp_M4.X";

	m_pWeapon_M4 = new LDYSkinnedMesh_Weapon(WP_M4Folder, M4Filename);
	m_pWeapon_M4->m_matWeapon = m_pHeroBody_Base->m_matWeapon;

	//pistol
	char WP_PistolFolder[1024];
	char* PistolwpFolder = "Weapon/";
	sprintf_s(WP_PistolFolder, "%s%s", Foldername, PistolwpFolder);

	char* PistolFilename = "Wp_Pistol.X";

	m_pWeapon_Pistol = new LDYSkinnedMesh_Weapon(WP_PistolFolder, PistolFilename);
	m_pWeapon_Pistol->m_matWeapon = m_pHeroBody_Base->m_matWeapon;

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
	if (m_pHeroBody_Base)m_pHeroBody_Base->UpdateAndRender();

	if (m_pHeroHead) 
	{ 
		D3DXMATRIX matT;
		D3DXMatrixTranslation(&matT, -0.0f, -4.5f, 0.0f);
		matT *= m_pHeroBody_Base->m_matHead;
		m_pHeroHead[0]->m_matHead = matT;
		m_pHeroHead[0]->UpdateAndRender();
	}
	if (m_pWeapon_MP5)
	{
		D3DXMATRIX matT;
		D3DXMatrixTranslation(&matT, -32.10f, 0.0f, 0.0f);
		matT *= m_pHeroBody_Base->m_matWeapon;
		m_pWeapon_MP5->m_matWeapon = matT;
		m_pWeapon_MP5->UpdateAndRender();
	}
}

void LDYCharacter::SetAnimationIndex(int nIndex)
{
	if (m_pHeroBody_Base)m_pHeroBody_Base->SetAnimationIndex(nIndex);
}

void LDYCharacter::SetMove(D3DXMATRIX & move)
{
	if (m_pHeroBody_Base)m_pHeroBody_Base->SetMove(move);
}

void LDYCharacter::SetRotationAngle(float angle)
{
	if (m_pHeroBody_Base)m_pHeroBody_Base->SetRotationAngle(angle);
}

void LDYCharacter::SetRotationMatrix(D3DXMATRIX rotation)
{
	if (m_pHeroBody_Base)m_pHeroBody_Base->SetRotationMatrix(rotation);
}

D3DXVECTOR3 * LDYCharacter::GetPositionPointer()
{
	if (m_pHeroBody_Base)
	{
		return m_pHeroBody_Base->GetPositionPointer();
	}
	return NULL;
}

D3DXVECTOR3 LDYCharacter::GetPosition()
{
	if (m_pHeroBody_Base)
	{
		return m_pHeroBody_Base->GetPosition();
	}
	return D3DXVECTOR3();
}

void LDYCharacter::SetPosition(D3DXVECTOR3 position)
{
	if (m_pHeroBody_Base)m_pHeroBody_Base->SetPosition(position);
}

void LDYCharacter::SetAnimation(int num)
{
	if (m_pHeroBody_Base)m_pHeroBody_Base->SetAnimation(num);
}

int LDYCharacter::GetAninum()
{
	if (m_pHeroBody_Base)
	{
		return m_pHeroBody_Base->GetAninum();
	}

	return 0;
}

void LDYCharacter::Update()
{
	m_pCtrl->Update();
	if (m_pCtrl != NULL)
	{

		SetPosition(*m_pCtrl->GetPosition());
		SetRotationAngle(m_pCtrl->getAngle());


		if (m_pCtrl->getMoving())
		{

			if (GetAninum() != 36)
			{
				SetAnimation(36);
			}
		}
		else if (m_pCtrl->getAttacking())
		{
			if (GetAninum() != 3)
			{
				SetAnimation(3);
			}
		}
		else
		{
			if (GetAninum() != 0)
			{
				SetAnimation(0);
			}
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
	if (m_pHeroBody_Base)
	{
		D3DXVECTOR3 pos = m_pHeroBody_Base->GetPosition();
		pos.y = 0;
		temp.x = pos.x;
		temp.y = -(pos.z);
		return temp;
	}

	temp.x = 0;
	temp.y = 0;
	return temp;
}

