#include "LevelLoader.h"
#include "StringHelper.h"


using namespace qfcbase;

std::shared_ptr<LevelCollection> LevelLoader::LoadLevels(Game* game, std::string path)
{
	std::shared_ptr<LevelCollection> collection(new LevelCollection());
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());

	tinyxml2::XMLElement* root = doc.FirstChildElement("collection");

	tinyxml2::XMLElement* next = root->FirstChild()->ToElement();

	while( next != nullptr )
	{
		
		int id = next->IntAttribute("id");

		std::shared_ptr<Level> level = LoadMap(id, std::string(next->Attribute("path")));
		level->BGM(next->Attribute("music"));
		level->Title(next->Attribute("title"));

		auto splitedNeighbours = split(std::string(next->Attribute("neighbors")), ',');
		for (int i = 0; i < NEIGHBOUR_COUNT; i++)
		{
			level->SetNeighbor((Direction) i, std::stoi(splitedNeighbours[i]));
		}

		collection->AddLevel(level);
		next = next->NextSibling()->ToElement();
	}

	return collection;
}

std::shared_ptr<Level> LevelLoader::LoadMap(int id, std::string tmxFile)
{
	return nullptr;
}

std::shared_ptr<Tileset> LevelLoader::LoadTileset(int firstgid, std::string tsxFile)
{
	return nullptr;
}

std::shared_ptr<Entity> LevelLoader::CreateEntity(EntityFactory* factory, int levelId, tinyxml2::XMLElement* node)
{
	return nullptr;
}