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
	, m_iBodyUpgrade(1)
	, m_pHeroHead(NULL)
	, m_pWeapon_AA12(NULL)
	, m_pWeapon_AR6(NULL)
	, m_pWeapon_M4(NULL)
	, m_pWeapon_MP5(NULL)
	, m_pWeapon_Pistol(NULL)
	, e_WPtype(WP_NONE)
	, m_iHeadUpgrade(2)
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;
	m_pCtrl = NULL;
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

	SAFE_DELETE(m_pHeroHead);
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
	sprintf_s(MP5buffFile, "%s%d%s", Filename,m_iBodyUpgrade, MP5File);

	m_pHeroBody_MP5 = new LDYSkinnedMesh(MP5buffFolder, MP5buffFile);
	m_pHeroBody_MP5->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_MP5->SetRandomTrackPosition();
	m_pHeroBody_MP5->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));


	char PistolbuffFolder[1024];
	char* PistolFolder = "Pistol_Aim_Anims/";
	sprintf_s(PistolbuffFolder, "%s%s", Foldername, PistolFolder);

	char PistolbuffFile[1024];
	char* PistolFile = "_Pistol.X";
	sprintf_s(PistolbuffFile, "%s%d%s", Filename, m_iBodyUpgrade, PistolFile);

	m_pHeroBody_Pistol = new LDYSkinnedMesh(PistolbuffFolder, PistolbuffFile);
	m_pHeroBody_Pistol->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_Pistol->SetRandomTrackPosition();
	m_pHeroBody_Pistol->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));


	char Melee5buff[1024];
	char* MeleeFolder = "Player_Melee/";
	sprintf_s(Melee5buff, "%s%s", Foldername, MeleeFolder);

	char Melee5buffFile[1024];
	char* MeleeFile = "_Player_Melee.X";
	sprintf_s(Melee5buffFile, "%s%d%s", Filename, m_iBodyUpgrade, MeleeFile);

	m_pHeroBody_Melee = new LDYSkinnedMesh(Melee5buff, Melee5buffFile);
	m_pHeroBody_Melee->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_Melee->SetRandomTrackPosition();
	m_pHeroBody_Melee->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));

	char Basebuff[1024];
	char* BaseFolder = "PR_Base_Anims/";
	sprintf_s(Basebuff, "%s%s", Foldername, BaseFolder);

	char BasebuffFile[1024];
	char* BaseFile = "_Base.X";
	sprintf_s(BasebuffFile, "%s%d%s", Filename, m_iBodyUpgrade, BaseFile);

	m_pHeroBody_Base = new LDYSkinnedMesh(Basebuff, BasebuffFile);
	m_pHeroBody_Base->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_Base->SetRandomTrackPosition();
	m_pHeroBody_Base->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));


	char Idlebuff[1024];
	char* IdleBreakFolder = "PR_IdleBreak_Anims/";
	sprintf_s(Idlebuff, "%s%s", Foldername, IdleBreakFolder);

	char IdlebuffFile[1024];
	char* IdleFile = "_IdleBreak.X";
	sprintf_s(IdlebuffFile, "%s%d%s", Filename, m_iBodyUpgrade, IdleFile);

	m_pHeroBody_IdleBreak = new LDYSkinnedMesh(Idlebuff, IdlebuffFile);
	m_pHeroBody_IdleBreak->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pHeroBody_IdleBreak->SetRandomTrackPosition();
	m_pHeroBody_IdleBreak->SetCallbackfunction(bind(&LDYCharacter::CallbackOn, this, 0));

	char SMGbuff[1024];
	char* SMGFolder = "SMG_Aim_Anims/";
	sprintf_s(SMGbuff, "%s%s", Foldername, SMGFolder);

	char SMGbuffFile[1024];
	char* SMGFile = "_SMG.X";
	sprintf_s(SMGbuffFile, "%s%d%s", Filename, m_iBodyUpgrade, SMGFile);

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
	sprintf_s(HeadbuffFile, "%s%d%s", HeadFile, m_iHeadUpgrade, XfileExtension);

	m_pHeroHead = new LDYSkinnedMesh_Head(HeadbuffFolder, HeadbuffFile);
	m_pHeroHead->m_matHead = m_pHeroBody_MP5->m_matHead;
	
	//무기 메쉬 세팅 
	char WP_MP5Folder[1024];
	char* MP5wpFolder = "Weapon/";
	sprintf_s(WP_MP5Folder, "%s%s", Foldername, MP5wpFolder);

	char* MP5Filename = "Wp_MP5.X";

	m_pWeapon_MP5 = new LDYSkinnedMesh_Weapon(WP_MP5Folder, MP5Filename);
	m_pWeapon_MP5->m_matWeapon = m_pHeroBody_MP5->m_matWeapon;

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
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->UpdateAndRender();

	if (m_pHeroHead) 
	{ 
		D3DXMATRIX matT;
		D3DXMatrixTranslation(&matT, -0.0f, -4.5f, 0.0f);
		matT *= m_pHeroBody_MP5->m_matHead;
		m_pHeroHead->m_matHead = matT;
		m_pHeroHead->UpdateAndRender();
	}
	if (m_pWeapon_MP5)
	{
		D3DXMATRIX matT;
		D3DXMatrixTranslation(&matT, -32.10f, 0.0f, 0.0f);
		matT *= m_pHeroBody_MP5->m_matWeapon;
		m_pWeapon_MP5->m_matWeapon = matT;
		m_pWeapon_MP5->UpdateAndRender();
	}
}

void LDYCharacter::SetAnimationIndex(int nIndex)
{
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->SetAnimationIndex(nIndex);
}

void LDYCharacter::SetMove(D3DXMATRIX & move)
{
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->SetMove(move);
}

void LDYCharacter::SetRotationAngle(float angle)
{
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->SetRotationAngle(angle);
}

void LDYCharacter::SetRotationMatrix(D3DXMATRIX rotation)
{
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->SetRotationMatrix(rotation);
}

D3DXVECTOR3 * LDYCharacter::GetPositionPointer()
{
	if (m_pHeroBody_MP5)
	{
		return m_pHeroBody_MP5->GetPositionPointer();
	}
	return NULL;
}

D3DXVECTOR3 LDYCharacter::GetPosition()
{
	if (m_pHeroBody_MP5)
	{
		return m_pHeroBody_MP5->GetPosition();
	}
	return D3DXVECTOR3();
}

void LDYCharacter::SetPosition(D3DXVECTOR3 position)
{
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->SetPosition(position);
}

void LDYCharacter::SetAnimation(int num)
{
	if (m_pHeroBody_MP5)m_pHeroBody_MP5->SetAnimation(num);
}

int LDYCharacter::GetAninum()
{
	if (m_pHeroBody_MP5)
	{
		return m_pHeroBody_MP5->GetAninum();
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

			if (GetAninum() != 1)
			{
				SetAnimation(1);
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
	if (m_pHeroBody_MP5)
	{
		D3DXVECTOR3 pos = m_pHeroBody_MP5->GetPosition();
		pos.y = 0;
		temp.x = pos.x;
		temp.y = -(pos.z);
		return temp;
	}

	temp.x = 0;
	temp.y = 0;
	return temp;
}

