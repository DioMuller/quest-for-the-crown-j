#pragma once
#include <vector>
#include <memory>
#include "Level.h"

namespace qfcbase
{
	typedef struct s_waypoint
	{
		int levelId;
		sf::Vector2i position;
	} Waypoint;

	class LevelCollection
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::vector<std::shared_ptr<Level>> levels;
			std::map<std::shared_ptr<Entity>, std::vector<Waypoint>> storedWaypoints;
			std::shared_ptr<Entity> currentPlayer;

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			std::shared_ptr<Level> CurrentLevel()
			{
				// TODO: Get levels with players.
			}
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			LevelCollection();
			~LevelCollection();
	};
}