#pragma once
#include "Scene.h"
#include "Map.h"

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

	class Level : Scene
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			int id;
			bool bgmStarted = false;
			Map* map;
			int neighbors[NEIGHBOUR_COUNT];
			std::vector<Entity*> players;
			LevelCollection* collectionParent;
			std::string bgm;
			std::string title;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Level(Game* game, int id, Map* map, std::vector<Entity*> entities = std::vector<Entity*>());
			~Level();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
			void Draw(sf::RenderWindow* renderer);
			void GoToNeighbour(Entity* entity, Direction direction);
			void GoToDungeon(Entity* entity, int dungeon);
	};


}