#include "LevelLoader.h"
#include "StringHelper.h"
#include "sfml-tmxloader/MapLoader.h"
#include "Log.h"

using namespace qfcbase;

std::shared_ptr<EntityFactory> LevelLoader::factory = nullptr;

std::shared_ptr<LevelCollection> LevelLoader::LoadLevels(Game* game, std::string path)
{
	std::shared_ptr<LevelCollection> collection(new LevelCollection());
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* root = doc.FirstChildElement("collection");

	tinyxml2::XMLElement* next = root->FirstChildElement("levels")->FirstChildElement("level");

	while( next != nullptr )
	{
		
		int id = next->IntAttribute("id");

		std::shared_ptr<Level> level = std::shared_ptr<Level>(LoadMap(game, id, std::string(next->Attribute("path"))));
		level->BGM(next->Attribute("music"));
		level->Title(next->Attribute("title"));

		auto splitedNeighbours = split(std::string(next->Attribute("neighbors")), ',');
		for (int i = 0; i < NEIGHBOUR_COUNT; i++)
		{
			level->SetNeighbor((Direction) i, std::stoi(splitedNeighbours[i]));
		}

		collection->AddLevel(level);
		next = next->NextSiblingElement("level");
	}

	return collection;
}

Level* LevelLoader::LoadMap(Game* game, int id, std::string tmxFile)
{
	tmx::MapLoader* map = new tmx::MapLoader(tmxFile.substr(0, tmxFile.find_last_of("\\/")));
	map->AddSearchPath("Content/tilesets/"); // For tsx files.
	map->AddSearchPath("Content/tiles/"); // For png files.
	map->Load(tmxFile.substr(tmxFile.find_last_of("\\/"), tmxFile.length()));
	Level* level = new Level(game, id, map);

	auto layers = map->GetLayers();

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