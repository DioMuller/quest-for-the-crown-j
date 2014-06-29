#include "LevelLoader.h"

using namespace qfcbase;

std::shared_ptr<LevelCollection> LevelLoader::LoadLevels(std::string path)
{
	return nullptr;
}

std::shared_ptr<Level> LevelLoader::LoadMap(int id, std::string tmxFile)
{
	return nullptr;
}

std::shared_ptr<Tileset> LevelLoader::LoadTileset(int firstgid, std::string tsxFile)
{
	return nullptr;
}

std::shared_ptr<Entity> LevelLoader::CreateEntity(EntityFactory* factory, int levelId)
{
	return nullptr;
}