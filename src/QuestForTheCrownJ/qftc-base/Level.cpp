#include "Level.h"

using namespace qfcbase;

Level::Level(Game* game, int id, tmx::MapLoader* map, std::vector<Entity*> entities) : Scene(game)
{
	this->map = map;
}


Level::~Level()
{
	delete map;
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
	renderer->draw(*map);
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