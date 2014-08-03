#pragma once
#include <memory>
#include "Definitions.h"

namespace qfcbase
{
	class Entity;

	class Behavior
	{
		/////////////////////////////////////
		// Properties
		/////////////////////////////////////
		public:
			autoprop(public, public, std::weak_ptr<Entity>, parent, Parent);

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Behavior(std::weak_ptr<Entity> parent);
			~Behavior();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
	};
}

