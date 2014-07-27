#pragma once
#include "Entity.h"

namespace qfcbase
{
	class BattleEntity : public Entity
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::shared_ptr<Entity> parent;

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			std::shared_ptr<Entity> Parent()
			{
				return parent;
			}
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			BattleEntity(std::shared_ptr<Entity> parent);
			~BattleEntity();
	};
}