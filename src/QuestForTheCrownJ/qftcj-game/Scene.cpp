#include "Scene.h"
#include "Game.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Scene::Scene(Game* parent)
{
	this->parent = parent;
}


Scene::~Scene()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Scene::Update(double dt)
{
	// Remove entities to be removed.
	while (!toRemove.empty())
	{
		Entity* e = toRemove.top();
		toRemove.pop();

		std::vector<Entity*>::iterator position = std::find(entities.begin(), entities.end(), e);
		if (position != entities.end()) entities.erase(position);

		delete e;
	}

	// Updates entities.
	for (Entity* e : entities)
	{
		e->Update(dt);
	}
}

void Scene::Draw(sf::RenderWindow* renderer)
{
	for (Entity* e : entities)
	{
		e->Draw(renderer);
	}
}

void Scene::AddEntity(Entity* e)
{
	entities.push_back(e);
}

void Scene::RemoveEntity(Entity* e)
{
	toRemove.push(e);
}