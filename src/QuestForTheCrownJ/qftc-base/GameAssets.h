#pragma once
#include <SFML/Graphics/Text.hpp>
#include <memory>

namespace qfcbase
{
	class GameAssets
	{
		private:
			static std::shared_ptr<sf::Font> defaultFont;
			static bool isFontLoaded;
		public:
			static std::shared_ptr<sf::Font> DefaultFont();
	};
}
