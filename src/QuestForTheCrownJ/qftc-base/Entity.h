#pragma once
#include "AnimatedSprite.h"
#include "Behavior.h"
#include "Container.h"

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
			Container<int> health;
			Container<int> magic;
			Container<int> arrows;

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

		/////////////////////////////////////
		// Properties
		/////////////////////////////////////
		public:
			void SetHealth(const Container<int>& health) { this->health = health; }
			void SetMagic(const Container<int>& magic) { this->magic = magic; }
			void SetArrows(const Container<int>& arrows) { this->arrows = arrows; }

			__declspec(property(put = SetHealth)) Container<int> Health;
			__declspec(property(put = SetMagic)) Container<int> Magic;
			__declspec(property(put = SetArrows)) Container<int> Arrows;

		protected:
			AnimatedSprite* GetSprite() { return this->sprite; }
			void SetSprite(AnimatedSprite* sprite) { this->sprite = sprite; }

			__declspec(property(put = SetSprite, get = GetSprite)) AnimatedSprite* Sprite;
	};
}

