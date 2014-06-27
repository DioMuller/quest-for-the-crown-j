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
		auto e = toRemove.top();
		toRemove.pop();

		auto position = std::find(entities.begin(), entities.end(), e);
		if (position != entities.end()) entities.erase(position);
	}

	// Updates entities.
	for (const auto& e : entities)
	{
		e->Update(dt);
	}
}

void Scene::Draw(sf::RenderWindow* renderer)
{
	for (const auto& e : entities)
	{
		e->Draw(renderer);
	}
}

void Scene::AddEntity(std::shared_ptr<Entity> e)
{
	e->scene = this;
	entities.push_back(e);
}

void Scene::RemoveEntity(std::shared_ptr<Entity> e)
{
	toRemove.push(e);
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(std::string category)
{
	return GetEntities([category](const std::shared_ptr<Entity>& e){return e->category == category; });
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(std::function<bool(const std::shared_ptr<Entity>&)> predicate)
{
	std::vector<std::shared_ptr<Entity>> bar;
	auto it = std::copy_if(entities.begin(), entities.end(), std::back_inserter(bar), predicate);
	return bar;
}