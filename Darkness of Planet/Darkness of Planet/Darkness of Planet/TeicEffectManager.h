#pragma once
#include "cSingletonBase.h"
#include "TeicEffectParent.h"

enum EffectType
{
	Skill_breath,
	Skill_Explosion,
	Skill_Blizzard,
	Character_Blood,
	Monseter_Blood,
	Monster_Chill,
	Flame,
	sample,
	Laser
};
class TeicEffectManager:public cSingletonBase<TeicEffectManager>
{
private:
	typedef vector<TeicEffectParent*> arrEffects;
	typedef vector<TeicEffectParent*>::iterator iterEffects;
	typedef map<string, arrEffects> arrEffect;
	typedef map<string, arrEffects>::iterator iterEffect;
	typedef vector<map<string, arrEffects>> arrTotalEffect;
	typedef vector<map<string, arrEffects>>::iterator iterTotalEffect;

private:
	arrTotalEffect _vTotalEffects;


public:
	void Setup();
	void Destroy();
	void Render();
	void Update();
	void AddEffect(char* Key,EffectType type, D3DXVECTOR3 position, D3DXVECTOR3 character, int buffer);
	
	void play(string effectName, D3DXVECTOR3 position, D3DXVECTOR3 character);
	void stop(string effectName);
	TeicEffectManager();
	~TeicEffectManager();
};

