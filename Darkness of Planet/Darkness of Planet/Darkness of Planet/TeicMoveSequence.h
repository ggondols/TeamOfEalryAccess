#pragma once
class TeicCollisionMove;
class TeicSkinnedMesh;



//###############################################
//#												#
//#		각종 move 집어넣어서 여러개 이동 만드는 #
//#		sequence 클래스 입니다.			        #
//#		2017-09-15								#	
//#     AddAction으로 집어 넣고			        #
//#     start로 시작합니다						#
//#		주의!!								    #
//#     SetSkinnedTarget, SetSpeed,setFrom      #
//#    SetTo 미리 설정해주고 Addaction해야합니다#
//#												#
//#					Made by 태영				#
//###############################################


class TeicMoveSequence
{
private:
	
	int					m_nCurrActionIndex;

	D3DXVECTOR3			m_To;
	float				m_fTime;
	float				m_Speed;
	CallbackBindFunction	m_Callback;



public:
	bool				m_bStart;
	vector<TeicCollisionMove*>	m_vecAction;
	void AddAction(TeicCollisionMove* pAction);
	void Start();
	void Update();
	void SetCallback(CallbackBindFunction func);
	void OnFinish();


	
	void SetClear(); 
	void MoveOnceCallback();
public:
	TeicMoveSequence();
	~TeicMoveSequence();
};

