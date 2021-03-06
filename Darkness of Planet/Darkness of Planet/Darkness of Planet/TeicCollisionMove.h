#pragma once


//###############################################
//#												#
//#		이동하는 도중 충돌이 일어나면 멈추는    #
//#		move 클래스 입니다.			            #
//#		2017-09-11								#	
//#     SetSkinnedTarget으로 초기화 해주고      #
//#     SetFrom,SetTo 한후 Start하면 움직입니다	#
//#		주의!! 프레임이 50이하로 떨어지면       #
//#     안움직입니다                            #
//#		SetCallbackfunction로 콜백 셋팅			#	
//#     다크니스 오브 플래닛 유닛은 스킨드메쉬를#
//#     사용하기 때문에 게임 오브젝트를 쓰지않고#
//#     스킨드 메쉬를 집어 넣어서 사용하였습니다#
//#												#
//#					Made by 태영				#
//###############################################

class TeicSkinnedMesh;

class TeicCollisionMove 
{

///액션에서 상속 받아온 부분
//protected:  
//	float m_fPassedActionTime;
//	SYNTHESIZE(float, m_fActionTime, ActionTime);
//	SYNTHESIZE(cGameObject*, m_pTarget, Target);
//	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);


private:
	D3DXVECTOR3 m_From;
	D3DXVECTOR3 m_To;
	SYNTHESIZE(float, m_Speed, Speed); //////////1초에 얼마나 가냐?
	SYNTHESIZE(TeicSkinnedMesh*, m_pSkinnedTarget, SkinnedTarget);
	
	D3DXVECTOR3 m_previous;
	D3DXVECTOR3 m_present;
//	D3DXMATRIX  m_RotationMat;

	bool		m_bStart;

	float		m_fTotalDistance;
	float		m_fNowDistance;
	float		m_fElapsedDistance;
	CallbackBindFunction	m_Callback;
	float		m_fRotationAngle;
public:
	void SetCallback(CallbackBindFunction func);
	void CalRotation();
	void Stop();
	void SetFrom(D3DXVECTOR3 from);
	void SetTo(D3DXVECTOR3 to);
	void SetFrom2(D3DXVECTOR3 from);
	void SetTo2(D3DXVECTOR3 to);
	void SetRoationAngle(float f) { m_fRotationAngle = f; }
	float GetRotationAngle() { return m_fRotationAngle; }
public:
	virtual void Start();
	virtual void Update();
public:
	TeicCollisionMove();
	virtual~TeicCollisionMove();
};

