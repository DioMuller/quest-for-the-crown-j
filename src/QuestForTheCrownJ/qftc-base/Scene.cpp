#include "Scene.h"
#include "Game.h"
#include <memory>

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Scene::Scene(std::weak_ptr<Game> parent, bool allowStacking)
	: entities_mutex()
{
	this->parent = parent;
	this->allowStacking = allowStacking;
	abortScene = false;
}


Scene::~Scene()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Scene::Update(double dt)
{
	for (const auto& e : GetEntities())
	{
		e->Update(dt);
		
		if (abortScene)
		{
			abortScene = false;
			break;
		}
	}
}

bool IsBehind(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
{
	return a->Sprite->Feet.y < b->Sprite->Feet.y;
}

void Scene::Draw(sf::RenderWindow* renderer)
{
	SortEntities();

	for (const auto& e : GetEntities())
	{
		e->Draw(renderer);
	}
}

void Scene::SortEntities()
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	std::sort(entities.begin(), entities.end(), IsBehind);
}

void Scene::AddEntity(std::shared_ptr<Entity> e)
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	e->scene = getptr();
	entities.push_back(e);
}

void Scene::RemoveEntity(std::shared_ptr<Entity> e)
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	auto position = std::find(entities.begin(), entities.end(), e);
	if (position != entities.end()) entities.erase(position);
}

void Scene::RemoveAllEntities()
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	entities.clear();
}

bool Scene::IsFinished()
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	return entities.size() < 2;
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities()
{
	return GetEntities([](const std::shared_ptr<Entity>& e) { return true; });
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(std::string category)
{
	return GetEntities([category](const std::shared_ptr<Entity>& e){return e->category == category; });
}

std::shared_ptr<Entity> Scene::GetEntity(int id)
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		auto ent = entities[i];
		if (ent && ent->Id == id)
			return ent;
	}
	return nullptr;
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(std::function<bool(const std::shared_ptr<Entity>&)> predicate)
{
	std::lock_guard<std::mutex> lock(entities_mutex);
	std::vector<std::shared_ptr<Entity>> bar;
	std::copy_if(entities.begin(), entities.end(), std::back_inserter(bar), predicate);
	return bar;
}

std::weak_ptr<Game> Scene::GetParent()
{
	return parent;
}

void Scene::OnResume() { }

bool qfcbase::Scene::UpdateAborted()
{
	return abortScene;
}

void qfcbase::Scene::AbortUpdate()
{
	abortScene = true;
}
