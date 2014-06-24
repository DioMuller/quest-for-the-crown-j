#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

namespace qfcbase
{
	class Tileset
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			int firstGID;
			std::string name;
			sf::Vector2i tileSize;
			std::string source;
			sf::Vector2i size;
			std::vector<Tile*> tiles;
			int tileCount;
			int numRows;
			int numCols;
			sf::Texture texture;
			sf::Sprite sprite;

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			int FirstTileId()
			{
				return firstGID;
			}

			int LastTileId()
			{
				return firstGID + (numCols * numRows);
			}

			std::vector<Tile*> Tiles()
			{
				return tiles;
			}

			sf::Sprite Sprite()
			{
				return sprite;
			}
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Tileset(int firstgid, std::string name, sf::Vector2i tileSize, std::string imageSource, sf::Vector2i imageSize);
			~Tileset();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			sf::IntRect GetRect(int tileId);
			Tile* GetTileById(int tileId);
	};
}
