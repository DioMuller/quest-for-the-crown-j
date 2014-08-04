#pragma once
#include "Behavior.h"
#include <SFML/System/Vector2.hpp>

namespace qfcgame
{
	class Walker :
		public qfcbase::Behavior
	{
	private:
		std::shared_ptr<sf::Vector2f> desired_position;

	public:
		Walker(std::weak_ptr<qfcbase::Entity> parent);
		~Walker();

		void WalkTo(sf::Vector2f position);
		void Update(double dt);
	};
}
