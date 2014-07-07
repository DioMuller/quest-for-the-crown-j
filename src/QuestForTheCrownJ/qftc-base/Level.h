#pragma once
#include "Scene.h"
#include "sfml-tmxloader/MapLoader.h"

namespace qfcbase
{
	class LevelCollection;

	#define NEIGHBOUR_COUNT 4

	enum Direction
	{
		WEST = 0,
		NORTH = 1,
		EAST = 2,
		SOUTH = 3,
		NONE = -1
	};

	class Level : public Scene
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			int id;
			bool bgmStarted = false;
			tmx::MapLoader* map;
			int neighbors[NEIGHBOUR_COUNT];
			std::vector<Entity*> players;
			LevelCollection* collectionParent;
			std::string bgm;
			std::string title;

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			void BGM(std::string value)
			{
				bgm = value;
			}

			std::string BGM()
			{
				return bgm;
			}

			void Title(std::string value)
			{
				title = value;
			}

			std::string Title()
			{
				return title;
			}

			tmx::MapLoader* Map()
			{
				return map;
			}
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Level(Game* game, int id, tmx::MapLoader* map, std::vector<Entity*> entities = std::vector<Entity*>());
			~Level();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
			void Draw(sf::RenderWindow* renderer);
			void GoToNeighbour(Entity* entity, Direction direction);
			void GoToDungeon(Entity* entity, int dungeon);
			void SetNeighbor(Direction direction, int neighborId);
	};


}