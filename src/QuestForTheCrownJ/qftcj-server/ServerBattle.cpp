#include "ServerBattle.h"
#include "BattleEntity.h"
#include "Server.h"

using namespace qfcserver;
using namespace qfcbase;

ServerBattle::ServerBattle(std::weak_ptr<qfcbase::Game> parent)
	: qfcbase::BattleScene(parent)
{
}


ServerBattle::~ServerBattle()
{
}

bool ServerBattle::SelectAction(std::shared_ptr<qfcbase::BattleEntity> currentEntity)
{
	std::shared_ptr<BattleEntity> targetEntity;

	for (auto ent : GetEntities())
	{
		auto be = std::static_pointer_cast<BattleEntity>(ent);
		if (be && be->Type() != currentEntity->Type())
		{
			targetEntity = be;
			break;
		}
	}

	if (!targetEntity) return false;

	Turn nextTurn;

	switch (currentEntity->Type())
	{
	case BattleEntityType::PLAYER:
		//TODO: Check if there is a player queued.
		//Log::Debug("Player Turn - No change");
		if (receivedTurns.size() > 0)
		{
			for (auto turn : receivedTurns)
			{
				if (turn.id == currentTurn && turn.entity->Type() == BattleEntityType::PLAYER)
				{
					turns.push_back(turn);
					return true;
				}
			}
		}

		if (lastLogTurn != currentTurn)
		{
			Log::Debug("Waiting for player action on turn " + std::to_string(currentTurn));
			lastLogTurn = currentTurn;
		}

		return false;
		break;
	case BattleEntityType::ENEMY:
		nextTurn = Turn{ currentTurn, currentEntity, targetEntity, (rand()%3 == 1) ? BattleAction::SPECIAL : BattleAction::ATTACK, 3 + rand() % 5 };

		for (auto entity : GetEntities())
		{
			auto be = std::static_pointer_cast<BattleEntity>(entity);

			if (be)
				SendTurn(nextTurn.action, nextTurn.entity->GetParent()->Id, nextTurn.target->GetParent()->Id, nextTurn.value, be->GetParent());
		}

		Log::Debug("Pushing back turn " + std::to_string(currentTurn));
		turns.push_back(nextTurn);

		return true;
	default:
		return false;
	}	
}

void ServerBattle::SendTurn(qfcbase::BattleAction command, int entity, int target, int additional_info, std::shared_ptr<Entity> currentEntity, int turn_id)
{
	if (turn_id == -1) turn_id = currentTurn;

	auto sceneParent = std::dynamic_pointer_cast<Server>(parent.lock());

	if (sceneParent)
	{
		sceneParent->SendTurn(turn_id, command, entity, target, additional_info, currentEntity);
	}
}

void ServerBattle::ReceiveTurn(int turn_id, qfcbase::BattleAction command, int additional_info, int entity_id, int target_id)
{
	std::shared_ptr<BattleEntity> currentEntity = nullptr;
	std::shared_ptr<BattleEntity> targetEntity = nullptr;

	for (auto entity : GetEntities())
	{
		auto bent = std::static_pointer_cast<BattleEntity>(entity);
		if (bent)
		{
			if (bent->GetParent()->Id == entity_id) currentEntity = bent;
			if (bent->GetParent()->Id == target_id) targetEntity = bent;
		}
	}

	if (currentEntity && targetEntity)
		receivedTurns.push_back(Turn{ turn_id, currentEntity, targetEntity, command, additional_info });
	else {
		if (!currentEntity)
			Log::Error("Unknown battle entity: " + std::to_string(entity_id));
		else
			Log::Error("Unknown battle target: " + std::to_string(target_id));
	}
}

void ServerBattle::SendTurnToClients(int turn_id)
{
	for (auto turn : turns)
	{
		if (turn.id == turn_id)
		{
			for (auto entity : GetEntities())
			{
				auto be = std::static_pointer_cast<BattleEntity>(entity);

				if (be)
					SendTurn(turn.action, turn.entity->GetParent()->Id, turn.target->GetParent()->Id, turn.value, be->GetParent(), turn.id);
			}

			return;
		}
	}
}