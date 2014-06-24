#include "Map.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Map::Map(std::string name, sf::Vector2i size, sf::Vector2i tileSize)
{
}


Map::~Map()
{
	//TODO: Delete pointers contents.
}

/////////////////////////////////////
// Methods
/////////////////////////////////////

// TODO: Finish this WIP.
bool Map::Collides(sf::IntRect rect, bool allowOutside)
{
	return false;
}

bool Map::IsOutsideBorders(sf::IntRect rect)
{
	return false;
}

void Map::Draw(sf::RenderWindow render, sf::Vector2i camera)
{

}

Tile* Map::GetTile(int tileId)
{
	return nullptr;
}

Tileset* Map::GetTileset(int tileId)
{
	return nullptr;
}

void Map::UpdateCollision()
{

}

void Map::UpdateTilesets()
{
	tileDictionary.empty();

	for (Tileset* ts : tilesets)
	{
		for (Tile* tile : ts->Tiles())
		{
			tileDictionary[tile->Id() + ts->FirstTileId()] = tile;
		}
	}
}