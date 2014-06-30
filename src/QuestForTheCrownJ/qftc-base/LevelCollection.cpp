#include "LevelCollection.h"

using namespace qfcbase;

LevelCollection::LevelCollection()
{
}


LevelCollection::~LevelCollection()
{
}

void LevelCollection::AddLevel(std::shared_ptr<Level> level)
{
	levels.push_back(level);
}