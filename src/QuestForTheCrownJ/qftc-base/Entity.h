#pragma once
#include "AnimatedSprite.h"
#include "Behavior.h"
#include "Container.h"
#include "Definitions.h"

namespace qfcbase
{
	typedef struct struct_status
	{
		int level;
		int hp;
		int mp;

		int base_hp;
		int base_mp;

		int base_attack;
		int base_defense;
		int base_magic;
		int base_defense_magic;
		int base_speed;

		int mul_hp;
		int mul_mp;

		int mul_attack;
		int mul_defense;
		int mul_magic;
		int mul_defense_magic;
		int mul_speed;
	} Status;

	class Entity
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::vector<Behavior*> behaviors;
		public:
			Status status;

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
			autoprop(public, protected, AnimatedSprite*, sprite, Sprite);
	};
}
