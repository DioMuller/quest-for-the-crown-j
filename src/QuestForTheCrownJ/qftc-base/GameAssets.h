#pragma once
#include <SFML/Graphics/Text.hpp>
#include <memory>

#define USE_OLD_FONT 1

#if USE_OLD_FONT
	#define DEFAULT_FONT "Content/fonts/8bitwonder.ttf"
#else
	#define DEFAULT_FONT "Content/fonts/8bitlim.ttf"
#endif

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
