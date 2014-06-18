#pragma once
#include "AnimatedSprite.h"
#include "Behavior.h"

namespace qfcbase
{
	class Entity
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::vector<Behavior*> behaviors;
			AnimatedSprite* sprite;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Entity();
			~Entity();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
			virtual void Draw(sf::RenderWindow* renderer);

			void AddBehavior(Behavior* b);
	};
}

