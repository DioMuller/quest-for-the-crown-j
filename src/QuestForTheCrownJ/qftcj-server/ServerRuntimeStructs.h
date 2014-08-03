#pragma once
#include "Entity.h"

typedef struct s_logged_user
{
	long long user_id;
	long long player_id;
	int map_id;
	std::shared_ptr<qfcbase::Entity> game_entity;
	std::shared_ptr<sockaddr_in> address;
	int address_size;
	// last command date time
} LoggedUser;
