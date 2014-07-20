#pragma once
#include <vector>

enum PacketType
{
	LAUNCHER_LOGIN_INFO,
	LAUNCHER_LOGIN_RESPONSE,
	CLIENT_CHARACTER_POSITION,
	CLIENT_CHARACTER_STATUS,
	CLIENT_CHARACTER_ITEM,
	CLIENT_BATTLE_BEGIN,
	CLIENT_BATTLE_NEXT_TURN,
	CLIENT_BATTLE_COMMAND,
	SERVER_ENTITY_POSITION,
	SERVER_MAP_TRANSITION,
	SERVER_BATTLE_START,
	SERVER_BATTLE_TURN
};

// Base header methods.
typedef struct s_header
{
	PacketType type;
} Header;

typedef struct s_entity_header
{
	int entityId;
} EntityHeader;

typedef struct
{
	std::string auth_token;
	std::string map_name;
	EntityInfo entity;
} PlayerInfo;

enum EntityType
{
	ENTITY_HERO,
	ENTITY_SLIME
};

typedef struct
{
	//unsigned int uid;
	EntityType type;
	int x, y;
} EntityInfo;
