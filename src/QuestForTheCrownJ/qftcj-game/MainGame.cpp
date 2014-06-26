#include "MainGame.h"
#include "AudioPlayer.h"

#include "Scene.h"
#include "Controllable.h"
#include "KeyboardInput.h"
#include "Hero.h"

using namespace qfcgame;

MainGame::MainGame()
{
	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetSoundPath("Content/sound/");

	auto scene = new qfcbase::Scene(this);
	auto hero = new Hero();
	hero->AddBehavior(new Controllable(hero, new KeyboardInput()));
	scene->AddEntity(hero);
	LoadScene(scene, false);
}


MainGame::~MainGame()
{
}

void MainGame::Update(double dt)
{
	Game::Update(dt);
}