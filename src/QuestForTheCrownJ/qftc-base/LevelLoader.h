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
			static void LoadLevels(std::string path);
			static std::shared_ptr<Level> LoadMap(std::weak_ptr<Game> game, int id, std::string tmxFile);
			static void SetFactory(std::shared_ptr<EntityFactory> factory);
			static std::shared_ptr<EntityFactory> GetFactory();
			static std::shared_ptr<Entity> CreateEntity(std::weak_ptr<Scene> scene, int levelId, tmx::MapObject object);
	};
}
