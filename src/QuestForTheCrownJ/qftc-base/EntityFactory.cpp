#include "EntityFactory.h"

using namespace qfcbase;

EntityFactory::EntityFactory()
{
}


EntityFactory::~EntityFactory()
{
}

std::shared_ptr<Entity> EntityFactory::GenerateEntity(std::weak_ptr<Scene> scene, std::string name, tmx::MapObject object)
{
	return nullptr;
}