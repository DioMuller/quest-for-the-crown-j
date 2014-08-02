#include "Scene.h"
#include "Game.h"
#include <memory>

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Scene::Scene(std::weak_ptr<Game> parent, bool allowStacking)
{
	this->parent = parent;
	this->allowStacking = allowStacking;
}


Scene::~Scene()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Scene::Update(double dt)
{
	AddRemoveEntities();


	// Updates entities.
	for (const auto& e : entities)
	{
		e->Update(dt);
	}
}

bool IsBehind(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
{
	return a->Sprite->Feet.y < b->Sprite->Feet.y;
}

void Scene::Draw(sf::RenderWindow* renderer)
{
	std::sort(entities.begin(), entities.end(), IsBehind);

	for (const auto& e : entities)
	{
		e->Draw(renderer);
	}
}

void Scene::AddEntity(std::shared_ptr<Entity> e)
{
	e->scene = getptr();
	toAdd.push(e);
}

void Scene::RemoveEntity(std::shared_ptr<Entity> e)
{
	toRemove.push(e);
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(std::string category)
{
	return GetEntities([category](const std::shared_ptr<Entity>& e){return e->category == category; });
}

std::shared_ptr<Entity> Scene::GetEntity(int id)
{
	auto ents = GetEntities([id](const std::shared_ptr<Entity>& e){return e->Id == id; });
	return ents.size() > 0? ents[0] : nullptr;
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(std::function<bool(const std::shared_ptr<Entity>&)> predicate)
{
	std::vector<std::shared_ptr<Entity>> bar;
	std::copy_if(entities.begin(), entities.end(), std::back_inserter(bar), predicate);
	return bar;
}

std::weak_ptr<Game> Scene::GetParent()
{
	return parent;
}

void Scene::OnResume() { }

void qfcbase::Scene::AddRemoveEntities()
{
	// Remove entities to be removed.
	while (!toRemove.empty())
	{
		auto e = toRemove.top();
		toRemove.pop();

		auto position = std::find(entities.begin(), entities.end(), e);
		if (position != entities.end()) entities.erase(position);
	}

	// Add entities to be added
	while (!toAdd.empty())
	{
		auto e = toAdd.top();
		toAdd.pop();

		entities.push_back(e);
	}
}
