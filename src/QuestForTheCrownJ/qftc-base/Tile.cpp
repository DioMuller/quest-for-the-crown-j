#include "Tile.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Tile::Tile(int id, Tileset* parent)
{
	this->id = id;
	this->parent = parent;
	for (int i = 0; i < COLISION_POSITIONS; i++ ) this->terrain[i] = 0;
}

Tile::~Tile()
{
	
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
int Tile::GetCollision(CollisionPosition position)
{
	return terrain[(int)position];
}

void Tile::SetCollision(CollisionPosition position, int value)
{
	terrain[(int)position] = value;
}