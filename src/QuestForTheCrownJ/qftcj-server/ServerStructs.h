#pragma once
#include "Entity.h"

typedef struct s_logged_user
{
	long long user_id;
	long long player_id;
	std::string map_name;
	std::shared_ptr<qfcbase::Entity> game_entity;
	// last command date time
} LoggedUser;
