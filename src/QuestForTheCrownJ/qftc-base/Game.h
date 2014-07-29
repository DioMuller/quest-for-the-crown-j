#pragma once

#include <stack>
#include <memory>

#include "Entity.h"
#include "Interfaces.h"

namespace qfcbase
{
	class Game :
		public IUpdateable,
		public Shareable<Game>
	{
		public:
			Game();
			~Game();

			virtual void StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2) = 0;
	};
}