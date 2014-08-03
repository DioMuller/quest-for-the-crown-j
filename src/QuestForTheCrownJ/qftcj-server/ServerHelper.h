#pragma once
#include <memory>

#include "StructBase.h"
#include "Hero.h"
#include "Slime.h"

static class ServerHelper
{
public:
	static EntityType GetEntityType(std::shared_ptr<qfcbase::Entity> e)
	{
		EntityType type;
		if (std::dynamic_pointer_cast<qfcbase::Hero>(e))
			type = ENTITY_HERO;
		else if (std::dynamic_pointer_cast<qfcbase::Slime>(e))
			type = ENTITY_SLIME;
		return type;
	}
};
