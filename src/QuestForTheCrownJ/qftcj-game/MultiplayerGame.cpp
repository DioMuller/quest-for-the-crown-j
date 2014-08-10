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
#include "Walker.h"
#include "ClientBattle.h"

using namespace qfcgame;
using namespace qfcbase;

std::shared_ptr<qfcbase::Entity> MultiplayerGame::CreateEntity(EntityHeader info, sf::Vector2f position)
{
	std::shared_ptr<qfcbase::Entity> entity;

	switch (info.type)
	{
	case EntityType::ENTITY_HERO:
		entity = std::make_shared<qfcbase::Hero>();
		break;
	case EntityType::ENTITY_SLIME:
		entity = std::make_shared<qfcbase::Slime>();
		break;
	default:
		Log::Error((std::string)"Entity type not supported by client: " + std::to_string(info.type));
	}

	entity->category = NetHelper::DecodeCategory(info.category);

	if (entity) {
		entity->Sprite->Position = position;
		entity->Id = info.id;
	}

	if (player_entity_id == entity->Id)
	{
		entity->AddBehavior<WatchPosition>([&](std::shared_ptr<Entity> e) {
			auto scene = std::dynamic_pointer_cast<Level>(e->Scene().lock());
			if (!scene) return;
			if (e->Sprite->CurrentAnimation != "")
				clientChannel.SendPlayerPosition(e->Sprite->CurrentAnimation, scene->Id(), e->Sprite->Position);
		}, NET_SECONDS_PER_FRAME);
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
	clientChannel.onEntity = [this](const ServerSendEntity& data) {
		std::lock_guard<std::mutex> lock_create(ent_update_mutex);
		auto level = std::dynamic_pointer_cast<Level>(currentScene);
		if (!level)
			return;

		auto updateEntity = currentScene->GetEntity(data.entity.info.id);

		if (data.entity.location.map_id != level->Id())
		{
			if (updateEntity)
				currentScene->RemoveEntity(updateEntity);
			return;
		}

		auto position = data.entity.location.position;

		if (!updateEntity) {
			Log::Debug((std::string)"Created Entity: " + std::to_string(data.entity.info.id) + " " + std::to_string(data.entity.info.type));
			auto entity = CreateEntity(data.entity.info, position);
			currentScene->AddEntity(entity);

			entity->AddBehavior<Walker>();

			if (player_entity_id == data.entity.info.id)
				SetPlayer(entity);
		}
		else {
			if (data.entity.info.id != player_entity_id)
			{
				auto walker = updateEntity->FindBehavior<Walker>();
				if (walker)
					walker->WalkTo(position);
				else
					updateEntity->Sprite->Position = sf::Vector2f(position.x, position.y);
				if (data.entity.view.animation >= 0)
					updateEntity->Sprite->SetCurrentAnimation(NetHelper::DecodeAnimation(data.entity.view.animation));
			}
		}
	};
	clientChannel.onEntityRemoved = [this](const ServerSendEntityRemoved data) {
		auto removedEntity = currentScene->GetEntity(data.entity_id);
		if (removedEntity)
			currentScene->RemoveEntity(removedEntity);
	};

	clientChannel.onBattleStart = [this](const ServerBattleStart data) 
	{
		Log::Message("Received a Battle Start Message");
		auto level = std::dynamic_pointer_cast<Level>(currentScene);
		if (!level)
		{
			Log::Message("Not on a Level. Battle Message dropped.");
			return;
		}


	};

	clientChannel.onTurnReceived = [this](const ServerBattleTurn data)
	{
		Log::Message("Received a Turn Info Message");
		auto level = std::dynamic_pointer_cast<BattleScene>(currentScene);
		if (!level)
		{
			Log::Message("Not on a Level. Battle Message dropped.");
			return;
		}
	};
}

void MultiplayerGame::RefreshSceneFromServer()
{
	clientChannel.GetPlayer([=](ServerResponsePlayerInfo& data) {
		auto level_info = LevelCollection::GetLevel(data.entity.location.map_id);

		auto scene = LevelLoader::LoadMap(this->getptr(), 1, level_info->mapFile);
		LoadScene(scene, false);
		this->player_entity_id = data.entity.info.id;
		clientChannel.GetEntities();
		Log::Debug((std::string)"Map loaded: " + level_info->mapFile);
	}, [&](std::exception& ex) {
		Log::Error(ex.what());
	});
}

void MultiplayerGame::StartConfront(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	auto e1Scene = e1->scene.lock();
	if (!e1Scene)
		return;

	e1Scene->RemoveEntity(e2);
	auto battle = std::make_shared<ClientBattle>(getptr());

	if (battle->PlayerJoin(e1) &&
		battle->AddMonster(e2))
		LoadScene(battle, true);
}
