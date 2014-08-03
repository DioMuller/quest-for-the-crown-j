#include "LevelCollection.h"

using namespace qfcbase;

std::vector<std::shared_ptr<LevelInfo>> LevelCollection::levels = std::vector<std::shared_ptr<LevelInfo>>();

void LevelCollection::AddLevel(int id, std::string level, std::string music, std::string title, int neighbours[4])
{
	LevelInfo* info = new LevelInfo { id, level, music, title, { neighbours[0], neighbours[1], neighbours[2], neighbours[3] } };
	levels.push_back(std::shared_ptr<LevelInfo>(info));
}

std::shared_ptr<LevelInfo> LevelCollection::GetLevel(int id)
{
	for (auto level : levels)
	{
		if (level->levelId == id) return level;
	}

	return nullptr;
}

std::shared_ptr<LevelInfo> LevelCollection::GetLevel(std::string mapfile)
{
	for (auto level : levels)
	{
		if (level->mapFile == mapfile) return level;
	}

	return nullptr;
}

std::shared_ptr<LevelInfo> LevelCollection::GetNeighbour(int level, Direction direction)
{
	for (auto l : levels)
	{
		if (l->levelId == level)
		{
			return GetLevel(l->neighbours[(int)direction]);
		}
	}

	return nullptr;
}
