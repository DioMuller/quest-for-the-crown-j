#pragma once

#include <stack>
#include <memory>

#include "Entity.h"
#include "Interfaces.h"
#include "Level.h"

namespace qfcbase
{
	class Game :
		public IUpdateable,
		public Shareable<Game>
	{
		public:
			Game();
			~Game();

			virtual void GoToNeighbour(std::shared_ptr<qfcbase::Entity> entity, qfcbase::Direction direction) = 0;
			virtual void StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2) = 0;

			virtual void UnstackScene(std::shared_ptr<qfcbase::Entity> entity) = 0;
	};
}