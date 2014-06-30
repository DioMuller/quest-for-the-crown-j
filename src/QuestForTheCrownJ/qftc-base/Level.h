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
			void SetNeighbor(Direction direction, int neighborId);
	};


}