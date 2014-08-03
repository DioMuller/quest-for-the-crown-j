#include "MultiplayerGame.h"

#include "Hero.h"
#include "Slime.h"
#include "Log.h"
#include "LevelLoader.h"
#include "Controllable.h"
#include "KeyboardInput.h"
#include "WatchPosition.h"
#include "Definitions.h"
#include "Window.h"
#include "NetDefinitions.h"
#include "NetHelper.h"

using namespace qfcgame;
using namespace qfcbase;

std::shared_ptr<qfcbase::Entity> MultiplayerGame::CreateEntity(int id, EntityType type, float x, float y)
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

	if (player_entity_id == entity->Id)
	{
		entity->AddBehavior(std::make_shared<Controllable>(entity, std::make_shared<KeyboardInput>()));
		entity->AddBehavior(std::make_shared<WatchPosition>(entity, [&](std::shared_ptr<Entity> e) {
			if (e->Sprite->CurrentAnimation != "")
				clientChannel.SendPlayerPosition(e->Sprite->CurrentAnimation, e->Sprite->Position.x, e->Sprite->Position.y);
		}, NET_SECONDS_PER_FRAME));
	}

	return entity;
}

MultiplayerGame::MultiplayerGame()
{
	Log::SetVerboseLevel(LOG_ALL);
}


MultiplayerGame::~MultiplayerGame()
{
}

void MultiplayerGame::Connect(std::string server_addr, std::string auth_token)
{
	clientChannel.Connect(0, server_addr, 12345);
	clientChannel.auth_token = auth_token;
	clientChannel.onEntity = [this](const ServerSendEntity& info) {
		std::lock_guard<std::mutex> lock_create(ent_update_mutex);

		auto updateEntity = currentScene->GetEntity(info.entity.entityId);
		if (!updateEntity) {
			Log::Debug((std::string)"Created Entity: " + std::to_string(info.entity.entityId) + " " + std::to_string(info.entity.type));
			auto entity = CreateEntity(info.entity.entityId, info.entity.type, info.position.x, info.position.y);
			currentScene->AddEntity(entity);

			if (player_entity_id == info.entity.entityId)
				SetPlayer(entity);
		}
		else {
			if (info.entity.entityId != player_entity_id)
			{
				updateEntity->Sprite->Position = sf::Vector2f(info.position.x, info.position.y);
				if(info.animation >= 0)
					updateEntity->Sprite->SetCurrentAnimation(NetHelper::DecodeAnimation(info.animation));
			}
		}
	};
}

void MultiplayerGame::RefreshSceneFromServer()
{
	clientChannel.GetPlayer([=](ServerResponsePlayerInfo& info) {
		auto scene = LevelLoader::LoadMap(this->getptr(), 1, (std::string)"Content/maps/" + (std::string)info.player.map_name + (std::string)".tmx");
		LoadScene(scene, false);
		this->player_entity_id = info.entity.entityId;
		clientChannel.GetEntities(info.player.map_name);
		Log::Debug((std::string)"Map loaded: " + std::string(info.player.map_name));
	}, [&](std::exception& ex) {
		Log::Error(ex.what());
	});
}
