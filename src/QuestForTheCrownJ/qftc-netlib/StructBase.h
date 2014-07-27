#pragma once
#include <vector>

enum PacketType
{
	LAUNCHER_LOGIN_INFO,
	LAUNCHER_LOGIN_RESPONSE,
	CLIENT_REQUEST_PLAYER_INFO,
	CLIENT_CHARACTER_POSITION,
	CLIENT_CHARACTER_STATUS,
	CLIENT_CHARACTER_ITEM,
	CLIENT_BATTLE_BEGIN,
	CLIENT_BATTLE_NEXT_TURN,
	CLIENT_BATTLE_COMMAND,
	SERVER_PLAYER_INFO,
	SERVER_ENTITY_INFO,
	SERVER_MAP_TRANSITION,
	SERVER_BATTLE_START,
	SERVER_BATTLE_TURN
};

enum EntityType
{
	ENTITY_HERO,
	ENTITY_SLIME
};

// Base header methods.
typedef struct s_header
{
	PacketType type;
} Header;

typedef struct s_entity_header
{
	int entityId;
	EntityType type;
} EntityHeader;