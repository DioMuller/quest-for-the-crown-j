#include "LevelCollection.h"

using namespace qfcbase;

std::vector<LevelInfo*> LevelCollection::levels = std::vector<LevelInfo*>();

void LevelCollection::AddLevel(int id, std::string level, std::string music, std::string title, int neighbours[4])
{
	levels.push_back(new LevelInfo{ id, level, music, title, { neighbours[0], neighbours[1], neighbours[2], neighbours[3] } });
}

LevelInfo* LevelCollection::GetLevel(int id)
{
	for (auto level : levels)
	{
		if (level->levelId == id) return level;
	}

	return nullptr;
}

LevelInfo* LevelCollection::GetLevel(std::string mapfile)
{
	for (auto level : levels)
	{
		if (level->mapFile == mapfile) return level;
	}

	return nullptr;
}

std::string LevelCollection::GetNeighbour(int level, Direction direction)
{
	for (auto l : levels)
	{
		if (l->levelId == level)
		{
			return GetLevel(l->neighbours[(int)direction])->mapFile;
		}
	}

	return "";
}
