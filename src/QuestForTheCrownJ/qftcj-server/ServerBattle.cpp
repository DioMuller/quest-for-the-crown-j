#include "ServerBattle.h"

using namespace qfcserver;

ServerBattle::ServerBattle(std::weak_ptr<qfcbase::Game> parent) : qfcbase::BattleScene(parent)
{
}


ServerBattle::~ServerBattle()
{
}

bool ServerBattle::SelectAction(std::shared_ptr<qfcbase::BattleEntity> entity)
{
	return false;
}