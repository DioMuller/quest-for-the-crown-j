#pragma once
#include <SFML\System\Vector2.hpp>
#include <math.h>

namespace qfcbase
{
	static class Vector
	{
		public:
			static float length(sf::Vector2f vec) { return sqrt(vec.x * vec.x + vec.y * vec.y ); }
			static sf::Vector2f normalize(sf::Vector2f vec) { return vec / length(vec); }
	};
}
