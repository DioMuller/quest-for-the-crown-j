#pragma once
#include "EntityFactory.h"

namespace qfcgame
{
	class GameEntityFactory : public qfcbase::EntityFactory
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			bool hasPlayer;
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