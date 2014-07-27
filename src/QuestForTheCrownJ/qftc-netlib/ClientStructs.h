#pragma once
#include <string>
#include "BattleDefinitions.h"
#include "StructBase.h"

typedef struct s_client_header
{
	Header header;
	char authKey[260];
} ClientHeader;

typedef struct s_request_player_info
{
	ClientHeader clientHeader;
} RequestPlayerInfo;

// Client current position.
typedef struct s_client_position
{
	ClientHeader clientHeader;
	EntityHeader entity;
	int x, y;
} ClientCharacterPosition;

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