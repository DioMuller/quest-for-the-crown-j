#include "Level.h"

using namespace qfcbase;

Level::Level(Game* game, int id, Map* map, std::vector<Entity*> entities) : Scene(game)
{
}


Level::~Level()
{
}


/////////////////////////////////////
// Methods
/////////////////////////////////////
void Level::Update(double dt)
{
	Scene::Update(dt);
}

void Level::Draw(sf::RenderWindow* renderer)
{
	// TODO: Replace camera vector.
	map->Draw(renderer, sf::Vector2i(0,0));
	Scene::Draw(renderer);
}

void Level::GoToNeighbour(Entity* entity, Direction direction)
{

}

void Level::GoToDungeon(Entity* entity, int dungeon)
{

}

void Level::SetNeighbor(Direction direction, int neighborId)
{
	this->neighbors[(int)direction] = neighborId;
}