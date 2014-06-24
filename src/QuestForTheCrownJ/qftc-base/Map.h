#pragma once
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Tileset.h"
#include "Layer.h"

namespace qfcbase
{
	class Map
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			int** collisionMap;
			std::map<int, Tile*> tileDictionary;

			std::string name;
			sf::Vector2i size;
			sf::Vector2i tileSize;
			sf::Vector2i pixelSize;
			std::vector<Tileset*> tilesets;
			std::vector<Layer*> layers;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Map(std::string name, sf::Vector2i size, sf::Vector2i tileSize);
			~Map();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			bool Collides(sf::IntRect rect, bool allowOutside = false);
			bool IsOutsideBorders(sf::IntRect rect);
			void Draw(sf::RenderWindow render, sf::Vector2i camera);
		private:
			Tile* GetTile(int tileId);
			Tileset* GetTileset(int tileId);
		private:
			void UpdateCollision();
			void UpdateTilesets();
	};
}