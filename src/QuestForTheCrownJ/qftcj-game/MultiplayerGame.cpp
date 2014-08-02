#include "MultiplayerGame.h"

#include "Hero.h"
#include "Slime.h"
#include "Log.h"
#include "LevelLoader.h"
#include "Controllable.h"
#include "KeyboardInput.h"

using namespace qfcgame;
using namespace qfcbase;

std::shared_ptr<qfcbase::Entity> CreateEntity(int id, EntityType type, float x, float y)
{
	std::shared_ptr<qfcbase::Entity> entity;

	switch (type)
	{
	case EntityType::ENTITY_HERO:
		entity = std::make_shared<qfcbase::Hero>();
		break;
	case EntityType::ENTITY_SLIME:
		entity = std::make_shared<qfcbase::Slime>();
		break;
	default:
		Log::Error("Entity type not supported by client: " + type);
	}

	if (entity) {
		entity->Sprite->Position = sf::Vector2f(x, y);
		entity->Id = id;
	}

	return entity;
}

MultiplayerGame::MultiplayerGame()
{
}


MultiplayerGame::~MultiplayerGame()
{
}

void MultiplayerGame::Connect(int localPort, std::string auth_token)
{
	clientChannel.Connect(localPort, "127.0.0.1", 12345);
	clientChannel.auth_token = auth_token;
	clientChannel.onEntity = [this](const ServerSendEntity& info) {
		auto updateEntities = currentScene->GetEntity(info.entity.entityId);
		if (!updateEntities) {
			auto entity = CreateEntity(info.entity.entityId, info.entity.type, info.position.x, info.position.y);
			currentScene->AddEntity(entity);
		}
		else {
			updateEntities->Sprite->Position = sf::Vector2f(info.position.x, info.position.y);
		}
	};
}

void MultiplayerGame::RefreshSceneFromServer()
{
	clientChannel.GetPlayer([=](ServerResponsePlayerInfo& info) {
		auto scene = LevelLoader::LoadMap(this->getptr(), 1, (std::string)"Content/maps/" + (std::string)info.player.map_name + (std::string)".tmx");
		LoadScene(scene, false);
		clientChannel.GetEntities(info.player.map_name);

		auto player = currentScene->GetEntity(info.entity.entityId);
		if (!player)
		{
			player = CreateEntity(info.entity.entityId, info.entity.type, info.player.position.x, info.player.position.y);
			player->AddBehavior(std::make_shared<qfcgame::Controllable>(player, std::make_shared<qfcgame::KeyboardInput>()));
			currentScene->AddEntity(player);
		}

		Log::Debug((std::string)"Map loaded: " + std::string(info.player.map_name));
	}, [&](std::exception& ex) {
		Log::Error(ex.what());
	});
}
