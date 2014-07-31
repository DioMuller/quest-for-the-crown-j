#pragma once
#include <memory>
#include "Entity.h"
#include "EntityFactory.h"
namespace qfcserver
{
	class ServerEntityFactory :
		public qfcbase::EntityFactory
	{
	public:
		ServerEntityFactory();
		~ServerEntityFactory();
	public:
		std::shared_ptr<qfcbase::Entity> GenerateEntity(std::weak_ptr<qfcbase::Scene> scene, std::string name, tmx::MapObject object);
	};
}
