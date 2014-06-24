#include "Map.h"
#include <math.h>

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Map::Map(std::string name, sf::Vector2i size, sf::Vector2i tileSize)
{
	this->name = name;
	this->size = size;
	this->tileSize = tileSize;
	this->pixelSize = sf::Vector2i(tileSize.x * size.x, tileSize.y * size.y);

	collisionMap = new int*[size.x * 2];
	for (int i = 0; i < size.x * 2; i++) collisionMap[i] = new int[size.y * 2];
}


Map::~Map()
{
	delete[] collisionMap;

	//TODO: Delete other pointer contents.
}

/////////////////////////////////////
// Methods
/////////////////////////////////////

// TODO: Finish this WIP.
bool Map::Collides(sf::IntRect rect, bool allowOutside)
{
	if (!allowOutside && IsOutsideBorders(rect)) return true;

	int mod_x = tileSize.x / 2;
	int mod_y = tileSize.y / 2;

	float min_x_real = (float)rect.left / mod_x;
	float min_y_real = (float)rect.top / mod_y;

	int min_x = (int) fmax(min_x_real, 0);
	int min_y = (int) fmax(min_y_real, 0);

	int max_x = min_x + (int)ceil((float) rect.width / mod_x + (min_x_real - min_x));
	int max_y = min_y + (int)ceil((float)rect.height / mod_y + (min_y_real - min_y));

	for (int x = min_x; x < max_x && x < size.x * 2; x++)
	{
		for (int y = min_y; y < max_y && y < size.y * 2; y++)
		{
			if (collisionMap[x][y] > 0) return true;
		}
	}

	return false;
}

bool Map::IsOutsideBorders(sf::IntRect rect)
{
	return (rect.left < 0 || rect.top < 0 || rect.left + rect.width > pixelSize.x || rect.top + rect.height > pixelSize.y);
}

void Map::Draw(sf::RenderWindow* window, sf::Vector2i camera)
{
	for (Layer* layer : layers)
	{
		for (int y = 0; y < layer->Size().y; y++)
		{
			for (int x = 0; x < layer->Size().x; x++)
			{
				int tileId = layer->GetData(x, y);

				if (tileId != 0)
				{
					Tile* tile = tileDictionary[tileId];
					sf::Sprite sprite = tile->Parent()->Sprite();

					sprite.setTextureRect(tile->Parent()->GetRect(tileId));
					sprite.setPosition(x * tileSize.x - camera.x, y * tileSize.y * camera.y);
					window->draw(sprite);
				}
			}
		}
	}
}

Tile* Map::GetTile(int tileId)
{
	if (tileId == 0) return nullptr;

	Tileset* set = GetTileset(tileId);

	if (set == nullptr) return nullptr;

	return set->GetTileById(tileId);
}

Tileset* Map::GetTileset(int tileId)
{
	for (Tileset* ts : tilesets)
	{
		if (ts->FirstTileId() <= tileId && ts->LastTileId() >= tileId) return ts;
	}

	return nullptr;
}

void Map::UpdateCollision()
{
	for (int y = 0; y < size.y * 2; y += 2)
	{
		for (int x = 0; x < size.x * 2; x += 2)
		{
			for (Layer* layer : layers)
			{
				int tileId = layer->GetData(x / 2, y / 2);

				if (tileId != 0)
				{
					Tile* tile = GetTile(tileId);

					collisionMap[x][y] = (int)fmax(tile->GetCollision(CollisionPosition::UPPER_LEFT), collisionMap[x][y]);
					collisionMap[x + 1][y] = (int)fmax(tile->GetCollision(CollisionPosition::UPPER_RIGHT), collisionMap[x + 1][y]);
					collisionMap[x][y + 1] = (int)fmax(tile->GetCollision(CollisionPosition::DOWN_LEFT), collisionMap[x][y + 1]);
					collisionMap[x + 1][y + 1] = (int)fmax(tile->GetCollision(CollisionPosition::DOWN_RIGHT), collisionMap[x + 1][y + 1]);
				}
			}
		}
	}
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