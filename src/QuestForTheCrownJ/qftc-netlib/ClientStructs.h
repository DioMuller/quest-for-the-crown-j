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

typedef struct s_client_send_player_full_position
{
	ClientHeader header = { { PacketType::CLIENT_SEND_PLAYER_FULL_POSITION } };
	PlayerFullPosition player;
} ClientSendPlayerFullPosition;

typedef struct s_client_send_player_position
{
	ClientHeader header = { { PacketType::CLIENT_SEND_PLAYER_POSITION } };
	sf::Vector2i position;
	int animation;
} ClientSendPlayerPosition;

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
	ClientHeader clientHeader;
	EntityHeader entity;
	int battle_id;
	int lastTurn;
} ClientCharacterBattleNextTurn;


typedef struct s_client_character_command
{
	ClientHeader clientHeader;
	EntityHeader entity;
	int battle_id;
	int turn_id;
	qftcbase::BattleCommand command;
	int additional_info; // Item id, Skill id.
} ClientCharacterBattleCommand;