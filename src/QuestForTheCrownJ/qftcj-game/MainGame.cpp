#include <memory>
#include "MainGame.h"
#include "AudioPlayer.h"

#include "Scene.h"
#include "Controllable.h"
#include "FollowBehavior.h"
#include "KeyboardInput.h"
#include "LevelLoader.h"
#include "Hero.h"
#include "Slime.h"

using namespace qfcgame;

qfcbase::Scene* CreateScene(qfcbase::Game* game);

MainGame::MainGame()
{
	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetSoundPath("Content/sound/");
}

qfcbase::Scene* CreateScene(qfcbase::Game* game, std::string mapName)
{
	/*
	auto scene = new qfcbase::Scene(game);
	auto hero = std::make_shared<Hero>();
	hero->category = "GoodGuy";
	hero->AddBehavior(std::make_shared<Controllable>(hero, std::make_shared<KeyboardInput>()));
	scene->AddEntity(hero);
	auto enemy = std::make_shared<Slime>();
	enemy->Sprite->Position = { 400, 400 };
	enemy->AddBehavior(std::make_shared<FollowBehavior>(enemy, "GoodGuy", 5, 32 * 4));
	scene->AddEntity(enemy);*/
	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");

	auto scene = qfcbase::LevelLoader::LoadMap(game, 1, "Content/maps/" + mapName + ".tmx");
	return &(*scene);
}

MainGame::~MainGame()
{
}

void MainGame::Update(double dt)
{
	Game::Update(dt);
}