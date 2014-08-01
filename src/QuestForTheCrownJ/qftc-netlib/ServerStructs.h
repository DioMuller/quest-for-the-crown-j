#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include "BattleDefinitions.h"
#include "StructBase.h"

/*
SERVER_ENTITY_POSITION,
SERVER_MAP_TRANSITION,
SERVER_BATTLE_START,
SERVER_BATTLE_TURN
*/

typedef struct s_server_battle_start
{
	Header header;
	EntityHeader entity;
	int battle_id;
} ServerBattleStart;

typedef struct s_server_battle_turn
{
	Header header;
	EntityHeader entity;
	int turn_id;
	qftcbase::BattleCommand command;
	int additional_info; // Item id, Skill id.
} ServerBattleTurn;