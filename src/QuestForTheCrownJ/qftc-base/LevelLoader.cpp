#include "LevelLoader.h"
#include "StringHelper.h"
#include "sfml-tmxloader/MapLoader.h"
#include "Log.h"

using namespace qfcbase;

std::shared_ptr<EntityFactory> LevelLoader::factory = nullptr;

void LevelLoader::LoadLevels(std::string path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* root = doc.FirstChildElement("collection");

	tinyxml2::XMLElement* next = root->FirstChildElement("levels")->FirstChildElement("level");

	while( next != nullptr )
	{
		int id = next->IntAttribute("id");
		std::string path = next->Attribute("path");
		std::string bgm = next->Attribute("music");
		std::string title = next->Attribute("title");

		auto splitedNeighbours = split(std::string(next->Attribute("neighbors")), ',');
		int neigh[4];
		for (int i = 0; i < NEIGHBOUR_COUNT; i++)
		{
			neigh[(Direction)i] = std::stoi(splitedNeighbours[i]);
		}

		LevelCollection::AddLevel(id, path, bgm, title, neigh);
		next = next->NextSiblingElement("level");
	}
}

Level* LevelLoader::LoadMap(Game* game, int id, std::string tmxFile)
{
	Level* level = new Level(game, id);
	level->LoadMap(tmxFile);

	auto layers = level->Map()->GetLayers();

	for (auto layer : layers)
	{
		if (layer.type == tmx::MapLayerType::ObjectGroup)
		{
			for (auto object : layer.objects)
			{
				std::shared_ptr<Entity> entity = CreateEntity(level, id, object);
				if (entity != nullptr) level->AddEntity(entity);
			}
		}
	}

	return level;
}

void LevelLoader::SetFactory(EntityFactory* factory)
{
	LevelLoader::factory = std::shared_ptr<EntityFactory>(factory);
}

std::shared_ptr<Entity> LevelLoader::CreateEntity(Scene* scene, int levelId, tmx::MapObject object)
{
	if (factory == nullptr)
	{
		Log::Warning("EntityFactory was not set. Entity " + object.GetName() + " could not be created.");
		return nullptr;
	}

	return factory->GenerateEntity(scene, object.GetType(), object);
}