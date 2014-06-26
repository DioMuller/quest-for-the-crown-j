#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include "BattleDefinitions.h"
#include "StructBase.h"

// Client current position.
typedef struct s_client_position
{
	Header header;
	EntityHeader entity;
	sf::Vector2i position;
} ClientCharacterPosition;

// Sends character status on the server.
typedef struct s_client_character_status
{
	Header header;
	EntityHeader entity;
	int level;
	int current_hp;
	int current_mp;
} ClientCharacterStatus;

// Updates character item quantity.
typedef struct s_client_character_item
{
	Header header;
	EntityHeader entity;
	int item_id;
	int quantity;
} ClientCharacterItemInfo;


typedef struct s_client_character_battle_begin
{
	Header header;
	EntityHeader entity;
	int enemy_id;
} ClientCharacterBattleBegin;

// Request next turn info from the server.
typedef struct s_client_character_next_turn
{
	Header header;
	EntityHeader entity;
	int battle_id;
	int lastTurn;
} ClientCharacterBattleNextTurn;


typedef struct s_client_character_command
{
	Header header;
	EntityHeader entity;
	int battle_id;
	int turn_id;
	qftcbase::BattleCommand command;
	int additional_info; // Item id, Skill id.
} ClientCharacterBattleCommand;