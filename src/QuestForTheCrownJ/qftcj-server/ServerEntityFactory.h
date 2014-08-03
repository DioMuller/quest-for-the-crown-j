#pragma once
#include <memory>
#include "ServerChannel.h"
#include "Entity.h"
#include "EntityFactory.h"

namespace qfcserver
{
	class ServerEntityFactory :
		public qfcbase::EntityFactory
	{
	private:
		std::weak_ptr<qfcnet::ServerChannel> channel;

	public:
		ServerEntityFactory(std::weak_ptr<qfcnet::ServerChannel> channel);
		~ServerEntityFactory();
	public:
		std::shared_ptr<qfcbase::Entity> GenerateEntity(std::weak_ptr<qfcbase::Scene> scene, std::string name, tmx::MapObject object);
	};
}
