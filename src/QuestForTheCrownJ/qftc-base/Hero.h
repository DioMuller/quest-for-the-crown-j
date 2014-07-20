#pragma once
#include "Entity.h"

namespace qfcbase
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
