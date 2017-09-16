#pragma once
class TeicCollisionMove;
class TeicSkinnedMesh;



//###############################################
//#												#
//#		���� move ����־ ������ �̵� ����� #
//#		sequence Ŭ���� �Դϴ�.			        #
//#		2017-09-15								#	
//#     AddAction���� ���� �ְ�			        #
//#     start�� �����մϴ�						#
//#		����!!								    #
//#     SetSkinnedTarget, SetSpeed,setFrom      #
//#    SetTo �̸� �������ְ� Addaction�ؾ��մϴ�#
//#												#
//#					Made by �¿�				#
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

