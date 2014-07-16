#pragma once
#include <memory>
#include <stack>
#include <functional>
#include "Entity.h"

namespace qfcbase
{
	class Game;

	class Scene
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		protected:
			std::vector<std::shared_ptr<Entity>> entities;
			std::stack<std::shared_ptr<Entity>> toRemove;
			std::stack<std::shared_ptr<Entity>> toAdd;
			Game* parent;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Scene(Game* parent);
			~Scene();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
			virtual void Draw(sf::RenderWindow* renderer);

			void AddEntity(std::shared_ptr<Entity> e);
			void RemoveEntity(std::shared_ptr<Entity> e);

			std::vector<std::shared_ptr<Entity>> GetEntities(std::string category);
			std::vector<std::shared_ptr<Entity>> GetEntities(std::function<bool(const std::shared_ptr<Entity>&)> predicate);
	};
}