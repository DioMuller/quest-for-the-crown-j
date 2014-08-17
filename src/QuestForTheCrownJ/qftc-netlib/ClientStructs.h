#pragma once
#include <string>
#include "BattleDefinitions.h"
#include "StructBase.h"

typedef struct s_client_header
{
	Header header;
	char authKey[260];
} ClientHeader;

typedef struct s_client_request_player_info
{
	ClientHeader header = { { PacketType::CLIENT_REQUEST_PLAYER_INFO } };
} ClientRequestPlayerInfo;

typedef struct s_client_request_entities
{
	ClientHeader header = { { PacketType::CLIENT_REQUEST_ENTITIES } };
} ClientRequestEntities;

typedef struct s_client_send_player_position
{
	ClientHeader header = { { PacketType::CLIENT_SEND_PLAYER_POSITION } };
	EntityLocation location;
	EntityView view;
} ClientSendPlayerPosition;

typedef struct c_client_send_disconnect
{
	ClientHeader header = { { PacketType::CLIENT_SEND_DISCONNECT } };
} ClientSendDisconnect;

// Sends character status on the server.
typedef struct s_client_character_status
{
	ClientHeader clientHeader;
	EntityHeader entity;
	int level;
	int current_hp;
	int current_mp;
} ClientCharacterStatus;

// Updates character item quantity.
typedef struct s_client_character_item
{
	ClientHeader clientHeader;
	EntityHeader entity;
	int item_id;
	int quantity;
} ClientCharacterItemInfo;


typedef struct s_client_character_battle_begin
{
	ClientHeader clientHeader;
	EntityHeader entity;
	int enemy_id;
} ClientCharacterBattleBegin;

// Request next turn info from the server.
typedef struct s_client_character_next_turn
{
	ClientHeader header = { { PacketType::CLIENT_BATTLE_NEXT_TURN } };
	int battle_id;
	int lastTurn;
} ClientCharacterBattleNextTurn;


typedef struct s_client_character_command
{
	ClientHeader header = { { PacketType::CLIENT_BATTLE_COMMAND } };
	int turn_id;
	qfcbase::BattleAction command;
	int entity_id;
	int target_id;
	int additional_info; // Item id, Skill id.
} ClientCharacterBattleCommand;