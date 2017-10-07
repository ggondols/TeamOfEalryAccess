#include "stdafx.h"
#include "TeicEffectManager.h"
#include "TeicIceBreath.h"
#include "TeicIceExplosion.h"
#include "TeicIceblizzard.h"
#include "TeicCharacterBlood.h"
#include "TeicMonsterBlood.h"
#include "TeicMonsterChill.h"
void TeicEffectManager::Setup()
{
}

void TeicEffectManager::Destroy()
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//����Ʈ�� �־�� ���� �˻�
	vIter = _vTotalEffects.begin();
	for (vIter; vIter != _vTotalEffects.end(); ++vIter)
	{
		mIter = vIter->begin();
		for (; mIter != vIter->end();)
		{
			if (mIter->second.size() != 0)
			{
				iterEffects vArrIter = mIter->second.begin();
				for (vArrIter; vArrIter != mIter->second.end();)
				{
					(*vArrIter)->Release();
					SAFE_DELETE(*vArrIter);
					vArrIter = mIter->second.erase(vArrIter);
				}
				mIter->second.clear();
			}
			else ++mIter;
		}
	}

	_vTotalEffects.clear();
}

void TeicEffectManager::Render()
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//����Ʈ�� �־�� ���� �˻�
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//��Ż����Ʈ ���� �ȿ� �ִ� ���� ����Ʈ Ű�� �˻��Ѵ�
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				(*vArrIter)->Render();
			}
		}
	}
}

void TeicEffectManager::Update()
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//����Ʈ�� �־�� ���� �˻�
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//��Ż����Ʈ ���� �ȿ� �ִ� ���� ����Ʈ Ű�� �˻��Ѵ�
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				(*vArrIter)->Update();
			}
		}
	}
}

void TeicEffectManager::AddEffect(char * Key, EffectType type, D3DXVECTOR3 position, D3DXVECTOR3 character, int buffer)
{
	arrEffects vEffectBuffer;
	arrEffect mArrEffect;

	if (type == Skill_Explosion)
	{
		//����Ʈ ���ͷ� Ǫ��.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicIceExplosion);
			vEffectBuffer[i]->Setup(position, character);
		}

		//����Ʈ ���۸� �ʿ� ��´�
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//Ű���� ���۸� ���� ���� �ٽ� _vTotalEffects�� �־��ش�
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Skill_breath)
	{
		//����Ʈ ���ͷ� Ǫ��.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicIceBreath);
			vEffectBuffer[i]->Setup(position, character);
		}

		//����Ʈ ���۸� �ʿ� ��´�
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//Ű���� ���۸� ���� ���� �ٽ� _vTotalEffects�� �־��ش�
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Skill_Blizzard)
	{
		//����Ʈ ���ͷ� Ǫ��.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicIceblizzard);
			vEffectBuffer[i]->Setup(position, character);
		}

		//����Ʈ ���۸� �ʿ� ��´�
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//Ű���� ���۸� ���� ���� �ٽ� _vTotalEffects�� �־��ش�
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Character_Blood)
	{
		//����Ʈ ���ͷ� Ǫ��.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicCharacterBlood);
			vEffectBuffer[i]->Setup(position, character);
		}

		//����Ʈ ���۸� �ʿ� ��´�
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//Ű���� ���۸� ���� ���� �ٽ� _vTotalEffects�� �־��ش�
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Monseter_Blood)
	{
		//����Ʈ ���ͷ� Ǫ��.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicMonsterBlood);
			vEffectBuffer[i]->Setup(position, character);
		}

		//����Ʈ ���۸� �ʿ� ��´�
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//Ű���� ���۸� ���� ���� �ٽ� _vTotalEffects�� �־��ش�
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Monster_Chill)
	{
		//����Ʈ ���ͷ� Ǫ��.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicMonsterChill);
			vEffectBuffer[i]->Setup(position, character);
		}

		//����Ʈ ���۸� �ʿ� ��´�
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//Ű���� ���۸� ���� ���� �ٽ� _vTotalEffects�� �־��ش�
		_vTotalEffects.push_back(mArrEffect);
	}

	
}


void TeicEffectManager::play(string effectName, D3DXVECTOR3 position, D3DXVECTOR3 character)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//����Ʈ�� �־�� ���� �˻�
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//��Ż����Ʈ ���� �ȿ� �ִ� ���� ����Ʈ Ű�� �˻��Ѵ�
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			//����Ʈ �̸��� ���� ���� ������ ���� ������ �Ѿ��...
			if (!(mIter->first == effectName)) break;

			//����Ʈ Ű�� ��ġ�ϸ� ����Ʈ ����...
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				if ((*vArrIter)->IsRunning()) continue;
				(*vArrIter)->SetPosition(position, character);
				(*vArrIter)->Start();
				return;
			}
			int a = 0;
		}
	}

}

void TeicEffectManager::stop(string effectName)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//����Ʈ�� �־�� ���� �˻�
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//��Ż����Ʈ ���� �ȿ� �ִ� ���� ����Ʈ Ű�� �˻��Ѵ�
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			//����Ʈ �̸��� ���� ���� ������ ���� ������ �Ѿ��...
			if (!(mIter->first == effectName)) break;

			//����Ʈ Ű�� ��ġ�ϸ� ����Ʈ ����...
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				if (!(*vArrIter)->IsRunning()) continue;
				(*vArrIter)->Stop();
				return;
			}
		}
	}
}

TeicEffectManager::TeicEffectManager()
{
}


TeicEffectManager::~TeicEffectManager()
{
}
