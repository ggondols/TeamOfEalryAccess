#pragma once

// 생성일 : 2017/09/03
// 작성자 : 여현구
// 최종 수정일 : 2017/09/03
// 최종 수정자 : 여현구

// 기능 : 타겟이 되는 오브젝트를 주어진 시간동안 동작시키는 액션의 틀

class cGameObject;
class cAction;

class iActionDelegate //콜백기능을 하는 인터페이스
{
public:
	virtual void OnFinish(cAction* pSender) = 0; 
};

class cAction : public cObject
{
protected:
	float m_fPassedActionTime;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);

public:
	cAction(void);
	virtual ~cAction(void);

	virtual void Start();
	virtual void Update();
};

