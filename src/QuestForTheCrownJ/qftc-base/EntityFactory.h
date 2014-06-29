#pragma once
#include <memory>
#include "Entity.h"

namespace qfcbase
{
	class EntityFactory
	{
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			EntityFactory();
			~EntityFactory();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual std::shared_ptr<Entity> GenerateEntity(std::string name);
	};
}
