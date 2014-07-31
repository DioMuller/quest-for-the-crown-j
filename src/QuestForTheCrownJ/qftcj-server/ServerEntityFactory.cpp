#include "ServerEntityFactory.h"

using namespace qfcserver;
using namespace qfcbase;


ServerEntityFactory::ServerEntityFactory()
{
}


ServerEntityFactory::~ServerEntityFactory()
{
}

std::shared_ptr<Entity> ServerEntityFactory::GenerateEntity(std::weak_ptr<Scene> scene, std::string name, tmx::MapObject object)
{
	if (name == "Player")
		return nullptr;
	return EntityFactory::GenerateEntity(scene, name, object);
}
