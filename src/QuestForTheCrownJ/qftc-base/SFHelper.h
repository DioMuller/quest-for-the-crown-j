#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <math.h>

namespace qfcbase
{
	static class SFHelper
	{
		public:
			static float length(sf::Vector2f vec) { return sqrt(vec.x * vec.x + vec.y * vec.y ); }
			static sf::Vector2f normalize(sf::Vector2f vec) { return vec / length(vec); }
			static sf::FloatRect translate(sf::FloatRect rect, sf::Vector2f vec) { return sf::FloatRect(rect.left + vec.x, rect.top + vec.y, rect.width, rect.height); }
	};
}
