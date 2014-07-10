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
    };
}
