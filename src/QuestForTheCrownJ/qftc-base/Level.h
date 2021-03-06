#pragma once
#include "Scene.h"
#include "EntityFactory.h"
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
			std::shared_ptr<tmx::MapLoader> map;
			int neighbors[NEIGHBOUR_COUNT];
			std::vector<Entity*> players;
			LevelCollection* collectionParent;
			std::string bgm;
			std::string battleBackground;
			std::string title;
			bool playBGM;

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

			void Id(int id)
			{
				this->id = id;
			}

			int Id()
			{
				return this->id;
			}

			void Title(std::string value)
			{
				title = value;
			}

			std::string Title()
			{
				return title;
			}

			std::shared_ptr<tmx::MapLoader> Map()
			{
				return map;
			}

			void BattleBackground(std::string value)
			{
				this->battleBackground = value;
			}

			std::string BattleBackground()
			{
				return this->battleBackground;
			}

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Level(std::weak_ptr<Game> game, int id, bool playBGM = true);
			~Level();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
			void Draw(sf::RenderWindow* renderer);

			void GoToNeighbour(std::shared_ptr<Entity> entity, Direction direction);
			void SetNeighbor(Direction direction, int neighborId);
			void LoadMap(std::string tmxfile);
			void OnResume();
	};


}