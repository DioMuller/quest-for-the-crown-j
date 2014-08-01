#pragma once
#include "Entity.h"

typedef struct s_logged_user
{
	long long user_id;
	long long player_id;
	std::string map_file;
	std::shared_ptr<qfcbase::Entity> game_entity;
	sockaddr_in address;
	int address_size;
	// last command date time
} LoggedUser;
