#pragma once
#include <memory>
#include <stack>
#include <functional>
#include "Entity.h"
#include "Shareable.h"

namespace qfcbase
{
	class Game;

	class Scene
		: public Shareable<Scene>
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		protected:
			std::vector<std::shared_ptr<Entity>> entities;
			std::stack<std::shared_ptr<Entity>> toRemove;
			std::stack<std::shared_ptr<Entity>> toAdd;
			bool allowStacking;
			std::weak_ptr<Game> parent;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Scene(std::weak_ptr<Game> parent, bool allowStacking = true);
			~Scene();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);

			void AddRemoveEntities();

			virtual void Draw(sf::RenderWindow* renderer);

			void AddEntity(std::shared_ptr<Entity> e);
			void RemoveEntity(std::shared_ptr<Entity> e);

			std::vector<std::shared_ptr<Entity>> GetEntities(std::string category);
			std::vector<std::shared_ptr<Entity>> GetEntities(std::function<bool(const std::shared_ptr<Entity>&)> predicate);
			//void LoadScene(std::shared_ptr<Scene> scene);
			//void UnloadScene();
			std::weak_ptr<Game> GetParent();
			virtual void OnResume();
	};
}