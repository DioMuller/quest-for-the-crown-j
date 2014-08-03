#include "EntityFactory.h"
#include "FollowBehavior.h"
#include "Hero.h"
#include "Slime.h"

using namespace qfcbase;

EntityFactory::EntityFactory()
	: _last_entity_id(0)
{
}


EntityFactory::~EntityFactory()
{
}

int EntityFactory::GenerateId()
{
	return ++_last_entity_id;
}

std::shared_ptr<Entity> EntityFactory::GenerateEntity(std::weak_ptr<Scene> scene, std::string name, tmx::MapObject object)
{
	std::shared_ptr<qfcbase::Entity> entity;

	if (name == "Enemy")
	{
		entity = std::shared_ptr<qfcbase::Entity>(new Slime());
		entity->AddBehavior(std::make_shared<FollowBehavior>(entity, "GoodGuy", 5, 32 * 4));
		entity->category = "Enemy";
		entity->scene = scene;
	}
	else if (name == "Player")
	{
		entity = std::shared_ptr<qfcbase::Entity>(new Hero());
		entity->category = "GoodGuy";
		entity->scene = scene;
	}
	else
	{
		return nullptr;
	}

	entity->Id = GenerateId();
	entity->Sprite->Position = object.GetPosition();
	return entity;
}