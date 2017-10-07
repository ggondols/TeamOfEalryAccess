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

	//이펙트를 넣어둔 벡터 검사
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

	//이펙트를 넣어둔 벡터 검사
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//토탈이펙트 벡터 안에 있는 맵의 이펙트 키를 검사한다
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

	//이펙트를 넣어둔 벡터 검사
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//토탈이펙트 벡터 안에 있는 맵의 이펙트 키를 검사한다
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
		//이펙트 벡터로 푸쉬.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicIceExplosion);
			vEffectBuffer[i]->Setup(position, character);
		}

		//이펙트 버퍼를 맵에 담는다
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//키값과 버퍼를 담은 맵을 다시 _vTotalEffects에 넣어준다
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Skill_breath)
	{
		//이펙트 벡터로 푸쉬.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicIceBreath);
			vEffectBuffer[i]->Setup(position, character);
		}

		//이펙트 버퍼를 맵에 담는다
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//키값과 버퍼를 담은 맵을 다시 _vTotalEffects에 넣어준다
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Skill_Blizzard)
	{
		//이펙트 벡터로 푸쉬.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicIceblizzard);
			vEffectBuffer[i]->Setup(position, character);
		}

		//이펙트 버퍼를 맵에 담는다
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//키값과 버퍼를 담은 맵을 다시 _vTotalEffects에 넣어준다
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Character_Blood)
	{
		//이펙트 벡터로 푸쉬.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicCharacterBlood);
			vEffectBuffer[i]->Setup(position, character);
		}

		//이펙트 버퍼를 맵에 담는다
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//키값과 버퍼를 담은 맵을 다시 _vTotalEffects에 넣어준다
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Monseter_Blood)
	{
		//이펙트 벡터로 푸쉬.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicMonsterBlood);
			vEffectBuffer[i]->Setup(position, character);
		}

		//이펙트 버퍼를 맵에 담는다
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//키값과 버퍼를 담은 맵을 다시 _vTotalEffects에 넣어준다
		_vTotalEffects.push_back(mArrEffect);
	}
	else if (type == Monster_Chill)
	{
		//이펙트 벡터로 푸쉬.....
		for (int i = 0; i < buffer; i++)
		{
			vEffectBuffer.push_back(new TeicMonsterChill);
			vEffectBuffer[i]->Setup(position, character);
		}

		//이펙트 버퍼를 맵에 담는다
		mArrEffect.insert(make_pair(Key, vEffectBuffer));

		//키값과 버퍼를 담은 맵을 다시 _vTotalEffects에 넣어준다
		_vTotalEffects.push_back(mArrEffect);
	}

	
}


void TeicEffectManager::play(string effectName, D3DXVECTOR3 position, D3DXVECTOR3 character)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//이펙트를 넣어둔 벡터 검사
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//토탈이펙트 벡터 안에 있는 맵의 이펙트 키를 검사한다
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			//이펙트 이름을 비교해 같지 않으면 다음 맵으로 넘어간다...
			if (!(mIter->first == effectName)) break;

			//이펙트 키와 일치하면 이펙트 실행...
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

	//이펙트를 넣어둔 벡터 검사
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//토탈이펙트 벡터 안에 있는 맵의 이펙트 키를 검사한다
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			//이펙트 이름을 비교해 같지 않으면 다음 맵으로 넘어간다...
			if (!(mIter->first == effectName)) break;

			//이펙트 키와 일치하면 이펙트 실행...
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
