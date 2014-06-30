#include "LevelLoader.h"
#include "StringHelper.h"


using namespace qfcbase;

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

		std::shared_ptr<Level> level = LoadMap(id, std::string(next->Attribute("path")));
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

std::shared_ptr<Level> LevelLoader::LoadMap(int id, std::string tmxFile)
{
	std::shared_ptr<Map> map = nullptr;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(tmxFile.c_str());

	// Create map
	tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
	std::string name = (std::string) (*split(tmxFile, '/').end());
	replace(name, std::string(".tmx"), std::string(""));
	sf::Vector2i size = sf::Vector2i(mapElement->IntAttribute("width"), mapElement->IntAttribute("height"));
	sf::Vector2i tileSize = sf::Vector2i(mapElement->IntAttribute("tilewidth"), mapElement->IntAttribute("tileheight"));

	map = std::shared_ptr<Map>(new Map(name, size, tileSize));

	// Tilesets
	tinyxml2::XMLElement* next = mapElement->FirstChildElement("tileset");

	while (next != nullptr)
	{
		std::shared_ptr<Tileset> tileset;
		int firstgid = next->IntAttribute("firstgid"); // replace ("../","") ?

		if (next->Attribute("source") == nullptr)
		{
			std::string tilename = next->Attribute("name");
			sf::Vector2i tileSize = sf::Vector2i(next->IntAttribute("tilewidth"), next->IntAttribute("tileheight"));
		}

		std::string tilename = "";
		next = next->NextSiblingElement("tileset");
	}

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