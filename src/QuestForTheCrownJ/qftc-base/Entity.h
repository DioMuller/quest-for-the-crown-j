#pragma once
#include "AnimatedSprite.h"
#include "Behavior.h"
#include "Container.h"
#include "Definitions.h"

namespace qfcbase
{
	typedef struct struct_status
	{
		int level = 1;
		int hp = 1;
		int mp = 1;

#ifndef CREATE_STATUS
#define CREATE_STATUS(type, field, prop) type base_##field = 0, mul_##field = 1; type Get##prop() { return (base_##field + mul_##field * level); } getdecl(type, prop)

		CREATE_STATUS(int, hp, HP);
		CREATE_STATUS(int, mp, MP);

		CREATE_STATUS(int, attack, Attack);
		CREATE_STATUS(int, defense, Defense);
		CREATE_STATUS(int, magic, Magic);
		CREATE_STATUS(int, defense_magic, DefenseMagic);
		CREATE_STATUS(int, speed, Speed);

#undef CREATE_STATUS
#endif
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
			autoprop(public, protected, float, speed, Speed);
	};
}
