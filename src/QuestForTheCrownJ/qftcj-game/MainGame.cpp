#include "MainGame.h"
#include "AudioPlayer.h"

using namespace qfcgame;

MainGame::MainGame()
{
	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetSoundPath("Content/sound/");
}


MainGame::~MainGame()
{
}

void MainGame::Update(double dt)
{
	Game::Update(dt);
}