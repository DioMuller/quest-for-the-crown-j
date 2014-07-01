#pragma once
#include <memory>
#include "LevelCollection.h"
#include "EntityFactory.h"
#include "tinyxml2/tinyxml2.h"
#include "Game.h"

namespace qfcbase
{
	class LevelLoader
	{
		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			static std::shared_ptr<LevelCollection> LoadLevels(Game* game, std::string path);
			static Level* LoadMap(Game* game, int id, std::string tmxFile);
			static std::shared_ptr<Entity> CreateEntity(EntityFactory* factory, int levelId, tinyxml2::XMLElement* node);			
	};
}
