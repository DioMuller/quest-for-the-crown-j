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

	typedef struct sTile
	{
		int id;
		Tileset* parent;
		int terrain[COLISION_POSITIONS];

	} Tile;
}