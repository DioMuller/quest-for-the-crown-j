#pragma once
#include "Entity.h"

namespace qfcbase
{
    class Hero :
		public qfcbase::Entity
    {
		public:
			Hero();
			~Hero();

		public:
			void CollideWith(std::shared_ptr<Entity> e);
    };
}
