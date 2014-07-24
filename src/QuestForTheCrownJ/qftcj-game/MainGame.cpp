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

std::shared_ptr<qfcbase::Entity> CreateEntity(const ServerEntityInfo& info)
{
	std::shared_ptr<qfcbase::Entity> entity;

	switch (info.entity.type)
	{
	case EntityType::ENTITY_HERO:
		entity = std::make_shared<qfcbase::Hero>();
		break;
	case EntityType::ENTITY_SLIME:
		entity = std::make_shared<qfcbase::Slime>();
		break;
	}

	if (entity)
		entity->Sprite->Position = sf::Vector2f(info.position.x, info.position.y);

	return entity;
}

MainGame::MainGame(std::string auth_token)
	: clientChannel("localhost", 12345)
{
	clientChannel.auth_token = auth_token;
	clientChannel.onEntity = [this](const ServerEntityInfo& entityInfo) {
		auto entity = CreateEntity(entityInfo);
		currentScene->AddEntity(entity);
	};

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

	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");

	auto scene = qfcbase::LevelLoader::LoadMap(this, 1, (std::string)"Content/maps/" + player.map_name + ".tmx");
	Game::LoadScene(scene, false);
	clientChannel.GetEntities(player.map_name);
}

MainGame::~MainGame()
{
}

void MainGame::Update(double dt)
{
	Game::Update(dt);
}