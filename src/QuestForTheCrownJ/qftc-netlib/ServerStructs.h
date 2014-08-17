#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include "BattleDefinitions.h"
#include "StructBase.h"
#include "Entity.h"

/*
SERVER_ENTITY_POSITION,
SERVER_MAP_TRANSITION,
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

typedef struct s_server_response_player_info
{
	Header header = { { PacketType::SERVER_RESPONSE_PLAYER_INFO } };
	EntityInfo entity;
	qfcbase::Items items;
} ServerResponsePlayerInfo;

typedef struct s_server_send_entity_name
{
	Header header = { { PacketType::SERVER_SEND_ENTITY_NAME } };
	int entity_id;
	char name[255];
} ServerSendEntityName;

// Battle
typedef struct s_server_battle_start
{
	Header header = { { PacketType::SERVER_SEND_BATTLE_START } };
	EntityHeader entity;
	int battle_id;
} ServerBattleStart;

typedef struct s_server_battle_turn
{
	Header header = { { PacketType::SERVER_BATTLE_TURN } };
	EntityHeader entity;
	int turn_id;
	qfcbase::BattleAction command;
	int target_id;
	int additional_info; // Item id, Skill id.
} ServerBattleTurn;