#include "ServerBattle.h"
#include "BattleEntity.h"
#include "Server.h"

using namespace qfcserver;
using namespace qfcbase;

ServerBattle::ServerBattle(std::weak_ptr<qfcbase::Game> parent) : qfcbase::BattleScene(parent)
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
		return false;
		break;
	case BattleEntityType::ENEMY:
		nextTurn = Turn{ currentTurn, currentEntity, targetEntity, (rand()%3 == 1) ? BattleAction::SPECIAL : BattleAction::ATTACK, 3 + rand() % 5 };
		SendTurn(nextTurn.action, nextTurn.target->Id, nextTurn.value);
		break;
	default:
		return false;
	}

	turns.push_back(nextTurn);

	return true;
}

void ServerBattle::SendTurn(qfcbase::BattleAction command, int target, int additional_info)
{
	auto sceneParent = std::dynamic_pointer_cast<Server>(parent.lock());

	if (sceneParent)
	{
		sceneParent->SendTurn(currentTurn, command, target, additional_info);
	}
}

void ServerBattle::ReceiveTurn(qfcbase::BattleAction command, int additional_info, int target_id)
{

}