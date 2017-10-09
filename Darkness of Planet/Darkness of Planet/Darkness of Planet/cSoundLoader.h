#pragma once
#include "Loading.h"



class cSoundLoader : public LoadItem
{
private: 
	typedef vector<string> vString;
	typedef vector<int> vInt;
	typedef vector<tagSoundList> vSoundList;
public:
	vString _soundKey, _soundPath, _soundName, _soundType;		
	vInt _bgm , _loop;
	int numberOfTracks;
	vSoundList _wholeSound;
public:
	cSoundLoader();
	~cSoundLoader();

	bool LoadSound(string fullPath = "Data/Script/SoundList.txt");

};

