#pragma once
#include <memory>
#include "sfml-tmxloader/MapObject.h"
#include "Entity.h"
#include "Scene.h"

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
			virtual std::shared_ptr<Entity> GenerateEntity(std::weak_ptr<Scene> scene, std::string name, tmx::MapObject object);
	};
}
