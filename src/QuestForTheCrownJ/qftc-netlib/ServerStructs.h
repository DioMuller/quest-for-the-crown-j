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

typedef struct s_server_send_entity
{
	Header header = { { PacketType::SERVER_SEND_ENTITY } };
	EntityInfo entity;
} ServerSendEntity;

typedef struct s_server_send_entity_removed
{
	Header header = { { PacketType::SERVER_SEND_ENTITY_REMOVED } };
	int entity_id;
} ServerSendEntityRemoved;

// TODO: Remove duplicated struct
typedef struct s_server_response_player_info
{
	Header header = { { PacketType::SERVER_RESPONSE_PLAYER_INFO } };
	EntityInfo entity;
} ServerResponsePlayerInfo;

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