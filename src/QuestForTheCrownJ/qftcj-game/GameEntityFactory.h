#pragma once
#include "EntityFactory.h"

namespace qfcgame
{
	class GameEntityFactory : public qfcbase::EntityFactory
	{
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
			std::shared_ptr<qfcbase::Entity> GenerateEntity(qfcbase::Scene* scene, std::string type, tmx::MapObject object);
	};
}