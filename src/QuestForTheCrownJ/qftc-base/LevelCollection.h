#pragma once
#include <vector>
#include <memory>
#include "Level.h"

namespace qfcbase
{
	typedef struct s_level_info
	{
		int levelId;
		std::string mapFile;
		std::string music;
		std::string background;
		std::string title;
		int neighbours[4];
	} LevelInfo;

	class LevelCollection
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			static std::vector<std::shared_ptr<LevelInfo>> levels;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			static void AddLevel(int id, std::string level, std::string music, std::string background, std::string title, int neighbours[4]);
			static std::shared_ptr<LevelInfo> GetLevel(int id);
			static std::shared_ptr<LevelInfo> GetLevel(std::string mapfile);
			static std::shared_ptr<LevelInfo> GetNeighbour(int level, Direction direction);
	};
}