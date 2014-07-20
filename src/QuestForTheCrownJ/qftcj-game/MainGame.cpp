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
#include "StructBase.h"

using namespace qfcgame;

MainGame::MainGame(std::string auth_token)
	: clientChannel("localhost", 12345)
{
	clientChannel.AuthToken = auth_token;

	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetSoundPath("Content/sound/");
}

void MainGame::RefreshSceneFromServer()
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

	auto player = clientChannel.GetPlayerInfo();
	auto entities = clientChannel.GetEntities(player.map_name);

	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");

	auto scene = qfcbase::LevelLoader::LoadMap(this, 1, "Content/maps/" + player.map_name + ".tmx");
	Game::LoadScene(scene, false);
	SetEntities(entities);
}

MainGame::~MainGame()
{
}

void MainGame::SetEntities(std::vector<std::shared_ptr<qfcbase::Entity>> entities)
{
	for (auto entity : entities)
		currentScene->AddEntity(entity);
}

void MainGame::Update(double dt)
{
	Game::Update(dt);
}