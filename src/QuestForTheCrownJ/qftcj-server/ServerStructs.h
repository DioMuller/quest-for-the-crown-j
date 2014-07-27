#pragma once
#include "Entity.h"

typedef struct s_logged_user
{
	int user_id;
	int player_id;
	std::string map_name;
	std::shared_ptr<qfcbase::Entity> game_entity;
	// last command date time
} LoggedUser;