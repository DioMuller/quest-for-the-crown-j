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

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			LevelCollection();
			~LevelCollection();
	};
}