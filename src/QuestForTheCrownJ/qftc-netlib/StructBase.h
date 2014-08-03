#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>

enum PacketType
{
	LAUNCHER_LOGIN_INFO,
	LAUNCHER_LOGIN_RESPONSE,

	CLIENT_REQUEST_PLAYER_INFO,
	SERVER_RESPONSE_PLAYER_INFO,
	CLIENT_SEND_PLAYER_POSITION,
	CLIENT_SEND_PLAYER_FULL_POSITION,
	CLIENT_REQUEST_ENTITIES,
	SERVER_SEND_ENTITY,

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
	UNKNOWN,
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
	int id;
	EntityType type;
} EntityHeader;

typedef struct s_entity_location
{
	int map_id;
	sf::Vector2f position;
} EntityLocation;

typedef struct s_entity_view
{
	int animation;
} EntityView;

typedef struct s_entity_info
{
	EntityHeader info;
	EntityLocation location;
	EntityView view;
} EntityInfo;