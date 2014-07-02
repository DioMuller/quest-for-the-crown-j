#pragma once
#include <memory>
#include "sfml-tmxloader/MapObject.h"
#include "Entity.h"

namespace qfcbase
{
	class EntityFactory
	{
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			EntityFactory();
			~EntityFactory();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual std::shared_ptr<Entity> GenerateEntity(std::string name, tmx::MapObject object);
	};
}
