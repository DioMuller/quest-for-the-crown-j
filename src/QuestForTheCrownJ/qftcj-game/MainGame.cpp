#include "MainGame.h"
#include "AudioPlayer.h"

#include "Scene.h"
#include "Controllable.h"
#include "KeyboardInput.h"
#include "Hero.h"
#include "Slime.h"

using namespace qfcgame;

qfcbase::Scene* CreateScene(qfcbase::Game* game);

MainGame::MainGame()
{
	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetSoundPath("Content/sound/");

	LoadScene(CreateScene(this), false);
}

qfcbase::Scene* CreateScene(qfcbase::Game* game)
{
	auto scene = new qfcbase::Scene(game);
	auto hero = new Hero();
	hero->AddBehavior(new Controllable(hero, new KeyboardInput()));
	scene->AddEntity(hero);
	auto enemy = new Slime();
	enemy->Sprite->Position = { 400, 400 };
	scene->AddEntity(enemy);
	return scene;
}

MainGame::~MainGame()
{
}

void MainGame::Update(double dt)
{
	Game::Update(dt);
}