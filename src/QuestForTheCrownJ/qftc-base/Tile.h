#pragma once

namespace qfcbase
{
	class Tileset;

	#define COLISION_POSITIONS 4

	enum CollisionPosition
	{
		UPPER_LEFT = 0,
		UPPER_RIGHT = 1,
		DOWN_LEFT = 2,
		DOWN_RIGHT = 3
	};

	class Tile
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			int id;
			Tileset* parent;
			int terrain[COLISION_POSITIONS];

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			int Id()
			{
				return this->id;
			}

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Tile(int id, Tileset* parent);
			~Tile();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			int GetCollision(CollisionPosition position);
			void SetCollision(CollisionPosition position, int value);
	};
}