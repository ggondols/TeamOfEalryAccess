#pragma once

// ������ : 2017/09/03
// �ۼ��� : ������
// ���� ������ : 2017/09/03
// ���� ������ : ������

// ��� : Ÿ���� �Ǵ� ������Ʈ�� �־��� �ð����� ���۽�Ű�� �׼��� Ʋ

class cGameObject;
class cAction;

class iActionDelegate //�ݹ����� �ϴ� �������̽�
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

