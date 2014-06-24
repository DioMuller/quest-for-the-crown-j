#include "Tileset.h"
#include "Log.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Tileset::Tileset(int firstgid, std::string name, sf::Vector2i tileSize, std::string imageSource, sf::Vector2i imageSize)
{
	this->firstGID = firstgid;
	this->name = name;
	this->tileSize = tileSize;
	this->source = imageSource;
	this->size = imageSize;

	if (!texture.loadFromFile(imageSource))
	{
		Log::Error("Error loading tileset texture file: " + imageSource);
		sprite.setTexture(texture);
	}
	else
	{
		numRows = size.y / tileSize.y;
		numCols = size.x / tileSize.x;

		tileCount = numRows * numCols;
		tiles.empty();

		for (int i = 0; i < numRows * numCols; i++)
		{
			tiles.push_back(new Tile{ i, this });
		}
	}
}


Tileset::~Tileset()
{
	// TODO: Delete all tiles and release textures.
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
sf::IntRect Tileset::GetRect(int tileId)
{
	tileId -= FirstTileId();

	if (tileId < 0)
		return sf::IntRect(0, 0, 0, 0);

	int row = tileId / numCols;

	if ( row >= numRows )
		return sf::IntRect(0, 0, 0, 0);

	int col = tileId % numCols;

	return sf::IntRect(col * tileSize.x, row*tileSize.y, tileSize.x, tileSize.y);
}

Tile* Tileset::GetTileById(int tileId)
{
	tileId -= firstGID;

	for (int i = 0; i < tileCount; i++)
	{
		if (tiles[i]->Id() == tileId) return tiles[i];
	}

	return nullptr;
}