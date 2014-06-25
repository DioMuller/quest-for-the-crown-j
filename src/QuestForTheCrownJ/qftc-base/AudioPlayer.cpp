#include "AudioPlayer.h"
#include "Log.h"

using namespace qfcbase;

std::string AudioPlayer::soundPath = "";
std::map<std::string, sf::Sound> AudioPlayer::sounds = std::map<std::string, sf::Sound>();
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

	}

	sounds[name].play();
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
