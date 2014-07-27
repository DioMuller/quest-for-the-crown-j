#include "Log.h"
#include "GameAssets.h"

using namespace qfcbase;

std::shared_ptr<sf::Font> GameAssets::defaultFont = std::shared_ptr<sf::Font>(new sf::Font());
bool GameAssets::isFontLoaded = false;

std::shared_ptr<sf::Font> GameAssets::DefaultFont()
{
	if (!isFontLoaded)
	{
		isFontLoaded = defaultFont->loadFromFile("Content/fonts/8bitwonder.ttf");

		if (!isFontLoaded) Log::Error("Could not load font!");
	}

	return defaultFont;
}