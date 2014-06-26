#include "AudioPlayer.h"
#include "Log.h"

using namespace qfcbase;

std::string AudioPlayer::soundPath = "";
std::map<std::string, sf::SoundBuffer> AudioPlayer::sounds = std::map<std::string, sf::SoundBuffer>();
sf::Sound AudioPlayer::soundPlayer = sf::Sound();
std::string AudioPlayer::bgmPath = "";
std::string AudioPlayer::currentBgmName = "";
sf::Music AudioPlayer::currentBgm = sf::Music();

/////////////////////////////////////
// Static Methods
/////////////////////////////////////
void AudioPlayer::SetSoundPath(std::string path)
{
	soundPath = path;
}

void AudioPlayer::SetMusicPath(std::string path)
{
	bgmPath = path;
}

void AudioPlayer::PlaySound(std::string name)
{
	if (sounds.count(name) == 0)
	{
		sounds[name] = sf::SoundBuffer();

		if (!sounds[name].loadFromFile(soundPath + name + ".wav"))
		{
			currentBgmName = "";
			Log::Error("Could not open sound file: " + name);
		}
	}

	soundPlayer.setBuffer(sounds[name]);
	soundPlayer.play();
}

void AudioPlayer::PlayBGM(std::string name)
{
	if (name != currentBgmName)
	{
		currentBgm.stop();
		// TODO: Check if I need to unload?

		if (!currentBgm.openFromFile(bgmPath + name + ".wav"))
		{
			currentBgmName = "";
			Log::Error("Could not open music file: " + name);
		}
		else
		{
			currentBgmName = name;
			currentBgm.play();
		}
	}
}
