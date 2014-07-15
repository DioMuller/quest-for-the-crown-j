#include "Level.h"

using namespace qfcbase;

Level::Level(Game* game, int id, std::vector<Entity*> entities) : Scene(game)
{
	this->map = nullptr;
}


Level::~Level()
{
	if(map) delete map;
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

void Level::LoadMap(std::string tmxFile)
{
	if (this->map) delete this->map;

	this->map = new tmx::MapLoader(tmxFile.substr(0, tmxFile.find_last_of("\\/")));
	this->map->AddSearchPath("Content/tilesets/"); // For tsx files.
	this->map->AddSearchPath("Content/tiles/"); // For png files.
	this->map->Load(tmxFile.substr(tmxFile.find_last_of("\\/"), tmxFile.length()));
}