#pragma once
#include "EntityFactory.h"

namespace qfcbase
{
	class GameEntityFactory : public qfcbase::EntityFactory
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		public:
			static std::shared_ptr<Entity> Player;
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			GameEntityFactory();
			~GameEntityFactory();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			std::shared_ptr<qfcbase::Entity> GenerateEntity(std::weak_ptr<qfcbase::Scene> scene, std::string type, tmx::MapObject object);
	};
}