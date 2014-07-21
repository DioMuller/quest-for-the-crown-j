#pragma once
#include "Entity.h"

namespace qfcgame
{
    class Hero :
		public qfcbase::Entity
    {
    public:
        Hero();
        ~Hero();

	public:
		void Draw(sf::RenderWindow* renderer);
		void CollideWith(std::shared_ptr<Entity> e);
    };
}
