#include "stdafx.h"
#include "cSoundLoader.h"
#include <sstream>
#include <fstream>


cSoundLoader::cSoundLoader()
	:numberOfTracks(0)
{
}


cSoundLoader::~cSoundLoader()
{
}

bool cSoundLoader::LoadSound(string fullPath)
{
	string  buffer, token;
	string scriptPath = fullPath;
	ifstream file(scriptPath);

	if (file.good())
		while (getline(file, buffer))
		{
			istringstream iss(buffer);

			iss >> token;

			if (token == "BGM")
			{
				string soundKey, soundPath, soundName;
				float bgm = 0, loop = 0;

				iss >> soundKey >> soundPath >> soundName >> bgm >> loop;
				
				_soundType.push_back(token);
				_soundKey.push_back(soundKey);
				_soundPath.push_back(soundPath);
				_soundName.push_back(soundName);
				_bgm.push_back(bgm);
				_loop.push_back(loop);
				
				
				tagSoundList temp;
				temp.numberOfTracks = numberOfTracks;
				temp._bgm = bgm;
				temp._loop = loop;
				temp._soundKey = soundKey;
				temp._soundType = token;
				temp._soundName = soundName;
				temp._soundPath = soundPath;

				_wholeSound.push_back(temp);

				numberOfTracks++;

				//SOUNDMANAGER->addSound(soundKey, soundPath, soundName, bgm, loop);
			}
		}

	return false;
}

