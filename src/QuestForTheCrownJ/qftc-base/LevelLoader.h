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
		// Attributes
		/////////////////////////////////////
		private:
			static std::shared_ptr<EntityFactory> factory;
		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			static std::shared_ptr<LevelCollection> LoadLevels(Game* game, std::string path);
			static Level* LoadMap(Game* game, int id, std::string tmxFile);
			static void SetFactory(EntityFactory* factory);
			static std::shared_ptr<Entity> CreateEntity(Scene* scene, int levelId, tmx::MapObject object);
	};
}
