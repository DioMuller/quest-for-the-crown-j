#pragma once
#include "Definitions.h"
#include <map>
#include <SFML/Audio.hpp>

namespace qfcbase
{
	class AudioPlayer
	{
		/////////////////////////////////////
		// Static Attributes
		/////////////////////////////////////
		private:
			static std::string soundPath;
			static std::map<std::string, sf::SoundBuffer> sounds;
			static sf::Sound soundPlayer;
			static std::string bgmPath;
			static std::string currentBgmName;
			static sf::Music currentBgm;


		/////////////////////////////////////
		// Static Methods
		/////////////////////////////////////
		public:
			static void SetSoundPath(std::string path);
			static void SetMusicPath(std::string path);
			static void PlaySound(std::string name);
			static void PlayBGM(std::string name);
	};
}

