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
		std::string title;
		int neighbours[4];
	} LevelInfo;

	class LevelCollection
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			static std::vector<LevelInfo*> levels;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			static void AddLevel(int id, std::string level, std::string music, std::string title, int neighbours[4]);
			static LevelInfo* GetLevel(int id);
			static LevelInfo* GetLevel(std::string mapfile);
			static std::string GetNeighbour(int level, Direction direction);
	};
}