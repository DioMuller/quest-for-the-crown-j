#pragma once
#include <memory>
#include "AnimatedSprite.h"
#include "Behavior.h"
#include "Container.h"
#include "Definitions.h"
#include "Scene.h"
#include "Shareable.h"

namespace qfcbase
{
	typedef struct struct_status
	{
		int level = 1;
		int hp = 10;
		int mp = 5;

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

	typedef struct struct_items
	{
		int gold = 0;
		int potions = 0;
	} Items;

	class Scene;

	class Entity : public Shareable<Entity>
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::vector<std::shared_ptr<Behavior>> behaviors;
			std::string lastDirectionName;
			sf::Text nameText;
			sf::Font nameFont;
		public:
			std::weak_ptr<qfcbase::Scene> scene;
			std::string category;
			Status status;
			Items items;

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

			void AddBehavior(std::shared_ptr<Behavior> b);
			void RemoveBehavior(std::shared_ptr<Behavior> b);
			template<class Behavior, class... ArgTypes>
			std::shared_ptr<Behavior> AddBehavior(ArgTypes&&... _Args)
			{
				auto b = std::make_shared<Behavior>(getptr(), _STD forward<ArgTypes>(_Args)...);
				AddBehavior(b);
				return b;
			}
			template <typename Behavior>
			std::shared_ptr<Behavior> FindBehavior()
			{
				for (auto& b : behaviors)
				{
					if (auto desired = std::dynamic_pointer_cast<Behavior>(b))
						return desired;
				}
				return nullptr;
			}

			void Walk(sf::Vector2f direction, double dt);
			virtual void CollideWith(std::shared_ptr<Entity> e);

			std::weak_ptr<qfcbase::Scene> Scene()
			{
				return scene;
			}

			std::string GetName() const { return nameText.getString(); }
			void SetName(const std::string& name) { nameText.setString(name); }
		private:
			bool ValidPosition(sf::Vector2f offset);

		/////////////////////////////////////
		// Properties
		/////////////////////////////////////
		public:
			getsetdecl(std::string, Name);
			autoprop(public, public, int, id, Id);
			autoprop(public, protected, std::shared_ptr<AnimatedSprite>, sprite, Sprite);
			autoprop(public, protected, float, speed, Speed);
	};
}
