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

typedef struct s_server_entity_info
{
	Header header;
	EntityHeader entity;
	sf::Vector2i position;
} ServerEntityInfo;

typedef struct s_player_info
{
	Header header;
	char map_name[255];
	EntityHeader entity;
	sf::Vector2i position;
} ServerPlayerInfo;

typedef struct s_server_map_transition
{
	Header header;
	EntityHeader entity;
	std::string newMap;
} ServerMapTransition;

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